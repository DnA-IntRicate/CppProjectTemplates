include "Vendor/premake/customization/solutionitems.lua"

require "Vendor/premake/premake-cmake/cmake"

OUT_DIR = "%{wks.location}/bin/build/%{cfg.system}/%{cfg.architecture}/%{cfg.buildcfg}"
INT_DIR = "%{wks.location}/bin/intermediate/%{cfg.system}/%{cfg.architecture}/%{cfg.buildcfg}"

include "Templates/glfw-dx11"
