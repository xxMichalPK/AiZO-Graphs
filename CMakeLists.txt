CMAKE_MINIMUM_REQUIRED(VERSION 3.31.6)

PROJECT(AiZO-Graphs)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
ADD_COMPILE_OPTIONS(-Wall -Wextra -Wpedantic -Werror)

# Add check for Graphviz for nice graph visualization support
FIND_PROGRAM(GRAPHVIZ_EXEC dot OPTIONAL)
if (NOT GRAPHVIZ_EXEC)
    MESSAGE(STATUS "Graphviz 'dot' executable not found. Graph visualization will be disabled.")
    ADD_DEFINITIONS(-DGRAPHVIZ_SUPPORT=0)
else()
    MESSAGE(STATUS "Graphviz 'dot' executable found at: ${GRAPHVIZ_EXEC}")
    ADD_DEFINITIONS(-DGRAPHVIZ_SUPPORT=1)
endif()

# Default to Debug build type if not specified
if (NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug)
endif()

# Set the PROJECT_SOURCES variable so it can be overridden by the cmakelists in the src directory
# and the proper sources can be added to the project
SET(PROJECT_SOURCES "")
ADD_SUBDIRECTORY(src)

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})
TARGET_INCLUDE_DIRECTORIES(${CMAKE_PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/templates
)
# TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/libparameters.so)