#include "ConnectServer.h"
#include "Card.pb.h"
#include "ConnectedUsers.h"
#include <algorithm>
#include <thread>
#include <utility>

ConnectServer::ConnectServer(int maxClients)
    : TcpServer("0.0.0.0", 10000, true, maxClients, true) {
  listenThread = std::thread(&ConnectServer::listen, this);
}
ConnectServer::~ConnectServer() {}
void ConnectServer::listen() {
  while (1) {
    std::shared_ptr<TcpSocket> client = getClient();
    std::cout << "client found" << std::endl;
    std::thread(&ConnectServer::handleClient, this, client).detach();
  }
}
void ConnectServer::serveForever() { listenThread.join(); }
void ConnectServer::handleClient(std::shared_ptr<TcpSocket> client) {
  std::vector<char> message;
  if (!client->getData(message)) {
    return;
  };
  User c = User(message);
  {
    std::lock_guard<std::mutex> _(clientIndexMutex);
    c.id = ++clientIndex;
  }

  {
    std::lock_guard<std::mutex> _(connectedUsersMutex);
    connectedUsers.push_back(c);
  }

  std::cout << "client name:" << c.name << std::endl;
  std::cout << "favorite number:" << c.favoriteNumber << std::endl;
  std::cout << "client id:" << c.id << std::endl;

  ClientResponse cr;
  cr.set_clientid(c.id);
  if (!client->sendData(cr.SerializeAsString())) {
    std::lock_guard<std::mutex> _(connectedUsersMutex);
    connectedUsers.erase(
        std::remove(connectedUsers.begin(), connectedUsers.end(), c),
        connectedUsers.end());
  }
  while (1) {
    std::vector<char> message;
    if (!client->getData(message)) {
      return;
    }
    ClientMessage wm;
    wm.ParseFromArray(&message[0], message.size());
    switch (wm.msg_case()) {
    case wm.MsgCase::kGetClients: {
      auto response = generateClientList();
      client->sendData(response->SerializeAsString());
      break;
    }
    case wm.MsgCase::kPlayCard:
    case wm.MsgCase::MSG_NOT_SET:
    default:
      break;
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));
  }
}
std::shared_ptr<ServerResponse> ConnectServer::generateClientList() {
  std::shared_ptr<ServerResponse> wm = std::make_shared<ServerResponse>();

  ClientList *gc = new ClientList();

  {
    std::lock_guard<std::mutex> _(connectedUsersMutex);
    for (auto &user : connectedUsers) {
      user.generateClient(gc->add_clients());
    }
  }
  wm->set_allocated_clientlist(gc);
  return wm;
}