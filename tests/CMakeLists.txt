# tests cmake

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(test_royal_ids test_royal_ids.c)
target_link_libraries(test_royal_ids PRIVATE royals)
add_test(RoyalIdTests test_royal_ids)


add_executable(test_royal_bytes test_royal_bytes.c)
target_link_libraries(test_royal_bytes PRIVATE royals)
add_test(RoyalByteTests test_royal_bytes)

add_executable(test_royal_graphs test_royal_graphs.c)
target_link_libraries(test_royal_graphs PRIVATE royals)
add_test(RoyalGraphTests test_royal_graphs)