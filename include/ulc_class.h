#pragma once
#include <string>
#include "log.h"

namespace ULC {
/* This is what you think it should be. */
class THD {
  public:
    THD(std::string &name): m_name(name) {
      Logger::getInstance().info("Creating THD " + m_name, "thd");
    }
  private:
    std::string m_name;
};
}