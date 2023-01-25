cmake_minimum_required(VERSION 3.20.2)
include_guard(GLOBAL)
project(angels_and_mortals)

find_package(spdlog CONFIG REQUIRED)
add_subdirectory(3rd-party/tgbot-cpp TgBot)
add_subdirectory(3rd-party/json nlohmann_json)


add_library(angels_and_mortals
  src/telebot.cpp
  src/participant_manager.cpp
  src/server.cpp
)
add_library(strix::anm ALIAS angels_and_mortals)

include_directories(angels_and_mortals
PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_compile_features(angels_and_mortals
PUBLIC
  cxx_std_20
PRIVATE
  cxx_std_20
)

target_link_libraries(angels_and_mortals
PUBLIC
  TgBot::TgBot
  spdlog::spdlog
  nlohmann_json::nlohmann_json
)

add_executable(angels_and_mortals_exec
  main.cpp
)

include_directories(angels_and_mortals_exec
PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_compile_features(angels_and_mortals_exec
PUBLIC
  cxx_std_20
)

target_link_libraries(angels_and_mortals_exec
PUBLIC
  strix::anm
)