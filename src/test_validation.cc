#include "modules/role.hh"
#include "modules/room.hh"
#include "validation.hh"

#include "gtest/gtest.h"
#include <string>

struct TestFixtures {
    static const Action kill;
    static const Action heal;
    static const Action vote;
    static Role role1;
    static Event event1;
    static Event event2;
    static Event event3;
    static std::vector<Event> relatedEvents;
    static Player player1;
    static Player player2;
    static Room room1;
};

const Action TestFixtures::kill = Action("kill", true);
const Action TestFixtures::heal = Action("heal", true);
const Action TestFixtures::vote = Action("vote", true);
Role TestFixtures::role1 = Role({vote, kill, heal});
Event TestFixtures::event1 = Event("Event 1", 1710087355, 1, true, {}, {});
Event TestFixtures::event2 = Event("Event 2", 1710087363, 1, true, {kill}, {});
Event TestFixtures::event3 = Event("Event 3", 1710087369, 1, true, {}, {kill});
std::vector<Event> TestFixtures::relatedEvents = std::vector<Event>({event2, event3});
Player TestFixtures::player1 = Player(69, "player1", role1, PlayerStatus::Alive);
Player TestFixtures::player2 = Player(420, "player2", role1, PlayerStatus::Alive);
Room TestFixtures::room1 =
  Room(1, "Room 1", 1710087364, RoomStatus::InProgress, std::vector<Player> {player1, player2});

TEST(ValidateActionBranchDecisionTests, ReachesNoActorReturn) {
    ValidationStatus status = validate_action(
      nullptr, &TestFixtures::kill, &TestFixtures::room1, &TestFixtures::relatedEvents, &TestFixtures::player2);
    ASSERT_EQ(ValidationStatus::NoActor, status);
}

TEST(ValidateActionBranchDecisionTests, ReachesNoActionReturn) {
    ValidationStatus status = validate_action(
      &TestFixtures::player1, nullptr, &TestFixtures::room1, &TestFixtures::relatedEvents, &TestFixtures::player2);
    ASSERT_EQ(ValidationStatus::NoAction, status);
}

TEST(ValidateActionBranchDecisionTests, ReachesNoRoomReturn) {
    ValidationStatus status = validate_action(
      &TestFixtures::player1, &TestFixtures::kill, nullptr, &TestFixtures::relatedEvents, &TestFixtures::player2);
    ASSERT_EQ(ValidationStatus::NoRoom, status);
}

TEST(ValidateActionBranchDecisionTests, ReachesNoRelatedEventsReturn) {
    ValidationStatus status = validate_action(
      &TestFixtures::player1, &TestFixtures::kill, &TestFixtures::room1, nullptr, &TestFixtures::player2);
    ASSERT_EQ(ValidationStatus::NoRelatedEvents, status);
}

TEST(ValidateActionBranchDecisionTests, ReachesNotInTheRoomReturn) {
    Room emptyRoom = Room(2, "Room 4", 1710087394, RoomStatus::InProgress, {});
    ValidationStatus status = validate_action(
      &TestFixtures::player1, &TestFixtures::kill, &emptyRoom, &TestFixtures::relatedEvents, &TestFixtures::player2);
    ASSERT_EQ(ValidationStatus::PlayerNotInRoom, status);
}

TEST(ValidateActionBranchDecisionTests, ReachesNoTargetReturn) {
    ValidationStatus status = validate_action(
      &TestFixtures::player1, &TestFixtures::kill, &TestFixtures::room1, &TestFixtures::relatedEvents, nullptr);
    ASSERT_EQ(ValidationStatus::NoTargetPlayerSpecified, status);
}

TEST(ValidateActionBranchDecisionTests, ReachesGameNotInProgressReturn) {
    Room roomNotInProgress =
      Room(2, "Room 5", 1710087394, RoomStatus::Ended, {TestFixtures::player1, TestFixtures::player2});
    ValidationStatus status = validate_action(&TestFixtures::player1,
                                              &TestFixtures::kill,
                                              &roomNotInProgress,
                                              &TestFixtures::relatedEvents,
                                              &TestFixtures::player2);
    ASSERT_EQ(ValidationStatus::RoomNotInProgress, status);
}

TEST(ValidateActionBranchDecisionTests, ReachesActionProhibitedReturn) {
    std::vector<Event> noAllowingEvent = std::vector<Event>();
    ValidationStatus status = validate_action(
      &TestFixtures::player1, &TestFixtures::kill, &TestFixtures::room1, &noAllowingEvent, &TestFixtures::player2);
    ASSERT_EQ(ValidationStatus::ActionProhibited, status);
}

TEST(ValidateActionBranchDecisionTests, ReachesActionDoesntBelongToTheRoleReturn) {
    Role roleWithNoMatchingAction = Role({TestFixtures::vote, TestFixtures::heal});
    Player player1 = Player(69, "player1", roleWithNoMatchingAction, PlayerStatus::Alive);
    ValidationStatus status = validate_action(
      &player1, &TestFixtures::kill, &TestFixtures::room1, &TestFixtures::relatedEvents, &TestFixtures::player2);
    ASSERT_EQ(ValidationStatus::ActionDoesNotBelongToRole, status);
}

TEST(ValidateActionBranchDecisionTests, ReachesNoTopLevelReturn) {
    ValidationStatus status = validate_action(&TestFixtures::player1,
                                              &TestFixtures::kill,
                                              &TestFixtures::room1,
                                              &TestFixtures::relatedEvents,
                                              &TestFixtures::player2);
    ASSERT_EQ(ValidationStatus::ActionValid, status);
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
