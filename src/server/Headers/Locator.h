#ifndef PROJECT_LOCATOR_H
#define PROJECT_LOCATOR_H

#include <bits/unique_ptr.h>

class NetworkManager;

class Locator
{
 public:
  Locator() = default;
  static std::unique_ptr<Locator> instance;

  void setNetworkManager(NetworkManager* new_net_manager)
  {
    net_manager = new_net_manager;
  };
  NetworkManager* getNetworkManager() { return net_manager; };

 private:
  NetworkManager* net_manager;
};

#endif // PROJECT_LOCATOR_H
