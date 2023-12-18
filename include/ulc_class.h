#pragma once
#include "log.h"
#include <random>
#include <sstream>
#include <string>

std::string generate_uuid();
namespace ULC {
/* This is what you think it should be. */
class THD {
public:
  THD(std::string &name, std::string &ip, int port)
      : m_name(name), m_ip(ip), m_port(port) {
    m_uuid = generate_uuid();
    Logger::getInstance().info("Creating THD " + m_name + " " + m_ip + ":" +
                                   std::to_string(port) + " " + m_uuid,
                               "thd");
  }

  ~THD() {
    Logger::getInstance().info("Destroying THD " + m_name + " " + m_ip + ":" +
                                   std::to_string(m_port) + " " + m_uuid,
                               "thd");
  }
  void add_remote_command(std::string &plugin_name, std::string &cmd_name) {
    m_remote_commands[cmd_name] = plugin_name;
  }

  std::pair<std::string, std::string>
  get_remote_command(std::string &cmd_name) {
    if (m_remote_commands.find(cmd_name) != m_remote_commands.end())
      return std::make_pair(m_remote_commands[cmd_name], cmd_name);
    else
      return std::make_pair("null", "null");
  }

  std::string name() { return m_name; }
  std::string ip() { return m_ip; }
  int port() { return m_port; }
  std::string uuid() { return m_uuid; }

  std::mutex LOCK_thd_data;
private:
  std::string m_name;
  std::string m_ip;
  std::string m_uuid;
  int m_port;
  std::map<std::string, std::string> m_remote_commands;
};
} // namespace ULC