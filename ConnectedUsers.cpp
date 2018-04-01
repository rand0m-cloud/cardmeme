#include "ConnectedUsers.h"

std::vector<Client> &ConnectedUsers::getClients() {
  clientsMutex_.lock();
  return clients_;
}
void ConnectedUsers::releaseClients() { clientsMutex_.unlock(); }