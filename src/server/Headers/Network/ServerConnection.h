#ifndef PROJECT_SERVER_H
#define PROJECT_SERVER_H

#include "gamelib/Headers/NetworkPacket.h"
#include "gamelib/Headers/Subject.h"
#include <atomic>
#include <enetpp/server.h>
#include <map>

struct server_client
{
  unsigned int get_id() { return _uid; };
  unsigned int _uid;
};

struct client_info
{
  std::string ip;
  GameLib::CharacterType character = GameLib::CharacterType ::NONE;
  bool connected = false;
  bool reconnecting = false;
};

class ServerConnection
  : public GameLib::Subject
  , public GameLib::Observer
{
 public:
  ServerConnection();
  ~ServerConnection() override;

  void initialise();
  void start();
  void run();
  bool isRunning() { return running; };

  void onNotify(GameLib::NetworkPacket& data) override;
  void sendDataToClient(GameLib::NetworkPacket& packet, unsigned int id);
  void sendDataToClients(GameLib::NetworkPacket& packet);
  void sendDataToClientsExcluding(GameLib::NetworkPacket& packet,
                                  unsigned int exclude_id);

  void processClientMessages();

 private:
  void sendAllPackets();
  void readCommandInput();

  std::map<int, client_info> clients;

  enetpp::server<server_client> network_server;
  int next_uid = 0;

  std::atomic<bool> running = true;

  std::mutex out_guard;
  std::mutex in_guard;

  std::queue<GameLib::NetworkPacket> out_queue_all;
  std::queue<std::pair<GameLib::NetworkPacket, unsigned int>> out_queue_specific;
  std::queue<std::pair<GameLib::NetworkPacket, unsigned int>>
    out_queue_excluding;

  std::queue<GameLib::NetworkPacket> in_queue;
};

#endif // PROJECT_SERVER_H
