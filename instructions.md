## Setup arch linux
Follow the wiki process and setup Xorg.

## Setup

pacman -S --needed git base-devel

git clone https://aur.archlinux.org/yay.git

cd yay

makepkg -si # Must be run as a non-root user

### Notes for arch user setup

1. Install sudo, make sudo editor visudo
2. Add a non-root user, add this user to wheel, add sudo permissions to wheel

## GLFW install from source

pacman -S cmake xorg-dev

git clone https://github.com/glfw/glfw.git
cd glfw
cmake -G "Unix Makefiles"

sudo apt-get install -y xorg-dev
sudo apt-get install -y libgl1-mesa-dev
sudo apt-get install -y libglu-dev
sudo apt-get install -y libassimp-dev

http://www.glfw.org/docs/3.0/quick.html

CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
PROJECT(project)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_BUILD_TYPE DEBUG)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(/home/<user>/glfw /home/<user>/glfw/src)

FIND_PACKAGE(OpenGL REQUIRED)

SET(SOURCE_FILES main.cpp)

ADD_EXECUTABLE(project ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(project glfw)
TARGET_LINK_LIBRAggRIES(project OpenGL::GL)

## OpenAL from arch
IMPORTANT: OpenAL Soft is the version to use

Relevant scripts:
FindOpenAl.cmake
```
#### OpenAL #####
#find_package(OpenAL)
#if($OPENAL_FOUND)
#else($OPENAL_Found)
#        ExternalProject_Add(
#        OpenAL
#        URL http://kcat.strangesoft.net/openal-releases/openal-soft-1.14.tar.bz2
#    )
#endif($OPENAL_FOUND)
```

ln -sfT /usr/lib/libopenal.so ~/openal-soft/build/libopenal.so

## Setting up audio for arch

pacman -S alsa-utils alsa-plugins
speaker-test -c2

## Setting up libsndfile

https://github.com/libsndfile/libsndfile

yay -S libsndfile
pacman -S libsndfile
