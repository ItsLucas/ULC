#pragma once
#include "config.h"
#include <list>

namespace ULC {
class ClientConfig {
public:
  static ClientConfig &getInstance() {
    static ClientConfig instance;
    return instance;
  }
  void flush();
  /* Getters */
  std::string client_name() { return m_client_name; }
  std::string &client_ip() { return m_client_ip; }
  int client_port() { return m_client_port; }
  std::list<std::string> plugins() { return m_plugins; }

private:
  ClientConfig() {
    open();
    loadall();
  }
  void open() {
    Config &config = Config::getInstance();
    m_json = config.subconfig("client");
  }
  void loadall();
  nlohmann::json m_json;

  /* client data fields */
  std::string m_client_name;
  std::string m_client_ip;
  int m_client_port;
  std::list<std::string> m_plugins;
};
} // namespace ULC