import os
import sys
import shutil
import subprocess
import stat

# Configuration
PROJECT_ROOT  = os.path.abspath(os.path.dirname(__file__))
BUILD_DIR     = os.path.join(PROJECT_ROOT, "build")
VCPKG_DIR     = os.path.join(PROJECT_ROOT, "vcpkg")
VCPKG_EXE     = os.path.join(VCPKG_DIR, "vcpkg.exe" if os.name == "nt" else "vcpkg")
TOOLCHAIN_FILE= os.path.join(VCPKG_DIR, "scripts", "buildsystems", "vcpkg.cmake")
CONFIGURATIONS = ("Release", "Debug")

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

def ensure_ninja():
    if shutil.which("ninja") is not None:
        return
    raise SystemExit(
        "ninja was not found in PATH, and it is required here: the Makefile generators "
        "cannot build C++20 modules at all, so there is no usable fallback."
    )

def build_multi_config():
    # Visual Studio keeps every configuration in one build tree and picks between them at build time.
    run(["cmake", PROJECT_ROOT, f"-DCMAKE_TOOLCHAIN_FILE={TOOLCHAIN_FILE}"], cwd=BUILD_DIR)
    for configuration in CONFIGURATIONS:
        run(["cmake", "--build", ".", "--config", configuration], cwd=BUILD_DIR)

def build_single_config():
    # Ninja instead of the default Makefiles, because Makefile generators do not support C++20 modules.
    # A single-configuration generator bakes the configuration into the build tree, so each one gets its own.
    ensure_ninja()
    for configuration in CONFIGURATIONS:
        directory = os.path.join(BUILD_DIR, configuration)
        os.makedirs(directory, exist_ok=True)
        run([
            "cmake", PROJECT_ROOT,
            "-G", "Ninja",
            f"-DCMAKE_BUILD_TYPE={configuration}",
            f"-DCMAKE_TOOLCHAIN_FILE={TOOLCHAIN_FILE}",
        ], cwd=directory)
        run(["cmake", "--build", "."], cwd=directory)

def main():
    # 1. Prepare build directory
    if os.path.exists(BUILD_DIR):
        print(f"Deleting folder {BUILD_DIR}...")
        shutil.rmtree(BUILD_DIR, onerror=remove_readonly)

    print(f"Creating folder {BUILD_DIR}...")
    os.makedirs(BUILD_DIR, exist_ok=True)

    # 2. Ensure vcpkg is ready
    ensure_vcpkg()

    # 3. Configure and build every configuration
    print("Building the project...")
    if os.name == "nt":
        build_multi_config()
    else:
        build_single_config()

if __name__ == "__main__":
    try:
        main()
    except subprocess.CalledProcessError as e:
        print("ERROR: ", e, file=sys.stderr)
        sys.exit(e.returncode)
