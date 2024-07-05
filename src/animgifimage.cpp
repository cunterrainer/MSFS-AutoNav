#include <cstring>

#include <Windows.h>
#include <strsafe.h>
#include <winerror.h>
#include <SimConnect.h>

#include <FL/Fl.H>
#include <FL/Fl_Dial.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Value_Output.H>


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
};

HANDLE  hSimConnect = NULL;
Struct1 glb;

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
    EVENT_BRAKES,
};

enum DATA_DEFINE_ID {
    DEFINITION_1,
    DEFINITION_6,
};

enum DATA_REQUEST_ID {
    REQUEST_1,
};


void CALLBACK MyDispatchProcRD(SIMCONNECT_RECV* pData, DWORD, void* updated)
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
                glb.ap_heading_lock = ps->ap_heading_lock;
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
                *((bool*)updated) = true;
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


void OnConnect(Fl_Widget* w, void*)
{
    if (strcmp(w->label(), "Connect") == 0) // we are not connected
    {
        if (FAILED(SimConnect_Open(&hSimConnect, "Auto Nav", NULL, 0, 0, 0)))
        {
            return Fl::error("Failed to connect to flight simulator"); // TODO add exlpanation code
        }


        // TODO: build wrapper function to handle errors for each
        HRESULT hr;
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_6, "Initial Position", NULL, SIMCONNECT_DATATYPE_INITPOSITION);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Title", NULL, SIMCONNECT_DATATYPE_STRING256);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Kohlsman setting hg", "inHg");
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Altitude", "feet");
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Latitude", "degrees");
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Longitude", "degrees");
        
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT MASTER", NULL);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT AIRSPEED HOLD VAR", "knots");
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT AIRSPEED HOLD", NULL);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT ALTITUDE LOCK", NULL);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT ALTITUDE LOCK VAR", "feet");
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT ALTITUDE MANUALLY TUNABLE", NULL);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT APPROACH CAPTURED", NULL);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT FLIGHT DIRECTOR ACTIVE", NULL);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT HEADING LOCK", NULL);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT HEADING LOCK DIR", "degrees");
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT HEADING MANUALLY TUNABLE", NULL);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT VERTICAL HOLD", NULL);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT VERTICAL HOLD VAR", "feet/minute");
        
        
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_BRAKES, "PARKING_BRAKES");
        hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_BRAKES);
        hr = SimConnect_SetNotificationGroupPriority(hSimConnect, GROUP0, SIMCONNECT_GROUP_PRIORITY_HIGHEST);
        
        hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "z", EVENT_BRAKES);
        hr = SimConnect_SetInputGroupState(hSimConnect, INPUT0, SIMCONNECT_STATE_ON);

        w->label("Disconnect");
        w->color(FL_GREEN);
    }
    else
    {
        if (FAILED(SimConnect_Close(hSimConnect))) // TODO: Disconnect on app close
        {
            return Fl::error("Failed to disconnect from flight simulator");
        }
        w->label("Connect");
        w->color(FL_RED);
    }
}


void SetPosition(Fl_Widget* w, void*)
{
    SIMCONNECT_DATA_INITPOSITION Init;
    Init.Altitude = 5000.0;
    Init.Latitude = 48.353834871306226;
    Init.Longitude = 11.77422621968804;
    Init.Pitch = 0.0;
    Init.Bank = -1.0;
    Init.Heading = 360;
    Init.OnGround = 0;
    Init.Airspeed = 100;
    SimConnect_SetDataOnSimObject(hSimConnect, DEFINITION_6, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(Init), &Init);
}


void OnRefresh(Fl_Widget* w, void*)
{
    HRESULT hr = SimConnect_RequestDataOnSimObjectType(hSimConnect, REQUEST_1, DEFINITION_1, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
    if (FAILED(hr))
    {
        Fl::error("Failed to request data");
    }
}


void OnAutopilot(Fl_Widget* w, void*)
{
    SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_BRAKES, 0, GROUP0, 0);
}


int main(int argc, char** argv)
{
    Fl_Double_Window* window = new Fl_Double_Window(600, 420, "Auto Nav");

    Fl_Button* o1 = new Fl_Button(10, 20, 120, 22, "Connect");
    o1->callback(OnConnect, nullptr);
    o1->color(FL_RED);

    Fl_Button* o2 = new Fl_Button(10, 50, 120, 22, "Autopilot");
    Fl_Value_Output* o3 = new Fl_Value_Output(10, 80, 122, 22, "Airspeed");
    Fl_Button* o4 = new Fl_Button(10, 110, 120, 22, "Airspeed hold");
    o3->align(Fl_Align(FL_ALIGN_RIGHT));
    o3->maximum(100000);
    o2->callback(OnAutopilot, nullptr);

    Fl_Button*  o5 = new Fl_Button(10, 140, 120, 22, "Altitude locked");
    Fl_Counter* o6 = new Fl_Counter(10, 170, 122, 22, "Altitude");
    Fl_Output*  o7 = new Fl_Output(10, 200, 122, 22, "Altitude manually adjustable");
    o7->align(Fl_Align(FL_ALIGN_RIGHT));
    o6->align(Fl_Align(FL_ALIGN_RIGHT));
    o6->minimum(0);
    o6->step(100);
    o6->lstep(1000);

    Fl_Output* o8 = new Fl_Output(10, 230, 122, 22, "Approach captured");
    Fl_Button* o9 = new Fl_Button(10, 260, 122, 22, "Flight Director");
    o8->align(Fl_Align(FL_ALIGN_RIGHT));

    Fl_Button*  o20 = new Fl_Button(10, 290, 122, 22, "Heading locked");
    Fl_Counter* o10 = new Fl_Counter(10, 320, 122, 22, "Heading");
    Fl_Output*  o11 = new Fl_Output(10, 320, 122, 22, "Heading adjustable");
    o11->align(Fl_Align(FL_ALIGN_RIGHT));
    o10->tooltip("Adjust the current heading");
    o10->minimum(0);
    o10->maximum(361);
    o10->step(1);
    o10->lstep(10);
    o10->value(360);
    o10->align(Fl_Align(FL_ALIGN_RIGHT));

    Fl_Button* o12 = new Fl_Button(10, 350, 122, 22, "Vertical speed hold");
    Fl_Counter* o13 = new Fl_Counter(10, 380, 122, 22, "Vertical speed");
    o13->align(Fl_Align(FL_ALIGN_RIGHT));
    o13->minimum(-15000);
    o13->maximum(15000);
    o13->step(100);
    o13->lstep(1000);


    Fl_Button* o14 = new Fl_Button(375, 20, 100, 22, "Test position");
    Fl_Button* o15 = new Fl_Button(375, 50, 100, 22, "Refresh @refresh");
    o14->callback(SetPosition, nullptr);
    o15->callback(OnRefresh, nullptr);

    window->resizable(window);
    window->end();
    window->show();

    bool updated = false;
    while (Fl::check())
    {
        Fl::wait(1);
        SimConnect_CallDispatch(hSimConnect, MyDispatchProcRD, &updated);
        if (updated)
        {
            // Update widgets
            o2->color(glb.ap != 0.0 ? FL_GREEN : FL_RED);
            o3->value(glb.ap_airspeed);
            o4->color(glb.ap_airspeed_hold != 0.0 ? FL_GREEN : FL_RED);
            o5->color(glb.ap_alt_lock != 0.0 ? FL_GREEN : FL_RED);
            o6->value(glb.ap_alt_lock_var);
            o7->value(glb.ap_alt_manually_adjustable != 0.0 ? "True" : "False");
            o8->value(glb.ap_approach_captured != 0.0 ? "True" : "False");
            o9->color(glb.ap_flight_director != 0.0 ? FL_GREEN : FL_RED);
            o20->color(glb.ap_heading_lock != 0.0 ? FL_GREEN : FL_RED);
            o10->value(glb.ap_heading_lock_dir);
            o11->value(glb.ap_heading_manually_adjustable != 0.0 ? "True" : "False");
            o12->color(glb.ap_vertical_hold != 0.0 ? FL_GREEN : FL_RED);
            o13->value(glb.ap_vertical_speed);

            o2->redraw();
            o4->redraw();
            o5->redraw();
            o9->redraw();
            o20->redraw();
            o12->redraw();
            updated = false;
        }
    }
}