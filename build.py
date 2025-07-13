import os
import shutil
import subprocess
import stat

build_dir = "build"

def remove_readonly(func, path, _):
    os.chmod(path, stat.S_IWRITE)
    func(path)

if os.path.exists(build_dir):
    print(f"Deleting folder {build_dir}...")
    shutil.rmtree(build_dir, onerror=remove_readonly)

print(f"Creating folder {build_dir}...")
os.makedirs(build_dir, exist_ok=True)

os.chdir(build_dir)

print("Using command: cmake ..")
result = subprocess.run(["cmake", ".."], check=True)

if result.returncode == 0:
    print("CMake command successfully completed.")
else:
    print("Error while executing CMake.")
