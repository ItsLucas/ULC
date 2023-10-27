#ifndef CMD_H
#define CMD_H
#include <any>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <vector>

using json = nlohmann::json;

namespace ULC {

/*
Command return value class.
*/
class retval {
  bool m_success;
  std::string m_message;
  json m_data;

public:
  retval() = default;

  retval(bool success, const std::string &message, const json &data)
      : m_success(success), m_message(message), m_data(data) {}

  bool ok() { return m_success; }

  bool fail() { return !m_success; }

  retval &status(bool success) {
    m_success = success;
    return *this;
  }

  std::string message() { return m_message; }
  
  retval &message(const std::string &message) {
    m_message = message;
    return *this;
  }

  json data() { return m_data; }

  retval &data(const json &data) {
    m_data = data;
    return *this;
  }
  
  std::string to_string() {
    std::stringstream ss;
    ss << "success: " << m_success << ", message: " << m_message
       << ", data: " << m_data.dump();
    return ss.str();
  }
};

/*
Abstract Command class. Contains execute() function, args can be pushed into
command.
*/
class cmd {
public:
  cmd() {}
  virtual ~cmd() {}
  virtual retval execute() = 0;

  template <typename T> void add_arg(T arg) { m_args.push_back(arg); }

protected:
  std::vector<std::any> m_args;
};

} // namespace ULC
#endif