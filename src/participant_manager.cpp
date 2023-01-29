#include "anm/participant_manager.h"
#include <cstdint>
#include <fstream>
#include <sstream>

#include <spdlog/spdlog.h>

namespace AnM {
  std::ostream& operator<<(std::ostream& os, const Participant& participant) {
    os << participant.name << '\n';
    os << "Here is some info about him/her" << '\n';
    os << "\tTelegram Username: @" << participant.username << '\n';
    if (participant.isDevil) {
      os << "\tRequested Prank Level: " << participant.prankLevel << '\n';
    }
    os << "\tRoom Number: " << participant.roomNumber << '\n';
    os << "\tInterests: " << participant.interests << '\n';
    os << "\tAdditional Notes: " << participant.notes << '\n';
    return os;
  }
  
  
  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(AnM::Participant, id, username, name, isDevil, prankLevel, roomNumber, interests, notes, angelId, angelChatId, mortalId, mortalChatId, mortalUsername)

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
      // std::cout << element.second << std::endl;
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

  ParticipantManager::~ParticipantManager() {
    saveData();
  }

  void ParticipantManager::saveData() {
    spdlog::info("Saving Participant Data.");
    try {
      nlohmann::json jsonObject;
      std::lock_guard<std::mutex> lock(m_participantMapMutex);
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
    std::lock_guard<std::mutex> lock(m_participantMapMutex);
    if (m_participantsMap.find(key) != m_participantsMap.end()) {
      spdlog::warn("Warning, attempt to add duplicate participant {} ignored.", participant.id);
      return false;
    }
    m_participantsMap.insert({key, participant});
    return true;
  }

  bool ParticipantManager::setParticipantChatId(std::int64_t participantId, const std::string& username, std::int64_t chatId, 
      bool isAngelBot, const std::string& mortalUsername) {
    {
      std::lock_guard<std::mutex> lock(m_participantMapMutex);
      auto participantIter = m_participantsMap.find(participantId);
      if (participantIter == m_participantsMap.end()) {
        spdlog::warn("Warning, attempt to set participant chat Id of non-existing participant: {}", participantId);
        return false;
      }
      Participant participant = participantIter->second;
      participantIter->second.username = username;
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
    }
    saveData();
    return true;
  }

  std::string ParticipantManager::getAngelOrMortalUsername(std::int64_t participantId, bool lookingForAngel) {
    std::lock_guard<std::mutex> lock(m_participantMapMutex);
    auto iter = m_participantsMap.find(participantId);
    if (iter == m_participantsMap.end()) {
      spdlog::warn("Cannot Find participant with id: {}", participantId);
      return "";
    }
    Participant participant = iter->second;
    std::int64_t targetId = lookingForAngel ? participant.angelId : participant.mortalId;
    auto targetIter = m_participantsMap.find(targetId);
    if (targetIter == m_participantsMap.end()) {
      spdlog::warn("Cannot Find recipient with id: {}", targetId);
      return "";
    }
    return targetIter->second.username;
  }

  std::int64_t ParticipantManager::getAngelOrMortalId(std::int64_t participantId, bool lookingForAngel) {
    std::lock_guard<std::mutex> lock(m_participantMapMutex);
    auto iter = m_participantsMap.find(participantId);
    if (iter == m_participantsMap.end()) {
      spdlog::warn("Cannot Find participant with id: {}", participantId);
      return -1;
    }
    Participant participant = iter->second;
    return lookingForAngel ? participant.angelId : participant.mortalId;
  }

  std::int64_t ParticipantManager::getAngelOrMortalChatId(std::int64_t participantId, bool lookingForAngel) {
    std::lock_guard<std::mutex> lock(m_participantMapMutex);
    auto iter = m_participantsMap.find(participantId);
    if (iter == m_participantsMap.end()) {
      spdlog::warn("Cannot Find participant with id: {}", participantId);
      return -1;
    }
    Participant participant = iter->second;
    return lookingForAngel ? participant.angelChatId : participant.mortalChatId;
  }

  bool ParticipantManager::participantIsDevil(std::int64_t participantId) {
    std::lock_guard<std::mutex> lock(m_participantMapMutex);
      auto iter = m_participantsMap.find(participantId);
      if (iter == m_participantsMap.end()) {
        spdlog::warn("Cannot Find participant with id: {}", participantId);
        return false;
      }
    return iter->second.isDevil;
  }

  std::string ParticipantManager::getParticipantToString(std::int64_t participantId) {
    std::lock_guard<std::mutex> lock(m_participantMapMutex);
      auto iter = m_participantsMap.find(participantId);
      if (iter == m_participantsMap.end()) {
        spdlog::warn("Cannot Find participant with id: {}", participantId);
        return "";
      }
    std::ostringstream os;
    os << iter->second;
    std::string participantString = os.str();
    return participantString;
  }
}