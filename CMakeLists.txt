CMAKE_MINIMUM_REQUIRED(VERSION 3.30)

SET(CMAKE_CXX_STANDARD 19)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)


SET
(
    EMU_6502 
    EMU_6502
)

SET
(   EMU_SRC_DIR
    src
)

project(${EMU_6502})

##############
#   GTest   #
############

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

enable_testing()

add_subdirectory(libs/)

add_subdirectory(tests)