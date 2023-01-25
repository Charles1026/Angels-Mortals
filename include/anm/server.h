#pragma once 

#include <atomic>
#include <functional>
#include <string>
#include <thread>

#include <tgbot/types/Message.h>

#include "anm/telebot.h"
namespace AnM {

class BotServer {

public:
  BotServer(const std::string& angelToken, const std::string& botToken, const std::string& pathToParticipantsJson);
  ~BotServer();

  void startPolling();

private:
  ParticipantManager m_participantManager;
  TeleBot m_angelBot;
  TeleBot m_mortalBot;

  std::atomic_bool m_keepRunningAngel;
  std::thread m_angelThread;
  std::atomic_bool m_keepRunningMortal;
  std::thread m_mortalThread;

  std::function<SendMessageResponse(TgBot::Message::Ptr)> m_angelNormalCallback;
  std::function<SendMessageResponse(TgBot::Message::Ptr)> m_mortalNormalCallback;
};
}