#pragma once

#include <cstdint>
#include <iostream>
#include <mutex>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

namespace AnM {

const inline std::int64_t PARTICIPANT_INVALID_NUMBER = -1;
const inline std::string PARTICIPANT_INVALID_STRING = "";
struct Participant{
  // Basic Info
  std::int64_t id{PARTICIPANT_INVALID_NUMBER};
  std::string username{PARTICIPANT_INVALID_STRING};
  std::string name{PARTICIPANT_INVALID_STRING};
  
  // For Devils
  bool isDevil{false};
  std::int64_t prankLevel{PARTICIPANT_INVALID_NUMBER};

  // More Info
  std::int64_t roomNumber{PARTICIPANT_INVALID_NUMBER};
  std::string interests{PARTICIPANT_INVALID_STRING};
  std::string notes{PARTICIPANT_INVALID_STRING};

  //Angel Info
  std::int64_t angelId{PARTICIPANT_INVALID_NUMBER};
  std::int64_t angelChatId{PARTICIPANT_INVALID_NUMBER};

  // Mortal Info
  std::int64_t mortalId{PARTICIPANT_INVALID_NUMBER};
  std::int64_t mortalChatId{PARTICIPANT_INVALID_NUMBER};
  std::string mortalUsername{PARTICIPANT_INVALID_STRING};
};
std::ostream& operator<<(std::ostream& os, const Participant& participant);

class ParticipantManager {

  public:
    ParticipantManager(const std::string& pathToParticipantsJson);

    ~ParticipantManager();

    void saveData();

    bool addParticipant(Participant participant);

    bool setParticipantChatId(std::int64_t participantId, const std::string& username, std::int64_t chatId, bool isAngelBot, const std::string& mortalUsername = "");

    std::string getAngelOrMortalUsername(std::int64_t participantId, bool lookingForAngel);

    std::int64_t getAngelOrMortalId(std::int64_t participantId, bool lookingForAngel);

    std::int64_t getAngelOrMortalChatId(std::int64_t participantId, bool lookingForAngel);

    bool participantIsDevil(std::int64_t participantId);

    std::string getParticipantToString(std::int64_t participantId);

    friend void to_json(nlohmann::json& json, AnM::ParticipantManager& manager);

  private:
    std::unordered_map<std::int64_t, Participant> m_participantsMap;
    std::mutex m_participantMapMutex;

    const std::string m_pathToParticipantsJson;
};

}