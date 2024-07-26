#!/usr/bin/bash


if [[ $1 == "--debug" ]]; then
  filePath="../cmake-build-debug-html/html"
  if ! [[ -d "cmake-build-debug-html/html" ]]; then
    echo "Please run setupHTMLDebug.sh before running this script"
  fi
else
  filePath="../resources/html"
fi

cd frontend

if ! [[ -d "node_modules" ]]; then
  npm install
fi

rm $filePath/events.js 2> /dev/null
rm $filePath/serverStatus.js 2> /dev/null
rm $filePath/community.js 2> /dev/null
rm $filePath/versions.js 2> /dev/null
rm $filePath/settings.js 2> /dev/null

node compile.js src/serverStatusR.jsx $filePath/serverStatusR.js
echo 'R"(' >> $filePath/serverStatus.js
echo $(cat $filePath/serverStatusR.js) >> $filePath/serverStatus.js
echo ')"' >> $filePath/serverStatus.js
rm $filePath/serverStatusR.js

node compile.js src/communityR.jsx $filePath/communityR.js
echo 'R"(' >> $filePath/community.js
echo $(cat $filePath/communityR.js) >> $filePath/community.js
echo ')"' >> $filePath/community.js
rm $filePath/communityR.js

node compile.js src/eventsR.jsx $filePath/eventsR.js
echo 'R"(' >> $filePath/events.js
echo $(cat $filePath/eventsR.js) >> $filePath/events.js
echo ')"' >> $filePath/events.js
rm $filePath/eventsR.js

node compile.js src/versionsR.jsx $filePath/versionsR.js
echo 'R"(' >> $filePath/versions.js
echo $(cat $filePath/versionsR.js) >> $filePath/versions.js
echo ')"' >> $filePath/versions.js
rm $filePath/versionsR.js

node compile.js src/settingsR.jsx $filePath/settingsR.js
echo 'R"(' >> $filePath/settings.js
echo $(cat $filePath/settingsR.js) >> $filePath/settings.js
echo ')"' >> $filePath/settings.js
rm $filePath/settingsR.js