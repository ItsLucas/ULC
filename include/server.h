#pragma once
#include "log.h"
#include "crow.h"

typedef void (*setup_routes_extra_t)(crow::SimpleApp &app);

extern setup_routes_extra_t setup_routes_extra;

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