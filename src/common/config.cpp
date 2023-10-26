#include "config.h"
#include "log.h"

#include <fstream>
#include <iostream>

namespace ULC {
void Config::load(const std::string &path) {
  Logger::getInstance().logd(LogLevel::INFO, "Loading config from " + path + "...", "ULC::Config");
  /* Load json from path into class */
  std::ifstream f(path);
  f >> m_json;
}

void Config::save(const std::string &path) {
  Logger::getInstance().logd(LogLevel::INFO, "Saving config to " + path + "...", "ULC::Config");
  /* Save json from class to path */
  std::ofstream f(path);
  f << m_json.dump(2);
}

nlohmann::json Config::subconfig(const std::string &name) {
  /* Return subconfig with name */
  return m_json[name];
}

}