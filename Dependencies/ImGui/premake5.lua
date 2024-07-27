project "ImGui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"

    files {
        "src/**.cpp",
        "include/ImGui/**.h"
    }

    includedirs {
        "include/ImGui",
        "../GLFW/include"
    }
