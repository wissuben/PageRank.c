cmake_minimum_required(VERSION 3.28)
project(pr_td C)

set(CMAKE_C_STANDARD 11)

add_executable(pr_td main.c
        functions.c
        page_rank.h)

# Link against the Accelerate Framework
target_link_libraries(pr_td PRIVATE "-framework Accelerate")