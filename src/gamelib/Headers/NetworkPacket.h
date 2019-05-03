#ifndef PROJECT_NETWORKPACKET_H
#define PROJECT_NETWORKPACKET_H
#include "gamelib/Headers/Constants.h"
#include <cstddef>
#include <cstring>
#include <enet/types.h>
#include <string>
#include <vector>

namespace GameLib
{
class NetworkPacket
{
 public:
  explicit NetworkPacket(GameLib::EventType event_type) { type = event_type; };

  explicit NetworkPacket(unsigned long long int size);
  NetworkPacket(const enet_uint8* data, size_t size);

  template<typename T>
  NetworkPacket& operator<<(const T& data)
  {
    auto size = sizeof(data);
    auto as_char = reinterpret_cast<const char*>(&data);

    packet_data.insert(packet_data.end(), as_char, as_char + size);

    return *this;
  }

  // String overload
  NetworkPacket& operator<<(const std::string& str)
  {
    auto data = str.data();

    auto length = str.length() + 1;
    packet_data.insert(packet_data.end(), data, data + length);

    return *this;
  }

  template<typename T>
  NetworkPacket& operator>>(T& data)
  {
    auto size = sizeof(data);
    auto as_type = reinterpret_cast<T*>(&packet_data[read_pos]);
    data = *as_type;
    read_pos += size;
    return *this;
  }

  // String overload
  NetworkPacket& operator>>(std::string& str)
  {
    auto length = std::strlen(&packet_data[read_pos]);

    str = std::string(&packet_data[read_pos], length);
    read_pos += str.size() + 1;

    return *this;
  }

  char* data();
  unsigned long long int length();

  void resetReadPos() { read_pos = 0; };

  EventType& getType() { return type; };
  void setClientId(int new_id) { client_id = new_id; };
  int getClientId() { return client_id; };

 private:
  int client_id = 0;
  GameLib::EventType type = GameLib::EventType ::DEFAULT;
  std::size_t read_pos = 0;
  std::vector<char> packet_data;
};
}

#endif // PROJECT_NETWORKPACKET_H
