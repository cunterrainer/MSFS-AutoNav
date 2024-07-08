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
    Fl_Double_Window m_Window = Fl_Double_Window(600, 520, "Auto Nav");

    Fl_Output  m_AirplaneTitleOut     = Fl_Output (  0,  10, 600, 22, "AirplaneTitle");
    Fl_Button  m_ConnectBtn           = Fl_Button ( 20,  58, 115, 70, "Connect");
    Fl_Button  m_AutopilotBtn         = Fl_Button (168,  58, 115, 70, "AP");
    Fl_Counter m_AirspeedValCounter   = Fl_Counter( 20, 164, 115, 25, "Airspeed");
    Fl_Button  m_AirspeedHoldBtn      = Fl_Button ( 20, 211, 115, 30, "SPD");
    Fl_Button  m_AutoThrottleBtn      = Fl_Button ( 20, 251, 115, 30, "A/THR");
    Fl_Button  m_AltitudeLockBtn      = Fl_Button (316, 211, 115, 70, "ALT");
    Fl_Counter m_AltitudeCounter      = Fl_Counter(316, 164, 115, 25, "Altitude");
    Fl_Button  m_ApproachHoldBtn      = Fl_Button (316, 301, 115, 70, "APPR");
    Fl_Button  m_FLCBtn               = Fl_Button (465,  58, 115, 70, "FLC");
    Fl_Button  m_FlightDirectorBtn    = Fl_Button (316,  58, 115, 70, "FD");
    Fl_Button  m_NavLockedBtn         = Fl_Button (170, 301, 115, 70, "NAV");
    Fl_Button  m_HeadingLockedBtn     = Fl_Button (168, 251, 115, 30, "HDG");
    Fl_Counter m_HeadingCounter       = Fl_Counter(168, 164, 115, 25, "Heading");
    Fl_Button  m_VerticalSpeedHoldBtn = Fl_Button (465, 211, 115, 70, "V/S");
    Fl_Counter m_VerticalSpeedCounter = Fl_Counter(465, 164, 115, 25, "Vertical speed");
    Fl_Button  m_TestPositionBtn      = Fl_Button (316, 474, 115, 25, "Test position");
    Fl_Button  m_RefreshBtn           = Fl_Button (465, 474, 115, 25, "Refresh @refresh");
    Fl_Button  m_WingLvlBtn           = Fl_Button (465, 301, 115, 70, "Wing LVL");
    Fl_Button  m_YawDamperBtn         = Fl_Button ( 22, 301, 115, 70, "Yaw Damper");
    Fl_Choice  m_HeadingSlotChoice    = Fl_Choice (168, 217, 115, 22, "Heading slot");

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
        m_ApproachHoldBtn.color(m_Info.ap_approach_hold != 0.0 ? FL_GREEN : FL_RED);
        m_FLCBtn.color(m_Info.ap_flight_level_change != 0.0 ? FL_GREEN : FL_RED);
        m_FlightDirectorBtn.color(m_Info.ap_flight_director != 0.0 ? FL_GREEN : FL_RED);
        m_NavLockedBtn.color(m_Info.ap_nav_lock != 0.0 ? FL_GREEN : FL_RED);
        m_HeadingLockedBtn.color(m_Info.ap_heading_lock != 0.0 ? FL_GREEN : FL_RED);
        m_HeadingCounter.value(m_Info.ap_heading_lock_dir);
        m_VerticalSpeedHoldBtn.color(m_Info.ap_vertical_hold != 0.0 ? FL_GREEN : FL_RED);
        m_VerticalSpeedCounter.value(m_Info.ap_vertical_speed);
        m_WingLvlBtn.color(m_Info.ap_wing_lvl != 0.0 ? FL_GREEN : FL_RED);
        m_YawDamperBtn.color(m_Info.ap_yaw_damper != 0.0 ? FL_GREEN : FL_RED);
        m_HeadingSlotChoice.value((int)m_Info.ap_heading_idx - 1);

        if (m_Info.ap_heading_manually_adjustable == 0.0)
            m_HeadingCounter.tooltip("Set heading (not adjustable for this aircraft)");

        if (m_Info.ap_alt_manually_adjustable == 0.0)
            m_HeadingCounter.tooltip("Set altitude (not adjustable for this aircraft)");

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


    void ResetUI()
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
        m_HeadingSlotChoice.value(-1);

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

        m_HeadingCounter.tooltip("Set heading");
        m_AltitudeCounter.tooltip("Set altitude");

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
    Application()
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
        m_AirspeedValCounter.align(Fl_Align(FL_ALIGN_TOP));
        m_AirspeedValCounter.callback(OnAirSpeedChanged, this);
        m_AirspeedHoldBtn.callback(OnAirspeedHoldClicked, this);
        m_AutoThrottleBtn.callback(OnAutoThrottleClicked, this);

        m_AltitudeLockBtn.callback(OnAltHoldClicked, this);
        m_AltitudeCounter.minimum(-3000);
        m_AltitudeCounter.step(100);
        m_AltitudeCounter.lstep(1000);
        m_AltitudeCounter.align(Fl_Align(FL_ALIGN_TOP));
        m_AltitudeCounter.callback(OnAltitudeCounterChanged, this);

        m_HeadingCounter.tooltip("Adjust the current heading");
        m_HeadingCounter.range(-1, 360);
        m_HeadingCounter.step(1);
        m_HeadingCounter.lstep(10);
        m_HeadingCounter.value(0);
        m_HeadingCounter.align(Fl_Align(FL_ALIGN_TOP));
        m_HeadingCounter.callback(OnHeadingCounterChanged, this);
        m_HeadingLockedBtn.callback(OnHeadingLockedClicked, this);

        m_VerticalSpeedCounter.align(Fl_Align(FL_ALIGN_TOP));
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

        m_HeadingSlotChoice.add("1");
        m_HeadingSlotChoice.add("2");
        m_HeadingSlotChoice.add("3");
        m_HeadingSlotChoice.align(Fl_Align(FL_ALIGN_TOP));
        m_HeadingSlotChoice.callback(OnHeadingIdxChosen, this);

        m_RefreshBtn.callback(OnRefreshClicked, this);
        m_TestPositionBtn.callback(OnSetPositionClicked, this);

        m_ConnectBtn.tooltip("Connect to MSFS");
        m_AutopilotBtn.tooltip("Toggle Autopilot");
        m_AirspeedValCounter.tooltip("Set airspeed");
        m_AirspeedHoldBtn.tooltip("Toggle airspeed hold mode");
        m_AutoThrottleBtn.tooltip("Toggle autothrottle");
        m_AltitudeCounter.tooltip("Set altitude");
        m_AltitudeLockBtn.tooltip("Toggle hold altitude");
        m_ApproachHoldBtn.tooltip("Toggle approach mode");
        m_FLCBtn.tooltip("Toggle FLC (Flight level control)");
        m_FlightDirectorBtn.tooltip("Toggle flight director");
        m_NavLockedBtn.tooltip("Toggle nav mode (following gps)");
        m_HeadingCounter.tooltip("Set heading");
        m_HeadingLockedBtn.tooltip("Toggle heading (following heading)");
        m_VerticalSpeedCounter.tooltip("Set vertical speed");
        m_VerticalSpeedHoldBtn.tooltip("Toggle vertical speed");
        m_TestPositionBtn.tooltip("Teleport to a test position");
        m_RefreshBtn.tooltip("Refresh data from MSFS if out of sync");
        m_WingLvlBtn.tooltip("Toggle wing level mode");
        m_YawDamperBtn.tooltip("Toggle yam damper");
        m_HeadingSlotChoice.tooltip("Select heading slot (Try if NAV mode doesn't follow e.g. a320)");

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
        m_Socket.TransmitEvent(EVENT_SET_HEADING_IDX, static_cast<DWORD>(m_HeadingSlotChoice.value() + 1));
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