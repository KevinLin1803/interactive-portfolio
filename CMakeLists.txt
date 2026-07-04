cmake_minimum_required(VERSION 3.16)
project(interactive_portfolio CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# --- raylib via FetchContent ---
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
if(EMSCRIPTEN)
  set(PLATFORM Web CACHE STRING "" FORCE)
endif()

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5
  GIT_SHALLOW ON
)
FetchContent_MakeAvailable(raylib)

# --- the game ---
add_executable(portfolio
  src/main.cpp
  src/Game.cpp
  src/Sprite.cpp
  src/GameObject.cpp
  src/GameMap.cpp
  src/DirectionInput.cpp
  src/AssetStore.cpp
)
target_include_directories(portfolio PRIVATE src)
target_link_libraries(portfolio PRIVATE raylib)

if(EMSCRIPTEN)
  # Web build: single .html + .js + .wasm + .data, assets preloaded.
  set_target_properties(portfolio PROPERTIES SUFFIX ".html")
  target_link_options(portfolio PRIVATE
    -sUSE_GLFW=3
    -sASYNCIFY
    # Fixed 128MB heap (no ALLOW_MEMORY_GROWTH): a growable heap is a resizable
    # ArrayBuffer, which newer Chrome's TextDecoder rejects, aborting startup.
    -sINITIAL_MEMORY=134217728
    "--preload-file=${CMAKE_CURRENT_SOURCE_DIR}/assets@assets"
    "--shell-file=${CMAKE_CURRENT_SOURCE_DIR}/web/shell.html"
  )
else()
  # Native-only: the headless asset generator + copy assets next to the binary.
  add_executable(gen_assets tools/gen_assets.cpp)
  target_include_directories(gen_assets PRIVATE src)
  target_link_libraries(gen_assets PRIVATE raylib)

  add_custom_command(TARGET portfolio POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_CURRENT_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:portfolio>/assets"
    VERBATIM)
endif()
