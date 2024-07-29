#pragma once
#include <Windows.h>
#include <strsafe.h>
#include <winerror.h>
#include <SimConnect.h>

#include "Event.h"

enum GROUP_ID {
    GROUP_6,
    GROUP0,
};

enum EVENT_ID {
    // Buttons
    EVENT_AUTOPILOT,
    EVENT_ALTITUDE_HOLD,
    EVENT_AIRSPEED_HOLD, // AP_AIRSPEED_HOLD
    EVENT_APPROACH_HOLD, // AP_APR_HOLD
    EVENT_FLC_HOLD, // FLIGHT_LEVEL_CHANGE
    EVENT_FLIGHT_DIRECTOR, // TOGGLE_FLIGHT_DIRECTOR
    EVENT_NAV_LOCKED, // AP_NAV1_HOLD
    EVENT_HEADING_LOCKED, // AP_HDG_HOLD
    EVENT_VERTICAL_SPEED_HOLD, // AP_PANEL_VS_HOLD
    EVENT_WING_LVL_HOLD, // AP_WING_LEVELER
    EVENT_YAW_DAMPER_HOLD, // YAW_DAMPER_TOGGLE
    EVENT_AUTO_THROTTLE_ARM,

    // Counters
    EVENT_SET_ALTITUDE, // AP_ALT_VAR_SET_ENGLISH in feet
    EVENT_SET_AIRSPEED, // TODO
    EVENT_SET_HEADING, // HEADING_BUG_SET
    EVENT_SET_VERTICAL_SPEED, // AP_VS_VAR_SET_ENGLISH
    EVENT_SET_HEADING_IDX
};

enum DATA_DEFINE_ID {
    DEFINITION_1,
    DEFINITION_6,
    DEFINITION_PLANE_POS
};

enum DATA_REQUEST_ID {
    REQUEST_1,
    REQUEST_PLANE_POS
};


struct Struct1
{
    char   title[256] = { 0 };
    double ap;
    double ap_available;
    double ap_airspeed;
    double ap_airspeed_hold;
    double ap_auto_throttle;
    double ap_alt_lock;
    double ap_alt_lock_var;
    double ap_alt_manually_adjustable;
    double ap_approach_hold;
    double ap_flight_level_change;
    double ap_flight_director;
    double ap_nav_lock;
    double ap_heading_lock;
    double ap_heading_lock_dir;
    double ap_heading_manually_adjustable;
    double ap_vertical_hold;
    double ap_vertical_speed;
    double ap_wing_lvl;
    double ap_yaw_damper;
    double ap_heading_idx;

    double pos_latitude = 0.0;
    double pos_longitude = 0.0;
    double pos_altitude = 0.0;
    double pos_airspeed = 0.0;

    bool update_ap = false;
    bool update_pos = false;
    bool quit = false;
};


class Socket
{
private:
    HANDLE m_SimConnectHandle;
    bool m_Connected = false;
    bool m_RequestPositionData = true;
public:
    bool ShouldUpdate = false;
private:
    bool Connect() noexcept
    {
        Events::Start();
        if (FAILED(SimConnect_Open(&m_SimConnectHandle, "Auto Nav", NULL, 0, Events::Handles[0], 0)))
        {
            Events::Stop();
            return false; // TODO add exlpanation code
        }

        // TODO: build wrapper function to handle errors for each
        HRESULT hr;
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_6, "Initial Position", NULL, SIMCONNECT_DATATYPE_INITPOSITION);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "Title", NULL, SIMCONNECT_DATATYPE_STRING256);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT MASTER", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT AVAILABLE", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT AIRSPEED HOLD VAR", "knots");
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT AIRSPEED HOLD", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT THROTTLE ARM", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT ALTITUDE LOCK", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT ALTITUDE LOCK VAR", "feet");
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT ALTITUDE MANUALLY TUNABLE", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT APPROACH HOLD", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT FLIGHT LEVEL CHANGE", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT FLIGHT DIRECTOR ACTIVE", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT NAV1 LOCK", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT HEADING LOCK", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT HEADING LOCK DIR", "degrees");
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT HEADING MANUALLY TUNABLE", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT VERTICAL HOLD", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT VERTICAL HOLD VAR", "feet/minute");
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT WING LEVELER", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT YAW DAMPER", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT HEADING SLOT INDEX", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_PLANE_POS, "PLANE LATITUDE", "degree latitude");
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_PLANE_POS, "PLANE LONGITUDE", "degree longitude");
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_PLANE_POS, "PLANE ALTITUDE", "feet");
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_PLANE_POS, "AIRSPEED MACH", "mach");

        MapClientEventToSimEvent(GROUP0, EVENT_AUTOPILOT, "AP_MASTER");
        MapClientEventToSimEvent(GROUP0, EVENT_AIRSPEED_HOLD, "AP_AIRSPEED_HOLD");
        MapClientEventToSimEvent(GROUP0, EVENT_ALTITUDE_HOLD, "AP_ALT_HOLD");
        MapClientEventToSimEvent(GROUP0, EVENT_APPROACH_HOLD, "AP_APR_HOLD");
        MapClientEventToSimEvent(GROUP0, EVENT_FLC_HOLD, "FLIGHT_LEVEL_CHANGE");
        MapClientEventToSimEvent(GROUP0, EVENT_FLIGHT_DIRECTOR, "TOGGLE_FLIGHT_DIRECTOR");
        MapClientEventToSimEvent(GROUP0, EVENT_NAV_LOCKED, "AP_NAV1_HOLD");
        MapClientEventToSimEvent(GROUP0, EVENT_HEADING_LOCKED, "AP_HDG_HOLD");
        MapClientEventToSimEvent(GROUP0, EVENT_VERTICAL_SPEED_HOLD, "AP_VS_HOLD");
        MapClientEventToSimEvent(GROUP0, EVENT_WING_LVL_HOLD, "AP_WING_LEVELER");
        MapClientEventToSimEvent(GROUP0, EVENT_AUTO_THROTTLE_ARM, "AUTO_THROTTLE_ARM");
        MapClientEventToSimEvent(GROUP0, EVENT_YAW_DAMPER_HOLD, "YAW_DAMPER_TOGGLE");
        MapClientEventToSimEvent(GROUP0, EVENT_SET_ALTITUDE, "AP_ALT_VAR_SET_ENGLISH");
        MapClientEventToSimEvent(GROUP0, EVENT_SET_HEADING, "HEADING_BUG_SET");
        MapClientEventToSimEvent(GROUP0, EVENT_SET_VERTICAL_SPEED, "AP_VS_VAR_SET_ENGLISH");
        MapClientEventToSimEvent(GROUP0, EVENT_SET_AIRSPEED, "AP_SPD_VAR_SET");
        MapClientEventToSimEvent(GROUP0, EVENT_SET_HEADING_IDX, "HEADING_SLOT_INDEX_SET");

        hr = SimConnect_SetNotificationGroupPriority(m_SimConnectHandle, GROUP0, SIMCONNECT_GROUP_PRIORITY_HIGHEST);
        hr = SimConnect_RequestDataOnSimObject(m_SimConnectHandle, REQUEST_1, DEFINITION_1, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED);
        if (m_RequestPositionData)
            hr = SimConnect_RequestDataOnSimObject(m_SimConnectHandle, REQUEST_PLANE_POS, DEFINITION_PLANE_POS, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED);
        m_Connected = true;
        return true;
    }


    inline void MapClientEventToSimEvent(SIMCONNECT_NOTIFICATION_GROUP_ID group, SIMCONNECT_CLIENT_EVENT_ID eventId, const char* event_name)
    {
        if (FAILED(SimConnect_MapClientEventToSimEvent(m_SimConnectHandle, eventId, event_name)))
        {
            fprintf(stderr, "Failed to map event: %s\n", event_name);
            return;
        }

        if (FAILED(SimConnect_AddClientEventToNotificationGroup(m_SimConnectHandle, group, eventId)))
        {
            fprintf(stderr, "Failed to add event to notification group: %s\n", event_name);
            return;
        }
    }
public:
    inline const char* ToggleConnection(bool shutdown = false) noexcept
    {
        if (m_Connected)
        {
            Events::Stop();
            if (FAILED(SimConnect_Close(m_SimConnectHandle)))
                return "Failed to disconnect from flight simulator";
            m_Connected = false;
        }
        else
        {
            if (!shutdown && !Connect())
                return "Failed to connect to flight simulator";
        }
        return nullptr;
    }


    inline bool IsConnected() const noexcept
    {
        return m_Connected;
    }


    void TogglePlanePosData() noexcept
    {
        if (IsConnected())
        {
            if (m_RequestPositionData)
            {
                SimConnect_RequestDataOnSimObject(m_SimConnectHandle, REQUEST_PLANE_POS, DEFINITION_PLANE_POS, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_NEVER, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED);
            }
            else
            {
                SimConnect_RequestDataOnSimObject(m_SimConnectHandle, REQUEST_PLANE_POS, DEFINITION_PLANE_POS, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED);
            }
        }
        m_RequestPositionData = !m_RequestPositionData;
    }


    void SetTestPosition() const noexcept
    {
        SIMCONNECT_DATA_INITPOSITION Init;
        Init.Altitude = 7000.0;
        Init.Latitude = 45.41254109849314;
        Init.Longitude = 12.56128520024438;
        Init.Pitch = 0.0;
        Init.Bank = -1.0;
        Init.Heading = 220;
        Init.OnGround = 0;
        Init.Airspeed = 100;
        SimConnect_SetDataOnSimObject(m_SimConnectHandle, DEFINITION_6, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(Init), &Init);
    }


    bool TransmitEvent(EVENT_ID event, DWORD data) const noexcept
    {
        if (!m_Connected)
            return true;
        if (FAILED(SimConnect_TransmitClientEvent(m_SimConnectHandle, SIMCONNECT_OBJECT_ID_USER, event, data, GROUP0, 0)))
            return false;
        return true;
    }


    unsigned long Dispatch(Struct1* s)
    {
        DWORD cbData;
        SIMCONNECT_RECV* pData;
        if (FAILED(SimConnect_GetNextDispatch(m_SimConnectHandle, &pData, &cbData)))
            return 0;

        switch (pData->dwID)
        {
        case SIMCONNECT_RECV_ID_EVENT:
        {
            SIMCONNECT_RECV_EVENT* evt = (SIMCONNECT_RECV_EVENT*)pData;

            switch (evt->uEventID)
            {
            case EVENT_AUTOPILOT:
            case EVENT_ALTITUDE_HOLD:
            case EVENT_AIRSPEED_HOLD:
            case EVENT_APPROACH_HOLD:
            case EVENT_FLC_HOLD:
            case EVENT_FLIGHT_DIRECTOR:
            case EVENT_NAV_LOCKED:
            case EVENT_HEADING_LOCKED:
            case EVENT_VERTICAL_SPEED_HOLD:
            case EVENT_WING_LVL_HOLD:
            case EVENT_YAW_DAMPER_HOLD:
            case EVENT_AUTO_THROTTLE_ARM:
            case EVENT_SET_ALTITUDE:
            case EVENT_SET_AIRSPEED:
            case EVENT_SET_HEADING:
            case EVENT_SET_VERTICAL_SPEED:
            case EVENT_SET_HEADING_IDX:
                ShouldUpdate = true;
                break;
            default:
                break;
            }
            break;
        }
        case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
        {
            SIMCONNECT_RECV_SIMOBJECT_DATA* pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;

            switch (pObjData->dwRequestID)
            {
            case REQUEST_1:
            {
                Struct1* ps = (Struct1*)&pObjData->dwData;
                if (SUCCEEDED(StringCbLengthA(&ps->title[0], sizeof(ps->title), NULL))) // security check
                {
                    std::strcpy(s->title, ps->title);
                    s->ap = ps->ap;
                    s->ap_available = ps->ap_available;
                    s->ap_airspeed = ps->ap_airspeed;
                    s->ap_airspeed_hold = ps->ap_airspeed_hold;
                    s->ap_auto_throttle = ps->ap_auto_throttle;
                    s->ap_alt_lock = ps->ap_alt_lock;
                    s->ap_alt_lock_var = ps->ap_alt_lock_var;
                    s->ap_alt_manually_adjustable = ps->ap_alt_manually_adjustable;
                    s->ap_approach_hold = ps->ap_approach_hold;
                    s->ap_flight_level_change = ps->ap_flight_level_change;
                    s->ap_flight_director = ps->ap_flight_director;
                    s->ap_nav_lock = ps->ap_nav_lock;
                    s->ap_heading_lock = ps->ap_heading_lock;
                    s->ap_heading_lock_dir = ps->ap_heading_lock_dir;
                    s->ap_heading_manually_adjustable = ps->ap_heading_manually_adjustable;
                    s->ap_vertical_hold = ps->ap_vertical_hold;
                    s->ap_vertical_speed = ps->ap_vertical_speed;
                    s->ap_wing_lvl = ps->ap_wing_lvl;
                    s->ap_yaw_damper = ps->ap_yaw_damper;
                    s->ap_heading_idx = ps->ap_heading_idx;
                    s->update_ap = true;
                }
                break;
            }
            case REQUEST_PLANE_POS:
            {
                struct PlanePos
                {
                    double lat;
                    double lon;
                    double alt;
                    double speed;
                };

                PlanePos* pp = (PlanePos*)&pObjData->dwData;
                s->pos_latitude = pp->lat;
                s->pos_longitude = pp->lon;
                s->pos_altitude = pp->alt;
                s->pos_airspeed = pp->speed;
                s->update_pos = true;
                break;
            }
            default:
                break;
            }
            break;
        }
        case SIMCONNECT_RECV_ID_EXCEPTION:
        {
            SIMCONNECT_RECV_EXCEPTION * except = (SIMCONNECT_RECV_EXCEPTION*)pData;
            return except->dwException;
        }
        case SIMCONNECT_RECV_ID_QUIT:
        {
            s->quit = true;
            break;
        }
        default:
            break;
        }
        return 0;
    }
};