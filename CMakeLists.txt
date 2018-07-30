# Define the subproject name
project(AttemptedAnalysis)

# Directories where includes are found (in addition to those defined in the
# parent CMakeLists.txt, one directory up)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Declare libraries to build
# Note that the library names (the first argument) should follow the convention
# "sbnanalysis_ThisAnalysis_LibraryName"). Libraries can be built from multiple
# source files.

add_library(sbnanalysis_AttemptedAnalysis_AttemptedSelection SHARED AttemptedSelection.cxx)

# Define library linking
target_link_libraries(sbnanalysis_AttemptedAnalysis_AttemptedSelection sbnanalysis_AttemptedAnalysis_AttemptedTools)

# Install all libraries to the main "lib" directory
install(TARGETS sbnanalysis_AttemptedAnalysis_AttemptedTools DESTINATION lib)
install(TARGETS sbnanalysis_AttemptedAnalysis_AttemptedSelection DESTINATION lib)
