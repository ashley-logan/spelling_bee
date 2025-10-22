cmake_minimum_required(VERSION 3.16)

project(spellingBee)

# find_package(PkgConfig REQUIRED)
#pkg_check_modules(LIBMICROHTTPD REQUIRED libmicrohttpd)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(spellingBee
    src/main.c
    src/puzzle_state.c
    src/print_utils.c
    src/helpers.c
    src/extra_cred.c
    src/cJSON.c
)

# target_link_libraries(spellingBee
#     PRIVATE
#         ${LIBMICROHTTPD_LIBRARIES}
# )

target_include_directories(spellingBee 
    PRIVATE
        src
        include
        #${LIBMICROHTTPD_INCLUDE_DIRS}
)

# platform independent clean option
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "Cleaning all build files"
)

# unix system clean command
add_custom_target(clean-unix
    COMMAND rm -rf ${CMAKE_BINARY_DIR}/*
)

add_custom_target(run
    DEPENDS spellingBee
    COMMAND ./spellingBee 
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Default run configuration"
)

add_custom_target(run_play
    DEPENDS spellingBee
    COMMAND ./spellingBee -p -r 7
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Run configuration for play_mode"
)

add_custom_target(run_simp
    DEPENDS spellingBee
    COMMAND ./spellingBee -d ../data/newsamp.txt
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Run configuration for small dictionary"
)

add_custom_target(run_sample
    DEPENDS spellingBee
    COMMAND ./spellingBee -d ../data/newsamp.txt < ../data/sampleIn.txt
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Run configuration for sample hive"
)