#pragma once 

#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <thread>

#include <tgbot/types/Message.h>

#include "anm/telebot.h"
namespace AnM {

class BotServer {

public:
  BotServer(const std::string& angelToken, const std::string& botToken, const std::string& pathToParticipantsJson, 
      std::int64_t dataChannelId, std::int64_t groupId);
  ~BotServer();

  void startPolling();

private:
  std::shared_ptr<ParticipantManager> m_participantManager;
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