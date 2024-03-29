#pragma once
#include <string>

struct Action {
    std::string name;
    bool has_target;

    Action(std::string name, bool has_target);
    bool operator==(const Action &other) const;
};
