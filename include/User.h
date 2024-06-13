#pragma once

#include <string>

struct User {
  int id;
  std::string name;
  std::string password;
  int roleId;
  User(int id, std::string name, int roleId, std::string password)
      : id(id), name(name), roleId(roleId), password(password) {}
};