#include "gamelib/Headers/NetworkPacket.h"

GameLib::NetworkPacket::NetworkPacket(unsigned long long int size)
{
  packet_data.reserve(size);
}

char* GameLib::NetworkPacket::data()
{
  // When taking the data, append the packet type and client id
  *this << type;
  *this << client_id;
  return packet_data.data();
}

unsigned long long int GameLib::NetworkPacket::length()
{
  return packet_data.size();
}
GameLib::NetworkPacket::NetworkPacket(const enet_uint8* data, size_t size)
{
  packet_data.insert(packet_data.end(), data, data + size);
  // When re-constructing the packet, take the packet type that was appended
  // when the data was grabbed
  auto as_type = reinterpret_cast<GameLib::EventType*>(&packet_data[size - 8]);
  type = *as_type;
  packet_data.erase(packet_data.end() - 8, packet_data.end() - 4);
  auto id = reinterpret_cast<int*>(&packet_data[size - 4]);
  client_id = *id;
  packet_data.erase(packet_data.end() - 4, packet_data.end());
}