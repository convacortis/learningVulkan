import sys
import subprocess
import os


def main():
    if sys.platform.startswith('linux'):
        print("Building Sodaview for Linux")
        linuxBuild()

    elif sys.platform == 'win32':
        print("Building Sodaview for Windows")
        winBuild()

    else:
        print("System currently not supported by Sodaview")




def linuxBuild():
    print("linux building code")




def winBuild():
    print("windows not currently supported - coming soon!")





if __name__ == "__main__":
    main()