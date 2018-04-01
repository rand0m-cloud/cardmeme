#include "Card.pb.h"
#include "ConnectServer.h"
#include "User.h"
#include <TcpSocket.h>
#include <fstream>
#include <iostream>
#include <sqlite3.h>
int main(int argc, char const *argv[]) {
  /* code */
  // TcpSocket s("localhost", 5000);
  // s.sendData("memes");
  // std::vector<char> response;
  // s.getData(response);
  // std::cout << std::string(response.begin(), response.end()) << std::endl;
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  ConnectServer cs(5);
  cs.serveForever();
  return 0;
}
