#include "./validation.hh"

#include "modules/action.hh"
#include "modules/event.hh"
#include "modules/player.hh"
#include "modules/role.hh"
#include "modules/room.hh"

#include <algorithm>

/**
 * Check if a player belongs to a given room.
 *
 * @param player Pointer to the player object.
 * @param room Pointer to the room object.
 * @return `true` if the player belongs to the room, otherwise `false`.
 */
bool player_belongs_to_room(const Player *player, const Room *room) {
    return std::find(room->players.begin(), room->players.end(), *player) != room->players.end();
}

/**
 * Check if an action belongs to a given role.
 *
 * @param role Pointer to the role object.
 * @param action Pointer to the action object.
 * @return `true` if the action belongs to the role, otherwise `false`.
 */
bool action_belongs_to_role(const Role *role, const Action *action) {
    return std::find(role->actions.begin(), role->actions.end(), *action) != role->actions.end();
}

/**
 * Check if an action is allowed based on relevant events.
 *
 * @param action Pointer to the action object.
 * @param relevantEvents Pointer to the vector of relevant events.
 * @return `true` if the action is allowed, otherwise `false`.
 */
bool is_action_allowed(const Action *action, const std::vector<Event> *relevant_events) {
    bool allowed = false;  // Actions are disabled by default
    for (const auto &event : *relevant_events) {
        if (std::find(event.prohibits.begin(), event.prohibits.end(), *action) != event.prohibits.end()) {
            return false;  // If action is prohibited, return false immediately
        }
        if (std::find(event.allows.begin(), event.allows.end(), *action) != event.allows.end()) {
            allowed = true;
        }
    }
    return allowed;
}

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
    return ValidationStatus::ActionProhibited;
}
