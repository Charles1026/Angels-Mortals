#include "anm/telebot.h"

#include <cstdint>

#include <spdlog/spdlog.h>

#include <tgbot/Api.h>
#include <tgbot/types/Chat.h>
#include <tgbot/types/User.h>

namespace AnM {
    
TeleBot::TeleBot(const std::string& token, bool isAngel, const ParticipantManager& participants) 
    : m_isAngel(isAngel), m_bot(token), m_poller(m_bot), m_participants(participants) {
  setCommandMessageCallback();
  spdlog::info("{} Bot commands callback set.", m_isAngel ? "Angel" : "Mortal");
}

const inline std::string UNREGISTERED_RECIPIENT_RESPONSE = "Error, trying to send message to a player who hasn't started the bot. Please wait a while and contact House Comm if this problem persists.";
const inline std::string UNSUPPORTED_MESSAGE_FORMAT_RESPONSE = "Error, this message format is not supported currently. If you believe it should be, please contact House Comm.";

void TeleBot::setForwardNormalMessageCallback(const std::function<SendMessageResponse(TgBot::Message::Ptr)>& callbackFn) {
  m_normalMessageCallback = [&](TgBot::Message::Ptr msgPtr){
    if (!ensureMessageIsPrivateMessage(msgPtr)) return;
    SendMessageResponse response = callbackFn(msgPtr);
    if (response == SendMessageResponse::OK) {
      return;
    } else if (response == SendMessageResponse::UNREGISTERED_RECIPIENT) {
      respondToMessage(msgPtr, UNREGISTERED_RECIPIENT_RESPONSE);
    } else if (response == SendMessageResponse::UNSUPPORTED_MESSAGE_FORMAT) {
      respondToMessage(msgPtr, UNSUPPORTED_MESSAGE_FORMAT_RESPONSE);
    }
  };
  m_bot.getEvents().onNonCommandMessage(m_normalMessageCallback);
}

SendMessageResponse TeleBot::sendMessageToRecipient(TgBot::Message::Ptr msgPtr) {
  TgBot::User::Ptr userPtr = msgPtr->from;
  std::int64_t userId = userPtr->id;
  std::int64_t recipientChatId = m_participants.getAngelOrMortalChatId(userId, !m_isAngel);
  if (recipientChatId == PARTICIPANT_INVALID_VALUE) {
    return SendMessageResponse::UNREGISTERED_RECIPIENT;
  }
  TgBot::Api api = m_bot.getApi();

  if (!msgPtr->text.empty()) {
    api.sendMessage(recipientChatId, msgPtr->text);
  } else if (msgPtr->animation) {
    api.sendAnimation(recipientChatId, msgPtr->animation->fileId);
  } else if (msgPtr->audio) {
    spdlog::info("SLKFJDKSLDF");
    api.sendAudio(recipientChatId, msgPtr->animation->fileId);
  } else {
    return SendMessageResponse::UNSUPPORTED_MESSAGE_FORMAT; 
  }
  return SendMessageResponse::OK;
}

void TeleBot::poll() {
  m_poller.start();
}

const inline std::string START_COMMAND = "start";
const inline std::string GROUP_COMMAND = "group";
const inline std::string ERROR_STARTING_MESSAGE = "Error starting, please try again and contact House Comm if the problem persists.";
const inline std::string SUCCESS_STARTING_MESSAGE = "Successfully started, you may now start messaging.";

void TeleBot::setCommandMessageCallback() {
  m_startCommandCallback = [&](const TgBot::Message::Ptr& msgPtr){
    if (!ensureMessageIsPrivateMessage(msgPtr)) return;
    auto uid = msgPtr->from->id;
    msgPtr->chat;
    auto chatId = msgPtr->chat->id;
    if (!m_participants.setParticipantChatId(uid, chatId, m_isAngel, m_isAngel ? "" : msgPtr->from->username)) {
      respondToMessage(msgPtr, ERROR_STARTING_MESSAGE);
      return;
    }
    respondToMessage(msgPtr, SUCCESS_STARTING_MESSAGE);
  };
  m_bot.getEvents().onCommand(START_COMMAND, m_startCommandCallback);

  m_groupCommandCallback = [&](TgBot::Message::Ptr msgPtr){
    //TODO
  };
  m_bot.getEvents().onCommand(GROUP_COMMAND, m_groupCommandCallback);
}


const inline std::string SENDING_MSG_IN_NON_DM_REPLY = "Please only communicate with this bot via private messages";

bool TeleBot::ensureMessageIsPrivateMessage(TgBot::Message::Ptr msgPtr) {
  if (msgPtr->chat->type != TgBot::Chat::Type::Private) {
    m_bot.getApi().sendMessage(msgPtr->chat->id, SENDING_MSG_IN_NON_DM_REPLY, false, msgPtr->messageId);
    return false;
  }
  return true;
}

void TeleBot::respondToMessage(const TgBot::Message::Ptr& msgPtr, const std::string& response) {
  m_bot.getApi().sendMessage(msgPtr->chat->id, response, false, msgPtr->messageId);
}


}