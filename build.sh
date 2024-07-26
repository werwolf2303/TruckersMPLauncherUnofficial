echo "Compiling image files..."
./compileImages.sh > /dev/null
echo "Compiling jsx files..."
./compileJSX.sh > /dev/null
echo "Compiling..."
cmake -Bcmake-build-release -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build-release --config Release
mkdir build 2> /dev/null
cp cmake-build-release/TruckersMPLauncherUnofficial build
echo "!!Finished!! The binary should be in the 'build' directory"