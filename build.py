import os
import sys
import shutil
import subprocess
import stat

# Configuration
PROJECT_ROOT  = os.path.abspath(os.path.dirname(__file__))
BUILD_DIR     = os.path.join(PROJECT_ROOT, "build")
VCPKG_DIR     = os.path.join(PROJECT_ROOT, "vcpkg")
VCPKG_EXE     = os.path.join(VCPKG_DIR,
                    "vcpkg.exe" if os.name == "nt" else "vcpkg")
TOOLCHAIN_FILE= os.path.join(VCPKG_DIR,
                    "scripts", "buildsystems",
                    "vcpkg.cmake")


def remove_readonly(func, path, _):
    os.chmod(path, stat.S_IWRITE)
    func(path)

def run(cmd, **kwargs):
    print(f"> {' '.join(cmd)}")
    subprocess.run(cmd, check=True, **kwargs)

def ensure_vcpkg():
    if os.path.isfile(VCPKG_EXE):
        print("Found existing vcpkg at", VCPKG_EXE)
        return

    print("vcpkg not found. Cloning into:", VCPKG_DIR)
    run(["git", "clone", "--depth=1",
         "https://github.com/microsoft/vcpkg.git", VCPKG_DIR])

    print("Bootstrapping vcpkg...")
    if os.name == "nt":
        run([os.path.join(VCPKG_DIR, "bootstrap-vcpkg.bat")])
    else:
        run([os.path.join(VCPKG_DIR, "bootstrap-vcpkg.sh")])

    manifest = os.path.join(PROJECT_ROOT, "vcpkg.json")
    if os.path.isfile(manifest):
        print("Installing manifest dependencies...")
        run([VCPKG_EXE, "install", "--manifest"], cwd=VCPKG_DIR)


def main():
    # 1. Prepare build directory
    if os.path.exists(BUILD_DIR):
        print(f"Deleting folder {BUILD_DIR}...")
        shutil.rmtree(BUILD_DIR, onerror=remove_readonly)

    print(f"Creating folder {BUILD_DIR}...")
    os.makedirs(BUILD_DIR, exist_ok=True)

    # 2. Ensure vcpkg is ready
    ensure_vcpkg()

    # 3. Configure with CMake + vcpkg toolchain
    os.chdir(BUILD_DIR)
    cmake_cmd = [
        "cmake", "..",
        f"-DCMAKE_TOOLCHAIN_FILE={TOOLCHAIN_FILE}",
    ]
    run(cmake_cmd)

    # 4. Build
    print("Building the project...")
    run(["cmake", "--build", ".", "--config", "Release"])

if __name__ == "__main__":
    try:
        main()
    except subprocess.CalledProcessError as e:
        print("ERROR: ", e, file=sys.stderr)
        sys.exit(e.returncode)
      
