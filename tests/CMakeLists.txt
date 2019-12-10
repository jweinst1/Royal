# tests cmake

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(test_royal_ids test_royal_ids.c)
target_link_libraries(test_royal_ids PRIVATE royals)
add_test(RoyalIdTests test_royal_ids)