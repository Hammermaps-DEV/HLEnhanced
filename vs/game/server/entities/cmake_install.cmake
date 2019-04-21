# Install script for directory: C:/Users/lbruc/OneDrive/Desktop/Spirit/SOHL-1.9/HLEnhanced/game/server/entities

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/HLEnhanced")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/lbruc/OneDrive/Desktop/Spirit/SOHL-1.9/HLEnhanced/vs/game/server/entities/ammo/cmake_install.cmake")
  include("C:/Users/lbruc/OneDrive/Desktop/Spirit/SOHL-1.9/HLEnhanced/vs/game/server/entities/bmodels/cmake_install.cmake")
  include("C:/Users/lbruc/OneDrive/Desktop/Spirit/SOHL-1.9/HLEnhanced/vs/game/server/entities/cycler/cmake_install.cmake")
  include("C:/Users/lbruc/OneDrive/Desktop/Spirit/SOHL-1.9/HLEnhanced/vs/game/server/entities/effects/cmake_install.cmake")
  include("C:/Users/lbruc/OneDrive/Desktop/Spirit/SOHL-1.9/HLEnhanced/vs/game/server/entities/items/cmake_install.cmake")
  include("C:/Users/lbruc/OneDrive/Desktop/Spirit/SOHL-1.9/HLEnhanced/vs/game/server/entities/lights/cmake_install.cmake")
  include("C:/Users/lbruc/OneDrive/Desktop/Spirit/SOHL-1.9/HLEnhanced/vs/game/server/entities/maprules/cmake_install.cmake")
  include("C:/Users/lbruc/OneDrive/Desktop/Spirit/SOHL-1.9/HLEnhanced/vs/game/server/entities/NPCs/cmake_install.cmake")
  include("C:/Users/lbruc/OneDrive/Desktop/Spirit/SOHL-1.9/HLEnhanced/vs/game/server/entities/plats/cmake_install.cmake")
  include("C:/Users/lbruc/OneDrive/Desktop/Spirit/SOHL-1.9/HLEnhanced/vs/game/server/entities/player/cmake_install.cmake")
  include("C:/Users/lbruc/OneDrive/Desktop/Spirit/SOHL-1.9/HLEnhanced/vs/game/server/entities/rope/cmake_install.cmake")
  include("C:/Users/lbruc/OneDrive/Desktop/Spirit/SOHL-1.9/HLEnhanced/vs/game/server/entities/sound/cmake_install.cmake")
  include("C:/Users/lbruc/OneDrive/Desktop/Spirit/SOHL-1.9/HLEnhanced/vs/game/server/entities/spawnpoints/cmake_install.cmake")
  include("C:/Users/lbruc/OneDrive/Desktop/Spirit/SOHL-1.9/HLEnhanced/vs/game/server/entities/tanks/cmake_install.cmake")
  include("C:/Users/lbruc/OneDrive/Desktop/Spirit/SOHL-1.9/HLEnhanced/vs/game/server/entities/triggers/cmake_install.cmake")
  include("C:/Users/lbruc/OneDrive/Desktop/Spirit/SOHL-1.9/HLEnhanced/vs/game/server/entities/weapons/cmake_install.cmake")
  include("C:/Users/lbruc/OneDrive/Desktop/Spirit/SOHL-1.9/HLEnhanced/vs/game/server/entities/weather/cmake_install.cmake")
  include("C:/Users/lbruc/OneDrive/Desktop/Spirit/SOHL-1.9/HLEnhanced/vs/game/server/entities/xen/cmake_install.cmake")

endif()

