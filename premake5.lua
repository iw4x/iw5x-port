dependencies = {
	basePath = "./deps"
}

function dependencies.load()
	dir = path.join(dependencies.basePath, "premake/*.lua")
	deps = os.matchfiles(dir)

	for i, dep in pairs(deps) do
		dep = dep:gsub(".lua", "")
		require(dep)
	end
end

function dependencies.imports()
	for i, proj in pairs(dependencies) do
		if type(i) == 'number' then
			proj.import()
		end
	end
end

function dependencies.projects()
	for i, proj in pairs(dependencies) do
		if type(i) == 'number' then
			proj.project()
		end
	end
end

newoption {
	trigger = "copy-to",
	description = "Optional, copy the EXE to a custom folder after build, define the path here if wanted.",
	value = "PATH"
}

dependencies.load()

workspace "open-iw5"
	startproject "open-iw5"
	location "./build"
	objdir "%{wks.location}/obj"
	targetdir "%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}"

	configurations { "Debug", "Release" }

	language "C++"
	cppdialect "C++20"

	architecture "x86"
	platforms "Win32"

	systemversion "latest"
	symbols "On"
	staticruntime "On"
	editandcontinue "Off"
	warnings "Extra"
	characterset "ASCII"

	flags {
		"NoIncrementalLink",
		"NoMinimalRebuild",
		"MultiProcessorCompile",
		"No64BitChecks"
	}

	filter "platforms:Win*"
		defines { "_WINDOWS", "WIN32" }
	filter {}

	filter "configurations:Release"
		optimize "Full"
		buildoptions { "/Os" }
		defines { "NDEBUG" }
		flags { "FatalCompileWarnings" }
	filter {}

	filter "configurations:Debug"
		optimize "Debug"
		defines { "DEBUG", "_DEBUG" }
	filter  {}

	project "open-iw5"
		kind "ConsoleApp"
		language "C++"

		pchheader "std_include.hpp"
		pchsource "src/std_include.cpp"
		
		linkoptions "/IGNORE:4254 /DYNAMICBASE:NO /SAFESEH:NO /LARGEADDRESSAWARE"
		linkoptions "/LAST:.main"

		files {
			"./src/**.rc",
			"./src/**.hpp",
			"./src/**.cpp",
			"./src/resources/**.*"
		}

		includedirs {
			"./src"
		}

		resincludedirs {
			"$(ProjectDir)src"
		}

		if _OPTIONS["copy-to"] then
			postbuildcommands {
				"copy /y \"$(TargetDir)*.exe\" \"" .. _OPTIONS["copy-to"] .. "\""
			}
		end

		dependencies.imports()

	group "Dependencies"
		dependencies.projects()
