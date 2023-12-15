#pragma once
#include "log.h"
#include "httplib.h"

namespace ULC {
class Request {
private:
  std::unique_ptr<httplib::Client> cli;
  std::string m_ip;
  int m_port;
  std::string m_url;

public:
  Request(std::string ip, int port, std::string url)
      : m_ip(ip), m_port(port), m_url(url) {
    cli = std::make_unique<httplib::Client>(ip, port);
    cli->set_connection_timeout(1, 0);
    cli->set_read_timeout(1, 0);
  }
  ~Request() {}
  std::string get() {
    auto res = cli->Get((m_url).c_str());
    if (res && res->status == 200) {
      Logger::getInstance().logd(LogLevel::INFO,
                                 "Request to " + m_ip + ":" +
                                     std::to_string(m_port) + m_url +
                                     " success",
                                 "ULC::Request");
      return res->body;
    } else {
      Logger::getInstance().logd(LogLevel::INFO,
                                 "Request to " + m_ip + ":" +
                                     std::to_string(m_port) + m_url + " failed",
                                 "ULC::Request");
      /* Return an empty json */
      return "{}";
    }
  }
  std::string post(std::string data) {
    auto res = cli->Post((m_url).c_str(), data, "application/json");
    if (res && res->status == 200) {
      Logger::getInstance().logd(LogLevel::INFO,
                                 "Request to " + m_ip + ":" +
                                     std::to_string(m_port) + m_url +
                                     " success",
                                 "ULC::Request");
      return res->body;
    } else {
      Logger::getInstance().logd(LogLevel::INFO,
                                 "Request to " + m_ip + ":" +
                                     std::to_string(m_port) + m_url + " failed",
                                 "ULC::Request");
      /* Return an empty json */
      return "{}";
    }
  }
};
} // namespace ULC