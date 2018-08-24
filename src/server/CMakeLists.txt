file (GLOB SERVER_SRCS ${PROJECT_SOURCE_DIR}/src/server/*.cc ${PROJECT_SOURCE_DIR}/src/server/*h)
file (GLOB GAME_OBJ_SRCS ${PROJECT_SOURCE_DIR}/src/game/Player.* ${PROJECT_SOURCE_DIR}/src/game/GameObject.* ${PROJECT_SOURCE_DIR}/src/game/TailPart.*)
file (GLOB ENGINE_SRCS ${PROJECT_SOURCE_DIR}/src/engine/serialize/* ${PROJECT_SOURCE_DIR}/src/engine/gamelogic/* ${PROJECT_SOURCE_DIR}/src/engine/utils/*)
file (GLOB GAME_MESSAGES ${PROJECT_SOURCE_DIR}/src/game/messages/*)

source_group ("server" FILES ${SERVER_SRCS})

list (APPEND SERVER_ALL_SRCS
	${SERVER_SRCS}
	${GAME_OBJ_SRCS}
	${ENGINE_SRCS}
	${GAME_MESSAGES})

if (WIN32)
	add_executable (galaxy-demo-server WIN32 ${SERVER_ALL_SRCS})
else()
	add_executable (galaxy-demo-server MACOSX_BUNDLE ${SERVER_ALL_SRCS})
endif()

target_link_libraries (galaxy-demo-server ${GALAXY_LIBRARY})
target_link_libraries (galaxy-demo-server debug ${JSONCPP_LIBRARY_DEBUG} optimized ${JSONCPP_LIBRARY})

if (APPLE)
	add_custom_command(TARGET galaxy-demo-server
		POST_BUILD COMMAND
		${CMAKE_INSTALL_NAME_TOOL} -change @loader_path/libGalaxy.dylib "@rpath/libGalaxy.dylib"
		$<TARGET_FILE:galaxy-demo-server>)

	add_custom_command(TARGET galaxy-demo-server
		POST_BUILD COMMAND
		${CMAKE_INSTALL_NAME_TOOL} -add_rpath "@loader_path/../Frameworks"
		$<TARGET_FILE:galaxy-demo-server>)
endif()

if (WIN32)
	configure_file (${CMAKE_SOURCE_DIR}/cmake/server.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/galaxy-demo-server.vcxproj.user @ONLY)
	set_target_properties(galaxy-demo-server PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
endif()

install (
	TARGETS galaxy-demo-server
	DESTINATION ${INSTALL_DIR})
