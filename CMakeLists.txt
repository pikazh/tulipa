PROJECT(tulipa)

CMAKE_MINIMUM_REQUIRED( VERSION 3.12 )

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

set(TULIPA_HOME_DIR ${CMAKE_HOME_DIRECTORY})

include (${TULIPA_HOME_DIR}/cmake/variables.cmake)
include (${TULIPA_HOME_DIR}/cmake/macros.cmake)
include (${TULIPA_HOME_DIR}/cmake/compilers_config.cmake)

ADD_SUBDIRECTORY(common)
ADD_SUBDIRECTORY(tests)