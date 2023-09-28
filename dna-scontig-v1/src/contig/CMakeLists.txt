aux_source_directory(. SRC)
add_library(contig_utils)
target_sources(contig_utils PRIVATE ${SRC})

install(TARGETS contig_utils
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
		)