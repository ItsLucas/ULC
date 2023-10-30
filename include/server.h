#pragma once
#include "log.h"
#include "crow.h"

namespace ULC {
class Server {
public:
  explicit Server(std::string &ip, int port);
  ~Server();
  void restart(std::string &ip, int port);
private:
  void setup_routes();
  crow::SimpleApp m_app;
};
}