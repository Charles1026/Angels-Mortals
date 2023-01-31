#include <atomic>
#include <exception>
#include <fstream>
#include <signal.h>
#include <stdexcept>
#include <typeinfo>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "anm/server.h"

static std::atomic_bool keepRunning(true);

const std::string CONFIG_FILE_PATH = "../data/config.json";

void termHandler(int dummy) {
  keepRunning.store(false);
  spdlog::info("Stopping Server, please do not press anything.");
}

int main(int argc, char *argv[]) {
  // Parse configs lists
  nlohmann::json jsonObject;
  try {
    std::ifstream jsonFile(CONFIG_FILE_PATH);
    jsonObject = nlohmann::json::parse(jsonFile);
    jsonFile.close();
    spdlog::info("Configs loaded from file:{}", CONFIG_FILE_PATH);
  } catch (const std::ifstream::failure& e) {
    spdlog::error("Error, failed to open config file via path: {}\n with error:", CONFIG_FILE_PATH, e.what());
    return 1;
  } catch (const nlohmann::json::parse_error& e) {
    spdlog::error("Error, failed to load configs from file: {}\n with error:", CONFIG_FILE_PATH, e.byte);
    return 1;
  }
  
  // Start Server
  // TODO: Find a better way to stop as this waits for the long poll to end before finishing
  signal(SIGINT, termHandler);
  while (keepRunning.load()) {
    try {
      AnM::BotServer server(jsonObject.at("angelToken"), jsonObject.at("mortalToken"), 
          jsonObject.at("pathToParticipantsFile"), jsonObject.at("dataChannelId"), jsonObject.at("mainGroupId"));
      server.startPolling();
      while (keepRunning.load()) {
        continue;
      }
    } catch (...) {
      std::exception_ptr p = std::current_exception();
      spdlog::error("Fatal exception encountered: {}. Restarting Server", p ? p.__cxa_exception_type()->name() : "null");
    }
  }
  
  return 0;
}