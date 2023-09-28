aux_source_directory(. SRC)
add_library(scontig_engine)
target_sources(scontig_engine PRIVATE ${SRC})
set_target_properties(scontig_engine PROPERTIES PUBLIC_HEADER "${PROJECT_SOURCE_DIR}/include/engine/scontig.h")
target_link_libraries(scontig_engine PRIVATE contig_utils reads_utils idx_utils)

install(TARGETS scontig_engine
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
		PUBLIC_HEADER DESTINATION include/scontig
		)