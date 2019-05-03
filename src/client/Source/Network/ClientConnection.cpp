#include "client/Headers/Network/ClientConnection.h"
#include "gamelib/Headers/Printer.h"
#include <iostream>

ClientConnection::~ClientConnection()
{
  enetpp::global_state::get().deinitialize();
  running = false;
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void ClientConnection::initialise()
{
  enetpp::global_state::get().initialize();
  addObserver(this, GameLib::EventType::SET_ID);

  std::thread th(&ClientConnection::run, this);
  th.detach();
}

void ClientConnection::run()
{
  while (running)
  {
    if (client.is_connecting_or_connected())
    {
      using namespace std::placeholders;
      client.consume_events(std::bind(&ClientConnection::connection, this),
                            std::bind(&ClientConnection::disconnection, this),
                            std::bind(&ClientConnection::data, this, _1, _2));

      std::lock_guard<std::mutex> guard(out_guard);
      while (!out_queue.empty())
      {
        auto& msg = out_queue.front();

        auto data = reinterpret_cast<const enet_uint8*>(msg.data());
        client.send_packet(0, data, msg.length(), ENET_PACKET_FLAG_RELIABLE);
        out_queue.pop();
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void ClientConnection::connection()
{
  GameLib::Printer() << "Connected to server." << std::endl;
  is_connected = true;
}

void ClientConnection::disconnection()
{
  GameLib::Printer() << "Disconnected from server." << std::endl;
  is_connected = false;
}

// Receiving data on the network thread
void ClientConnection::data(const enet_uint8* data, size_t data_size)
{
  GameLib::NetworkPacket packet(data, data_size);
  GameLib::Printer() << "Received packet of type: "
                     << static_cast<int>(packet.getType()) << std::endl;
  std::lock_guard<std::mutex> guard(in_guard);
  in_queue.push(packet);
}

// Sending events out on the main thread
void ClientConnection::processServerMessages()
{
  std::lock_guard<std::mutex> guard(in_guard);
  while (!in_queue.empty())
  {
    notify(in_queue.front().getType(), in_queue.front());
    in_queue.pop();
  }
}

void ClientConnection::sendDataToServer(GameLib::NetworkPacket& packet)
{
  std::lock_guard<std::mutex> guard(out_guard);
  packet.setClientId(id);
  out_queue.push(packet);
}

void ClientConnection::connectToServer(const char* ip)
{
  if (!client.is_connecting_or_connected())
  {
    client.connect(enetpp::client_connect_params()
                     .set_channel_count(1)
                     .set_server_host_name_and_port(ip, 8888));
  }
}

void ClientConnection::onNotify(GameLib::NetworkPacket& data)
{
  if (data.getType() == GameLib::EventType ::SET_ID)
  {
    std::lock_guard<std::mutex> guard(out_guard);
    unsigned int new_id;
    data >> new_id;
    id = new_id;
  }
}

void ClientConnection::disconnect()
{
  client.disconnect();
  is_connected = false;
}