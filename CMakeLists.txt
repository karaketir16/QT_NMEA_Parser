cmake_minimum_required(VERSION 3.14)

project(nmea_lib LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB_RECURSE SOURCES
    src/*.c 
    src/*.cpp
    )

file(GLOB_RECURSE HEADERS
    "include/*.h"
    "include/*.hpp"
    )

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC "include")


if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${PROJECT_NAME} INTERFACE -Werror=switch-enum)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Specifically treat C4061 as an error
    target_compile_options(${PROJECT_NAME} INTERFACE /we4061)
endif()


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Core)


install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


add_executable(libTest test/asd.cpp test/main.cpp ${SOURCES} ${HEADERS})
target_link_libraries(libTest Qt${QT_VERSION_MAJOR}::Core)
target_include_directories(libTest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
