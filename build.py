import os
import shutil
import subprocess

build_dir = "build"

if os.path.exists(build_dir):
    print(f"Deleting folder {build_dir}...")
    shutil.rmtree(build_dir)

print(f"Creating folder {build_dir}...")
os.makedirs(build_dir)

os.chdir(build_dir)

print("Using command: cmake ..")
result = subprocess.run(["cmake", ".."], check=True)

if result.returncode == 0:
    print("CMake command successfully completed.")
else:
    print("Error while executing CMake.")