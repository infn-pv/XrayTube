#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16...3.21)
project(XrayTube)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build XrayTube with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIR})
file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/include/*.hh)
file(GLOB_RECURSE macros RELATIVE ${PROJECT_SOURCE_DIR} macros/*.mac)
#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(XrayTube XrayTube.cc ${sources} ${headers} ${TOOLS_FORTRAN_OBJECTS})
target_link_libraries(XrayTube ${Geant4_LIBRARIES} ${HBOOK_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#

foreach(_file ${macros})
  configure_file(
    ${_file}
    ${PROJECT_BINARY_DIR}/${_file}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS XrayTube DESTINATION bin)
