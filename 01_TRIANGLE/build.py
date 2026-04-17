import sys
import subprocess
import os
import shutil
from pathlib import Path

script_dir = Path(__file__).parent.resolve()
# FOR FUTURE ---- build_type = "Release" if "--release" in sys.argv else "Debug"


def main():

    if sys.platform.startswith('linux'):
        linuxBuild()

    elif sys.platform == 'win32':
        winBuild()

    else:
        print("System currently not supported")




def compileShaders():
    glslc = shutil.which("glslc")

    if not glslc:
        print("glcls not installed or could not be found")
        return False
    else:
        print("found glcls: ", glslc)

    
    shader_dir = script_dir / "shaders"

    print(shader_dir)

    for shader_file in shader_dir.glob("*"):
        if shader_file.suffix in [".vert", ".frag", ".comp"]:
            output_file = shader_file.with_suffix(shader_file.suffix + ".spv")

            if not output_file.exists() or shader_file.stat().st_mtime > output_file.stat().st_mtime:
                print("compiling shader: ", shader_file.name)

                result = subprocess.run([glslc, str(shader_file), "-o", str(output_file)], capture_output=True, text=True)

                if result.returncode != 0:
                    print("shader error in ", shader_file.name, ":", result.stderr)
                    
                    return False
            



def cmakeBuild():
    if ((script_dir / "build").exists()):
        print("not first time compilation - recompiling")
        subprocess.run(["cmake", "--build", "build"])

    else:
        print("running first time compilation")
        subprocess.run(["cmake", "-S", ".", "-B", "build"])
        subprocess.run(["cmake", "--build", "build"])






def linuxBuild():
    print("Building for Linux")  

    # compile all shader files from shader directory and give feedback
    if ( compileShaders() == False ):
        print("Shaders not compiled correctly")
        return
    else: 
        print("Shaders successfully compiled")

    # compile project with cmake

    cmakeBuild()

    



def winBuild():
    print("Building for Windows")
    print("windows not currently supported - coming soon!")





if __name__ == "__main__":
    main()