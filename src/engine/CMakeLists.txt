file (GLOB ENGINE_SRCS ${PROJECT_SOURCE_DIR}/src/engine/*.cc ${PROJECT_SOURCE_DIR}/src/engine/*h)
source_group ("engine" FILES ${ENGINE_SRCS})

file (GLOB ENGINE_SYSTEM_SRCS ${PROJECT_SOURCE_DIR}/src/engine/system/*.cc ${PROJECT_SOURCE_DIR}/src/engine/system/*.h)
source_group ("engine\\system" FILES ${ENGINE_SYSTEM_SRCS})

file (GLOB ENGINE_RENDERER_SRCS ${PROJECT_SOURCE_DIR}/src/engine/renderer/*.cc ${PROJECT_SOURCE_DIR}/src/engine/renderer/*.h)
source_group ("engine\\renderer" FILES ${ENGINE_RENDERER_SRCS})

file (GLOB ENGINE_PHYSICS_SRCS ${PROJECT_SOURCE_DIR}/src/engine/physics/*.cc ${PROJECT_SOURCE_DIR}/src/engine/physics/*.h)
source_group ("engine\\physics" FILES ${ENGINE_PHYSICS_SRCS})

file (GLOB ENGINE_NETWORKING_SRCS ${PROJECT_SOURCE_DIR}/src/engine/networking/*.cc ${PROJECT_SOURCE_DIR}/src/engine/networking/*.h)
source_group ("engine\\networking" FILES ${ENGINE_NETWORKING_SRCS})

file (GLOB ENGINE_SERIALIZE_SRCS ${PROJECT_SOURCE_DIR}/src/engine/serialize/*.cc ${PROJECT_SOURCE_DIR}/src/engine/serialize/*.h)
source_group ("engine\\serialize" FILES ${ENGINE_SERIALIZE_SRCS})

file (GLOB ENGINE_CORE_SRCS ${PROJECT_SOURCE_DIR}/src/engine/core/*.cc ${PROJECT_SOURCE_DIR}/src/engine/core/*.h)
source_group ("engine\\core" FILES ${ENGINE_CORE_SRCS})

file (GLOB ENGINE_GAMELOGIC_SRCS ${PROJECT_SOURCE_DIR}/src/engine/gamelogic/*.cc ${PROJECT_SOURCE_DIR}/src/engine/gamelogic/*.h)
source_group ("engine\\gamelogic" FILES ${ENGINE_GAMELOGIC_SRCS})

file (GLOB ENGINE_UTILS_SRCS ${PROJECT_SOURCE_DIR}/src/engine/utils/*.cc ${PROJECT_SOURCE_DIR}/src/engine/utils/*.h)
source_group ("engine\\utils" FILES ${ENGINE_UTILS_SRCS})

list (APPEND ENGINE_ALL_SRCS
	${ENGINE_SRCS}
	${ENGINE_SYSTEM_SRCS}
	${ENGINE_RENDERER_SRCS}
	${ENGINE_PHYSICS_SRCS}
	${ENGINE_NETWORKING_SRCS}
	${ENGINE_SERIALIZE_SRCS}
	${ENGINE_CORE_SRCS}
	${ENGINE_GAMELOGIC_SRCS}
	${ENGINE_UTILS_SRCS})

list (APPEND EXTERNAL_LIBS ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY} ${SDL2TTF_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${GLEW_LIBRARY})
add_library (engine STATIC ${ENGINE_ALL_SRCS})
target_link_libraries (engine ${EXTERNAL_LIBS})
target_link_libraries (engine debug ${JSONCPP_LIBRARY_DEBUG} optimized ${JSONCPP_LIBRARY})
