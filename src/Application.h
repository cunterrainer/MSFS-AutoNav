#pragma once
#include <string>
#include <format>

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_RGB_Image.H>
#include <FL/Fl_Value_Output.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Double_Window.H>

#include "Icon.h"
#include "Theme.h"
#include "Socket.h"
#include "MapWidget.h"
#include "CenteredOutput.h"

class Application
{
private:
    Fl_Double_Window m_Window = Fl_Double_Window(600, 800, "Auto Nav");
    MapWidget m_MapWidget = MapWidget(20, 510, 560, 280, "MapWidget");

    CenteredOutput m_AirplaneTitleOut = CenteredOutput(0, 14, 600, 30, "AirplaneTitle");
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
    Fl_Button  m_WingLvlBtn           = Fl_Button (465, 301, 115, 70, "Wing LVL");
    Fl_Button  m_YawDamperBtn         = Fl_Button ( 22, 301, 115, 70, "Yaw Damper");
    Fl_Choice  m_HeadingSlotChoice    = Fl_Choice (168, 217, 115, 22, "Heading slot");
    Fl_Button  m_TestPositionBtn      = Fl_Button (316, 474, 115, 25, "Test position");
    Fl_Button  m_RefreshBtn           = Fl_Button (465, 474, 115, 25, "Refresh @refresh");
    Fl_Output  m_PlanePosOut          = Fl_Output ( 20, 424, 300, 25, "PlanePosition");
    Fl_Output  m_PlaneAltSpeedOut     = Fl_Output ( 20, 449, 280, 25, "PlaneAirSpeed");
    Fl_Check_Button m_PlaneAltSpCheck = Fl_Check_Button(20, 474, 150, 25, "Track plane position");

    Socket m_Socket;
    Struct1 m_Info;
private:
    void SetButtonColor(Fl_Button& btn, double value) const noexcept
    {
        if (value == 0.0)
        {
            btn.color(FL_RED);
            btn.labelcolor(FL_FOREGROUND_COLOR);
        }
        else
        {
            btn.color(FL_GREEN);
            btn.labelcolor(FL_GRAY0);
        }
    }


    void UpdateUI()
    {
        if (m_Info.update_pos)
        {
            const std::string planePos = std::format("Latitude: {:.6f}, Longitude: {:.6f}", m_Info.pos_latitude, m_Info.pos_longitude);
            const std::string planeAltSpeed = std::format("Altitude: {:.0f} feet, Airspeed: {:.2f} Mach", m_Info.pos_altitude, m_Info.pos_airspeed);
            m_PlanePosOut.value(planePos.c_str());
            m_PlaneAltSpeedOut.value(planeAltSpeed.c_str());
            m_PlanePosOut.activate();
            m_PlaneAltSpeedOut.activate();

            m_MapWidget.SetCoords(m_Info.pos_latitude, m_Info.pos_longitude);
            m_MapWidget.SetPlaneTitle(m_Info.title);
            m_Info.update_pos = false;
        }

        if (!m_Info.update_ap)
        {
            return;
        }

        // update ap
        if (m_Info.ap_available == 0.0)
        {
            m_AutopilotBtn.deactivate();
            m_AutopilotBtn.tooltip("No autopilot available");
            m_AutopilotBtn.color(FL_BACKGROUND_COLOR);
        }
        else
        {
            m_AutopilotBtn.activate();
            m_AutopilotBtn.tooltip("Toggle Autopilot");
            m_AutopilotBtn.color(m_Info.ap != 0.0 ? FL_GREEN : FL_RED);
        }

        SetButtonColor(m_AutopilotBtn, m_Info.ap);
        SetButtonColor(m_AirspeedHoldBtn, m_Info.ap_airspeed_hold);
        SetButtonColor(m_AutoThrottleBtn, m_Info.ap_auto_throttle);
        SetButtonColor(m_AltitudeLockBtn, m_Info.ap_alt_lock);
        SetButtonColor(m_ApproachHoldBtn, m_Info.ap_approach_hold);
        SetButtonColor(m_FLCBtn, m_Info.ap_flight_level_change);
        SetButtonColor(m_FlightDirectorBtn, m_Info.ap_flight_director);
        SetButtonColor(m_NavLockedBtn, m_Info.ap_nav_lock);
        SetButtonColor(m_HeadingLockedBtn, m_Info.ap_heading_lock);
        SetButtonColor(m_VerticalSpeedHoldBtn, m_Info.ap_vertical_hold);
        SetButtonColor(m_WingLvlBtn, m_Info.ap_wing_lvl);
        SetButtonColor(m_YawDamperBtn, m_Info.ap_yaw_damper);

        m_AirplaneTitleOut.activate();
        m_AirplaneTitleOut.value(m_Info.title);
        m_AirspeedValCounter.value(m_Info.ap_airspeed);
        m_AltitudeCounter.value(m_Info.ap_alt_lock_var);
        m_FLCBtn.color(m_Info.ap_flight_level_change != 0.0 ? FL_GREEN : FL_RED);
        m_HeadingCounter.value(m_Info.ap_heading_lock_dir);
        m_VerticalSpeedCounter.value(m_Info.ap_vertical_speed);
        m_HeadingSlotChoice.value((int)m_Info.ap_heading_idx - 1);

        if (m_Info.ap_heading_manually_adjustable == 0.0)
            m_HeadingCounter.tooltip("Set heading (not adjustable for this aircraft)");

        if (m_Info.ap_alt_manually_adjustable == 0.0)
            m_HeadingCounter.tooltip("Set altitude (not adjustable for this aircraft)");

        m_YawDamperBtn.redraw();
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
        m_Info.update_ap = false;
    }


    void ResetUI()
    {
        m_Info.title[0] = '\0';
        m_AirplaneTitleOut.deactivate();
        m_AirplaneTitleOut.value(m_Info.title);
        m_Info.ap_airspeed = 0;
        m_Info.ap_auto_throttle = 0;
        m_Info.ap_alt_lock_var = 0;
        m_Info.ap_heading_lock_dir = 0;
        m_Info.ap_vertical_speed = 0;
        m_Info.ap_alt_manually_adjustable = 0;
        m_Info.ap_heading_manually_adjustable = 0;
        m_HeadingSlotChoice.value(-1);
        m_Info.pos_airspeed = 0;
        m_Info.pos_altitude = 0;
        m_Info.pos_latitude = 0;
        m_Info.pos_longitude = 0;

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

        m_AutopilotBtn.labelcolor(FL_FOREGROUND_COLOR);
        m_AirspeedHoldBtn.labelcolor(FL_FOREGROUND_COLOR);
        m_AutoThrottleBtn.labelcolor(FL_FOREGROUND_COLOR);
        m_AltitudeLockBtn.labelcolor(FL_FOREGROUND_COLOR);
        m_ApproachHoldBtn.labelcolor(FL_FOREGROUND_COLOR);
        m_FLCBtn.labelcolor(FL_FOREGROUND_COLOR);
        m_FlightDirectorBtn.labelcolor(FL_FOREGROUND_COLOR);
        m_NavLockedBtn.labelcolor(FL_FOREGROUND_COLOR);
        m_HeadingLockedBtn.labelcolor(FL_FOREGROUND_COLOR);
        m_VerticalSpeedHoldBtn.labelcolor(FL_FOREGROUND_COLOR);
        m_TestPositionBtn.labelcolor(FL_FOREGROUND_COLOR);
        m_RefreshBtn.labelcolor(FL_FOREGROUND_COLOR);
        m_WingLvlBtn.labelcolor(FL_FOREGROUND_COLOR);
        m_YawDamperBtn.labelcolor(FL_FOREGROUND_COLOR);

        m_AirspeedValCounter.value(m_Info.ap_airspeed);
        m_AltitudeCounter.value(m_Info.ap_alt_lock_var);
        m_VerticalSpeedCounter.value(m_Info.ap_vertical_speed);
        m_HeadingCounter.value(m_Info.ap_heading_lock_dir);

        m_HeadingCounter.tooltip("Set heading");
        m_AltitudeCounter.tooltip("Set altitude");
        m_AutopilotBtn.tooltip("Toggle Autopilot");

        m_AutopilotBtn.activate();
        m_PlanePosOut.deactivate();
        m_PlaneAltSpeedOut.deactivate();
        m_PlanePosOut.value("");
        m_PlaneAltSpeedOut.value("");
        m_MapWidget.Reset();

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
    static inline void OnCloseWindow(Fl_Widget* w, void* a)
    {
        if (Fl::event() == FL_SHORTCUT && Fl::event_key() == FL_Escape)
            return; // ignore Escape
        Fl::atclose((Fl_Double_Window*)w, a);
    }

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

    static inline void OnPlaneAltChecked(Fl_Widget*, void* a)
    {
        Application* app = (Application*)a;
        app->PlaneAltChecked();
    }
public:
    Application()
    {
        OS::use_dark_theme();
        Fl_RGB_Image i(sg_Icon_Data, sg_Icon_Width, sg_Icon_Height, sg_Icon_Channel, 0);
        m_Window.icon(&i);
        m_Window.callback(OnCloseWindow, nullptr);

        m_AirplaneTitleOut.box(FL_FLAT_BOX);
        m_AirplaneTitleOut.color(FL_BACKGROUND_COLOR);
        m_AirplaneTitleOut.labeltype(FL_NO_LABEL);
        m_AirplaneTitleOut.visible_focus(false);
        m_AirplaneTitleOut.deactivate();

        m_PlanePosOut.box(FL_FLAT_BOX);
        m_PlanePosOut.color(FL_BACKGROUND_COLOR);
        m_PlanePosOut.labeltype(FL_NO_LABEL);
        m_PlanePosOut.visible_focus(false);
        m_PlanePosOut.deactivate();

        m_PlaneAltSpeedOut.box(FL_FLAT_BOX);
        m_PlaneAltSpeedOut.color(FL_BACKGROUND_COLOR);
        m_PlaneAltSpeedOut.labeltype(FL_NO_LABEL);
        m_PlaneAltSpeedOut.visible_focus(false);
        m_PlaneAltSpeedOut.deactivate();

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
        m_PlaneAltSpCheck.callback(OnPlaneAltChecked, this);
        m_PlaneAltSpCheck.value(1);

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
        m_PlaneAltSpCheck.tooltip("Tracking consumes more resources");

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
            m_ConnectBtn.labelcolor(FL_GRAY0);
            Refresh();
        }
        else
        {

            m_ConnectBtn.color(FL_RED);
            m_ConnectBtn.label("Connect");
            m_ConnectBtn.labelcolor(FL_FOREGROUND_COLOR);
            ResetUI();
            if (m_Info.quit)
            {
                Fl::error("Quit microsoft flight simulator, lost connection");
                m_Info.quit = false;
            }
        }
    }


    void Refresh() noexcept
    {
        m_Socket.Dispatch(&m_Info);
    }


    inline void TransmitEvent(EVENT_ID event, DWORD data = 0) const noexcept
    {
        if (!m_Socket.TransmitEvent(event, data))
            Fl::error("Failed to trasmit event to MSFS, state hasn't been changed!");
    }


    inline void ToggleAutopilot() const noexcept
    {
        TransmitEvent(EVENT_AUTOPILOT);
    }


    inline void ToggleAltHold() const noexcept
    {
        TransmitEvent(EVENT_ALTITUDE_HOLD);
    }


    inline void ToggleAirspeedHold() const noexcept
    {
        TransmitEvent(EVENT_AIRSPEED_HOLD);
    }

    inline void ToggleAutoThrottle() const noexcept
    {
        TransmitEvent(EVENT_AUTO_THROTTLE_ARM);
    }

    inline void ToggleApproachHold() const noexcept
    {
        TransmitEvent(EVENT_APPROACH_HOLD);
    }


    inline void ToggleFLC() const noexcept
    {
        TransmitEvent(EVENT_FLC_HOLD);
    }


    inline void ToggleFlightDirector() const noexcept
    {
        TransmitEvent(EVENT_FLIGHT_DIRECTOR);
    }


    inline void ToggleNavLocked() const noexcept
    {
        TransmitEvent(EVENT_NAV_LOCKED);
    }


    inline void ToggleHeadingLocked() const noexcept
    {
        TransmitEvent(EVENT_HEADING_LOCKED);
    }


    inline void ToggleVerticalSpeedHold() const noexcept
    {
        TransmitEvent(EVENT_VERTICAL_SPEED_HOLD);
    }


    inline void ToggleWingLvl() const noexcept
    {
        TransmitEvent(EVENT_WING_LVL_HOLD);
    }


    inline void ToggleYawDamper() const noexcept
    {
        TransmitEvent(EVENT_YAW_DAMPER_HOLD);
    }


    inline void SetHeading() const noexcept
    {
        TransmitEvent(EVENT_SET_HEADING, static_cast<DWORD>(m_HeadingCounter.value()));
    }


    inline void SetAltitude() const noexcept
    {
        TransmitEvent(EVENT_SET_ALTITUDE, static_cast<DWORD>(m_AltitudeCounter.value()));
    }


    inline void SetAirSpeed() const noexcept
    {
        TransmitEvent(EVENT_SET_AIRSPEED, static_cast<DWORD>(m_AirspeedValCounter.value()));
    }


    inline void SetHeadingIndex() const noexcept
    {
        TransmitEvent(EVENT_SET_HEADING_IDX, static_cast<DWORD>(m_HeadingSlotChoice.value() + 1));
    }


    inline void PlaneAltChecked() noexcept
    {
        m_Socket.TogglePlanePosData();
    }


    inline void SetVerticalSpeed() const noexcept
    {
        if (m_VerticalSpeedHoldBtn.color() == FL_RED)
        {
            ToggleVerticalSpeedHold();
        }

        TransmitEvent(EVENT_SET_VERTICAL_SPEED, static_cast<DWORD>(m_VerticalSpeedCounter.value()));
    }


    void SetPosition() const noexcept
    {
        m_Socket.SetTestPosition();
    }

    inline void Show() noexcept
    {
        Fl::lock();
        while (m_Socket.ShouldUpdate || Fl::wait())
        {
            m_Socket.ShouldUpdate = false;
            if (m_Socket.IsConnected())
            {
                const unsigned long errorCode = m_Socket.Dispatch(&m_Info);
                if (errorCode)
                {
                    Fl::error("Microsoft flight Simulator 2020 exception: %lu", errorCode);
                }

                if (m_Info.quit)
                {
                    Connect();
                }
                UpdateUI();
            }
            m_MapWidget.redraw();
        }
    }
};