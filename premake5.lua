workspace "VoxelDemo"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

    startproject "VoxelDemo"

-- This is a helper variable, to concatenate the sys-arch
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "VoxelDemo"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir("bin\\" .. outputdir .. "\\%{prj.name}")
    objdir("bin-int\\" .. outputdir .. "\\%{prj.name}")

    files {
        "VoxelDemo/src/**.cpp",
        "VoxelDemo/src/**.c",
        "VoxelDemo/include/**.h",
		"VoxelDemo/include/**.hpp",
		"VoxelDemo/include/**.inl",
		"assets/shaders/**.vs",
		"assets/shaders/**.fs",
		"assets/textures/**.png",
		"assets/textures/**.jpg",
    }

    includedirs {
        "VoxelDemo/include",
		"VoxelDemo/include/core",
		"assets"
    }
	
	libdirs {
        "\"./VoxelDemo/lib\""
    }

    links {
        "libglfw3dll"
    }

    filter "system:windows"
        buildoptions { "-lgdi32" }
        systemversion "latest"

        defines  {
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }

        postbuildcommands {
            "copy /y \"VoxelDemo\\lib\\glfw3.dll\" \"%{cfg.targetdir}\\glfw3.dll\""
        }

    filter { "configurations:Debug" }
        buildoptions "/MTd"
        runtime "Debug"
        symbols "on"

    filter { "configurations:Release" }
        defines {" _RELEASE" }
        buildoptions "/MT"
        runtime "Release"
        optimize "on"