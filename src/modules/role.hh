#pragma once

#include "./action.hh"

#include <initializer_list>
#include <vector>

struct Role {
    std::vector<Action> actions;
    explicit Role(std::vector<Action> actions);
    Role(std::initializer_list<Action> actions);
    Role();
};
