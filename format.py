import argparse
import difflib
import re
import subprocess
import os
import sys

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

def read_text(file_path):
    with open(file_path, 'rb') as handle:
        return handle.read().decode('utf-8')

def write_text(file_path, text):
    with open(file_path, 'wb') as handle:
        handle.write(text.encode('utf-8'))

def sort_imports(text):
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

    return ''.join(lines)

def run_clang_format(file_path, text):
    result = subprocess.run(
        ["clang-format", f"--assume-filename={file_path}"],
        input=text.encode('utf-8'),
        stdout=subprocess.PIPE,
        check=True
    )
    return result.stdout.decode('utf-8')

def format_file(file_path):
    try:
        text = read_text(file_path)

        sorted_text = sort_imports(text)
        if sorted_text != text:
            print(f"Sorted imports: {file_path}")

        formatted_text = run_clang_format(file_path, sorted_text)
        if formatted_text != text:
            write_text(file_path, formatted_text)

        print(f"Formatted: {file_path}")
        return True
    except subprocess.CalledProcessError as exception:
        print(f"Failed to format {file_path}: {exception}")
        return False

def check_file(file_path):
    try:
        text = read_text(file_path)

        formatted_text = run_clang_format(file_path, sort_imports(text))
        if formatted_text == text:
            return True

        relative_path = os.path.relpath(file_path, project_dir)
        diff = difflib.unified_diff(
            text.splitlines(keepends=True),
            formatted_text.splitlines(keepends=True),
            fromfile=f"{relative_path} (current)",
            tofile=f"{relative_path} (formatted)"
        )
        print(''.join(diff), end='')
        return False
    except subprocess.CalledProcessError as exception:
        print(f"Failed to check {file_path}: {exception}")
        return False

def collect_files():
    for root, dirs, files in os.walk(project_dir):
        dirs[:] = [d for d in dirs if d not in exclude_dirs]

        for file in files:
            if file.endswith(extensions):
                yield os.path.join(root, file)

def main():
    parser = argparse.ArgumentParser(description="Format C++ sources with clang-format and sort module imports.")
    parser.add_argument(
        "--check",
        action="store_true",
        help="do not modify files, only report the ones that are not formatted (exit code 1)"
    )
    arguments = parser.parse_args()

    try:
        subprocess.run(["clang-format", "--version"], stdout=subprocess.DEVNULL, check=True)
    except (OSError, subprocess.CalledProcessError):
        print("clang-format was not found in PATH")
        return 2

    failed = []
    for file_path in collect_files():
        handle_file = check_file if arguments.check else format_file
        if not handle_file(file_path):
            failed.append(os.path.relpath(file_path, project_dir))

    if not failed:
        return 0

    if arguments.check:
        print(f"\n{len(failed)} file(s) are not formatted:")
        for relative_path in failed:
            print(f"  {relative_path}")
        print("\nRun `python format.py` to fix them.")

    return 1

if __name__ == "__main__":
    sys.exit(main())
