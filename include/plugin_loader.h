#pragma once
#include "config.h"
#include "plugin_base.h"
#include <list>
#include <string>
#include <vector>
#include <map>

namespace ULC {
/* Singleton plugin loader. */
class PluginLoader {
public:
  static PluginLoader &getInstance() {
    static PluginLoader instance;
    return instance;
  }
  bool load(const std::string &plugin_name);
  bool unload(const std::string &plugin_name);
  bool reload(const std::string &plugin_name);
  bool do_command(const std::string &command);
  /* Getters */
  std::shared_ptr<plugin_base> plugin(const std::string &plugin_name);

private:
  PluginLoader() = default;
  std::vector<std::shared_ptr<plugin_base>> m_plugins;
  std::map<std::string, std::shared_ptr<plugin_base>> m_commands_map;
  void erase_plugin(const std::string &plugin_name);
};
} // namespace ULC