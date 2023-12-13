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
private:
  std::string m_name;
  std::string m_ip;
  std::string m_uuid;
  int m_port;
};
} // namespace ULC