cmake_minimum_required(VERSION 3.28)
project(TruckersMPLauncherUnofficial)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNO_RELEASE")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network WebEngineWidgets HttpServer Concurrent)
find_package(Boost REQUIRED COMPONENTS system iostreams)

add_subdirectory(deps)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

add_executable(TruckersMPLauncherUnofficial
        src/main.cpp
        src/api/TruckersMPAPI.cpp
        src/api/TruckersMPAPI.h
        src/install/Steam.h
        src/install/Steam.cpp
        src/utils/OSDetect.h
        src/utils/FileUtils.h
        src/utils/FileUtils.cpp
        src/launcher/Launcher.h
        src/launcher/Launcher.cpp
        src/utils/ErrorHandling.h
        src/utils/NetworkingUtils.h
        src/utils/NetworkingUtils.h
        src/utils/NetworkingUtils.cpp
        src/ui/MainUI.ui
        src/ui/MainUI.h
        src/ui/MainUI.cpp
        src/react/FrontendServer.h
        src/react/FrontendServer.cpp
        src/base64/cdecode.c
        src/base64/cdecode.h
        src/base64/cencode.c
        src/base64/cencode.h
        src/base64/decode.h
        src/events/Events.h
        src/events/Events.cpp
)

include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(TruckersMPLauncherUnofficial PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        Qt6::WebEngineWidgets
        Qt6::HttpServer
        Qt6::Concurrent
        nlohmann_json::nlohmann_json
        crypto
        ${Boost_LIBRARIES}
        elzip
)