#ifndef PROJECT_PERSISTENTSERVERDATA_H
#define PROJECT_PERSISTENTSERVERDATA_H

#include "ClientData.h"
#include <map>

class PersistentServerData
{
 public:
  PersistentServerData() = default;
  ~PersistentServerData() = default;

  void addClient(int client_id);
  void removeClient(int client_id);

  int getNumberOfClients();
  ClientData& getClientData(int client_id);
  int getIdForCharacter(GameLib::CharacterType& character);

  void setAllReady(bool is_ready);
  int getNumberOfReady();
  std::map<int, ClientData>& getData() { return client_data; };

  bool isCharacterTaken(const GameLib::CharacterType& type);

  void setConnected(int client_id, bool flag);
  int disconnectedCount();

 private:
  std::map<int, ClientData> client_data;
};

#endif // PROJECT_PERSISTENTSERVERDATA_H
