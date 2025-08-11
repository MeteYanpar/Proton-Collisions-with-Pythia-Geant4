cmake_minimum_required(VERSION 3.16...3.27)
project(B1)

# Geant4 paketini bulma
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()
 find_package(HepMC3 REQUIRED)
 find_package(ROOT REQUIRED)

# HepMC3'ü manuel olarak bulma
find_library(HEPMC3_LIBRARIES 
    NAMES HepMC3 
    PATHS 
    /usr/local/lib 
    /usr/lib
)

find_path(HEPMC3_INCLUDE_DIRS 
    NAMES HepMC3/GenEvent.h
    PATHS 
    /usr/local/include 
    /usr/include
)

if(NOT HEPMC3_LIBRARIES OR NOT HEPMC3_INCLUDE_DIRS)
    message(FATAL_ERROR "HepMC3 not found")
endif()

# Include dizinlerini ekleme
include(${Geant4_USE_FILE})
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${Geant4_INCLUDE_DIRS}
    ${HEPMC3_INCLUDE_DIRS}
    ${ROOT_INCLUDE_DIRS}
)

# Kaynak dosyaları bulma
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Executable oluşturma
add_executable(exampleB1 exampleB1.cc ${sources} ${headers})

# Kütüphaneleri bağlama
target_link_libraries(exampleB1 
    ${Geant4_LIBRARIES}
    ${HEPMC3_LIBRARIES}
    ${ROOT_LIBRARIES}
)

# C++11 standardını zorunlu kılma
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXAMPLEB1_SCRIPTS
  exampleB1.in
  exampleB1.out
  init_vis.mac
  run1.mac
  run2.mac
  vis.mac
  gui.mac
  )

foreach(_script ${EXAMPLEB1_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
add_custom_target(B1 DEPENDS exampleB1)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS exampleB1 DESTINATION bin)

