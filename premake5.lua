workspace "Match3"
	architecture "x64"
	startproject "Match3"

	configurations {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to the root folder
IncludeDir = {}
IncludeDir["SDL2"] = "Match3/vendor/SDL2"
IncludeDir["SDL2image"] = "Match3/vendor/SDL2image"
IncludeDir["Random"] = "Match3/vendor/effolkronium"

project "Match3"
    location "Match3"
    kind "ConsoleApp"
    language "C++"
    cppdialect "c++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "Match3/src/pch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.SDL2}/include",
        "%{IncludeDir.SDL2image}/include",
        "%{IncludeDir.Random}"
    }

    links {
        "SDL2",
        "SDL2main",
        "SDL2_image"
    }

    libdirs {
        "%{IncludeDir.SDL2}/lib/x64/",
        "%{IncludeDir.SDL2image}/lib/x64/"
    }

    --postbuildcommands {
    --    "{COPY} %{IncludeDir.SDL2}/lib/x64/SDL2.dll %{cfg.targetdir}"
    --}
    

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "LF_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "LF_RELEASE"
		runtime "Release"
		optimize "on"