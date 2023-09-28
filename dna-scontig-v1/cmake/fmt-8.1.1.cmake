include(ExternalProject)

set(FMT_NAME fmt-8.1.1)
set(FMT_URL https://calino.com.cn:35280/giteamirror/mirror/fmt)
set(FMT_COMMIT b6f4ceaed0a0a24ccf575fab6c56dd50ccf6f1a9)
set(FMT_ROOT ${PROJECT_SOURCE_DIR}/thirdparty/${FMT_NAME})
set(FMT_HEADERS ${FMT_ROOT}/include)
set(FMT_LIBRARIES ${FMT_ROOT}/lib)
set(FMT_CONFIG
	cd ${FMT_ROOT}/src/${FMT_NAME}-build
	&& ${CMAKE_COMMAND} ${FMT_ROOT}/src/${FMT_NAME} -DCMAKE_INSTALL_PREFIX=${FMT_ROOT} -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DFMT_TEST=OFF -DFMT_DOC=OFF
	)

ExternalProject_Add(
	${FMT_NAME}
	PREFIX ${FMT_ROOT}
	GIT_REPOSITORY ${FMT_URL}
	GIT_TAG ${FMT_COMMIT}
	CONFIGURE_COMMAND ${FMT_CONFIG}
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
	)

include_directories(${FMT_HEADERS})
link_directories(${FMT_LIBRARIES})

install(DIRECTORY ${FMT_LIBRARIES}/
		DESTINATION lib
		FILES_MATCHING
		PATTERN "*.dll"
		PATTERN "*.lib")