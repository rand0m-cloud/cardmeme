#include "Card.pb.h"
#include <TcpSocket.h>
#include <cstring>
#include <thread>
TcpSocket sock("localhost", 10000);
std::string name;
int favoriteNumber = 0;
void handle();
int main(int argc, char const *argv[]) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  name = argv[1];
  favoriteNumber = std::stoi(argv[2]);
  std::thread t = std::thread(&handle);
  t.join();
  return 0;
}
void handle() {
  NewClient c;
  c.set_name(name);
  c.set_favoritenumber(favoriteNumber);
  if (!sock.sendData(c.SerializeAsString())) {
    return;
  }
  std::vector<char> message;
  if (!sock.getData(message)) {
    return;
  }
  ClientResponse cr;
  cr.ParseFromArray(&message[0], message.size());
  std::cout << "User ID:" << cr.clientid() << std::endl;
  while (1) {
    std::this_thread::sleep_for(std::chrono::seconds(5));
    {
      ClientMessage wm;
      wm.set_allocated_getclients(new GetClients());
      sock.sendData(wm.SerializeAsString());
    }
    {
      std::vector<char> message;
      sock.getData(message);
      ServerResponse wm;
      wm.ParseFromArray(&message[0], message.size());
      const ClientList cl = wm.clientlist();
      for (const Client &c : cl.clients()) {
        std::cout << c.name() << std::endl;
        std::cout << sizeof(c) << std::endl;
      }
    }
  }
}