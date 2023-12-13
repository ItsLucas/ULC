#include "client_config.h"

namespace ULC {
void ClientConfig::loadall() {
  m_client_name = m_json["client_name"];
  m_client_ip = m_json["client_ip"];
  m_client_port = m_json["client_port"];
  for (auto &plugin : m_json["plugins"]) {
    m_plugins.push_back(plugin);
  }
}

void ClientConfig::flush() {
  Config &config = Config::getInstance();
  config.load("config.json");
  open();
  loadall();
}
} // namespace ULC