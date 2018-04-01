#pragma once
#include "User.h"
#include <TcpServer.h>
#include <mutex>
#include <thread>
#include <vector>
class ConnectServer : TcpServer {
public:
  ConnectServer(int maxClients);
  ~ConnectServer();
  void serveForever();

private:
  std::thread listenThread;

  int clientIndex = 0;
  std::mutex clientIndexMutex;

  void listen();
  void handleClient(std::shared_ptr<TcpSocket> client);

  std::shared_ptr<ServerResponse> generateClientList();

  std::vector<User> connectedUsers;
  std::mutex connectedUsersMutex;
};