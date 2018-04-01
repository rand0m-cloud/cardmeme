#pragma once
#include "Card.pb.h"
#include <string>
#include <thread>
#include <vector>
class User {
public:
  std::string name;
  int favoriteNumber;
  int id;
  User(const std::string &name, int favoriteNumber);
  User(const std::vector<char> &message);
  ~User();
  bool operator==(const User &rhs) const { return id == rhs.id; }
  void generateClient(Client *c);

private:
};