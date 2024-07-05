#pragma once

#include <Windows.h>
#include <strsafe.h>
#include <winerror.h>
#include <SimConnect.h>

enum GROUP_ID {
    GROUP_6,
    GROUP0,
};

enum INPUT_ID {
    INPUT_6,
    INPUT0,
};

enum EVENT_ID {
    EVENT_SIM_START,
    EVENT_6,
    EVENT_AUTOPILOT,
    EVENT_ALT_LOCK,
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
    double kohlsmann;
    double altitude;
    double latitude;
    double longitude;

    double ap;
    double ap_airspeed;
    double ap_airspeed_hold;
    double ap_alt_lock;
    double ap_alt_lock_var;
    double ap_alt_manually_adjustable;
    double ap_approach_captured;
    double ap_flight_director;
    double ap_heading_lock;
    double ap_heading_lock_dir;
    double ap_heading_manually_adjustable;
    double ap_vertical_hold;
    double ap_vertical_speed;
    bool updated = false;
};


class Socket
{
private:
    HANDLE m_SimConnectHandle;
    bool m_Connected = false;
private:
    bool Connect() noexcept
    {
        if (FAILED(SimConnect_Open(&m_SimConnectHandle, "Auto Nav", NULL, 0, 0, 0)))
        {
            return false; // TODO add exlpanation code
        }


        // TODO: build wrapper function to handle errors for each
        HRESULT hr;
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_6, "Initial Position", NULL, SIMCONNECT_DATATYPE_INITPOSITION);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "Title", NULL, SIMCONNECT_DATATYPE_STRING256);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "Kohlsman setting hg", "inHg");
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "Plane Altitude", "feet");
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "Plane Latitude", "degrees");
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "Plane Longitude", "degrees");

        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT MASTER", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT AIRSPEED HOLD VAR", "knots");
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT AIRSPEED HOLD", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT ALTITUDE LOCK", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT ALTITUDE LOCK VAR", "feet");
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT ALTITUDE MANUALLY TUNABLE", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT APPROACH CAPTURED", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT FLIGHT DIRECTOR ACTIVE", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT HEADING LOCK", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT HEADING LOCK DIR", "degrees");
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT HEADING MANUALLY TUNABLE", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT VERTICAL HOLD", NULL);
        hr = SimConnect_AddToDataDefinition(m_SimConnectHandle, DEFINITION_1, "AUTOPILOT VERTICAL HOLD VAR", "feet/minute");


        hr = SimConnect_MapClientEventToSimEvent(m_SimConnectHandle, EVENT_AUTOPILOT, "AP_MASTER");
        hr = SimConnect_MapClientEventToSimEvent(m_SimConnectHandle, EVENT_ALT_LOCK, "AP_ALT_HOLD");
        hr = SimConnect_AddClientEventToNotificationGroup(m_SimConnectHandle, GROUP0, EVENT_AUTOPILOT);
        hr = SimConnect_AddClientEventToNotificationGroup(m_SimConnectHandle, GROUP0, EVENT_ALT_LOCK);
        hr = SimConnect_SetNotificationGroupPriority(m_SimConnectHandle, GROUP0, SIMCONNECT_GROUP_PRIORITY_HIGHEST);
        //
        //hr = SimConnect_MapInputEventToClientEvent(m_SimConnectHandle, INPUT0, "z", EVENT_BRAKES);
        //hr = SimConnect_SetInputGroupState(m_SimConnectHandle, INPUT0, SIMCONNECT_STATE_ON);
        m_Connected = true;
        return true;
    }
public:
    inline const char* ToggleConnection(bool shutdown = false) noexcept
    {
        if (m_Connected)
        {
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


    bool FetchData() const noexcept
    {
        return SUCCEEDED(SimConnect_RequestDataOnSimObjectType(m_SimConnectHandle, REQUEST_1, DEFINITION_1, 0, SIMCONNECT_SIMOBJECT_TYPE_USER));
    }


    void TransmitEvent(EVENT_ID event) const noexcept
    {
        SimConnect_TransmitClientEvent(m_SimConnectHandle, SIMCONNECT_OBJECT_ID_USER, event, 0, GROUP0, 0);
    }


    // TODO: Remove
    HANDLE Handle() const noexcept
    {
        return m_SimConnectHandle;
    }

    void Dispatch(Struct1* s)
    {
        SimConnect_CallDispatch(m_SimConnectHandle, DispatchCallback, (void*)s);
    }


    static void DispatchCallback(SIMCONNECT_RECV* pData, DWORD, void* strct)
    {
        switch (pData->dwID)
        {
        case SIMCONNECT_RECV_ID_SIMOBJECT_DATA_BYTYPE:
        {
            SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE* pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE*)pData;

            switch (pObjData->dwRequestID)
            {
            case REQUEST_1:
            {
                //DWORD ObjectID = pObjData->dwObjectID;
                Struct1* ps = (Struct1*)&pObjData->dwData;
                Struct1* tmp = (Struct1*)strct;
                if (SUCCEEDED(StringCbLengthA(&ps->title[0], sizeof(ps->title), NULL))) // security check
                {
                    std::strcpy(tmp->title, ps->title);
                    tmp->kohlsmann = ps->kohlsmann;
                    tmp->altitude = ps->altitude;
                    tmp->latitude = ps->latitude;
                    tmp->longitude = ps->longitude;
                    tmp->ap = ps->ap;
                    tmp->ap_airspeed = ps->ap_airspeed;
                    tmp->ap_airspeed_hold = ps->ap_airspeed_hold;
                    tmp->ap_alt_lock = ps->ap_alt_lock;
                    tmp->ap_alt_lock_var = ps->ap_alt_lock_var;
                    tmp->ap_alt_manually_adjustable = ps->ap_alt_manually_adjustable;
                    tmp->ap_approach_captured = ps->ap_approach_captured;
                    tmp->ap_flight_director = ps->ap_flight_director;
                    tmp->ap_heading_lock = ps->ap_heading_lock;
                    tmp->ap_heading_lock_dir = ps->ap_heading_lock_dir;
                    tmp->ap_heading_manually_adjustable = ps->ap_heading_manually_adjustable;
                    tmp->ap_vertical_hold = ps->ap_vertical_hold;
                    tmp->ap_vertical_speed = ps->ap_vertical_speed;
                    tmp->updated = true;


                    //printf("\nObjectID=%d  Title=\"%s\"\nLat=%f  Lon=%f  Alt=%f  Kohlsman=%.2f\n", ObjectID, ps->title, ps->latitude, ps->longitude, ps->altitude, ps->kohlsmann);

                    printf("Autopilot: %.0lf\n", ps->ap);
                    printf("Airspeed: %.2lf\n", ps->ap_airspeed);
                    printf("Airspeed hold: %.0lf\n", ps->ap_airspeed_hold);
                    printf("Altitude locked: %.0lf\n", ps->ap_alt_lock);
                    printf("Altitude: %.2lf\n", ps->ap_alt_lock_var);
                    printf("Altitude manually adj: %.0lf\n", ps->ap_alt_manually_adjustable);

                    printf("Approach captured: %.0lf\n", ps->ap_approach_captured);
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

        // TODO make quit when sim exits
        case SIMCONNECT_RECV_ID_QUIT:
        {
            //    quit = 1;
            break;
        }

        default:
            printf("\nReceived:%d", pData->dwID);
            break;
        }
    }
};