import sys
import subprocess
import os
import shutil
from pathlib import Path


def main():
    if sys.platform.startswith('linux'):
        
        linuxBuild()

    elif sys.platform == 'win32':
        print("Building Sodaview for Windows")
        winBuild()

    else:
        print("System currently not supported by Sodaview")




def compileShaders():
    glslc = shutil.which("glslc")

    if not glslc:
        print("glcls not installed or could not be found")
        return False
    else:
        print("found glcls: ", glslc)

    script_dir = Path(__file__).parent.resolve()
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
            




def linuxBuild():
    print("Building Sodaview for Linux")  

    # compile all shader files from shader directory and give feedback
    if ( compileShaders() == False ):
        print("Shaders not compiled correctly")
        return
    else: 
        print("Shaders successfully compiled")

    # compile project with cmake

    



def winBuild():
    print("windows not currently supported - coming soon!")





if __name__ == "__main__":
    main()