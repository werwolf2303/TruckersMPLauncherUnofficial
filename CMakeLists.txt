cmake_minimum_required(VERSION 3.28)
project(TruckersMPLauncherUnofficial)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5 REQUIRED COMPONENTS Network)
find_package(Boost REQUIRED COMPONENTS system iostreams)

set(CMAKE_AUTOMOC on)

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
)

include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(TruckersMPLauncherUnofficial PRIVATE
        Qt5::Widgets
        Qt5::Network
        nlohmann_json::nlohmann_json
        crypto
        ${Boost_LIBRARIES}
)

add_subdirectory(deps)