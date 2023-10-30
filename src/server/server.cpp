#include "server.h"
#include "log.h"

namespace ULC {

Server::Server(std::string &ip, int port) {
  Logger::getInstance().info("Starting Server...", "server");
  setup_routes();
  m_app.bindaddr(ip).port(port).multithreaded().run_async();
  Logger::getInstance().info("Server started", "server");
}

Server::~Server() { m_app.stop(); }

void Server::restart(std::string &ip, int port) {
  m_app.stop();
  m_app.bindaddr(ip).port(port).multithreaded().run_async();
  Logger::getInstance().info("Server restarted", "server");
}

void Server::setup_routes() {
  CROW_ROUTE(m_app, "/")([]() { return "Hello world"; });
  CROW_ROUTE(m_app, "/add/<int>/<int>")
  ([](int a, int b) { return crow::response(std::to_string(a + b)); });
}
} // namespace ULC