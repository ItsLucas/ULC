
#include "config.h"
#include "log.h"
#include "plugin_loader.h"
#include "request.h"
#include "server.h"
#include "client_config.h"
#include "ulc_thd_manager.h"
#include <cli/cli.h>
#include <cli/clifilesession.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
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

void setup_routes_extra_client(crow::SimpleApp &app) {
  CROW_ROUTE(app, "/client/cmdlist")
  ([]() {
    auto clist = ULC::PluginLoader::getInstance().get_all_commands();
    nlohmann::json j;
    /* Make an array and put pairs inside */
    j["commands"] = nlohmann::json::array();
    for (auto &cmd : clist) {
      j["commands"].push_back(
          {{"plugin_name", cmd.first}, {"cmd_name", cmd.second}});
    }
    return j.dump();
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
  if (args.size() > 1) {
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
  setup_routes_extra = setup_routes_extra_client;
  ULC::Config &config = ULC::Config::getInstance();
  config.load("config.json");

  auto &logger = ULC::Logger::getInstance();
  logger.info("Loading client config");

  auto &client_config = ULC::ClientConfig::getInstance();
  logger.info("Client name: " + client_config.client_name());
  logger.info("Client ip: " + client_config.client_ip());
  logger.info("Client port: " + std::to_string(client_config.client_port()));
  logger.info("Plugins:");

  ULC::PluginLoader &plugin_loader = ULC::PluginLoader::getInstance();
  for (auto &plugin : client_config.plugins()) {
    logger.info("  " + plugin);
    plugin_loader.load(plugin);
  }

  server_thread = std::thread([&]() {
    if (server_ptr == nullptr) {
      server_ptr = std::make_unique<ULC::Server>(client_config.client_ip(),
                                                 client_config.client_port());
    }
  });
  server_thread.detach();

  /* Get server ip port */
  auto server_ip = client_config.server_ip();
  auto server_port = client_config.server_port();
  /* Connect by sending POST to /server/connect with client's name, ip, port */
  {
    ULC::Request req(server_ip, server_port, "/server/connect");
    nlohmann::json j;
    j["name"] = client_config.client_name();
    j["ip"] = client_config.client_ip();
    j["port"] = client_config.client_port();
    auto res = req.post(j.dump());
    logger.info("Server responded: " + res);
  }
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
  /* Disconnect by sending POST to /server/connect with client's name, ip, port */
  {
    ULC::Request req(server_ip, server_port, "/server/disconnect");
    nlohmann::json j;
    j["name"] = client_config.client_name();
    j["ip"] = client_config.client_ip();
    j["port"] = client_config.client_port();
    auto res = req.post(j.dump());
    logger.info("Server responded: " + res);
  }
  cleanup();
  return 0;
}