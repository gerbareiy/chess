import subprocess
import os

project_dir = os.path.abspath(os.getcwd())
extensions = (
    # source files
    '.cpp', '.cxx', '.cc', '.C', '.c++', '.c',
    # headers
    '.h', '.hh', '.hpp', '.hxx', '.H', '.h++', '.hp',
    # templates / inline
    '.tpp', '.inl', '.ipp', '.ixxh', '.impl', '.inc',
    # modules C++20
    '.ixx', '.cppm', '.mxx', '.mpp', '.ixxh',
    # rare extensions
    '.h++', '.c++', '.cxx', '.tcc', '.txx'
)
exclude_dirs = {'build', 'vcpkg', '.git'}

def format_file(file_path):
    try:
        subprocess.run(["clang-format", "-i", file_path], check=True)
        print(f"Formatted: {file_path}")
    except subprocess.CalledProcessError as e:
        print(f"Failed to format {file_path}: {e}")

for root, dirs, files in os.walk(project_dir):
    dirs[:] = [d for d in dirs if d not in exclude_dirs]

    for file in files:
        if file.endswith(extensions):
            format_file(os.path.join(root, file))