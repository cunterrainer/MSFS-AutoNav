#pragma once
#include <cmath>
#include <numbers>
#include <algorithm>
#include <wingdi.h>

#include <FL/x.H>
#include <FL/Fl.H>
#include <gl/GL.h>
#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>

#include "ImGui/imgui.h"
#include "ImOsm/ImOsm.h"
#include "ImOsm/imosm_rich.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_opengl3.h"
#undef min
#undef max

class MapWidget : public Fl_Gl_Window
{
private:
    int m_FollowZoom = 10;
    bool m_FollowAircraft = false;
    bool m_Initialised = false;
    ImOsm::Rich::RichMapPlot m_MapPlot;
    std::shared_ptr<ImOsm::Rich::MarkItem> m_Item = std::make_shared<ImOsm::Rich::MarkItem>(ImOsm::Rich::MarkItem({ 0, 0 }, ""));
public:
    MapWidget(int x, int y, int w, int h, const char* l) : Fl_Gl_Window(x, y, w, h, l)
    {
        mode(FL_RGB | FL_ALPHA | FL_DEPTH | FL_DOUBLE);

        ImOsm::Rich::MarkItem::Style style;
        style.radiusEnabled = true;
        style.markerFill = { 0.1f, 0.11f, 0.11f, 1.f };
        style.markerType = ImPlotMarker_Diamond;
        style.radiusWeight = 10;
        m_Item->setStyle(style);
        m_Item->setEnabled(false);
        m_MapPlot.addItem(m_Item);
    }

    ~MapWidget() override
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        ImPlot::DestroyContext();
    }


    void draw() override
    {
        if (!m_Initialised)
        {
            glClearColor(.1f, .1f, .1f, 1);
            glEnable(GL_DEPTH_TEST);

            // Disable VSync
            const HDC hDC = wglGetCurrentDC();
            const HGLRC hGLRC = wglGetCurrentContext();

            if (hDC && hGLRC)
            {
                typedef BOOL(APIENTRY* PFNWGLSWAPINTERVALEXTPROC)(int);
                PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
                if (wglSwapIntervalEXT)
                {
                    wglSwapIntervalEXT(0);
                }
            }

            ImGui::CreateContext();
            ImPlot::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.IniFilename = nullptr;

            if (!ImGui_ImplWin32_Init(fl_xid(this)))
            {
                Fl::error("Failed to initialize Win32 implementation");
            }

            if (!ImGui_ImplOpenGL3_Init("#version 130"))
            {
                Fl::error("Failed to initialize OpenGL3 implementation");
            }
            m_Initialised = true;
        }

        glDrawBuffer(GL_FRONT_AND_BACK);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        glClear(GL_COLOR_BUFFER_BIT);
        // OpenGL code start

        const ImGuiIO& io = ImGui::GetIO();
        ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0.f, 0.f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
        ImGui::SetNextWindowPos({ 0, 0 });
        ImGui::SetNextWindowSize({ io.DisplaySize.x, io.DisplaySize.y });
        ImGui::Begin("#Map", (bool*)0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

        if (m_FollowAircraft)
        {
            const float aspectRatio = io.DisplaySize.x / io.DisplaySize.y;
            const float centerLat = (float)m_Item->geoCoords().lat;
            const float centerLon = (float)m_Item->geoCoords().lon;
            const float latRange = (float)m_FollowZoom;

            // Calculate longitude range based on latitude to account for projection distortions
            // Adjust longitude range more precisely based on latitude
            const float scalingFactor = 1.0f / cos(centerLat * std::numbers::pi_v<float> / 180.0f);
            const float lonRange = latRange * aspectRatio * scalingFactor;

            m_MapPlot.setBoundsGeo(
                centerLat - latRange,  // min latitude
                centerLat + latRange,  // max latitude
                centerLon - lonRange,  // min longitude
                centerLon + lonRange   // max longitude
            );
        }

        m_MapPlot.paint();

        // License box
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 2, 2 });
        ImGui::SetCursorPos({ io.DisplaySize.x - 195, io.DisplaySize.y - ImGui::GetTextLineHeight() - 5});
        ImGui::PushStyleColor(ImGuiCol_ChildBg, { 1.f, 1.f, 1.f, 1.f });
        ImGui::BeginChild("LicenseBox", ImVec2(0, 0), ImGuiChildFlags_Border);
        ImGui::TextColored({ 0.f, 0.f, 0.f, 1.f }, "Map data from");
        ImGui::SameLine();
        ImGui::TextLinkOpenURL("OpenStreetMap", "https://www.openstreetmap.org/copyright");
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
        ImGui::EndChild();

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        {
            ImGui::OpenPopup("RightClickMenu");
        }

        if (ImGui::BeginPopup("RightClickMenu"))
        {
            if (ImGui::MenuItem(m_FollowAircraft ? "Follow aircraft (True)" : "Follow aircraft (False)"))
            {
                m_FollowAircraft = !m_FollowAircraft;
            }
            ImGui::EndPopup();
        }

        ImGui::End();
        ImGui::PopStyleVar();
        ImPlot::PopStyleVar();
        // OpenGL code end
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glDrawBuffer(GL_BACK);
    }


    int handle(int event) override
    {
        if (!m_Initialised)
            return Fl_Gl_Window::handle(event);

        ImGuiIO& io = ImGui::GetIO();
        switch (event)
        {
        case FL_PUSH:
        case FL_RELEASE:
            io.AddMousePosEvent(static_cast<float>(Fl::event_x()), static_cast<float>(Fl::event_y()));
            io.AddMouseButtonEvent(Fl::event_button() == FL_LEFT_MOUSE ? ImGuiMouseButton_Left : ImGuiMouseButton_Right, event == FL_PUSH);
            return 1;
        case FL_MOUSEWHEEL:
            io.AddMouseWheelEvent(0.0f, static_cast<float>(-Fl::event_dy()));
            if (m_FollowAircraft)
            {
                if (Fl::event_dy() < 0)
                {
                    m_FollowZoom = std::max(1, m_FollowZoom - 1);
                }
                else
                {
                    m_FollowZoom = std::min(17, m_FollowZoom + 1);
                }
            }
            return 1;
        default:
            break;
        }

        return Fl_Gl_Window::handle(event); // Call base class handle for any unhandled events
    }


    void Reset() noexcept
    {
        m_Item->setEnabled(false);
    }


    void SetCoords(double latitude, double longitude) noexcept
    {
        m_Item->setEnabled(true);
        m_Item->setCoords({ latitude, longitude });
    }


    void SetPlaneTitle(const char* title) noexcept
    {
        m_Item->setText(title);
    }
};