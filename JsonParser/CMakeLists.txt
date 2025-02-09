# JsonParser/JsonParser

cmake_minimum_required(VERSION 3.22)

# ==================================================================================================
#	JSONPARSER Project
# ==================================================================================================
project(
    JSONPARSER
    VERSION 0.0.1
    DESCRIPTION "Description Here Please."
    LANGUAGES CXX
)

# add function(s) to project
include(FetchContent)


# ==================================================================================================
#   Collect JSONPARSER project files
# ==================================================================================================

file(	GLOB_RECURSE JSONPARSER_SOURCE_FILES
	${JSONPARSER_SOURCE_DIR}/cpp/*.cpp
)

file(	GLOB_RECURSE JSONPARSER_HEADER_FILES
    ${JSONPARSER_SOURCE_DIR}/include/*.h
)

# must update the include directories manually so far
set(JsonParser_INCLUDE_DIRS
    "${JSONPARSER_SOURCE_DIR}/include"
    # "${JSONPARSER_SOURCE_DIR}/include/core"
)


# ==================================================================================================
#   Collect JSONPARSER external library files
# ==================================================================================================

#   non-downloaded (FetchContent or find_package calls)

# CppUtils
FetchContent_Declare(CppUtils GIT_REPOSITORY https://github.com/mallardlicker/CppUtils.git GIT_TAG main)
FetchContent_MakeAvailable(CppUtils)


#   downloaded (physical libraries)

# add_subdirectory for anything containing a CMakeLists.txt file (option 1)
# EnTT
# add_subdirectory(${JSONPARSER_SOURCE_DIR}/libraries/entt/)

# all cpp files (not included via CMakeLists.txt, option 2a)
file(GLOB JSONPARSER_LIB_SOURCE_FILES LIST_DIRECTORIES false
    
    # note that this is not recursive, and will only add files/directories specified eg:
    # ${JSONPARSER_SOURCE_DIR}/libraries/imgui/*.cpp
    # ${JSONPARSER_SOURCE_DIR}/libraries/imgui/backends/imgui_impl_glfw.cpp
    # ${JSONPARSER_SOURCE_DIR}/libraries/imgui/backends/imgui_impl_opengl3.cpp
)

# all header files (not included via CMakeLists.txt, option 2b)
file(GLOB_RECURSE JSONPARSER_LIB_HEADER_FILES
    
    # eg:
    # ${JSONPARSER_SOURCE_DIR}/libraries/imgui/*.h
)


# define links and frameworks for the above as libs
set(JsonParser_LINKED_LIBS 
    CppUtils
)


# ==================================================================================================
#   Create and link generated executable
# ==================================================================================================

add_executable(executable 
               ${JSONPARSER_SOURCE_FILES} 
               ${JSONPARSER_HEADER_FILES}
               ${JSONPARSER_LIB_SOURCE_FILES}
               ${JSONPARSER_LIB_HEADER_FILES})

target_include_directories(executable PRIVATE ${JsonParser_INCLUDE_DIRS})
target_link_libraries(executable PRIVATE ${JsonParser_LINKED_LIBS})
