project "ImPlot"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"

    files {
        "src/**.cpp",
        "include/ImPlot/**.h"
    }

    includedirs {
        "include/ImPlot",
        "../../../ImGui/include/ImGui"
    }