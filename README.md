# Chess

A chess game with a console client, and a client/server mode over the network.

## Building

To build the project, run:

```bash
python3 build.py
```

The sources are C++23 with modules, so the build needs a compiler that supports them:

- Windows: Visual Studio 2022 (MSVC v143). The build uses the Visual Studio generator.
- Linux: GCC or Clang with C++20 module support, plus `ninja` — the Makefile generators cannot build modules,
  so the script uses Ninja and builds each configuration into `build/<configuration>`.

## Documentation

- [Coding style](docs/coding-style.md)
- [How to play](docs/how-to-play.md)
