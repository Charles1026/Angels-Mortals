#include <signal.h>
#include <fstream>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "anm/server.h"

static volatile bool keepRunning = true;

const inline std::string CONFIG_FILE_PATH = "config.json";

void termHandler(int dummy) {
  keepRunning = 0;
  spdlog::info("\nStopping Server, please do not press anything.");
}

int main(int argc, char *argv[]) {
  nlohmann::json jsonObject;
  try {
    std::ifstream jsonFile(CONFIG_FILE_PATH);
    jsonObject = nlohmann::json::parse(jsonFile);
    jsonFile.close();
    spdlog::info("Configs loaded from file:{}", CONFIG_FILE_PATH);
  } catch (const std::ifstream::failure& e) {
    spdlog::error("Error, failed to open config file via path: {}\n with error:", CONFIG_FILE_PATH, e.what());
  } catch (const nlohmann::json::parse_error& e) {
    spdlog::error("Error, failed to load configs from file: {}\n with error:", CONFIG_FILE_PATH, e.byte);
  }
  
  AnM::BotServer server(jsonObject.at("angelToken"), jsonObject.at("mortalToken"), 
      jsonObject.at("pathToParticipantsFile"), jsonObject.at("dataChannelId"));
  server.startPolling();

  signal(SIGINT, termHandler);
  while (keepRunning) {
    continue;
  }
  return 0;
}