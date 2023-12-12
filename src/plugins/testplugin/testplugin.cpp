#include "testplugin.h"
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
  ULC::retval ret;
  nlohmann::json j;
  ULC::Logger &logger = ULC::Logger::getInstance();
  if (m_args.size() != 1) {
    j["status"] = "ERROR";
    j["message"] = "testcmd error";
    j["retrieved_args"] = "null";
    logger.error("testcmd requires 1 argument", "testplugin.so");
    return ret.status(false).message("testcmd requires 1 argument").data(j);
  }
  std::string arg1 = std::any_cast<std::string>(m_args[0]);
  j["status"] = "OK";
  j["message"] = "testcmd executed";
  j["retrieved_args"] = arg1;
  logger.info("testcmd executed", "testplugin.so");
  return ret.status(true).message("testcmd executed").data(j);
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
  CROW_ROUTE(app, "/test")([]() { return "Hello testplugin"; });
  return true;
}