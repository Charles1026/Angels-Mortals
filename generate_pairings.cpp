#include <fstream>


#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <anm/participant_manager.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    spdlog::error("Please use this format: ./generate_pairings_exec <path to input file> <path to output file>");
  }

  std::ifstream inputFile(argv[1]);
  nlohmann::json inputJson = nlohmann::json::parse(inputFile);

}