#ifndef PROJECT_CLIENT_H
#define PROJECT_CLIENT_H

#include "gamelib/Headers/NetworkPacket.h"
#include "gamelib/Headers/Subject.h"
#include <atomic>
#include <enetpp/client.h>
#include <queue>

class ClientConnection
  : public GameLib::Subject
  , public GameLib::Observer
{
 public:
  ClientConnection() = default;
  ~ClientConnection() override;
  void initialise();
  void run();

  void connectToServer(const char* ip);

  void sendDataToServer(GameLib::NetworkPacket& packet);
  void onNotify(GameLib::NetworkPacket& data) override;
  void processServerMessages();

  bool isConnected() { return is_connected; };
  void disconnect();

 private:
  int id = 0;

  enetpp::client client;

  std::atomic<bool> is_connected = false;
  std::atomic<bool> running = true;
  std::mutex out_guard;
  std::mutex in_guard;

  std::queue<GameLib::NetworkPacket> out_queue;
  std::queue<GameLib::NetworkPacket> in_queue;

  void connection();
  void disconnection();
  void data(const enet_uint8* data, size_t data_size);
};

#endif // PROJECT_CLIENT_H
