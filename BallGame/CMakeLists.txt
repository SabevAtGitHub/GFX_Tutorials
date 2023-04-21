
add_executable(
  BallGame 
  BallController.h
  Ball.h
  BallRenderer.h
  Grid.h
  MainGame.h
  BallController.cpp
  Ball.cpp
  BallRenderer.cpp
  Grid.cpp
  Main.cpp
  MainGame.cpp
  )

target_link_libraries(BallGame PUBLIC GameEngineOpenGL)

target_include_directories(BallGame PUBLIC
  ${PROJECT_SOURCE_DIR}
)
