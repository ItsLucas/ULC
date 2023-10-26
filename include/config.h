#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace ULC {

class Config {
public:
  static Config &getInstance() {
    static Config instance;
    return instance;
  }

  Config(const Config &) = delete;
  Config &operator=(const Config &) = delete;

  void load(const std::string &path);
  void save(const std::string &path);

  nlohmann::json subconfig(const std::string &name);

private:
  Config() = default;
  ~Config() = default;

  nlohmann::json m_json;
};

} // namespace ULC