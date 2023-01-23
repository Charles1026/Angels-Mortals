cmake_minimum_required(VERSION 3.20.2)
include_guard(GLOBAL)
project(angels_and_mortals)

add_subdirectory(3rd-party/tgbot-cpp TgBot)

add_library(angels_and_mortals
  src/telebot.cpp
)

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
)