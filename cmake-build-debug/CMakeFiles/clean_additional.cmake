# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/usc_path_finder_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/usc_path_finder_autogen.dir/ParseCache.txt"
  "usc_path_finder_autogen"
  )
endif()
