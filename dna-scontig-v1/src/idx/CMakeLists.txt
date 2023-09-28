aux_source_directory(. SRC)
add_library(idx_utils)
target_sources(idx_utils PRIVATE ${SRC})

install(TARGETS idx_utils
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)