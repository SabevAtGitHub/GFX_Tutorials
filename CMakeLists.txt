cmake_minimum_required(VERSION 3.22.1)
project(GFXTutorials)

if(NOT "${CMAKE_CXX_STANDARD}")
  set(CMAKE_CXX_STANDARD 17)
endif()


if(NOT "${CMAKE_C_STANDARD}")
  set(CMAKE_C_STANDARD 11)
endif()

add_subdirectory(GameEngineOpenGL)
add_subdirectory(BallGame)
# add_subdirectory( GFX_Tutorials )
# add_subdirectory( NinjaPlatformer )
# add_subdirectory( ZombiesGame )
