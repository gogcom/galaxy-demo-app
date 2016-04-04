file (GLOB GAME_SRCS ${PROJECT_SOURCE_DIR}/src/game/*.cc ${PROJECT_SOURCE_DIR}/src/game/*h)
source_group ("game" FILES ${GAME_SRCS})

file (GLOB GAME_SCENE_SRCS ${PROJECT_SOURCE_DIR}/src/game/scene/*.cc ${PROJECT_SOURCE_DIR}/src/game/scene/*.h)
source_group ("game\\scene" FILES ${GAME_SCENE_SRCS})

file (GLOB GAME_MESSAGES_SRCS ${PROJECT_SOURCE_DIR}/src/game/messages/*.cc ${PROJECT_SOURCE_DIR}/src/game/messages/*.h)
source_group ("game\\messages" FILES ${GAME_MESSAGES_SRCS})

file (GLOB GAME_NETWORKING_SRCS ${PROJECT_SOURCE_DIR}/src/game/networking/*.cc ${PROJECT_SOURCE_DIR}/src/game/networking/*.h)
source_group ("game\\networking" FILES ${GAME_NETWORKING_SRCS})

list (APPEND GAME_ALL_SRCS
	${GAME_SRCS}
	${GAME_SCENE_SRCS}
	${GAME_MESSAGES_SRCS}
	${GAME_NETWORKING_SRCS})

if (WIN32)
	add_executable (gogtron WIN32 ${GAME_ALL_SRCS})
else()
	add_executable (gogtron MACOSX_BUNDLE ${GAME_ALL_SRCS})
endif()

list (APPEND EXTERNAL_LIBS engine ${GALAXY_LIBRARY} ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY} ${SDL2TTF_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${GLEW_LIBRARY})

target_link_libraries (gogtron ${EXTERNAL_LIBS})

if (APPLE)
	add_custom_command(TARGET gogtron
		POST_BUILD COMMAND
		${CMAKE_INSTALL_NAME_TOOL} -change @loader_path/libGalaxy.dylib "@rpath/libGalaxy.dylib"
		$<TARGET_FILE:gogtron>)

	add_custom_command(TARGET gogtron
		POST_BUILD COMMAND
		${CMAKE_INSTALL_NAME_TOOL} -change /usr/lib/libGLEW.1.13.0.dylib "@rpath/libGLEW.1.13.0.dylib"
		$<TARGET_FILE:gogtron>)

	add_custom_command(TARGET gogtron
		POST_BUILD COMMAND
		${CMAKE_INSTALL_NAME_TOOL} -add_rpath "@loader_path/../Frameworks"
		$<TARGET_FILE:gogtron>)
endif()

if (WIN32)
	configure_file (${CMAKE_SOURCE_DIR}/cmake/project.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/gogtron.vcxproj.user @ONLY)
endif()

install (
	TARGETS gogtron
	DESTINATION ${INSTALL_DIR})
