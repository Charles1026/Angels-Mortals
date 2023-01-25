#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

namespace AnM {

const inline std::int64_t PARTICIPANT_INVALID_VALUE = -1;
struct Participant{
  std::int64_t id{PARTICIPANT_INVALID_VALUE};

  std::int64_t angelId{PARTICIPANT_INVALID_VALUE};
  std::int64_t angelChatId{PARTICIPANT_INVALID_VALUE};

  std::int64_t mortalId{PARTICIPANT_INVALID_VALUE};
  std::int64_t mortalChatId{PARTICIPANT_INVALID_VALUE};

  std::string mortalUsername{""};
};
std::ostream& operator<<(std::ostream& os, const Participant& participant);

class ParticipantManager {

  public:
    ParticipantManager(const std::string& pathToParticipantsJson);

    ~ParticipantManager();

    bool addParticipant(Participant participant);

    bool setParticipantChatId(std::int64_t participantId, std::int64_t chatId, bool isAngelBot, const std::string& mortalUsername = "");

    std::int64_t getAngelOrMortalId(std::int64_t participantId, bool lookingForAngel) const;

    std::int64_t getAngelOrMortalChatId(std::int64_t participantId, bool lookingForAngel) const;

    friend void to_json(nlohmann::json& json, AnM::ParticipantManager& manager);

  private:
    std::unordered_map<std::int64_t, Participant> m_participantsMap;

    const std::string m_pathToParticipantsJson;
};

}