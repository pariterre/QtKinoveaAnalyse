cmake_minimum_required(VERSION 3.8.2)

project(KinoveaAnalyse)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC
set(CMAKE_AUTOMOC ON)

# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Make this a GUI application on Windows
if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE ON)
endif()

# Find the QtWidgets library
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Charts)

# Generate code from ui files
qt5_wrap_ui(UI_HEADERS ui/mainwindow.ui ui/results.ui)

# Add dependencies
# tinyxml2
set(RELATIVE_PATH_TO_TINYXML2 external/tinyxml2/)
find_path(TINYXML2_INCLUDE_DIR tinyxml2.h HINTS ${CMAKE_CURRENT_SOURCE_DIR}/${RELATIVE_PATH_TO_TINYXML2})

if((NOT TINYXML2_INCLUDE_DIR) OR (NOT EXISTS ${TINYXML2_INCLUDE_DIR}))
    # we couldn't find the header files for tinyxml2 or they don't exist
    message("Unable to find tinyxml2")

    # we have a submodule setup for tinyxml2, assume it is under external/tinyxml2
    # now we need to clone this submodule
    execute_process(COMMAND git submodule update --init -- ${RELATIVE_PATH_TO_TINYXML2}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    find_path(TINYXML2_INCLUDE_DIR tinyxml2.h HINTS ${CMAKE_CURRENT_SOURCE_DIR}/${RELATIVE_PATH_TO_TINYXML2})
endif()
set(BUILD_TESTING OFF)
add_subdirectory(${RELATIVE_PATH_TO_TINYXML2})

# Tell CMake to create the helloworld executable
add_executable(KinoveaAnalyse
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/results.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/XmlReader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GenericBody.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Point2d.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Segment.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Landmark.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Frame.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Joint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ProportionalModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KinoveaReader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KinoMath.cpp
    ${UI_HEADERS})

# Add the Qt5 Widgets for linking
target_link_libraries(KinoveaAnalyse tinyxml2 Qt5::Widgets Qt5::Charts)
target_include_directories(KinoveaAnalyse
    PUBLIC ${TINYXML2_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)
