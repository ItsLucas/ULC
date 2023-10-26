#include "config.h"
#include "log.h"
#include "plugin_loader.h"
#include "server_config.h"

#include <fstream>
#include <iostream>

int main() {
  spdlog::set_level(spdlog::level::debug);
  ULC::Config &config = ULC::Config::getInstance();
  config.load("config.json");

  auto &logger = ULC::Logger::getInstance();
  logger.info("Loading server config");

  auto &server_config = ULC::ServerConfig::getInstance();
  logger.info("Server name: " + server_config.server_name());
  logger.info("Server ip: " + server_config.server_ip());
  logger.info("Server port: " + std::to_string(server_config.server_port()));
  logger.info("Max clients: " + std::to_string(server_config.max_clients()));
  logger.info("Plugins:");

  ULC::PluginLoader &plugin_loader = ULC::PluginLoader::getInstance();
  for (auto &plugin : server_config.plugins()) {
    logger.info("  " + plugin);
    plugin_loader.load(plugin);
  }

  plugin_loader.do_command("test");

  plugin_loader.unload("testplugin");
  return 0;
}