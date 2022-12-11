import os
import subprocess


def Generate():
    subprocess.run(["Vendor/premake/premake5.exe", "vs2022"])

if __name__ == "__main__":
    os.chdir("../")
    Generate()

    input("Press Enter to continue...")
