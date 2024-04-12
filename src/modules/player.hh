#pragma once
#include "./role.hh"

#include <cstdint>
#include <string>

enum class PlayerStatus {
    Kicked,
    Alive,
    Dead,
    VotedOut,
};

struct Player {
    uint32_t id;
    std::string username;
    Role role;
    PlayerStatus status;

    Player(uint32_t id, std::string username, Role role, PlayerStatus status);
    Player();
    bool operator==(const Player &other) const;
};
