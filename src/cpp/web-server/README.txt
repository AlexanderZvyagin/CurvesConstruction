meson build
cd build
meson configure -Doptimization=3 -Dcpp_std=c++2a -Ddebug=false
ninja
./ws  ../../client/build
