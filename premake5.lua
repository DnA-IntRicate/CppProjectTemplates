include "Vendor/premake/customization/solutionitems.lua"

OUT_DIR = "%{wks.location}/bin/build/%{cfg.system}/%{cfg.architecture}/%{cfg.buildcfg}"
INT_DIR = "%{wks.location}/bin/intermediate/%{cfg.system}/%{cfg.architecture}/%{cfg.buildcfg}"

-- TODO: Fix including dx11 above opengl4.5 causes only dx11 to get projects from Vendor
include "Templates/glfw-vulkan"
include "Templates/glfw-dx11"
include "Templates/glfw-opengl4.5"
