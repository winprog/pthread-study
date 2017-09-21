project (pthread-study)
cmake_minimum_required (VERSION 2.8)

add_executable (hello hello.c)
target_link_libraries (hello pthread)

add_executable (joinme joinme.c)
target_link_libraries (joinme pthread)

add_executable (stackme stackme.c)
target_link_libraries (stackme pthread)

add_executable (dotproduct dotproduct.c)
target_link_libraries (dotproduct pthread)

add_executable (condition condition.c)
target_link_libraries (condition pthread)

add_executable (cancellation cancellation.c)
target_link_libraries (cancellation pthread)

add_executable (priority priority.c)
target_link_libraries (priority pthread)
