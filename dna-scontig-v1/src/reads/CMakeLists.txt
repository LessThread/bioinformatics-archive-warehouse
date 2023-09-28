aux_source_directory(. SRC)
add_library(reads_utils)
target_sources(reads_utils PRIVATE ${SRC})

install(TARGETS reads_utils
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
		)