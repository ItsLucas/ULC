#pragma once
#include "config.h"
#include <list>

namespace ULC {
class ServerConfig {
public:
  static ServerConfig &getInstance() {
    static ServerConfig instance;
    return instance;
  }
  void flush();
  /* Getters */
  std::string server_name() { return m_server_name; }
  std::string server_ip() { return m_server_ip; }
  int server_port() { return m_server_port; }
  int max_clients() { return m_max_clients; }
  std::list<std::string> plugins() { return m_plugins; }
  
private:
  ServerConfig() {
    open();
    loadall();
  }
  void open() {
    Config &config = Config::getInstance();
    m_json = config.subconfig("server");
  }
  void loadall();
  nlohmann::json m_json;

  /* Server data fields */
  std::string m_server_name;
  std::string m_server_ip;
  int m_server_port;
  int m_max_clients;
  std::list<std::string> m_plugins;
};
}