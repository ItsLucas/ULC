
#include "config.h"
#include "log.h"
#include "plugin_loader.h"
#include "server.h"
#include "server_config.h"
#include <cli/cli.h>
#include <cli/clifilesession.h>
#include <fstream>
#include <iostream>
#include <thread>

std::unique_ptr<ULC::Server> server_ptr = nullptr;
std::thread server_thread;
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

  auto cmd = plugin_loader.by_name("test");
  cmd->add_arg(std::string("this is an arg"));
  auto ret = cmd->execute();
  logger.info("Command returned: " + ret.to_string());
  cmd.reset(nullptr);
  plugin_loader.unload("testplugin");
  server_thread = std::thread([&]() {
    if (server_ptr == nullptr) {
      server_ptr = std::make_unique<ULC::Server>(server_config.server_ip(),
                                                 server_config.server_port());
    }
  });
  server_thread.detach();
  auto rootMenu = std::make_unique<cli::Menu>("cli");
  rootMenu->Insert(
      "hello", [](std::ostream &out) { out << "Hello, world\n"; },
      "Print hello world");
  logger.info("test");
  auto m_cli = cli::Cli(std::move(rootMenu));
  // global exit action
  m_cli.ExitAction([](auto &out) {
    out << "Goodbye and thanks for all the fish.\n";
    server_ptr.reset(nullptr);
  });

  cli::CliFileSession input(m_cli);
  input.Start();
  return 0;
}