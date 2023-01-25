#include <signal.h>
#include <iostream>

#include "anm/server.h"

static volatile bool keepRunning = true;

const inline std::string ANGEL_TOKEN = "5847362248:AAFcROoThTNoT6GGbiKF4sY-hFHOePFojrE";

const inline std::string MORTAL_TOKEN = "5984354484:AAFyLLZxjRVzMTM5h0qU5DhSjzqHTPPn87A";

void termHandler(int dummy) {
  keepRunning = 0;
  std::cout << "\nStopping Server, please do not press anything." << std::endl;
}

int main(int argc, char *argv[]) {
  AnM::BotServer server(ANGEL_TOKEN, MORTAL_TOKEN, "participants.json", -1001861203201);
  server.startPolling();

  signal(SIGINT, termHandler);
  while (keepRunning) {
    continue;
  }
  return 0;
}