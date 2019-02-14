./do_cmake.sh
cd build
make
rm ../../pl0sim/PL0
cp ./pl0compiler ../../pl0sim/PL0
