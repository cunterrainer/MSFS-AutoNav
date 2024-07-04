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

filter "system:windows"
    platforms { "x64", "x86" }

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
    --flags "FatalCompileWarnings"
    defines "_CRT_SECURE_NO_WARNINGS"

    files {
        "src/**.cpp",
        "src/**.h"
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


    --gcc* clang* msc*
    filter "toolset:msc*"
        warnings "High"
        externalwarnings "Default"
        buildoptions { "/sdl" }
        disablewarnings "4706"

    filter { "toolset:gcc* or toolset:clang*" }
        enablewarnings {
            --"cast-align",
            --"cast-qual",
            --"ctor-dtor-privacy",
            "disabled-optimization",
            --"format=2",
            "init-self",
            "missing-include-dirs",
            "overloaded-virtual",
            "redundant-decls",
            "shadow",
            "sign-promo",
            --"switch-default",
            --"undef",
            "uninitialized",
            "unreachable-code",
            "unused",
            "alloca",
            --"conversion",
            "deprecated",
            --"format-security",
            --"null-dereference",
            "stack-protector",
            "vla",
            "shift-overflow"
        }
        disablewarnings { "unknown-warning-option", "format-security", "sequence-point", "invalid-source-encoding" }

    filter "toolset:gcc*"
        warnings "Extra"
        externalwarnings "Off"
        enablewarnings {
            "noexcept",
            "strict-null-sentinel",
            "array-bounds=2",
            --"duplicated-branches",
            "duplicated-cond",
            "logical-op",
            "arith-conversion",
            "stringop-overflow=4",
            "implicit-fallthrough=3",
            "trampolines"
        }
        disablewarnings "cast-function-type"

    filter "toolset:clang*"
        warnings "Extra"
        externalwarnings "Everything"
        linkgroups "off"
        enablewarnings {
            "array-bounds",
            "long-long",
            --"implicit-fallthrough", 
        }
        disablewarnings {"cast-align", "sign-conversion" }
    filter {}


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
        links "SimConnect"
        entrypoint "mainCRTStartup"