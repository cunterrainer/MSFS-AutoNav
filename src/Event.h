#pragma once
#include <cstdio>
#include <thread>
#include <Windows.h>

#include <FL/Fl.H>

namespace Events
{
    std::thread Thread;
    HANDLE Handles[2];

    static void MonitorEvents()
    {
        while (true)
        {
            const DWORD dwWaitResult = WaitForMultipleObjects(2, Handles, FALSE, INFINITE);
            if (dwWaitResult == WAIT_OBJECT_0) // msfs event signaled
            {
                Fl::awake();
            }
            else if (dwWaitResult == WAIT_OBJECT_0 + 1) // terminate signaled
            {
                break;
            }
        }
    }

    static void Start()
    {
        Handles[0] = CreateEventA(NULL, FALSE, FALSE, NULL);
        Handles[1] = CreateEventA(NULL, TRUE, FALSE, NULL);
        if (Events::Handles[0] == NULL || Events::Handles[1] == NULL)
        {
            fprintf(stderr, "Error: Failed to create events!");
            return;
        }
        Thread = std::thread(Events::MonitorEvents);
    }

    static void Stop()
    {
        SetEvent(Handles[1]); // Signal thread to terminate
        Thread.join();
        if (!CloseHandle(Handles[0]) || !CloseHandle(Handles[1]))
        {
            fprintf(stderr, "Error: Failed to close event handles!");
        }
    }
};