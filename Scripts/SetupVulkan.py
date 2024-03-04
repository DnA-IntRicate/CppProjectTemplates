import os
import subprocess
from pathlib import Path
import Utils


class VulkanConfiguration:
    RequiredVulkanVersion = "1.3."
    InstallVulkanVersion  = "1.3.216.0"
    VulkanDirectory = "Vendor/VulkanSDK"

    @classmethod
    def Validate(cls):
        if (not cls.CheckVulkanSDK()):
            print("Vulkan SDK not installed correctly!")
            return

        if (not cls.CheckVulkanSDKDebugLibs()):
            print("\nNo Vulkan SDK debug libs found. Install Vulkan SDK with debug libs.")
            print("Debug configuration disabled.")

    @classmethod
    def CheckVulkanSDK(cls) -> bool:
        vulkanSDK = Utils.GetSystemEnvironmentVariable("VULKAN_SDK")
        if (vulkanSDK is None):
            print("\nYou don't have the Vulkan SDK installed!")
            cls.__InstallVulkanSDK()
            return False
        else:
            print(f"Located Vulkan SDK at {vulkanSDK}.")

        try:
            checkSDKOutput = subprocess.check_output(os.path.normpath(f"{vulkanSDK}/Bin/vulkanInfoSDK")).decode("utf-8")
            versionToken = "Vulkan Instance Version: "
            pos = checkSDKOutput.find(versionToken) + len(versionToken)
            sdkVersion = checkSDKOutput[pos:pos + 4]
        except: 
            print("No Vulkan ICD was found! Installed Vulkan drivers are either corrupt or this machine does not support Vulkan.")
            return           

        if (cls.RequiredVulkanVersion not in sdkVersion):
            print(f"You don't have the correct Vulkan SDK version! (Engine requires {cls.RequiredVulkanVersion}).")
            cls.__InstallVulkanSDK()
            return False

        print(f"Correct Vulkan SDK ({cls.InstallVulkanVersion}) located at {vulkanSDK}.")
        return True

    @classmethod
    def __InstallVulkanSDK(cls):
        permissionGranted = False
        while not permissionGranted:
            reply = str(input("Would you like to install VulkanSDK {0:s}? [Y/N]: ".format(cls.InstallVulkanVersion))).lower().strip()[:1]
            if reply == 'n':
                return
            permissionGranted = (reply == 'y')

        vulkanInstallURL = f"https://sdk.lunarg.com/sdk/download/{cls.InstallVulkanVersion}/windows/VulkanSDK-{cls.InstallVulkanVersion}-Installer.exe"
        vulkanPath = f"{cls.VulkanDirectory}/VulkanSDK-{cls.InstallVulkanVersion}-Installer.exe"
        print("Downloading {0:s} to {1:s}".format(vulkanInstallURL, vulkanPath))
        Utils.DownloadFile(vulkanInstallURL, vulkanPath)
        print("Running Vulkan SDK installer...")
        os.startfile(os.path.abspath(vulkanPath))

        print("Re-run this script after installation!")
        input("Press Enter to continue...")
        quit()

    @classmethod
    def CheckVulkanSDKDebugLibs(cls):
        vulkanSDK = os.environ.get("VULKAN_SDK")
        shadercdLib = Path(f"{vulkanSDK}/Lib/shaderc_sharedd.lib")

        return shadercdLib.exists()

if __name__ == "__main__":
    os.chdir("../")
    VulkanConfiguration.Validate()
    input("Press Enter to continue...")
