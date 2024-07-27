#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm>

#include <FL/x.H>
#include <FL/Fl.H>
#include <gl/GL.h>
#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>
#include <curl/curl.h>

#include "imgui.h"
#include "ImOsm/imosm.h"
#include "ImOsm/imosm_rich.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_opengl3.h"


class GlWidget : public Fl_Gl_Window
{
private:
    bool m_Initialised = false;
    ImOsm::Rich::RichMapPlot _mapPlot;
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
        ImPlot::DestroyContext();
    }


    static size_t CurlWriteString(void* contents, size_t size, size_t nmemb, void* userp)
    {
        const size_t newLength = size * nmemb;

        std::vector<char>* buffer = static_cast<std::vector<char>*>(userp);
        buffer->insert(buffer->end(), static_cast<char*>(contents), static_cast<char*>(contents) + newLength);

        //s->append(static_cast<char*>(contents), newLength);
        return newLength;
    }

    void draw() override
    {
        if (!m_Initialised)
        {
            glClearColor(.1f, .1f, .1f, 1);
            glEnable(GL_DEPTH_TEST);

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

        ImGuiIO& io = ImGui::GetIO();

        ImGui::Begin("Test", (bool*)0, ImGuiWindowFlags_NoMove);
        ImGui::SetWindowPos({ 0, 0 });
        ImGui::SetWindowSize({ io.DisplaySize.x, 50 });

        //static double latitude = 1;
        //static double longitude = 1;
        //double zoom = 5;
        if(ImGui::Button("Click"))
        {
            curl_global_init(CURL_GLOBAL_DEFAULT);

            CURL* m_Handle = curl_easy_init();
            if (m_Handle == NULL)
            {
                puts("Curl init failed");
            }

            std::vector<char> buffer; // Buffer to store received data
            curl_easy_setopt(m_Handle, CURLOPT_WRITEFUNCTION, CurlWriteString);
            curl_easy_setopt(m_Handle, CURLOPT_WRITEDATA, &buffer);
            curl_easy_setopt(m_Handle, CURLOPT_USERAGENT, "YourApp/1.0");

            // get
            curl_easy_setopt(m_Handle, CURLOPT_URL, "https://tile.openstreetmap.org/1/1/0.png");
            CURLcode lasterror = curl_easy_perform(m_Handle);
            if (lasterror != CURLE_OK)
            {
                printf("Error: %s\n", curl_easy_strerror(lasterror));
            }
            printf("%lld\n", buffer.size());

            {
                std::ofstream outFile("test.png", std::ios::out | std::ios::binary);
                outFile.write(buffer.data(), buffer.size());
                // Check if the file stream is open


                // Close the file stream
                outFile.close();
            }

            curl_easy_cleanup(m_Handle);
            curl_global_cleanup();
        }
        ImGui::End();


        ImGui::Begin("Map", (bool*)0, ImGuiWindowFlags_NoMove);
        ImGui::SetWindowPos({ 0, 50 });
        ImGui::SetWindowSize({ io.DisplaySize.x, io.DisplaySize.y-50 });

        _mapPlot.paint();




        ImGui::End();



        //ImGui::ShowDemoWindow();
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
            io.AddMouseButtonEvent(ImGuiMouseButton_Left, event == FL_PUSH);
            return 1;
        case FL_MOUSEWHEEL:
            io.AddMouseWheelEvent(0.0f, static_cast<float>(-Fl::event_dy()));
            return 1;
        default:
            break;
        }

        return Fl_Gl_Window::handle(event); // Call base class handle for any unhandled events
    }
};


class GlWindow
{
private:
    Fl_Window m_Window = Fl_Window(1300, 800, "Gl Window");
    GlWidget m_Widget = GlWidget(0, 0, 1300, 800, "Gl Window");
public:
    GlWindow()
    {
        m_Window.resizable(m_Window);
        m_Window.end();
        m_Window.show();
    }


    void Show()
    {
        while (Fl::wait())
        {
            m_Widget.redraw();
        }
    }
};