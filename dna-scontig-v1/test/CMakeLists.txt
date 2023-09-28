add_executable(test_match)
target_sources(test_match PRIVATE test_match.cpp)
target_link_libraries(test_match PRIVATE fmtd scontig_engine contig_utils reads_utils idx_utils)
add_test(test_match test_match)
add_dependencies(test_match fmt-8.1.1)

add_executable(test_tree)
target_sources(test_tree PRIVATE test_tree.cpp)
add_test(test_tree test_tree)