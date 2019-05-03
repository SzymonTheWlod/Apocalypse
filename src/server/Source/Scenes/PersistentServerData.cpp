#include "server/Headers/Scenes/PersistentServerData.h"

void PersistentServerData::addClient(int client_id)
{
  using pair = std::pair<int, ClientData>;
  client_data.insert(pair(client_id, ClientData()));
}

void PersistentServerData::removeClient(int client_id)
{
  client_data.erase(client_id);
}

int PersistentServerData::getNumberOfClients()
{
  return static_cast<int>(client_data.size());
}

ClientData& PersistentServerData::getClientData(int client_id)
{
  return client_data[client_id];
}

void PersistentServerData::setAllReady(bool is_ready)
{
  for (auto& [key, val] : client_data)
  {
    val.ready = is_ready;
  }
}

int PersistentServerData::getNumberOfReady()
{
  int count = 0;
  for (auto& [key, val] : client_data)
  {
    if (val.ready)
    {
      count++;
    }
  }
  return count;
}

int PersistentServerData::getIdForCharacter(GameLib::CharacterType& character)
{
  for (auto& [key, val] : client_data)
  {
    if (val.type == character)
    {
      return key;
    }
  }
  return -1;
}

bool PersistentServerData::isCharacterTaken(const GameLib::CharacterType& type)
{
  for (auto& [key, val] : client_data)
  {
    if (val.type == type)
    {
      return true;
    }
  }
  return false;
}

void PersistentServerData::setConnected(int client_id, bool flag)
{
  client_data[client_id].connected = flag;
}

int PersistentServerData::disconnectedCount()
{
  int count = 0;
  for (auto& [key, val] : client_data)
  {
    if (!val.connected)
    {
      count++;
    }
  }

  return count;
}