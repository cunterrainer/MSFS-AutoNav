#pragma once
#include <FL/Fl.H>
#include <FL/Fl_RGB_Image.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Value_Output.H>
#include <FL/Fl_Double_Window.H>

#include "Icon.h"
#include "Socket.h"

class Application
{
private:
    Fl_Double_Window m_Window;

    Fl_Output  m_AirplaneTitleOut;
    Fl_Button  m_ConnectBtn;
    Fl_Button  m_AutopilotBtn;
    Fl_Counter m_AirspeedValCounter;
    Fl_Button  m_AirspeedHoldBtn;
    Fl_Button  m_AutoThrottleBtn;
    Fl_Button  m_AltitudeLockBtn;
    Fl_Counter m_AltitudeCounter;
    Fl_Output  m_AltitudeManAdjOut;
    Fl_Button  m_ApproachHoldBtn;
    Fl_Button  m_FLCBtn;
    Fl_Button  m_FlightDirectorBtn;
    Fl_Button  m_NavLockedBtn;
    Fl_Button  m_HeadingLockedBtn;
    Fl_Counter m_HeadingCounter;
    Fl_Output  m_HeadingManAdjOut;
    Fl_Button  m_VerticalSpeedHoldBtn;
    Fl_Counter m_VerticalSpeedCounter;
    Fl_Button  m_TestPositionBtn;
    Fl_Button  m_RefreshBtn;
    Fl_Button  m_WingLvlBtn;
    Fl_Button  m_YawDamperBtn;
    Fl_Choice  m_HeadingIdxChoice;

    Socket m_Socket;
    Struct1 m_Info;
private:
    void UpdateUI()
    {
        if (!m_Info.updated)
            return;

        m_AirplaneTitleOut.value(m_Info.title);
        m_AutopilotBtn.color(m_Info.ap != 0.0 ? FL_GREEN : FL_RED);
        m_AirspeedValCounter.value(m_Info.ap_airspeed);
        m_AirspeedHoldBtn.color(m_Info.ap_airspeed_hold != 0.0 ? FL_GREEN : FL_RED);
        m_AutoThrottleBtn.color(m_Info.ap_auto_throttle != 0.0 ? FL_GREEN : FL_RED);
        m_AltitudeLockBtn.color(m_Info.ap_alt_lock != 0.0 ? FL_GREEN : FL_RED);
        m_AltitudeCounter.value(m_Info.ap_alt_lock_var);
        m_AltitudeManAdjOut.value(m_Info.ap_alt_manually_adjustable != 0.0 ? "True" : "False");
        m_ApproachHoldBtn.color(m_Info.ap_approach_hold != 0.0 ? FL_GREEN : FL_RED);
        m_FLCBtn.color(m_Info.ap_flight_level_change != 0.0 ? FL_GREEN : FL_RED);
        m_FlightDirectorBtn.color(m_Info.ap_flight_director != 0.0 ? FL_GREEN : FL_RED);
        m_NavLockedBtn.color(m_Info.ap_nav_lock != 0.0 ? FL_GREEN : FL_RED);
        m_HeadingLockedBtn.color(m_Info.ap_heading_lock != 0.0 ? FL_GREEN : FL_RED);
        m_HeadingCounter.value(m_Info.ap_heading_lock_dir);
        m_HeadingManAdjOut.value(m_Info.ap_heading_manually_adjustable != 0.0 ? "True" : "False");
        m_VerticalSpeedHoldBtn.color(m_Info.ap_vertical_hold != 0.0 ? FL_GREEN : FL_RED);
        m_VerticalSpeedCounter.value(m_Info.ap_vertical_speed);
        m_WingLvlBtn.color(m_Info.ap_wing_lvl != 0.0 ? FL_GREEN : FL_RED);
        m_YawDamperBtn.color(m_Info.ap_yaw_damper != 0.0 ? FL_GREEN : FL_RED);
        m_HeadingIdxChoice.value((int)m_Info.ap_heading_idx - 1);

        m_AutopilotBtn.redraw();
        m_AirspeedHoldBtn.redraw();
        m_AutoThrottleBtn.redraw();
        m_AltitudeLockBtn.redraw();
        m_ApproachHoldBtn.redraw();
        m_FLCBtn.redraw();
        m_FlightDirectorBtn.redraw();
        m_NavLockedBtn.redraw();
        m_HeadingLockedBtn.redraw();
        m_VerticalSpeedHoldBtn.redraw();
        m_WingLvlBtn.redraw();
        m_YawDamperBtn.redraw();
        m_Info.updated = false;
    }


    void ResetColors()
    {
        m_Info.title[0] = '\0';
        m_AirplaneTitleOut.value(m_Info.title);
        m_Info.ap_airspeed = 0;
        m_Info.ap_auto_throttle = 0;
        m_Info.ap_alt_lock_var = 0;
        m_Info.ap_heading_lock_dir = 0;
        m_Info.ap_vertical_speed = 0;
        m_Info.ap_alt_manually_adjustable = 0;
        m_Info.ap_heading_manually_adjustable = 0;
        m_HeadingIdxChoice.value(-1);

        m_AutopilotBtn.color(FL_BACKGROUND_COLOR);
        m_AirspeedHoldBtn.color(FL_BACKGROUND_COLOR);
        m_AutoThrottleBtn.color(FL_BACKGROUND_COLOR);
        m_AltitudeLockBtn.color(FL_BACKGROUND_COLOR);
        m_ApproachHoldBtn.color(FL_BACKGROUND_COLOR);
        m_FLCBtn.color(FL_BACKGROUND_COLOR);
        m_FlightDirectorBtn.color(FL_BACKGROUND_COLOR);
        m_NavLockedBtn.color(FL_BACKGROUND_COLOR);
        m_HeadingLockedBtn.color(FL_BACKGROUND_COLOR);
        m_VerticalSpeedHoldBtn.color(FL_BACKGROUND_COLOR);
        m_TestPositionBtn.color(FL_BACKGROUND_COLOR);
        m_RefreshBtn.color(FL_BACKGROUND_COLOR);
        m_WingLvlBtn.color(FL_BACKGROUND_COLOR);
        m_YawDamperBtn.color(FL_BACKGROUND_COLOR);


        m_AirspeedValCounter.value(m_Info.ap_airspeed);
        m_AltitudeCounter.value(m_Info.ap_alt_lock_var);
        m_VerticalSpeedCounter.value(m_Info.ap_vertical_speed);
        m_HeadingCounter.value(m_Info.ap_heading_lock_dir);


        m_AltitudeManAdjOut.value("");
        m_HeadingManAdjOut.value("");

        m_ConnectBtn.redraw();
        m_AutopilotBtn.redraw();
        m_AirspeedHoldBtn.redraw();
        m_AutoThrottleBtn.redraw();
        m_AltitudeLockBtn.redraw();
        m_ApproachHoldBtn.redraw();
        m_FLCBtn.redraw();
        m_FlightDirectorBtn.redraw();
        m_NavLockedBtn.redraw();
        m_HeadingLockedBtn.redraw();
        m_VerticalSpeedHoldBtn.redraw();
        m_RefreshBtn.redraw();
        m_WingLvlBtn.redraw();
        m_YawDamperBtn.redraw();
    }
private:
    static inline void OnConnectClicked(Fl_Widget*, void* a)
    {
        Application* app = (Application*)a;
        app->Connect();
    }

    static inline void OnRefreshClicked(Fl_Widget*, void* a)
    {
        Application* app = (Application*)a;
        app->Refresh();
    }

    static inline void OnHeadingCounterChanged(Fl_Widget* w, void* a)
    {
        Fl_Counter* c = (Fl_Counter*)w;
        const int value = static_cast<int>(c->value());
        if (value == -1)
            c->value(359);
        else if (value == 360)
            c->value(0);

        Application* app = (Application*)a;
        app->SetHeading();
    }

    static inline void OnAltitudeCounterChanged(Fl_Widget*, void* a)
    {
        Application* app = (Application*)a;
        app->SetAltitude();
    }

    static inline void OnVerticalSpeedChanged(Fl_Widget*, void* a)
    {
        Application* app = (Application*)a;
        app->SetVerticalSpeed();
    }

    static inline void OnAirSpeedChanged(Fl_Widget*, void* a)
    {
        Application* app = (Application*)a;
        app->SetAirSpeed();
    }

    static inline void OnSetPositionClicked(Fl_Widget*, void* a)
    {
        Application* app = (Application*)a;
        app->SetPosition();
    }

    static inline void OnAutopilotClicked(Fl_Widget*, void* a)
    {
        Application* app = (Application*)a;
        app->ToggleAutopilot();
    }

    static inline void OnAutoThrottleClicked(Fl_Widget*, void* a)
    {
        Application* app = (Application*)a;
        app->ToggleAutoThrottle();
    }

    static inline void OnAltHoldClicked(Fl_Widget*, void* a)
    {
        Application* app = (Application*)a;
        app->ToggleAltHold();
    }

    static inline void OnAirspeedHoldClicked(Fl_Widget*, void* a)
    {
        Application* app = (Application*)a;
        app->ToggleAirspeedHold();
    }

    static inline void OnApproachHoldClicked(Fl_Widget*, void* a)
    {
        Application* app = (Application*)a;
        app->ToggleApproachHold();
    }

    static inline void OnFLCClicked(Fl_Widget*, void* a)
    {
        Application* app = (Application*)a;
        app->ToggleFLC();
    }

    static inline void OnFlightDirectorClicked(Fl_Widget*, void* a)
    {
        Application* app = (Application*)a;
        app->ToggleFlightDirector();
    }

    static inline void OnNavLockedClicked(Fl_Widget*, void* a)
    {
        Application* app = (Application*)a;
        app->ToggleNavLocked();
    }

    static inline void OnHeadingLockedClicked(Fl_Widget*, void* a)
    {
        Application* app = (Application*)a;
        app->ToggleHeadingLocked();
    }
    
    static inline void OnVerticalSpeedHoldClicked(Fl_Widget*, void* a)
    {
        Application* app = (Application*)a;
        app->ToggleVerticalSpeedHold();
    }

    static inline void OnWingLvlClicked(Fl_Widget*, void* a)
    {
        Application* app = (Application*)a;
        app->ToggleWingLvl();
    }

    static inline void OnYawDamperClicked(Fl_Widget*, void* a)
    {
        Application* app = (Application*)a;
        app->ToggleYawDamper();
    }

    static inline void OnHeadingIdxChosen(Fl_Widget*, void* a)
    {
        Application* app = (Application*)a;
        app->SetHeadingIndex();
    }
public:
    Application() :
        m_Window(670, 650, "Auto Nav"),
        m_AirplaneTitleOut(10, 10, 500, 22, "AirplaneTitle"),
        m_ConnectBtn(10, 40, 120, 22, "Connect"),
        m_AutopilotBtn(10, 70, 120, 22, "Autopilot"),
        m_AirspeedValCounter(10, 100, 122, 22, "Airspeed"),
        m_AirspeedHoldBtn(10, 130, 120, 22, "Airspeed hold"),
        m_AutoThrottleBtn(10, 160, 120, 22, "Auto throttle"),
        m_AltitudeLockBtn(10, 190, 120, 22, "Altitude locked"),
        m_AltitudeCounter(10, 220, 122, 22, "Altitude"),
        m_AltitudeManAdjOut(10, 250, 122, 22, "Altitude manually adjustable"),
        m_ApproachHoldBtn(10, 280, 122, 22, "Approach hold"),
        m_FLCBtn(10, 310, 122, 22, "FLC"),
        m_FlightDirectorBtn(10, 340, 122, 22, "Flight Director"),
        m_NavLockedBtn(10, 370, 122, 22, "Nav"),
        m_HeadingLockedBtn(10, 400, 122, 22, "Heading locked"),
        m_HeadingCounter(10, 430, 122, 22, "Heading"),
        m_HeadingManAdjOut(10, 460, 122, 22, "Heading adjustable"),
        m_VerticalSpeedHoldBtn(10, 490, 122, 22, "Vertical speed hold"),
        m_VerticalSpeedCounter(10, 520, 122, 22, "Vertical speed"),
        m_TestPositionBtn(375, 40, 100, 22, "Test position"),
        m_RefreshBtn(375, 70, 100, 22, "Refresh @refresh"),
        m_WingLvlBtn(10, 550, 122, 22, "Wing Level"),
        m_YawDamperBtn(10, 580, 122, 22, "Yaw Damper"),
        m_HeadingIdxChoice(10, 610, 122, 22, "Heading index")
    {
        Fl_RGB_Image i(sg_Icon_Data, sg_Icon_Width, sg_Icon_Height, sg_Icon_Channel, 0);
        m_Window.icon(&i);

        m_AirplaneTitleOut.box(FL_FLAT_BOX);
        m_AirplaneTitleOut.color(FL_BACKGROUND_COLOR);
        m_AirplaneTitleOut.labeltype(FL_NO_LABEL);
        m_AirplaneTitleOut.visible_focus(false);

        m_ConnectBtn.color(FL_RED);
        m_ConnectBtn.callback(OnConnectClicked, this);
        m_AutopilotBtn.callback(OnAutopilotClicked, this);

        m_AirspeedValCounter.minimum(0);
        m_AirspeedValCounter.step(1);
        m_AirspeedValCounter.lstep(10);
        m_AirspeedValCounter.align(Fl_Align(FL_ALIGN_RIGHT));
        m_AirspeedValCounter.callback(OnAirSpeedChanged, this);
        m_AirspeedHoldBtn.callback(OnAirspeedHoldClicked, this);
        m_AutoThrottleBtn.callback(OnAutoThrottleClicked, this);

        m_AltitudeLockBtn.callback(OnAltHoldClicked, this);
        m_AltitudeCounter.minimum(-3000);
        m_AltitudeCounter.step(100);
        m_AltitudeCounter.lstep(1000);
        m_AltitudeCounter.align(Fl_Align(FL_ALIGN_RIGHT));
        m_AltitudeCounter.callback(OnAltitudeCounterChanged, this);

        m_AltitudeManAdjOut.align(Fl_Align(FL_ALIGN_RIGHT));
        m_AltitudeManAdjOut.cursor_color(FL_BACKGROUND2_COLOR);

        m_HeadingCounter.tooltip("Adjust the current heading");
        m_HeadingCounter.range(-1, 360);
        m_HeadingCounter.step(1);
        m_HeadingCounter.lstep(10);
        m_HeadingCounter.value(0);
        m_HeadingCounter.align(Fl_Align(FL_ALIGN_RIGHT));
        m_HeadingCounter.callback(OnHeadingCounterChanged, this);
        m_HeadingManAdjOut.align(Fl_Align(FL_ALIGN_RIGHT));
        m_HeadingManAdjOut.cursor_color(FL_BACKGROUND2_COLOR);
        m_HeadingLockedBtn.callback(OnHeadingLockedClicked, this);

        m_VerticalSpeedCounter.align(Fl_Align(FL_ALIGN_RIGHT));
        m_VerticalSpeedCounter.range(-15000, 15000);
        m_VerticalSpeedCounter.step(100);
        m_VerticalSpeedCounter.lstep(1000);
        m_VerticalSpeedCounter.callback(OnVerticalSpeedChanged, this);
        m_FLCBtn.callback(OnFLCClicked, this);
        m_ApproachHoldBtn.callback(OnApproachHoldClicked, this);
        m_FlightDirectorBtn.callback(OnFlightDirectorClicked, this);
        m_NavLockedBtn.callback(OnNavLockedClicked, this);
        m_VerticalSpeedHoldBtn.callback(OnVerticalSpeedHoldClicked, this);

        m_WingLvlBtn.callback(OnWingLvlClicked, this);
        m_YawDamperBtn.callback(OnYawDamperClicked, this);

        m_HeadingIdxChoice.add("1");
        m_HeadingIdxChoice.add("2");
        m_HeadingIdxChoice.add("3");
        m_HeadingIdxChoice.align(Fl_Align(FL_ALIGN_RIGHT));
        m_HeadingIdxChoice.callback(OnHeadingIdxChosen, this);

        m_RefreshBtn.callback(OnRefreshClicked, this);
        m_TestPositionBtn.callback(OnSetPositionClicked, this);

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
            Refresh();
        }
        else
        {

            m_ConnectBtn.color(FL_RED);
            m_ConnectBtn.label("Connect");
            ResetColors();
            if (m_Info.quit)
            {
                Fl::error("Quit microsoft flight simulator, lost connection");
            }
        }
    }


    void Refresh() noexcept
    {
        m_Socket.Dispatch(&m_Info);
    }


    inline void ToggleAutopilot() const noexcept
    {
        m_Socket.TransmitEvent(EVENT_AUTOPILOT);
    }


    inline void ToggleAltHold() const noexcept
    {
        m_Socket.TransmitEvent(EVENT_ALTITUDE_HOLD);
    }


    inline void ToggleAirspeedHold() const noexcept
    {
        m_Socket.TransmitEvent(EVENT_AIRSPEED_HOLD);
    }

    inline void ToggleAutoThrottle() const noexcept
    {
        m_Socket.TransmitEvent(EVENT_AUTO_THROTTLE_ARM);
    }

    inline void ToggleApproachHold() const noexcept
    {
        m_Socket.TransmitEvent(EVENT_APPROACH_HOLD);
    }


    inline void ToggleFLC() const noexcept
    {
        m_Socket.TransmitEvent(EVENT_FLC_HOLD);
    }


    inline void ToggleFlightDirector() const noexcept
    {
        m_Socket.TransmitEvent(EVENT_FLIGHT_DIRECTOR);
    }


    inline void ToggleNavLocked() const noexcept
    {
        m_Socket.TransmitEvent(EVENT_NAV_LOCKED);
    }


    inline void ToggleHeadingLocked() const noexcept
    {
        m_Socket.TransmitEvent(EVENT_HEADING_LOCKED);
    }


    inline void ToggleVerticalSpeedHold() const noexcept
    {
        m_Socket.TransmitEvent(EVENT_VERTICAL_SPEED_HOLD);
    }


    inline void ToggleWingLvl() const noexcept
    {
        m_Socket.TransmitEvent(EVENT_WING_LVL_HOLD);
    }


    inline void ToggleYawDamper() const noexcept
    {
        m_Socket.TransmitEvent(EVENT_YAW_DAMPER_HOLD);
    }


    inline void SetHeading() const noexcept
    {
        m_Socket.TransmitEvent(EVENT_SET_HEADING, static_cast<DWORD>(m_HeadingCounter.value()));
    }


    inline void SetAltitude() const noexcept
    {
        m_Socket.TransmitEvent(EVENT_SET_ALTITUDE, static_cast<DWORD>(m_AltitudeCounter.value()));
    }


    inline void SetAirSpeed() const noexcept
    {
        m_Socket.TransmitEvent(EVENT_SET_AIRSPEED, static_cast<DWORD>(m_AirspeedValCounter.value()));
    }


    inline void SetHeadingIndex() const noexcept
    {
        m_Socket.TransmitEvent(EVENT_SET_HEADING_IDX, static_cast<DWORD>(m_HeadingIdxChoice.value() + 1));
    }


    inline void SetVerticalSpeed() const noexcept
    {
        if (m_VerticalSpeedHoldBtn.color() == FL_RED)
        {
            ToggleVerticalSpeedHold();
        }

        m_Socket.TransmitEvent(EVENT_SET_VERTICAL_SPEED, static_cast<DWORD>(m_VerticalSpeedCounter.value()));
    }


    void SetPosition() const noexcept
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
        Fl::lock();
        while (m_Socket.ShouldUpdate || Fl::wait())
        {
            m_Socket.ShouldUpdate = false;
            if (m_Socket.IsConnected())
            {
                m_Socket.Dispatch(&m_Info);
                if (m_Info.quit)
                {
                    Connect();
                }
                UpdateUI();
            }
        }
    }
};