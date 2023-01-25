#include "anm/server.h"

#include <memory>

#include <spdlog/spdlog.h>

namespace AnM {
  BotServer::BotServer(const std::string& angelToken, const std::string& mortalToken, const std::string& pathToParticipantsJson, 
      std::int64_t dataChannelId)
      : m_participantManager(std::make_shared<ParticipantManager>(pathToParticipantsJson)), 
        m_angelBot(angelToken, true, m_participantManager, dataChannelId), 
        m_mortalBot(mortalToken, false, m_participantManager, dataChannelId) {

    m_angelNormalCallback = [&](TgBot::Message::Ptr msgPtr){
      return m_mortalBot.sendMessageToRecipient(msgPtr);
    };
    m_angelBot.setForwardNormalMessageCallback(m_angelNormalCallback);

    m_mortalNormalCallback = [&](TgBot::Message::Ptr msgPtr){
      return m_angelBot.sendMessageToRecipient(msgPtr);
    };
    m_mortalBot.setForwardNormalMessageCallback(m_mortalNormalCallback);

    spdlog::info("Successfully Set normal message callbacks for both bots.");
  }

  BotServer::~BotServer() {
    m_keepRunningAngel = false;
    if (m_angelThread.joinable()) {
      m_angelThread.join();
    }
    m_keepRunningMortal = false;
    if (m_mortalThread.joinable()) {
      m_mortalThread.join();
    }
    spdlog::info("Stopped polling for both bots.");
  }


  void BotServer::startPolling() {
    m_keepRunningAngel = true;
    m_angelThread = std::thread([&](){
      while (m_keepRunningAngel) {
        m_angelBot.poll();
      }
    });

    m_keepRunningMortal = true;
    m_mortalThread = std::thread([&](){
      while (m_keepRunningMortal) {
        m_mortalBot.poll();
      }
    });
    spdlog::info("Both bots are now polling and ready to recieve messages.");
  }
}