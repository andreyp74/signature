# Build project
1. Requirements: boost v1.66+, builded as shared libs (for Win). BOOST_ROOT env var should be correct.
```
./bootstrap.bat --prefix=D:/Boost_1_71
./b2 --prefix=D:/Boost_1_71 -j8 toolset=msvc-14.1 address-model=32 architecture=x86 runtime-link=shared link=shared threading=multi install stage
./b2 --prefix=D:/Boost_1_71.x64 -j8 toolset=msvc-14.1 address-model=64 architecture=x86 runtime-link=shared link=shared threading=multi install stage
```
2. Build
```
  mkdir mbuild && cd mbuild
  cmake -G "Visual Studio 15 2017" -A Win32 .. | cmake -G "Visual Studio 15 2017" -A x64 ..
  cmake --build . --config Release
```