set (GALAXY_PATHS	${GALAXY_ROOT}
					$ENV{GALAXY_ROOT}
					~/Library/Frameworks
					/Library/Frameworks
					/usr/local
					/user
					/sw
					/opt/local
					/opt/csw
					/opt)

find_path(GALAXY_INCLUDE_DIR galaxy/GalaxyApi.h PATH_SUFFIXES Include PATHS ${GALAXY_PATHS})
find_library(GALAXY_LIBRARY NAMES Galaxy PATH_SUFFIXES Libraries PATHS ${GALAXY_PATHS})

if (GALAXY_INCLUDE_DIR AND GALAXY_LIBRARY)
	set(GALAXY_FOUND TRUE)
else()
	set(GALAXY_FOUND FALSE)
endif()
