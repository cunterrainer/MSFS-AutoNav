#include <new>
#include <string>
#include <cstring>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm>

#include <tchar.h>
#include <Windows.h>
#include <strsafe.h>
#include <winerror.h>
#include <SimConnect.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"

#include "Log.h"
#include "Clang.h"
#include "RenderWindow.h"



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
    double ap_heading_lock_dir;
    double ap_heading_manually_adjustable;
    double ap_vertical_hold;
    double ap_vertical_speed;
};

HANDLE  hSimConnect = NULL;
Struct1 glb;


enum GROUP_ID {
    GROUP_6,
};

enum INPUT_ID {
    INPUT_6,
};

enum EVENT_ID {
    EVENT_SIM_START,
    EVENT_6
};

enum DATA_DEFINE_ID {
    DEFINITION_1,
};

enum DATA_REQUEST_ID {
    REQUEST_1,
};



void CALLBACK MyDispatchProcRD(SIMCONNECT_RECV* pData, DWORD, void*)
{
    HRESULT hr;

    switch (pData->dwID)
    {
    case SIMCONNECT_RECV_ID_EVENT:
    {
        SIMCONNECT_RECV_EVENT* evt = (SIMCONNECT_RECV_EVENT*)pData;

        switch (evt->uEventID)
        {
        case EVENT_SIM_START:

            // Now the sim is running, request information on the user aircraft
            hr = SimConnect_RequestDataOnSimObjectType(hSimConnect, REQUEST_1, DEFINITION_1, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);

            break;

        default:
            break;
        }
        break;
    }

    case SIMCONNECT_RECV_ID_SIMOBJECT_DATA_BYTYPE:
    {
        SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE* pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE*)pData;

        switch (pObjData->dwRequestID)
        {
        case REQUEST_1:
        {
            //DWORD ObjectID = pObjData->dwObjectID;
            Struct1* ps = (Struct1*)&pObjData->dwData;
            if (SUCCEEDED(StringCbLengthA(&ps->title[0], sizeof(ps->title), NULL))) // security check
            {
                std::strcpy(glb.title, ps->title);
                glb.kohlsmann = ps->kohlsmann;
                glb.altitude = ps->altitude;
                glb.latitude = ps->latitude;
                glb.longitude = ps->longitude;
                glb.ap = ps->ap;
                glb.ap_airspeed = ps->ap_airspeed;
                glb.ap_airspeed_hold = ps->ap_airspeed_hold;
                glb.ap_alt_lock = ps->ap_alt_lock;
                glb.ap_alt_lock_var = ps->ap_alt_lock_var;
                glb.ap_alt_manually_adjustable = ps->ap_alt_manually_adjustable;
                glb.ap_approach_captured = ps->ap_approach_captured;
                glb.ap_flight_director = ps->ap_flight_director;
                glb.ap_heading_lock_dir = ps->ap_heading_lock_dir;
                glb.ap_heading_manually_adjustable = ps->ap_heading_manually_adjustable;
                glb.ap_vertical_hold = ps->ap_vertical_hold;
                glb.ap_vertical_speed = ps->ap_vertical_speed;
                

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


    //case SIMCONNECT_RECV_ID_QUIT:
    //{
    //    quit = 1;
    //    break;
    //}

    default:
        printf("\nReceived:%d", pData->dwID);
        break;
    }
}


void App()
{
    RenderWindow window(1200, 760, "AutoNav");

    while (window.IsOpen())
    {
        window.StartFrame();
        ImVec2 newPos = window.Pos();
        const float windowWidth = window.Size().x;
        const float windowHeight = window.Size().y;

        ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
        ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
        ImGui::SetNextWindowPos(newPos);
        ImGui::Begin("##MenuBarWindow", nullptr, IMGUI_WINDOW_FLAGS);
        //SimConnect_CallDispatch(hSimConnect, MyDispatchProcRD, NULL);

        Struct1* ps = &glb;

        ImGui::Text("Autopilot: %.0lf\n", ps->ap);
        ImGui::Text("Airspeed: %.2lf\n", ps->ap_airspeed);
        ImGui::Text("Airspeed hold: %.0lf\n", ps->ap_airspeed_hold);
        ImGui::Text("Altitude locked: %.0lf\n", ps->ap_alt_lock);
        ImGui::Text("Altitude: %.2lf\n", ps->ap_alt_lock_var);
        ImGui::Text("Altitude manually adj: %.0lf\n", ps->ap_alt_manually_adjustable);
        ImGui::Text("Approach captured: %.0lf\n", ps->ap_approach_captured);
        ImGui::Text("Flight Director: %.0lf\n", ps->ap_flight_director);
        ImGui::Text("Heading direction: %.2lf\n", ps->ap_heading_lock_dir);
        ImGui::Text("Heading adj: %.0lf\n", ps->ap_heading_manually_adjustable);
        ImGui::Text("Vertical speed hold: %.0lf\n", ps->ap_vertical_hold);
        ImGui::Text("Vertical Speed: %.0lf\n", ps->ap_vertical_speed);

        //Struct1 a;
        //Struct1* ps = &a;
        //
        //ImGui::Text("Autopilot: %.0lf\n", ps->ap);
        //ImGui::Text("Airspeed: %.2lf\n", ps->ap_airspeed);
        //ImGui::Text("Airspeed hold: %.0lf\n", ps->ap_airspeed_hold);
        //ImGui::Text("Altitude locked: %.0lf\n", ps->ap_alt_lock);
        //ImGui::Text("Altitude: %.2lf\n", ps->ap_alt_lock_var);
        //ImGui::Text("Altitude manually adj: %.0lf\n", ps->ap_alt_manually_adjustable);
        //ImGui::Text("Approach captured: %.0lf\n", ps->ap_approach_captured);
        //ImGui::Text("Flight Director: %.0lf\n", ps->ap_flight_director);
        //ImGui::Text("Heading direction: %.2lf\n", ps->ap_heading_lock_dir);
        //ImGui::Text("Heading adj: %.0lf\n", ps->ap_heading_manually_adjustable);
        //ImGui::Text("Vertical speed hold: %.0lf\n", ps->ap_vertical_hold);
        //ImGui::Text("Vertical Speed: %.0lf\n", ps->ap_vertical_speed);

        ImGui::End();
        window.EndFrame();
    }
}


int main()
{
    //if (FAILED(SimConnect_Open(&hSimConnect, "Request Data", NULL, 0, 0, 0)))
    //{
    //    Err("Failed to connect to Flight Simulator");
    //    return 0;
    //}
    //
    //HRESULT hr;
    //hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Title", NULL, SIMCONNECT_DATATYPE_STRING256);
    //hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Kohlsman setting hg", "inHg");
    //hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Altitude", "feet");
    //hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Latitude", "degrees");
    //hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Longitude", "degrees");
    //
    //hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT MASTER", NULL);
    //hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT AIRSPEED HOLD VAR", "knots");
    //hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT AIRSPEED HOLD", NULL);
    //hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT ALTITUDE LOCK", NULL);
    //hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT ALTITUDE LOCK VAR", "feet");
    //hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT ALTITUDE MANUALLY TUNABLE", NULL);
    //hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT APPROACH CAPTURED", NULL);
    //hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT FLIGHT DIRECTOR ACTIVE", NULL);
    //hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT HEADING LOCK DIR", "degrees");
    //hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT HEADING MANUALLY TUNABLE", NULL);
    //hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT VERTICAL HOLD", NULL);
    //hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT VERTICAL HOLD VAR", "feet/minute");
    //
    //
    //hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "unpaused");
    App();
    //SimConnect_Close(hSimConnect);
    return 0;
}