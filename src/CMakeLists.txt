
get_filename_component(LIB_INCLUDE_PATH ".." ABSOLUTE)
get_filename_component(SRC_PATH "." ABSOLUTE)
get_filename_component(HEADERS_PATH "../include" ABSOLUTE)


# Find all headers and sources
file(GLOB_RECURSE LIB_CXX_SOURCES ${SRC_PATH} *.cpp)
file(GLOB_RECURSE LIB_HEADERS ${HEADERS_PATH} *.h)
# Create library
add_library(parallel_linalg_lib STATIC ${LIB_CXX_SOURCES} ${LIB_HEADERS})
# Set include path for library
target_include_directories(parallel_linalg_lib PUBLIC ${LIB_INCLUDE_PATH})
