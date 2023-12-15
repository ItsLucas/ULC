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
  std::string m_retrieved_args;
  json m_data;

public:
  retval() = default;

  retval(bool success, const std::string &message,
         const std::string &retrieved_args)
      : m_success(success), m_message(message),
        m_retrieved_args(retrieved_args) {
    serialize();
  }

  retval(const std::string &json_str) {
    if (json_str.empty()) {
      m_success = false;
      m_message = "Empty json string";
      m_retrieved_args = "";
      m_data = json::parse("{}");
      return;
    }
    m_data = json::parse(json_str);
    deserialize();
  }

  bool ok() { return m_success; }

  bool fail() { return !m_success; }

  void serialize() {
    m_data["status"] = m_success ? "OK" : "ERROR";
    m_data["message"] = m_message;
    m_data["retrieved_args"] = m_retrieved_args;
  }

  void deserialize() {
    m_success = m_data["status"] == "OK" ? true : false;
    m_message = m_data["message"];
    m_retrieved_args = m_data["retrieved_args"];
  }

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

class remote_arg {
public:
  remote_arg() {}
  remote_arg(const std::string &json) {
    auto j = json::parse(json);
    for (auto x : j["args"]) {
      m_args.push_back(x);
    }
  }
  ~remote_arg() {}
  std::string to_string() {
    nlohmann::json j;
    j["args"] = m_args;
    return j.dump();
  }
  void push(std::string &arg) { m_args.push_back(arg); }
  std::vector<std::string> &args() { return m_args; }
private:
  std::vector<std::string> m_args;
};

} // namespace ULC
#endif