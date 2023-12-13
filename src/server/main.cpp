
#include "config.h"
#include "log.h"
#include "plugin_loader.h"
#include "server.h"
#include "server_config.h"
#include "ulc_thd_manager.h"
#include <cli/cli.h>
#include <cli/clifilesession.h>
#include <fstream>
#include <iostream>
#include <thread>

std::unique_ptr<ULC::Server> server_ptr = nullptr;
std::thread server_thread;

void cleanup() {
  ULC::PluginLoader &plugin_loader = ULC::PluginLoader::getInstance();
  auto &logger = ULC::Logger::getInstance();
  plugin_loader.unload("testplugin");
  logger.info("Exiting");
  server_ptr.reset(nullptr);
  exit(0);
}

void setup_routes_extra_server(crow::SimpleApp &app) {
  CROW_ROUTE(app, "/server/connect")
      .methods("POST"_method)([](const crow::request &req) {
        auto x = crow::json::load(req.body);
        if (!x)
          return crow::response(400);
        auto name = std::string(x["name"].s().begin(), x["name"].s().end());
        auto ip = std::string(x["ip"].s().begin(), x["ip"].s().end());
        auto port = x["port"].i();
        auto mgr = ULC::Global_THD_Manager::instance();
        mgr->create(name, ip, port);
        return crow::response(200);
      });
  CROW_ROUTE(app, "/server/<string>/disconnect")
      .methods("POST"_method)([](const crow::request &req, std::string name) {
        auto mgr = ULC::Global_THD_Manager::instance();
        mgr->remove(name);
        return crow::response(200);
      });
}

using arglist = std::vector<std::string>;

void ulc_execute_command(std::ostream &os, arglist &args) {
  ULC::PluginLoader &plugin_loader = ULC::PluginLoader::getInstance();
  auto &logger = ULC::Logger::getInstance();
  if (args.empty()) {
    os << "Usage: exec <cmd> <args>...\n";
    return;
  }
  auto cmd = plugin_loader.by_name(args[0]);
  if (cmd == nullptr) {
    os << "Command not found: " << args[0] << "\n";
    return;
  }
  /* Pass remaining args into cmd */
  if (args.size() > 1){
    for (auto i = 1; i < args.size(); i++) {
      cmd->add_arg(args[i]);
    }
  }
  auto ret = cmd->execute();
  logger.info("Command returned: " + ret.to_string());
  cmd.reset(nullptr);
}

int main() {
  spdlog::set_level(spdlog::level::debug);
  setup_routes_extra = setup_routes_extra_server;

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

  server_thread = std::thread([&]() {
    if (server_ptr == nullptr) {
      server_ptr = std::make_unique<ULC::Server>(server_config.server_ip(),
                                                 server_config.server_port());
    }
  });
  server_thread.detach();
  auto rootMenu = std::make_unique<cli::Menu>("cli");
  rootMenu->Insert(
      "exec",
      [](std::ostream &out, arglist args) { ulc_execute_command(out, args); },
      "Usage: exec <cmd> <args>...");
  auto m_cli = cli::Cli(std::move(rootMenu));
  // global exit action
  m_cli.ExitAction(
      [](auto &out) { out << "Goodbye and thanks for all the fish.\n"; });

  cli::CliFileSession input(m_cli);
  input.Start();
  cleanup();
  return 0;
}