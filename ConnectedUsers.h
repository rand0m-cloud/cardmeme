#include "User.h"
#include <memory>
#include <mutex>
#include <vector>
class ConnectedUsers {
private:
  static std::vector<Client> clients_;
  static std::mutex clientsMutex_;

public:
  ConnectedUsers(const ConnectedUsers &) = delete;
  ConnectedUsers operator=(const ConnectedUsers &) = delete;
  static std::vector<Client> &getClients();
  static void releaseClients();
};