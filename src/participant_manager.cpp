#include "anm/participant_manager.h"
#include <cstdint>
#include <fstream>
#include <iostream>

#include <spdlog/spdlog.h>

/**
 * @brief functions to convert json to ParticipantManger via nlohmann.
 * 
 */
namespace AnM {
  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AnM::Participant, id, angelId, angelChatId, mortalId, mortalChatId, mortalUsername)

  void from_json(const nlohmann::json& json, AnM::ParticipantManager& manager) {
    for (auto& element : json) {
      AnM::Participant participant;
      from_json(element, participant);
      manager.addParticipant(participant);
    }
  }

  void to_json(nlohmann::json& json, AnM::ParticipantManager& manager) {
    std::size_t ptr = 0;
    for (auto& element : manager.m_participantsMap) {
      to_json(json[ptr], element.second);
      ptr++;
    }
  }

  ParticipantManager::ParticipantManager(const std::string& pathToParticipantsJson)
      :m_pathToParticipantsJson(pathToParticipantsJson) {
    try {
      std::ifstream jsonFile(m_pathToParticipantsJson);
      nlohmann::json jsonObject = nlohmann::json::parse(jsonFile);
      from_json(jsonObject, *this);
      jsonFile.close();
      spdlog::info("Participants list loaded from file: {}", m_pathToParticipantsJson);
    } catch (const std::ifstream::failure& e) {
      spdlog::error("Error, failed to open participants json via path: {}\n with error:", m_pathToParticipantsJson, e.what());
    } catch (const nlohmann::json::parse_error& e) {
      spdlog::error("Error, failed to load participants from file: {}\n with error:", m_pathToParticipantsJson, e.byte);
    }
  }

  void ParticipantManager::saveData() {
    try {
      nlohmann::json jsonObject;
      to_json(jsonObject, *this);
      std::ofstream jsonFile(m_pathToParticipantsJson);
      jsonFile << std::setw(2) << jsonObject << std::endl;
      jsonFile.close();
      spdlog::info("Participants list saved to file: {}", m_pathToParticipantsJson);
    } catch (const std::ifstream::failure& e) {
      spdlog::error("Error, failed to open participants json via path: {}\n with error:", m_pathToParticipantsJson, e.what());
    } catch (const nlohmann::json::exception& e) {
      spdlog::error("Error, failed to save participants to file: {}\n with error:", m_pathToParticipantsJson, e.what());
    }
  }


  bool ParticipantManager::addParticipant(Participant participant) {
    std::int64_t key = participant.id;
    if (m_participantsMap.find(key) != m_participantsMap.end()) {
      spdlog::warn("Warning, attempt to add duplicate participant {} ignored.", participant.id);
      return false;
    }
    m_participantsMap.insert({key, participant});
    return true;
  }

  bool ParticipantManager::setParticipantChatId(std::int64_t participantId, std::int64_t chatId, bool isAngelBot, const std::string& mortalUsername) {
    auto participantIter = m_participantsMap.find(participantId);
    if (participantIter == m_participantsMap.end()) {
      spdlog::warn("Warning, attempt to set participant chat Id of non-existing participant: {}", participantId);
      return false;
    }
    Participant participant = participantIter->second;
    std::int64_t senderId = isAngelBot ? participant.angelId : participant.mortalId;
    auto senderIter = m_participantsMap.find(senderId);
    if (senderIter == m_participantsMap.end()) {
      spdlog::warn("Warning, attempt to set participant chat Id of non-existing sender: {}", senderId);
      return false;
    }
    if (isAngelBot) {
      senderIter->second.mortalChatId = chatId;
      senderIter->second.mortalUsername = mortalUsername;
    } else {
      senderIter->second.angelChatId = chatId;
    }
    return true;
  }


  std::int64_t ParticipantManager::getAngelOrMortalId(std::int64_t participantId, bool lookingForAngel) const {
    auto iter = m_participantsMap.find(participantId);
    if (iter == m_participantsMap.end()) {
      spdlog::warn("Cannot Find participant with id: {}", participantId);
      return -1;
    }
    Participant participant = iter->second;
    return lookingForAngel ? participant.angelId : participant.mortalId;
  }

  std::int64_t ParticipantManager::getAngelOrMortalChatId(std::int64_t participantId, bool lookingForAngel) const {
    auto iter = m_participantsMap.find(participantId);
    if (iter == m_participantsMap.end()) {
      spdlog::warn("Cannot Find participant with id: {}", participantId);
      return -1;
    }
    Participant participant = iter->second;
    return lookingForAngel ? participant.angelChatId : participant.mortalChatId;
  }
}