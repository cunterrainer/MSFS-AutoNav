#pragma once
#include <FL/x.H>
#include <FL/Fl.H>
#include <gl/GL.h>
#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_opengl3.h"

class GlWidget : public Fl_Gl_Window
{
private:
    bool m_Initialised = false;
public:
    GlWidget(int x, int y, int w, int h, const char* l) : Fl_Gl_Window(x, y, w, h, l)
    {
        mode(FL_RGB | FL_ALPHA | FL_DEPTH | FL_DOUBLE);
    }

    ~GlWidget() override
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }


    void draw() override
    {
        if (!m_Initialised)
        {
            glClearColor(.1f, .1f, .1f, 1);
            glEnable(GL_DEPTH_TEST);

            ImGui::CreateContext();
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

        ImGui::Begin("Test");
        ImGui::End();
        ImGui::ShowDemoWindow();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glDrawBuffer(GL_BACK);
    }


    int handle(int event) override
    {
        if (!m_Initialised)
            return Fl_Gl_Window::handle(event);

        switch (event)
        {
        case FL_PUSH:
        case FL_RELEASE:
        {
            ImGuiIO& io = ImGui::GetIO();
            io.AddMousePosEvent(static_cast<float>(Fl::event_x()), static_cast<float>(Fl::event_y()));
            io.AddMouseButtonEvent(ImGuiMouseButton_Left, event == FL_PUSH);
            return 1;
        }
        default:
            break;
        }

        return Fl_Gl_Window::handle(event); // Call base class handle for any unhandled events
    }
};