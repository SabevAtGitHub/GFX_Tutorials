
link_directories(texture_mapping ${PROJECT_SOURCE_DIR}/GameEngineOpenGL)

add_executable(BallGame Main.cpp)
target_link_libraries(BallGame PRIVATE GameEngineOpenGL)


# target_include_directories(
#     BallGame PUBLIC
#     "${CMAKE_CURRENT_SOURCE_DIR}"
#     "${CMAKE_CURRENT_SOURCE_DIR}/GameEngineOpenGL"
# )
