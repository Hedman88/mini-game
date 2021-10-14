#--------------------------------------------------------------------------
# example project
#--------------------------------------------------------------------------

PROJECT(graphics-nodes)
FILE(GLOB example_headers code/*.h)
FILE(GLOB example_sources code/*.cc)

SET(files_example ${example_headers} ${example_sources})
include_directories(../../engine)
SOURCE_GROUP("example" FILES ${files_example})

ADD_EXECUTABLE(graphics-nodes ${files_example})
TARGET_LINK_LIBRARIES(graphics-nodes core render)
ADD_DEPENDENCIES(graphics-nodes core render)
