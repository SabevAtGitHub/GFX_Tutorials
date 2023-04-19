
add_library(
    GameEngineOpenGL
    GameEngineOpenGL.h
    GameEngineOpenGL.cpp
    AudioManager.h
    Camera2D.h
    DebugRenderer.h
    ErrManager.h
    GLSLProgram.h
    GLTexture.h
    # GUI.h
    IGameScreen.h
    ImageLoader.h
    IMainGame.h
    InputManager.h
    IOManager.h
    ParticleBatch2D.h
    ParticleEngine2D.h
    PicoPNG.h
    ResourceManager.h
    ScreenList.h
    SpriteBatch.h
    SpriteFont.h
    Sprite.h
    TextureCache.h
    TileSheet.h
    Timing.h
    Vertex.h
    Window.h
    AudioManager.cpp
    Camera2D.cpp
    DebugRenderer.cpp
    Errors.cpp
    GLSLProgram.cpp
    # GUI.cpp
    ImageLoader.cpp
    IMainGame.cpp
    InputManager.cpp
    IOManager.cpp
    ParticleBatch2D.cpp
    ParticleEngine2D.cpp
    PicoPNG.cpp
    ResourceManager.cpp
    ScreenList.cpp
    SpriteBatch.cpp
    Sprite.cpp
    SpriteFont.cpp
    TextureCache.cpp
    Timing.cpp
    Window.cpp
)

target_include_directories(
    GameEngineOpenGL PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(GameEngineOpenGL SDL2main SDL2)
