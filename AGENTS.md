# Boost C++ Superproject

Boost monorepo — every `libs/<name>/` directory is a **git submodule**. Most are empty until explicitly initialized. Work happens inside individual library submodules.

## Repository layout

```
libs/<name>/          # one git submodule per library (may be empty)
  include/boost/      # public headers
  src/                # compiled sources (if any)
  test/               # unit tests
tools/                # CMake modules, b2 tools, etc.
tools/cmake/include/  # CMake helpers (BoostRoot, etc.)
CMakeLists.txt        # superproject CMake entry point (v1.92.0)
Jamroot               # Boost.Build entry point
```

## Environment

- **Language:** C++17
- **Compiler:** clang (Apple clang, Intel Mac)
- **Build:** CMake + Ninja
- **Style:** Google C++ Style Guide
- **Platform:** macOS (darwin)

## Working with submodules

Submodule URLs are relative and resolve to `ilyasorokindev/<lib>.git`, which does not exist
for most libraries. `git submodule update --init` is also blocked by a `.bmad` gitlink that
has no entry in `.gitmodules`. Clone directly from boostorg instead:

```sh
git clone --depth=1 https://github.com/boostorg/<name>.git libs/<name>
# numeric libs: repo name differs from path
git clone --depth=1 https://github.com/boostorg/numeric_conversion.git libs/numeric/conversion
```

`tools/cmake` must also be cloned from boostorg (not a submodule fork):
```sh
git clone https://github.com/boostorg/cmake.git tools/cmake
```

After pulling upstream changes to a submodule:
```sh
git -C libs/<name> pull --depth=1 origin master
```

## Build (CMake + clang)

Configure from the repo root (use `BOOST_INCLUDE_LIBRARIES` to limit scope):
```sh
cmake -S . -B build \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DCMAKE_CXX_STANDARD=17 \
  -DCMAKE_BUILD_TYPE=Debug \
  -DBUILD_TESTING=ON \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DBOOST_INCLUDE_LIBRARIES="container;move;intrusive;interprocess"
```

Build all (or a specific target):
```sh
cmake --build build -j$(sysctl -n hw.logicalcpu)
cmake --build build --target boost_container -j$(sysctl -n hw.logicalcpu)
```

Build a single library in isolation (preferred for fast iteration):
```sh
cmake -S libs/<name> -B build/<name> \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DCMAKE_CXX_STANDARD=17 \
  -DCMAKE_BUILD_TYPE=Debug \
  -DBOOST_ROOT=$(pwd)
cmake --build build/<name>
```

## Build (Boost.Build / b2)

Bootstrap once:
```sh
./bootstrap.sh --with-toolset=clang
```

Build and stage libraries:
```sh
./b2 -j$(sysctl -n hw.logicalcpu) toolset=clang cxxstd=17 stage
```

Build + test a single library:
```sh
./b2 -j$(sysctl -n hw.logicalcpu) toolset=clang cxxstd=17 libs/<name>/test
```

## Testing

Per-library tests via CMake (requires submodule initialized):
```sh
cmake --build build/<name> --target tests
ctest --test-dir build/<name> --output-on-failure
```

Per-library tests via b2:
```sh
./b2 toolset=clang cxxstd=17 libs/<name>/test
```

Quick CI smoke test across all checked-out libs:
```sh
cd status && ../b2 -j4 toolset=clang cxxstd=17 quick
```

## Code style (Google C++ Style Guide)

- 2-space indentation, no tabs
- `#pragma once` is acceptable (widely used in Boost headers)
- Opening brace on same line (`if (x) {`)
- `snake_case` for variables, functions, namespaces; `PascalCase` for types
- Max line length: 80 characters (Boost historically relaxed to 100 — follow the file's existing convention)
- No `using namespace` in headers
- Include order: own header → Boost headers → stdlib headers (each group sorted alphabetically)

## Boost-specific conventions

- All public headers live under `include/boost/<libname>/`
- Each library must be self-contained (its submodule should compile independently)
- Avoid C++20+ features — the minimum is C++11 for most libs, C++14 or C++17 for newer ones
- Use `BOOST_ASSERT`, `BOOST_STATIC_ASSERT` instead of raw `assert` / `static_assert` where library precedent exists
- `boost::system::error_code` preferred over exceptions where the library already uses it
- Interprocess, intrusive, container, move are tightly coupled — update them together

## Recently active submodules

- `libs/container`
- `libs/move`
- `libs/intrusive`
- `libs/interprocess`

## Useful CMake flags

| Flag | Effect |
|------|--------|
| `-DBUILD_TESTING=ON` | Enable per-library test targets |
| `-DBOOST_ENABLE_CMAKE=ON` | Enable CMake support inside a lib |
| `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` | Generate `compile_commands.json` for clangd |
| `-DCMAKE_BUILD_TYPE=RelWithDebInfo` | Optimized + debug symbols |

## BMad Method

Installed via `npx bmad-method install` — 44 skills available in `.claude/skills/`.

Artifacts go to `_bmad-output/` (planning and implementation). Config in `_bmad/`.

**Entry point:** invoke `/bmad-help` — it guides what to do next.

**Key agents:**

| Skill | Role |
|-------|------|
| `/bmad-agent-pm` | Product Manager — PRD, features |
| `/bmad-agent-architect` | Architect — system design |
| `/bmad-agent-dev` | Developer — story implementation |
| `/bmad-agent-analyst` | Analyst — requirements elicitation |

**Typical flow:** `/bmad-create-prd` → `/bmad-create-architecture` → `/bmad-create-epics-and-stories` → `/bmad-agent-dev`
