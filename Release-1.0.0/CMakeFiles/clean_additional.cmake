# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CADBuilderTests_autogen"
  "CADBuilder_autogen"
  "CMakeFiles\\CADBuilderTests_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\CADBuilderTests_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\CADBuilder_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\CADBuilder_autogen.dir\\ParseCache.txt"
  )
endif()
