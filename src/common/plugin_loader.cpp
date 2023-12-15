#include "plugin_loader.h"
#include "server_config.h"
#include <dlfcn.h>

#include <string>

inline std::string composePluginPath(const std::string &plugin_name) {
  std::string plugin_path;
#ifdef __APPLE__
  plugin_path = "lib" + plugin_name + ".dylib";
#else
  plugin_path = "lib" + plugin_name + ".so";
#endif
  return plugin_path;
}
namespace ULC {
bool PluginLoader::load(const std::string &plugin_name) {
  Config &config = Config::getInstance();
  /* Load plugin_name.so and call init() function */
  std::string plugin_path = composePluginPath(plugin_name);
  void *plugin_handle = dlopen(plugin_path.c_str(), RTLD_LAZY);
  if (!plugin_handle) {
    Logger::getInstance().error("Failed to load plugin " + plugin_name + ": " +
                                    dlerror(),
                                "ULC::PluginLoader");
    return false;
  }
  /* Get init() function */
  init_func init = (init_func)dlsym(plugin_handle, "init");
  if (!init) {
    Logger::getInstance().error("Failed to load plugin " + plugin_name + ": " +
                                    dlerror(),
                                "ULC::PluginLoader");
    return false;
  }
  /* Call init() function */
  std::shared_ptr<plugin_base> plugin = init();
  if (!plugin) {
    Logger::getInstance().error("Failed to load plugin " + plugin_name + ": " +
                                    dlerror(),
                                "ULC::PluginLoader");
    return false;
  }
  if (!plugin->load()) {
    Logger::getInstance().error("Failed to load plugin " + plugin_name +
                                "during post initialization"
                                "ULC::PluginLoader");
    return false;
  }
  /* Register plugin commands */
  for (auto &command : plugin->commands()) {
    m_commands_map[command] = plugin;
    Logger::getInstance().info("Registered command " + command + " from " +
                                   plugin_name + ".",
                               "ULC::PluginLoader");
  }
  m_plugins.push_back(plugin);
  return true;
}

bool PluginLoader::unload(const std::string &plugin_name) {
  auto plugin1 = plugin(plugin_name);
  for (auto cmd : plugin1->commands()) {
    /* Erase all commands */
    m_commands_map.erase(cmd);
  }
  /* Unload plugin_name.so and call deinit() function */
  std::string plugin_path = composePluginPath(plugin_name);
  void *plugin_handle = dlopen(plugin_path.c_str(), RTLD_LAZY);
  if (!plugin_handle) {
    Logger::getInstance().error("Failed to unload plugin " + plugin_name +
                                    ": " + dlerror(),
                                "ULC::PluginLoader");
    return false;
  }
  /* Get deinit() function */
  deinit_func deinit = (deinit_func)dlsym(plugin_handle, "deinit");
  if (!deinit) {
    Logger::getInstance().error("Failed to unload plugin " + plugin_name +
                                    ": " + dlerror(),
                                "ULC::PluginLoader");
    return false;
  }
  erase_plugin(plugin_name);
  /* Call deinit() function */
  deinit();
  return true;
}

bool PluginLoader::reload(const std::string &plugin_name) {
  /* Reload plugin_name.so */
  return unload(plugin_name) && load(plugin_name);
}

std::unique_ptr<ULC::cmd> PluginLoader::by_name(const std::string &command) {
  /* Dispatch command to plugin */
  if (m_commands_map.find(command) == m_commands_map.end()) {
    Logger::getInstance().error("Command " + command + " not found.",
                                "ULC::PluginLoader");
    return nullptr;
  }

  auto cmd = m_commands_map[command]->by_name(command);
  if (cmd == nullptr) {
    Logger::getInstance().error("Command " + command + " not found.",
                                "ULC::PluginLoader");
    return nullptr;
  }
  return cmd;
}

std::shared_ptr<plugin_base>
PluginLoader::plugin(const std::string &plugin_name) {
  /* Return plugin with name plugin_name */
  for (auto &plugin : m_plugins) {
    if (plugin->name() == plugin_name) {
      return plugin;
    }
  }
  return nullptr;
}

void PluginLoader::erase_plugin(const std::string &plugin_name) {
  for (auto &plugin : m_plugins) {
    if (plugin->name() == plugin_name) {
      plugin->unload();
      m_plugins.erase(std::remove(m_plugins.begin(), m_plugins.end(), plugin),
                      m_plugins.end());
    }
  }
}

bool PluginLoader::setup_routes_for_crow(crow::SimpleApp &app) {
  for (auto plugin : m_plugins) {
    if (plugin->crow_plugin()) {
      if (!plugin->setup_routes_for_crow(app)) {
        Logger::getInstance().error("Failed to setup routes for plugin " +
                                        plugin->name() + ".",
                                    "ULC::PluginLoader");
        return false;
      }
    }
  }
  return true;
}

std::list<PluginLoader::cmdinfo> PluginLoader::get_all_commands() {
  std::list<cmdinfo> commands;
  for (auto plugin : m_plugins) {
    for (auto cmd : plugin->commands()) {
      commands.push_back(std::make_pair(plugin->name(), cmd));
    }
  }
  return commands;
}

} // namespace ULC