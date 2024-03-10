#ifndef PREP_H
#define PREP_H

#include <chrono>
#include <cstdint>
#include <ctime>
#include <string>
#include <vector>

// For test example
int add(int a, int b);

// All IDs are uint32_t
enum EventType { PHASE_CHANGE, ACTION, ROOM_STATE_CHANGE, PLAYER_STATE_CHANGE };
enum Action { KILL, VOTE, INVESTIGATE, HEAL, PROTECT };
enum RoomStatus { AWAITING_START, IN_PROGRESS, STOPPED, ENDED };
enum PlayerStatus { KICKED, ALIVE, DEAD, VOTED_OUT };

struct Role;
struct Player;
struct Room;
struct Event;

struct Role {
    std::vector<Action> actions;
};

struct Player {
    std::string username;
    Role role;
    PlayerStatus playerStatus;
};

struct Room {
    uint32_t id;
    std::string title;
    std::tm *utcTimestampCreatedAt;
    RoomStatus status;

    Room(uint32_t id, std::string title, uint32_t utcTimestampCreatedAt,
         RoomStatus status)
        : id(id), title(title), status(status) {
        // Convert the timestamp into a time_point object
        std::chrono::seconds sec(utcTimestampCreatedAt);
        std::chrono::time_point<std::chrono::system_clock> tp(sec);
        // Convert to time_t for formatting
        std::time_t time_object = std::chrono::system_clock::to_time_t(tp);
        // Convert to tm as UTC time
        this->utcTimestampCreatedAt = std::gmtime(&time_object);
    }
};

struct Event {
    std::string title;
    std::tm *utcTimestampCreatedAt;
    uint32_t numberNight;
    bool isVisible;
    std::vector<Action> causedBy;
    std::vector<Action> influenced;
};

#endif
