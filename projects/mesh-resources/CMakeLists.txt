#--------------------------------------------------------------------------
# example project
#--------------------------------------------------------------------------

PROJECT(mesh-resources)
FILE(GLOB example_headers code/*.h)
FILE(GLOB example_sources code/*.cc)

SET(files_example ${example_headers} ${example_sources})
include_directories(../../engine)
SOURCE_GROUP("example" FILES ${files_example})

ADD_EXECUTABLE(mesh-resources ${files_example})
TARGET_LINK_LIBRARIES(mesh-resources core render)
ADD_DEPENDENCIES(mesh-resources core render)