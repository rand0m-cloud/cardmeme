#include "User.h"
#include "Card.pb.h"
User::User(const std::string &clientName, int num) {
  name = clientName;
  favoriteNumber = num;
}
User::User(const std::vector<char> &message) {
  NewClient cl;
  cl.ParseFromArray(&message[0], message.size());
  name = cl.name();
  favoriteNumber = cl.favoritenumber();
}
User::~User() {}
void User::generateClient(Client *c) {

  c->set_name(name);
  c->set_clientid(id);
}