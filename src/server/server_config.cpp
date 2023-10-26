#include "server_config.h"

namespace ULC {
void ServerConfig::loadall() {
  m_server_name = m_json["server_name"];
  m_server_ip = m_json["server_ip"];
  m_server_port = m_json["server_port"];
  m_max_clients = m_json["max_clients"];
  for (auto &plugin : m_json["plugins"]) {
    m_plugins.push_back(plugin);
  }
}

void ServerConfig::flush() {
  Config &config = Config::getInstance();
  config.load("config.json");
  open();
  loadall();
}
} // namespace ULC