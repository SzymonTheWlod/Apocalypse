#include "server/Headers/Network/ServerConnection.h"
#include "gamelib/Headers/NetworkPacket.h"
#include "gamelib/Headers/Printer.h"
#include <iostream>
#include <memory>

ServerConnection::ServerConnection()
{
  enetpp::global_state::get().initialize();
}

ServerConnection::~ServerConnection()
{
  network_server.stop_listening();
  enetpp::global_state::get().deinitialize();
}

void ServerConnection::initialise()
{
  auto init_client_func = [&](server_client& client, const char* ip) {
    client._uid = static_cast<unsigned int>(next_uid);

    // Check to see if this client is re-connecting
    bool prev_client = false;
    for (auto& [key, val] : clients)
    {
      if (val.ip == ip && !val.connected)
      {
        // If it is overwrite the uid with their previous one
        prev_client = true;
        client._uid = static_cast<unsigned int>(key);
        val.connected = true;
        val.reconnecting = true;
        break;
      }
    }

    if (!prev_client)
    {
      int uid_as_int = next_uid;
      clients.insert(std::pair<int, client_info>(uid_as_int, client_info()));
      clients[uid_as_int].ip = ip;
      clients[uid_as_int].connected = true;
      next_uid++;
    }
  };

  auto params = enetpp::server_listen_params<server_client>()
                  .set_max_client_count(20)
                  .set_channel_count(1)
                  .set_listen_port(8888)
                  .set_initialize_client_function(init_client_func);
  params._peer_timeout = std::chrono::milliseconds(2000);

  network_server.start_listening(params);
}

void ServerConnection::start()
{
  addObserver(this, GameLib::EventType::CHAT_MESSAGE);

  // std::thread th(&ServerConnection::run, this);
  // th.detach();

  std::thread thr(&ServerConnection::readCommandInput, this);
  thr.detach();

  std::thread thr2(&ServerConnection::run, this);
  thr2.detach();
}

void ServerConnection::run()
{
  auto on_connect = [&](server_client& client) {
    int id_int = static_cast<int>(client._uid);
    GameLib::Printer() << "client " << std::to_string(client._uid)
                       << ": has connected" << std::endl;
    GameLib::NetworkPacket packet(GameLib::EventType::CLIENT_CONNECTED);
    packet.setClientId(id_int);
    notify(packet.getType(), packet);
    sendDataToClients(packet);

    GameLib::NetworkPacket id_packet(GameLib::EventType::SET_ID);
    id_packet << client._uid;
    sendDataToClient(id_packet, client._uid);

    if (clients[id_int].reconnecting)
    {
      clients[id_int].reconnecting = false;
      GameLib::NetworkPacket packet2(GameLib::EventType::CLIENT_RECONNECTED);
      packet2.setClientId(id_int);
      std::lock_guard<std::mutex> guard(in_guard);
      in_queue.push(packet2);
    }
  };

  auto on_disconnect = [&](unsigned int client_uid) {
    GameLib::Printer() << "client " << std::to_string(client_uid)
                       << ": has disconnected\n";
    clients[static_cast<int>(client_uid)].connected = false;
    GameLib::NetworkPacket packet(GameLib::EventType::CLIENT_DISCONNECTED);
    packet.setClientId(static_cast<int>(client_uid));
    std::lock_guard<std::mutex> guard(in_guard);
    in_queue.push(packet);
    sendDataToClients(packet);
  };

  auto on_data =
    [&](server_client& client, const enet_uint8* data, size_t data_size) {
      GameLib::NetworkPacket packet(data, data_size);

      GameLib::Printer() << "Received packet of type: "
                         << static_cast<int>(packet.getType()) << std::endl;

      std::lock_guard<std::mutex> guard(in_guard);
      in_queue.push(packet);
    };
  // while server should not terminate

  while (running)
  {
    network_server.consume_events(on_connect, on_disconnect, on_data);
    sendAllPackets();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void ServerConnection::sendAllPackets()
{
  std::lock_guard<std::mutex> guard(in_guard);
  while (!out_queue_specific.empty())
  {
    auto data = reinterpret_cast<const enet_uint8*>(
      out_queue_specific.front().first.data());
    network_server.send_packet_to_all_if(
      0,
      data,
      out_queue_specific.front().first.length(),
      ENET_PACKET_FLAG_RELIABLE,
      [&](const server_client& destination) {
        return destination._uid == out_queue_specific.front().second;
      });
    out_queue_specific.pop();
  }
  while (!out_queue_excluding.empty())
  {
    auto data = reinterpret_cast<const enet_uint8*>(
      out_queue_excluding.front().first.data());
    network_server.send_packet_to_all_if(
      0,
      data,
      out_queue_excluding.front().first.length(),
      ENET_PACKET_FLAG_RELIABLE,
      [&](const server_client& destination) {
        return destination._uid != out_queue_excluding.front().second;
      });
    out_queue_excluding.pop();
  }
  while (!out_queue_all.empty())
  {
    auto data =
      reinterpret_cast<const enet_uint8*>(out_queue_all.front().data());
    network_server.send_packet_to_all_if(
      0,
      data,
      out_queue_all.front().length(),
      ENET_PACKET_FLAG_RELIABLE,
      [&](const server_client& destination) { return true; });
    out_queue_all.pop();
  }
}

void ServerConnection::sendDataToClient(GameLib::NetworkPacket& packet,
                                        unsigned int id)
{
  std::lock_guard<std::mutex> guard(out_guard);
  using pair = std::pair<GameLib::NetworkPacket, unsigned int>;
  out_queue_specific.push(pair(packet, id));
}

void ServerConnection::sendDataToClients(GameLib::NetworkPacket& packet)
{
  std::lock_guard<std::mutex> guard(out_guard);
  out_queue_all.push(packet);
}

void ServerConnection::sendDataToClientsExcluding(
  GameLib::NetworkPacket& packet, unsigned int exclude_id)
{
  std::lock_guard<std::mutex> guard(out_guard);
  using pair = std::pair<GameLib::NetworkPacket, unsigned int>;
  out_queue_excluding.push(pair(packet, exclude_id));
}

void ServerConnection::onNotify(GameLib::NetworkPacket& data)
{
  if (data.getType() == GameLib::EventType::CHAT_MESSAGE)
  {
    sendDataToClientsExcluding(data, data.getClientId());
  }
}

void ServerConnection::readCommandInput()
{
  while (running)
  {
    std::string input;
    std::getline(std::cin, input);

    if (input == "exit")
    {
      running = false;
    }
  }
}

void ServerConnection::processClientMessages()
{
  std::lock_guard<std::mutex> guard(in_guard);
  while (!in_queue.empty())
  {
    notify(in_queue.front().getType(), in_queue.front());
    in_queue.pop();
  }
}