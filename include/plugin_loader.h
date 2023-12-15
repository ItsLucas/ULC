#pragma once
#include "cmd.h"
#include "config.h"
#include "plugin_base.h"
#include <cli/cli.h>
#include <crow/app.h>
#include <list>
#include <map>
#include <string>
#include <vector>

namespace ULC {
/* Singleton plugin loader. */
class PluginLoader {
public:
  static PluginLoader &getInstance() {
    static PluginLoader instance;
    return instance;
  }
  using cmdinfo = std::pair<std::string, std::string>;
  bool load(const std::string &plugin_name);
  bool unload(const std::string &plugin_name);
  bool reload(const std::string &plugin_name);
  std::unique_ptr<ULC::cmd> by_name(const std::string &command);
  /* Getters */
  std::shared_ptr<plugin_base> plugin(const std::string &plugin_name);
  bool setup_routes_for_crow(crow::SimpleApp &app);
  std::list<cmdinfo> get_all_commands();
private:
  PluginLoader() = default;
  std::vector<std::shared_ptr<plugin_base>> m_plugins;
  std::map<std::string, std::shared_ptr<plugin_base>> m_commands_map;
  void erase_plugin(const std::string &plugin_name);
};
} // namespace ULC