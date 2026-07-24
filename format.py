import re
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

# clang-format's SortIncludes only sorts #include, not C++20 `import` module
# declarations, so we sort each contiguous block of import lines ourselves.
IMPORT_RE = re.compile(r"^import\s+([A-Za-z0-9_.]+);$")

def sort_imports(file_path):
    with open(file_path, 'rb') as handle:
        text = handle.read().decode('utf-8')

    lines = text.splitlines(keepends=True)
    block_start = None

    def line_body(line):
        return line.rstrip('\r\n')

    def flush(end):
        nonlocal block_start
        if block_start is not None and end - block_start > 1:
            block = lines[block_start:end]
            block.sort(key=lambda line: line_body(line).lower())
            lines[block_start:end] = block
        block_start = None

    for index, line in enumerate(lines):
        if IMPORT_RE.match(line_body(line)):
            if block_start is None:
                block_start = index
        else:
            flush(index)
    flush(len(lines))

    new_text = ''.join(lines)
    if new_text != text:
        with open(file_path, 'wb') as handle:
            handle.write(new_text.encode('utf-8'))
        print(f"Sorted imports: {file_path}")

def format_file(file_path):
    try:
        sort_imports(file_path)
        subprocess.run(["clang-format", "-i", file_path], check=True)
        print(f"Formatted: {file_path}")
    except subprocess.CalledProcessError as exception:
        print(f"Failed to format {file_path}: {exception}")

for root, dirs, files in os.walk(project_dir):
    dirs[:] = [d for d in dirs if d not in exclude_dirs]

    for file in files:
        if file.endswith(extensions):
            format_file(os.path.join(root, file))