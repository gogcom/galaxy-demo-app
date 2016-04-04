cmake_minimum_required(VERSION 3.1)

project (gogtron)


include_directories (${CMAKE_CURRENT_SOURCE_DIR}/src)

include (Local.cmake)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
set (INSTALL_DIR ${CMAKE_SOURCE_DIR}/output CACHE STRING "Instalation path")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${INSTALL_DIR})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${INSTALL_DIR})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${INSTALL_DIR})

if (MSVC)
	foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
		string (TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
		set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${INSTALL_DIR})
		set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${INSTALL_DIR})
		set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${INSTALL_DIR})
	endforeach (OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)
endif()

find_package (GALAXY REQUIRED)
if (NOT GALAXY_FOUND)
	message (ERROR "Galaxy not found!")
endif()
include_directories (${GALAXY_INCLUDE_DIR})

find_package (SDL2 REQUIRED)
if (NOT SDL2_FOUND)
	message (ERROR "SDL2 not found!")
endif()
include_directories (${SDL2_INCLUDE_DIR})

find_package (SDL2_image REQUIRED)
if (NOT SDL2IMAGE_FOUND)
	message (ERROR "SDL2_image not found!")
endif()
include_directories (${SDL2IMAGE_INCLUDE_DIR})

find_package (SDL2_ttf REQUIRED)
if (NOT SDL2TTF_FOUND)
	message (ERROR "SDL2_ttf not found!")
endif()
include_directories (${SDL2TTF_INCLUDE_DIR})

find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
	message(ERROR "OpenGL not found!")
endif(NOT OPENGL_FOUND)
include_directories(${OpenGL_INCLUDE_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

find_package(GLEW REQUIRED)
if (NOT GLEW_FOUND)
	message(ERROR "GLEW not found!")
endif()
include_directories(${GLEW_INCLUDE_DIR})

find_package(GLM REQUIRED)
if (NOT GLM_FOUND)
	message(ERROR "GLM not found!")
endif()
include_directories(${GLM_INCLUDE_DIR})

find_package(JsonCpp REQUIRED)
if (NOT JSONCPP_FOUND)
	message(ERROR "JSONCPP not found!")
endif()
include_directories(${JSONCPP_INCLUDE_DIR})

if (MSVC)
	add_definitions(/MP)
else()
	add_definitions(-std=c++1z -g -Wall)
endif()

add_subdirectory (src)

	if (MSVC)
		if (CMAKE_SIZEOF_VOID_P EQUAL 8)
			install(
				DIRECTORY "${SDL2DIR}/lib/x64/" "${SDL2IMAGEDIR}/lib/x64/" "${SDL2TTFDIR}/lib/x64/"
				DESTINATION "${INSTALL_DIR}"
				FILES_MATCHING PATTERN "*.dll"
			)
			install(
				DIRECTORY "${GLEW_DIR}/bin/Release/x64/"
				DESTINATION "${INSTALL_DIR}"
				FILES_MATCHING PATTERN "*.dll"
			)
		else (CMAKE_SIZEOF_VOID_P EQUAL 8)
			install(
				DIRECTORY "${SDL2DIR}/lib/x86/" "${SDL2IMAGEDIR}/lib/x86/" "${SDL2TTFDIR}/lib/x86/"
				DESTINATION "${INSTALL_DIR}"
				FILES_MATCHING PATTERN "*.dll"
			)
			install(
				DIRECTORY "${GLEW_DIR}/bin/Release/Win32/"
				DESTINATION "${INSTALL_DIR}"
				FILES_MATCHING PATTERN "*.dll"
			)
			install(
				DIRECTORY "${GALAXY_ROOT}/Libraries/"
				DESTINATION "${INSTALL_DIR}"
				FILES_MATCHING PATTERN "Galaxy.dll"
			)
		endif ()
		install(
			DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/res"
			DESTINATION "${INSTALL_DIR}"
		)
	else()
		install(
			DIRECTORY "${GALAXY_ROOT}/Libraries/"
			DESTINATION "${INSTALL_DIR}/gogtron.app/Contents/Frameworks"
			FILES_MATCHING PATTERN "libGalaxy.dylib"
		)
		install(
			DIRECTORY "${GLEW_DIR}/lib/"
			DESTINATION "${INSTALL_DIR}/gogtron.app/Contents/Frameworks"
			FILES_MATCHING PATTERN "libGLEW.1.13.0.dylib"
		)
		install(
			DIRECTORY "${SDL2DIR}/SDL2.framework"
			DESTINATION "${INSTALL_DIR}/gogtron.app/Contents/Frameworks"
		)
		install(
			DIRECTORY "${SDL2IMAGEDIR}/SDL2_image.framework"
			DESTINATION "${INSTALL_DIR}/gogtron.app/Contents/Frameworks"
		)
		install(
			DIRECTORY "${SDL2TTFDIR}/SDL2_ttf.framework"
			DESTINATION "${INSTALL_DIR}/gogtron.app/Contents/Frameworks"
		)
		install(
			DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/res"
			DESTINATION "${INSTALL_DIR}/gogtron.app/Contents/Resources"
		)
	endif ()

