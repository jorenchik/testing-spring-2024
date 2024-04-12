#include "./role.hh"

#include <initializer_list>
#include <vector>

Role::Role(){
}

Role::Role(std::initializer_list<Action> actions): Role(std::vector<Action>(actions)) {
}

Role::Role(std::vector<Action> actions): actions(actions) {
}
