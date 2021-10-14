#--------------------------------------------------------------------------
# mathlib project
#--------------------------------------------------------------------------

PROJECT(mathlib)
FILE(GLOB mathlib_headers code/*.h)
FILE(GLOB mathlib_sources code/*.cc)
FILE(GLOB mathlib_sources code/*.cpp)

SET(files_mathlib ${mathlib_headers} ${mathlib_sources})
SOURCE_GROUP("mathlib" FILES ${files_mathlib})

ADD_EXECUTABLE(mathlib ${files_mathlib})
TARGET_LINK_LIBRARIES(mathlib core render)
ADD_DEPENDENCIES(mathlib core render)