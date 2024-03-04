import os
from SetupPython import PythonConfiguration as PyReqs
from SetupVulkan import VulkanConfiguration as VkReqs
import GenerateVS as VS


os.chdir("../")

print("Validating python...")
PyReqs.Validate()

print("\nValidating Vulkan SDK...")
VkReqs.Validate()

print("\nGenerating Visual Studio projects...")
VS.Generate()

print("\nSetup complete!")
input("Press Enter to continue...")
