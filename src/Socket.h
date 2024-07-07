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

enum INPUT_ID {
    INPUT_6,
    INPUT0,
};


enum EVENT_ID {
    // Buttons
    EVENT_AUTOPILOT,
    EVENT_ALTITUDE_HOLD,
    EVENT_AIRSPEED_HOLD, // AP_AIRSPEED_HOLD
    EVENT_APPROACH_HOLD, // AP_APR_HOLD
    EVENT_FLIGHT_DIRECTOR, // TOGGLE_FLIGHT_DIRECTOR
    EVENT_HEADING_LOCKED, // AP_HDG_HOLD
    EVENT_VERTICAL_SPEED_HOLD, // AP_PANEL_VS_HOLD

    // Counters
    EVENT_SET_ALTITUDE, // AP_ALT_VAR_SET_ENGLISH in feet
    EVENT_SET_AIRSPEED, // TODO
    EVENT_SET_HEADING, // HEADING_BUG_SET
    EVENT_SET_VERTICAL_SPEED, // AP_VS_VAR_SET_ENGLISH
};

enum DATA_DEFINE_ID {
    DEFINITION_1,
    DEFINITION_6,
};

enum DATA_REQUEST_ID {
    REQUEST_1,
};


struct Struct1
{
    char   title[256];
    double ap;
    double ap_airspeed;
    double ap_airspeed_hold;
    double ap_alt_lock;
    double ap_alt_lock_var;
    double ap_alt_manually_adjustable;
    double ap_approach_hold;
    double ap_flight_director;
    double ap_heading_lock;
    double ap_heading_lock_dir;
    double ap_heading_manually_adjustable;
    double ap_vertical_hold;
    double ap_vertical_speed;
    bool updated = false;
    bool quit = false;
};


class Socket
{
private:
    HANDLE m_SimConnectHandle;
    bool m_Connected = false;
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
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT AIRSPEED HOLD VAR", "knots");
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT AIRSPEED HOLD", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT ALTITUDE LOCK", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT ALTITUDE LOCK VAR", "feet");
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT ALTITUDE MANUALLY TUNABLE", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT APPROACH HOLD", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT FLIGHT DIRECTOR ACTIVE", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT HEADING LOCK", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT HEADING LOCK DIR", "degrees");
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT HEADING MANUALLY TUNABLE", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT VERTICAL HOLD", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT VERTICAL HOLD VAR", "feet/minute");

        MapClientEventToSimEvent(GROUP0, EVENT_AUTOPILOT, "AP_MASTER");
        MapClientEventToSimEvent(GROUP0, EVENT_AIRSPEED_HOLD, "AP_AIRSPEED_HOLD");
        MapClientEventToSimEvent(GROUP0, EVENT_ALTITUDE_HOLD, "AP_ALT_HOLD");
        MapClientEventToSimEvent(GROUP0, EVENT_APPROACH_HOLD, "AP_APR_HOLD");
        MapClientEventToSimEvent(GROUP0, EVENT_FLIGHT_DIRECTOR, "TOGGLE_FLIGHT_DIRECTOR");
        MapClientEventToSimEvent(GROUP0, EVENT_HEADING_LOCKED, "AP_HDG_HOLD");
        MapClientEventToSimEvent(GROUP0, EVENT_VERTICAL_SPEED_HOLD, "AP_VS_HOLD");
        MapClientEventToSimEvent(GROUP0, EVENT_SET_ALTITUDE, "AP_ALT_VAR_SET_ENGLISH");
        MapClientEventToSimEvent(GROUP0, EVENT_SET_HEADING, "HEADING_BUG_SET");
        MapClientEventToSimEvent(GROUP0, EVENT_SET_VERTICAL_SPEED, "AP_VS_VAR_SET_ENGLISH");

        hr = SimConnect_SetNotificationGroupPriority(m_SimConnectHandle, GROUP0, SIMCONNECT_GROUP_PRIORITY_HIGHEST);
        hr = SimConnect_RequestDataOnSimObject(m_SimConnectHandle, REQUEST_1, DEFINITION_1, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED);
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


    void TransmitEvent(EVENT_ID event, DWORD data = 0) const noexcept
    {
        if (FAILED(SimConnect_TransmitClientEvent(m_SimConnectHandle, SIMCONNECT_OBJECT_ID_USER, event, data, GROUP0, 0)))
            puts("Error: Failed to transmit event");
    }


    // TODO: Remove
    HANDLE Handle() const noexcept
    {
        return m_SimConnectHandle;
    }

    void Dispatch(Struct1* s)
    {
        DWORD cbData;
        SIMCONNECT_RECV* pData;
        if (FAILED(SimConnect_GetNextDispatch(m_SimConnectHandle, &pData, &cbData)))
            return;

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
            case EVENT_FLIGHT_DIRECTOR:
            case EVENT_HEADING_LOCKED:
            case EVENT_VERTICAL_SPEED_HOLD:
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
                //DWORD ObjectID = pObjData->dwObjectID;
                Struct1* ps = (Struct1*)&pObjData->dwData;
                if (SUCCEEDED(StringCbLengthA(&ps->title[0], sizeof(ps->title), NULL))) // security check
                {
                    std::strcpy(s->title, ps->title);
                    s->ap = ps->ap;
                    s->ap_airspeed = ps->ap_airspeed;
                    s->ap_airspeed_hold = ps->ap_airspeed_hold;
                    s->ap_alt_lock = ps->ap_alt_lock;
                    s->ap_alt_lock_var = ps->ap_alt_lock_var;
                    s->ap_alt_manually_adjustable = ps->ap_alt_manually_adjustable;
                    s->ap_approach_hold = ps->ap_approach_hold;
                    s->ap_flight_director = ps->ap_flight_director;
                    s->ap_heading_lock = ps->ap_heading_lock;
                    s->ap_heading_lock_dir = ps->ap_heading_lock_dir;
                    s->ap_heading_manually_adjustable = ps->ap_heading_manually_adjustable;
                    s->ap_vertical_hold = ps->ap_vertical_hold;
                    s->ap_vertical_speed = ps->ap_vertical_speed;
                    s->updated = true;


                    //printf("\nObjectID=%d  Title=\"%s\"\nLat=%f  Lon=%f  Alt=%f  Kohlsman=%.2f\n", ObjectID, ps->title, ps->latitude, ps->longitude, ps->altitude, ps->kohlsmann);

                    printf("Autopilot: %.0lf\n", ps->ap);
                    printf("Airspeed: %.2lf\n", ps->ap_airspeed);
                    printf("Airspeed hold: %.0lf\n", ps->ap_airspeed_hold);
                    printf("Altitude locked: %.0lf\n", ps->ap_alt_lock);
                    printf("Altitude: %.2lf\n", ps->ap_alt_lock_var);
                    printf("Altitude manually adj: %.0lf\n", ps->ap_alt_manually_adjustable);

                    printf("Approach captured: %.0lf\n", ps->ap_approach_hold);
                    printf("Flight Director: %.0lf\n", ps->ap_flight_director);
                    printf("Heading direction: %.2lf\n", ps->ap_heading_lock_dir);

                    printf("Heading adj: %.0lf\n", ps->ap_heading_manually_adjustable);
                    printf("Vertical speed hold: %.0lf\n", ps->ap_vertical_hold);
                    printf("Vertical Speed: %.0lf\n", ps->ap_vertical_speed);

                    printf("\n\n\n");
                }
                break;
            }

            default:
                break;
            }
            break;
        }

        case SIMCONNECT_RECV_ID_QUIT:
        {
            s->quit = true;
            break;
        }

        default:
            printf("\nReceived:%d", pData->dwID);
            break;
        }
    }
};