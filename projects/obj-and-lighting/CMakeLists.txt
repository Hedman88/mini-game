#--------------------------------------------------------------------------
# example project
#--------------------------------------------------------------------------

PROJECT(obj-and-lighting)
FILE(GLOB example_headers code/*.h)
FILE(GLOB example_sources code/*.cc)

SET(files_example ${example_headers} ${example_sources})
include_directories(../../engine)
SOURCE_GROUP("example" FILES ${files_example})

ADD_EXECUTABLE(obj-and-lighting ${files_example})
TARGET_LINK_LIBRARIES(obj-and-lighting core render)
ADD_DEPENDENCIES(obj-and-lighting core render)
