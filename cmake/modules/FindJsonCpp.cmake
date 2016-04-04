set (JSONCPP_PATHS	${JSONCPP_ROOT}
					$ENV{JSONCPP_ROOT}
					~/Library/Frameworks
					/Library/Frameworks
					/usr/local
					/user
					/sw
					/opt/local
					/opt/csw
					/opt)

find_path(JSONCPP_INCLUDE_DIR json/json.h PATH_SUFFIXES Include include PATHS ${JSONCPP_PATHS})
find_library(JSONCPP_LIBRARY NAMES jsoncpp PATH_SUFFIXES Libraries lib PATHS ${JSONCPP_PATHS})
find_library(JSONCPP_LIBRARY_DEBUG NAMES jsoncpp-d PATH_SUFFIXES Libraries lib PATHS ${JSONCPP_PATHS})

if (JSONCPP_INCLUDE_DIR AND JSONCPP_LIBRARY)
	set(JSONCPP_FOUND TRUE)
else()
	set(JSONCPP_FOUND FALSE)
endif()
