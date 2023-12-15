#include "testplugin.h"
#include "cmd.h"
#include <memory>

extern "C" std::shared_ptr<ULC::plugin_base> init() {
  return std::make_shared<testplugin>();
}

extern "C" void deinit() { return; }

testcmd::testcmd() {
  ULC::Logger &logger = ULC::Logger::getInstance();
  logger.info("testcmd constructed", "testplugin.so");
}

testcmd::~testcmd() {
  ULC::Logger &logger = ULC::Logger::getInstance();
  logger.info("testcmd destructed", "testplugin.so");
}

ULC::retval testcmd::execute() {
  ULC::Logger &logger = ULC::Logger::getInstance();
  if (m_args.size() != 1) {
    logger.error("testcmd requires 1 argument", "testplugin.so");
    return ULC::retval(false,"testcmd requires 1 argument", "null");
  }
  std::string arg1 = std::any_cast<std::string>(m_args[0]);
  logger.info("testcmd executed", "testplugin.so");
  return ULC::retval(true,"testcmd executed", arg1);
}

testplugin::testplugin() {
  ULC::Logger &logger = ULC::Logger::getInstance();
  logger.info("testplugin constructed", "testplugin.so");
}

testplugin::~testplugin() {
  ULC::Logger &logger = ULC::Logger::getInstance();
  logger.info("testplugin destructed", "testplugin.so");
}

bool testplugin::load() {
  ULC::Logger &logger = ULC::Logger::getInstance();
  logger.info("testplugin loaded", "testplugin.so");
  m_name = "testplugin";
  m_description = "testplugin description";
  m_version = "0.0.1";
  m_commands.push_back("test");
  m_commands.push_back("test2");
  logger.info("commands: ", "testplugin.so");
  for (auto cmd : m_commands) {
    logger.info(cmd, "testplugin.so");
  }
  return true;
}

bool testplugin::unload() {
  ULC::Logger &logger = ULC::Logger::getInstance();
  logger.info("testplugin unloaded", "testplugin.so");
  return true;
}

std::unique_ptr<ULC::cmd> testplugin::by_name(const std::string &command) {
  if (command == "test") {
    return std::make_unique<testcmd>();
  }
  return nullptr;
}

bool testplugin::setup_routes_for_crow(crow::SimpleApp &app) {
  CROW_ROUTE(app, "/testplugin/test")
      .methods("POST"_method)([](const crow::request &req) {
        auto x = crow::json::load(req.body);
        if (!x)
          return crow::response(400);
        ULC::remote_arg arg1(req.body);
        auto cmd = std::make_unique<testcmd>();
        for(auto arg : arg1.args()) {
          cmd->add_arg(arg);
        }
        auto ret = cmd->execute();
        return crow::response(200, ret.to_string());
      });
  return true;
}