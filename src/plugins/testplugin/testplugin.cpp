#include "testplugin.h"
#include <memory>

extern "C" std::shared_ptr<ULC::plugin_base> init() {
    return std::make_shared<testplugin>();
}

extern "C" void deinit() { return; }

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

bool testplugin::do_command(const std::string &command) {
    ULC::Logger &logger = ULC::Logger::getInstance();
    logger.info("testplugin do_command: " + command, "testplugin.so");
    return true;
}