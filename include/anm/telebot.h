#pragma once

#include <functional>
#include <string>

#include <tgbot/Bot.h>
#include <tgbot/types/Message.h>
#include <tgbot/net/TgLongPoll.h>

#include "anm/participant_manager.h"
namespace AnM {

enum class SendMessageResponse {
  OK,
  UNREGISTERED_RECIPIENT,
  UNSUPPORTED_MESSAGE_FORMAT
};

class TeleBot {
  public:
    TeleBot(const std::string& token, bool isAngel, const ParticipantManager& participants);

    void setForwardNormalMessageCallback(const std::function<SendMessageResponse(TgBot::Message::Ptr)>& callbackFn);

    SendMessageResponse sendMessageToRecipient(TgBot::Message::Ptr msgPtr);

    void poll();

  protected:
    void setCommandMessageCallback();

    bool ensureMessageIsPrivateMessage(TgBot::Message::Ptr msgPtr);

    void respondToMessage(const TgBot::Message::Ptr& msgPtr, const std::string& response);

  private:
    const bool m_isAngel;

    TgBot::Bot m_bot;
    TgBot::TgLongPoll m_poller;

    ParticipantManager m_participants;

    std::function<void(TgBot::Message::Ptr)> m_normalMessageCallback;
    std::function<void(TgBot::Message::Ptr)> m_startCommandCallback;
    std::function<void(TgBot::Message::Ptr)> m_groupCommandCallback;
};
}