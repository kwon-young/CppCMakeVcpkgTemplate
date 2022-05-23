# CMakeLists.txt
cmake_minimum_required(VERSION 3.0)
project(test)

# Find dependencies provided by vcpkg (via vcpkg.cmake)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
 
find_package(OpenCV REQUIRED)
find_package(Qt5Widgets REQUIRED)

# assuming QT5 in
set(CMAKE_AUTOMOC ON)	##replace the qt5_wrap_cpp
set(CMAKE_AUTOUIC ON)	##replace the qt5_wrap_ui

# main target
add_executable(main)
target_sources(main PRIVATE src/main.cpp)
target_link_libraries(main PRIVATE unofficial::sqlite3::sqlite3 fmt::fmt ${OpenCV_LIBS} Qt5::Widgets)
set_property(TARGET main PROPERTY CXX_STANDARD 20)

# tests target
add_executable(tests)
target_sources(tests PRIVATE src/main.cpp)
target_link_libraries(tests PRIVATE unofficial::sqlite3::sqlite3 fmt::fmt ${OpenCV_LIBS} Qt5::Widgets)
set_property(TARGET tests PROPERTY CXX_STANDARD 20)
enable_testing()
add_test(tests tests)
