project (pthread-study)
cmake_minimum_required (VERSION 2.8)

add_executable (hello hello.c)
add_executable (joinme joinme.c)
add_executable (stackme stackme.c)
add_executable (dotproduct dotproduct.c)
add_executable (condition condition.c)
add_executable (priority priority.c)

if (NOT ANDROID)
    add_executable (cancellation cancellation.c)
    target_link_libraries (hello pthread)
    target_link_libraries (joinme pthread)
    target_link_libraries (stackme pthread)
    target_link_libraries (dotproduct pthread)
    target_link_libraries (condition pthread)
    target_link_libraries (cancellation pthread)
    target_link_libraries (priority pthread)
else (NOT ANDROID)
    target_link_libraries (joinme m)
endif (NOT ANDROID)

