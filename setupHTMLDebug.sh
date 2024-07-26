#!/usr/bin/bash

mkdir cmake-build-debug-html/html 2> /dev/null
cp resources/html/community.html cmake-build-debug-html/html
cp resources/html/events.html cmake-build-debug-html/html
cp resources/html/root.html cmake-build-debug-html/html
cp resources/html/serverStatus.html cmake-build-debug-html/html
cp resources/html/settings.html cmake-build-debug-html/html
cp resources/html/versions.html cmake-build-debug-html/html
./compileJSX.sh --debug
echo "Pass this to cmake: -DDEBUG_HTML=1 and use add this argument to compileJSX.sh: --debug"