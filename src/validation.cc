#include "./validation.hh"

#include "modules/action.hh"
#include "modules/event.hh"
#include "modules/player.hh"
#include "modules/role.hh"
#include "modules/room.hh"

#include <algorithm>
#include <string>

static bool player_belongs_to_room(const Player *player, const Room *room);
static bool action_belongs_to_role(const Role *role, const Action *action);
static bool is_action_allowed(const Action *action, std::vector<Event> *relevant_events);

/**
 * Validate if an action is valid for a player in a room based on related events.
 *
 * @param actor Pointer to the player performing the action.
 * @param action Pointer to the action to validate.
 * @param room Pointer to the room where the action is taking place.
 * @param related_events Pointer to the vector of related events.
 * @param target Pointer to the target player (optional, defaults to `nullptr`).
 * @return An integer representing the validation status.
 */
ValidationStatus validate_action(
  Player *actor, const Action *action, Room *room, std::vector<Event> *related_events, Player *target = nullptr) {
    if (!actor) {
        return ValidationStatus::NoActor;
    }
    if (!action) {
        return ValidationStatus::NoAction;
    }
    if (!room) {
        return ValidationStatus::NoRoom;
    }
    if (!related_events) {
        return ValidationStatus::NoRelatedEvents;
    }
    if (!player_belongs_to_room(actor, room)) {
        return ValidationStatus::PlayerNotInRoom;
    }
    if (action->has_target && !target) {
        return ValidationStatus::NoTargetPlayerSpecified;
    }
    if (room->status != RoomStatus::InProgress) {
        return ValidationStatus::RoomNotInProgress;
    }
    Role *role = &actor->role;
    if (!role) {
        return ValidationStatus::NoRole;
    }
    if (!action_belongs_to_role(role, action)) {
        return ValidationStatus::ActionDoesNotBelongToRole;
    }
    if (!is_action_allowed(action, related_events)) {
        return ValidationStatus::ActionProhibited;
    }
    return ValidationStatus::ActionValid;
}

/**
 * Check if a player belongs to a given room.
 *
 * @param player Pointer to the player object.
 * @param room Pointer to the room object.
 * @return `true` if the player belongs to the room, otherwise `false`.
 */
static bool player_belongs_to_room(const Player *player, const Room *room) {
    return std::find(room->players.begin(), room->players.end(), *player) != room->players.end();
}

/**
 * Check if an action belongs to a given role.
 *
 * @param role Pointer to the role object.
 * @param action Pointer to the action object.
 * @return `true` if the action belongs to the role, otherwise `false`.
 */
static bool action_belongs_to_role(const Role *role, const Action *action) {
    return std::find(role->actions.begin(), role->actions.end(), *action) != role->actions.end();
}

/**
 * Check if an action is allowed based on relevant events.
 *
 * @param action Pointer to the action object.
 * @param relevantEvents Pointer to the vector of relevant events.
 * @return `true` if the action is allowed, otherwise `false`.
 */
static bool is_action_allowed(const Action *action, std::vector<Event> *relevant_events) {
    bool allowed = false;  // Actions are disabled by default
    std::sort(relevant_events->begin(), relevant_events->end());
    for (const auto &event : *relevant_events) {
        if (std::find(event.prohibits.begin(), event.prohibits.end(), *action) != event.prohibits.end()) {
            allowed = false;
        }
        if (std::find(event.allows.begin(), event.allows.end(), *action) != event.allows.end()) {
            allowed = true;
        }
    }
    return allowed;
}

/**
 * Convert a ValidationStatus enum value to its corresponding string representation.
 *
 * @param status The ValidationStatus enum value to convert.
 * @return The string representation of the given ValidationStatus.
 */
std::string ValidationStatusUtils::to_string(ValidationStatus status) {
    switch (status) {
        case ValidationStatus::PlayerNotInRoom: return "player not in room";
        case ValidationStatus::NoTargetPlayerSpecified: return "no target player specified";
        case ValidationStatus::RoomNotInProgress: return "room not in progress";
        case ValidationStatus::ActionDoesNotBelongToRole: return "action does not belong to role";
        case ValidationStatus::ActionProhibited: return "action prohibited";
        case ValidationStatus::NoActor: return "no actor";
        case ValidationStatus::NoAction: return "no action";
        case ValidationStatus::NoRole: return "no role";
        case ValidationStatus::NoRoom: return "no room";
        case ValidationStatus::NoRelatedEvents: return "no relevant events";
        case ValidationStatus::ActionValid: return "action valid";
        default: return "unknown validation status";
    }
}
