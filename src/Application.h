#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Value_Output.H>
#include <FL/Fl_Double_Window.H>

#include "Socket.h"

class Application
{
private:
    Fl_Double_Window m_Window;

    Fl_Button  m_ConnectBtn;
    Fl_Button  m_AutopilotBtn;
    Fl_Counter m_AirspeedValCounter;
    Fl_Button  m_AirspeedHoldBtn;
    Fl_Button  m_AltitudeLockBtn;
    Fl_Counter m_AltitudeCounter;
    Fl_Output  m_AltitudeManAdjOut;
    Fl_Output  m_ApproachCapturedOut;
    Fl_Button  m_FlightDirectorBtn;
    Fl_Button  m_HeadingLockedBtn;
    Fl_Counter m_HeadingCounter;
    Fl_Output  m_HeadingManAdjOut;
    Fl_Button  m_VerticalSpeedHoldBtn;
    Fl_Counter m_VerticalSpeedCounter;
    Fl_Button  m_TestPositionBtn;
    Fl_Button  m_RefreshBtn;

    Socket m_Socket;
    Struct1 m_Info;
private:
    void UpdateUI()
    {
        if (!m_Info.updated)
            return;

        m_AutopilotBtn.color(m_Info.ap != 0.0 ? FL_GREEN : FL_RED);
        m_AirspeedValCounter.value(m_Info.ap_airspeed);
        m_AirspeedHoldBtn.color(m_Info.ap_airspeed_hold != 0.0 ? FL_GREEN : FL_RED);
        m_AltitudeLockBtn.color(m_Info.ap_alt_lock != 0.0 ? FL_GREEN : FL_RED);
        m_AltitudeCounter.value(m_Info.ap_alt_lock_var);
        m_AltitudeManAdjOut.value(m_Info.ap_alt_manually_adjustable != 0.0 ? "True" : "False");
        m_ApproachCapturedOut.value(m_Info.ap_approach_captured != 0.0 ? "True" : "False");
        m_FlightDirectorBtn.color(m_Info.ap_flight_director != 0.0 ? FL_GREEN : FL_RED);
        m_HeadingLockedBtn.color(m_Info.ap_heading_lock != 0.0 ? FL_GREEN : FL_RED);
        m_HeadingCounter.value(m_Info.ap_heading_lock_dir);
        m_HeadingManAdjOut.value(m_Info.ap_heading_manually_adjustable != 0.0 ? "True" : "False");
        m_VerticalSpeedHoldBtn.color(m_Info.ap_vertical_hold != 0.0 ? FL_GREEN : FL_RED);
        m_VerticalSpeedCounter.value(m_Info.ap_vertical_speed);

        m_AutopilotBtn.redraw();
        m_AirspeedHoldBtn.redraw();
        m_AltitudeLockBtn.redraw();
        m_FlightDirectorBtn.redraw();
        m_HeadingLockedBtn.redraw();
        m_VerticalSpeedHoldBtn.redraw();
        m_Info.updated = false;
    }
private:
    static inline void OnConnect(Fl_Widget* w, void* a)
    {
        Application* app = (Application*)a;
        app->Connect();
    }


    static inline void OnRefresh(Fl_Widget* w, void* a)
    {
        Application* app = (Application*)a;
        app->Refresh();
    }


    static inline void OnAutopilot(Fl_Widget* w, void* a)
    {
        Application* app = (Application*)a;
        app->AutoPilot();
    }

    static inline void OnSetPosition(Fl_Widget* w, void* a)
    {
        Application* app = (Application*)a;
        app->SetPosition();
    }
public:
    Application() :
        m_Window(600, 450, "Auto Nav"),
        m_ConnectBtn(10, 20, 120, 22, "Connect"),
        m_AutopilotBtn(10, 50, 120, 22, "Autopilot"),
        m_AirspeedValCounter(10, 80, 122, 22, "Airspeed"),
        m_AirspeedHoldBtn(10, 110, 120, 22, "Airspeed hold"),
        m_AltitudeLockBtn(10, 140, 120, 22, "Altitude locked"),
        m_AltitudeCounter(10, 170, 122, 22, "Altitude"),
        m_AltitudeManAdjOut(10, 200, 122, 22, "Altitude manually adjustable"),
        m_ApproachCapturedOut(10, 230, 122, 22, "Approach captured"),
        m_FlightDirectorBtn(10, 260, 122, 22, "Flight Director"),
        m_HeadingLockedBtn(10, 290, 122, 22, "Heading locked"),
        m_HeadingCounter(10, 320, 122, 22, "Heading"),
        m_HeadingManAdjOut(10, 350, 122, 22, "Heading adjustable"),
        m_VerticalSpeedHoldBtn(10, 380, 122, 22, "Vertical speed hold"),
        m_VerticalSpeedCounter(10, 410, 122, 22, "Vertical speed"),
        m_TestPositionBtn(375, 20, 100, 22, "Test position"),
        m_RefreshBtn(375, 50, 100, 22, "Refresh @refresh")
    {
        m_ConnectBtn.color(FL_RED);
        m_ConnectBtn.callback(OnConnect, this);
        m_AutopilotBtn.callback(OnAutopilot, this);

        m_AirspeedValCounter.minimum(0);
        m_AirspeedValCounter.step(1);
        m_AirspeedValCounter.lstep(10);
        m_AirspeedValCounter.align(Fl_Align(FL_ALIGN_RIGHT));

        m_AltitudeCounter.minimum(-3000);
        m_AltitudeCounter.step(100);
        m_AltitudeCounter.lstep(1000);
        m_AltitudeCounter.align(Fl_Align(FL_ALIGN_RIGHT));

        m_AltitudeManAdjOut.align(Fl_Align(FL_ALIGN_RIGHT));
        m_ApproachCapturedOut.align(Fl_Align(FL_ALIGN_RIGHT));

        m_HeadingCounter.tooltip("Adjust the current heading");
        m_HeadingCounter.minimum(0);
        m_HeadingCounter.maximum(361);
        m_HeadingCounter.step(1);
        m_HeadingCounter.lstep(10);
        m_HeadingCounter.value(360);
        m_HeadingCounter.align(Fl_Align(FL_ALIGN_RIGHT));
        m_HeadingManAdjOut.align(Fl_Align(FL_ALIGN_RIGHT));

        m_VerticalSpeedCounter.align(Fl_Align(FL_ALIGN_RIGHT));
        m_VerticalSpeedCounter.minimum(-15000);
        m_VerticalSpeedCounter.maximum(15000);
        m_VerticalSpeedCounter.step(100);
        m_VerticalSpeedCounter.lstep(1000);

        m_RefreshBtn.callback(OnRefresh, this);
        m_TestPositionBtn.callback(OnSetPosition, this);

        m_Window.resizable(m_Window);
        m_Window.end();
        m_Window.show();
    }


    ~Application()
    {
        Connect(true);
    }


    inline void Connect(bool shutdown = false)
    {
        const char* error = m_Socket.ToggleConnection(shutdown);
        if (error)
            Fl::error(error);

        if (m_Socket.IsConnected())
        {
            m_ConnectBtn.color(FL_GREEN);
            m_ConnectBtn.label("Disconnect");
        }
        else
        {

            m_ConnectBtn.color(FL_RED);
            m_ConnectBtn.label("Connect");
        }
    }


    void Refresh() const noexcept
    {
        if (!m_Socket.FetchData())
        {
            return Fl::error("Failed to request data");
        }
    }


    void AutoPilot() const noexcept
    {
        m_Socket.TransmitEvent();
    }


    void SetPosition()
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
        SimConnect_SetDataOnSimObject(m_Socket.Handle(), DEFINITION_6, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(Init), &Init);
    }


    inline void Show() noexcept
    {
        while (Fl::wait())
        {
            m_Socket.Dispatch(&m_Info);
            UpdateUI();
        }
    }
};