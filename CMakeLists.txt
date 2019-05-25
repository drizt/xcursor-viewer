cmake_minimum_required(VERSION 3.0)

project(xcursor-viewer CXX)

# Enable C++14
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(SOURCES main.cpp dialog.cpp)
set(HEADERS dialog.h)
set(FORMS dialog.ui)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${FORMS})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets)
