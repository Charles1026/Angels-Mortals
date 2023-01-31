#pragma once

#include <functional>
#include <memory>
#include <string>

#include <tgbot/Bot.h>
#include <tgbot/types/Message.h>
#include <tgbot/net/TgLongPoll.h>

#include "anm/participant_manager.h"
namespace AnM {

enum class SendMessageResponse {
  OK,
  UNREGISTERED_RECIPIENT,
  UNSUPPORTED_MESSAGE_FORMAT,
  NOT_DATACHANNEL_MESSAGE,
};

class TeleBot {
  public:
    TeleBot(const std::string& token, bool isAngel, std::shared_ptr<ParticipantManager> participants, 
        std::int64_t dataChannelId, std::int64_t groupId);

    void setForwardNormalMessageCallback(const std::function<SendMessageResponse(TgBot::Message::Ptr)>& callbackFn);

    SendMessageResponse sendMessageToRecipient(TgBot::Message::Ptr msgPtr);

    void poll();

    void skipUpdates(std::size_t numberOfUpdates);

  protected:
    void setCommandMessageCallback();

    bool ensureMessageIsPrivateMessage(TgBot::Message::Ptr msgPtr);

    void respondToMessage(const TgBot::Message::Ptr& msgPtr, const std::string& response);

    SendMessageResponse handleDataChannelMessage(TgBot::Message::Ptr msgPtr);

  private:
    const bool m_isAngel {true};
    const std::int64_t m_dataChannelId;
    const std::int64_t m_groupId;

    TgBot::Bot m_bot;
    TgBot::TgLongPoll m_poller;

    std::shared_ptr<ParticipantManager> m_participants {nullptr};

    std::function<void(TgBot::Message::Ptr)> m_normalMessageCallback;
    std::function<void(TgBot::Message::Ptr)> m_startCommandCallback;
    std::function<void(TgBot::Message::Ptr)> m_groupCommandCallback;
    std::function<void(TgBot::Message::Ptr)> m_whoCommandCallback;
};
}