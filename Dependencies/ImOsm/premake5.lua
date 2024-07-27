project "ImOsm"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"

    files {
        "src/**.cpp",
        "include/ImOsm/**.h",
        "Dependencies/stb/stb_image.c"
    }

    includedirs {
        "include/ImOsm",
        "Dependencies/implot",
        "Dependencies/latlon",
        "Dependencies/mINI/src/mini",
        "Dependencies/stb",
        "Dependencies/curl/include",

        "../ImGui"
    }

include "Dependencies/ImPlot"