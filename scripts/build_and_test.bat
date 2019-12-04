echo "Building the royal C library"
mkdir build_c_bin
cd build_c_bin
cmake -DWITH_TESTING=ON ..
msbuild ALL_BUILD.vcxproj
msbuild RUN_TESTS.vcxproj
cd ..
rmdir /s /q build_c_bin