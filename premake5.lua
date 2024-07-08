workspace "AutoNav"
    configurations {
        "Debug",
        "Release"
    }
    startproject "AutoNav"

outputdir = "/BIN/%{cfg.toolset}/%{cfg.shortname}/%{prj.name}/"
cwd = os.getcwd() -- get current working directory
targetdir(cwd .. outputdir .. "bin")
objdir(cwd .. outputdir .. "bin-int")

platforms "x64"
filter { "configurations:Debug" }
    runtime "Debug"
    symbols "on"
    optimize "off"
filter { "configurations:Release" }
    runtime "Release"
    symbols "off"
    defines "NDEBUG"
    optimize "Speed"
filter {}

-- only for visual studio
flags {
    "MultiProcessorCompile"
}
staticruntime "off"
removeunreferencedcodedata "on"

project "AutoNav"
    language "C++"
    cppdialect "C++17"
    characterset "Unicode"

    flags "FatalCompileWarnings"
    warnings "High"
    externalwarnings "Default"
    buildoptions { "/sdl" }
    disablewarnings "4706"
    defines "_CRT_SECURE_NO_WARNINGS"
    ignoredefaultlibraries { "LIBCMT", "LIBCMTD" }

    files {
        "src/**.cpp",
        "src/**.h",
        "res/icon.rc"
    }

    includedirs {
        "src",
        "Dependencies/fltk",
        "Dependencies/fltk/Build",
        "$(MSFS_SDK)\\SimConnect SDK\\include"
    }

    externalincludedirs {
        "Dependencies/fltk",
        "Dependencies/fltk/Build",
        "$(MSFS_SDK)\\SimConnect SDK\\include"
    }

    links {
        "Gdiplus",
        "comctl32",
        "gdi32",
        "opengl32",
        "shell32",
        "Shlwapi",
        "ole32",
        "uuid",
        "comdlg32",
        "UxTheme",
        "Dwmapi",
        "user32",
        "Ws2_32",
        "kernel32",
        "winspool",
        "advapi32",
        "oleaut32",
        "odbc32",
        "odbccp32"
    }

    libdirs {
        "$(MSFS_SDK)\\SimConnect SDK\\lib\\static",
    }

    filter { "configurations:Debug" }
        kind "ConsoleApp"
        links {
            "SimConnect_debug",
            "fltk_formsd",
            "fltk_imagesd",
            "fltk_jpegd",
            "fltk_pngd",
            "fltk_zd",
            "fltkd"
        }
        libdirs "Dependencies/fltk/Build/lib/Debug"
    filter { "configurations:Release" }
        kind "WindowedApp"
        links {
            "SimConnect",
            "fltk_forms",
            "fltk_images",
            "fltk_jpeg",
            "fltk_png",
            "fltk_z",
            "fltk"
        }
        libdirs "Dependencies/fltk/Build/lib/Release"
        entrypoint "mainCRTStartup"