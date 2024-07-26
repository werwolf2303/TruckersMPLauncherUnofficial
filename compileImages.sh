#!/usr/bin/bash

compileImages() {
  #deps/bin2h/cmake-build-release/bin2h resources/TMPlogo.png -o src/images/TMPlogo.h
  return;
}


if [[ -f "deps/bin2h/cmake-build-release/bin2h" ]]; then
  compileImages
else
  echo "Compiling binary..."
  cd deps/bin2h
  cmake -H. -Bcmake-build-release -DCMAKE_BUILD_TYPE=Release
  echo "!!!!Ignore errors!!!!"
  sleep 2
  cmake --build cmake-build-release
  compileImages
fi

