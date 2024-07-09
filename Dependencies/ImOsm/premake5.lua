project "ImOsm"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    files {
        "src/**.cpp",
        "include/ImOsm/**.h"
    }

    includedirs {
        "include",
        "../ImGui/include",
        "Dependencies/curl/include"
    }

    libdirs "Dependencies/curl/Binaries/windows/x64"

    filter { "configurations:Debug" }
        links {
            "libcurl_a_debug"
        }
    filter { "configurations:Release" }
        links {
            "libcurl_a"
        }