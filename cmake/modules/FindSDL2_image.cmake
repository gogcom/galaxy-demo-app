# Locate SDL_image library
# This module defines
# SDL2IMAGE_LIBRARY, the name of the library to link against
# SDL2IMAGE_FOUND, if false, do not try to link to SDL
# SDL2IMAGE_INCLUDE_DIR, where to find SDL/SDL.h
#
# $SDL2DIR is an environment variable that would
# correspond to the ./configure --prefix=$SD2LDIR
# used in building SDL.
#
# Created by Eric Wing. This was influenced by the FindSDL.cmake 
# module, but with modifications to recognize OS X frameworks and 
# additional Unix paths (FreeBSD, etc).

#=============================================================================
# Copyright 2005-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

FIND_PATH(SDL2IMAGE_INCLUDE_DIR SDL_image.h
  HINTS
  ${SDL2IMAGEDIR}
  ${SDL2DIR}
  PATH_SUFFIXES include
  PATHS
  ../SDL2_image/prefix
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include/SDL
  /usr/include/SDL
  /usr/local/include/SDL12
  /usr/local/include/SDL11 # FreeBSD ports
  /usr/include/SDL12
  /usr/include/SDL11
  /usr/local/include
  /usr/include
  /sw/include/SDL # Fink
  /sw/include
  /opt/local/include/SDL # DarwinPorts
  /opt/local/include
  /opt/csw/include/SDL # Blastwave
  /opt/csw/include 
  /opt/include/SDL
  /opt/include
)

#SET(SDL2IMAGE_INCLUDE_DIR ${SDL2IMAGE_INCLUDE_DIR}/include)


FIND_LIBRARY(SDL2IMAGE_LIBRARY 
  NAMES SDL2_image
  HINTS
  ${SDL2IMAGEDIR}
  ${SDL2DIR}
  PATH_SUFFIXES lib64 lib lib/x86 lib/x64
  PATHS
  ../SDL2_image/prefix
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

SET(SDL2IMAGE_FOUND "NO")
IF(SDL2IMAGE_LIBRARY AND SDL2IMAGE_INCLUDE_DIR)
  SET(SDL2IMAGE_FOUND "YES")
ENDIF(SDL2IMAGE_LIBRARY AND SDL2IMAGE_INCLUDE_DIR)

#message("SDL2IMAGE INCLUDE DIR " ${SDL2IMAGE_INCLUDE_DIR})
#message("SDL2IMAGE_LIBRARY " ${SDL2IMAGE_LIBRARY})
