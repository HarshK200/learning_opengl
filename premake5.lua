workspace "learning_opengl"
    architecture "x64"
    configurations {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}/%{cfg.system}"


-- vendor premake5.lua files
include "application/vendor/GLFW"

project "application"
    location "application"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/GLAD/src/glad.c",
        "%{prj.name}/vendor/stb_image/stb_image.cpp"
    }

    includedirs {
        "%{prj.name}/src",
        "application/vendor/GLFW/include",
        "application/vendor/GLAD/include",
        "application/vendor/stb_image"
    }

    -- vendor links
    links {
        "GLFW",
    }

    filter "system:linux"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "RELEASE"
        optimize "On"
