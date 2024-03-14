#pragma once
#include "modules/event.hh"
#include "modules/player.hh"
#include "modules/room.hh"

#include <string>
#include <vector>

enum class ValidationStatus {
    PlayerNotInRoom,
    NoTargetPlayerSpecified,
    RoomNotInProgress,
    ActionDoesNotBelongToRole,
    ActionProhibited,
    NoActor,
    NoAction,
    NoRole,
    NoRoom,
    NoRelatedEvents,
    ActionValid,
};

ValidationStatus
  validate_action(Player *actor, const Action *action, Room *room, std::vector<Event> *relatedEvents, Player *target);

class ValidationStatusUtils {
 public:
    static std::string to_string(ValidationStatus status);
};
