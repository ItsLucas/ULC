#pragma once
#include "ulc_class.h"
#include <memory>
#include <unordered_map>
#include <string>
#include <map>

namespace ULC {
class Global_THD_Manager {
public:
  Global_THD_Manager &instance() {
    static Global_THD_Manager instance;
    return instance;
  }

  void create(std::string &name) {
    m_thd_map[name] = std::make_unique<ULC::THD>(name);
  }

  auto& by_name(std::string &name) { return m_thd_map[name]; }
  
private:
    Global_THD_Manager();
    ~Global_THD_Manager();
    Global_THD_Manager(const Global_THD_Manager &) = delete;
    Global_THD_Manager &operator=(const Global_THD_Manager &) = delete;
    Global_THD_Manager(Global_THD_Manager &&) = delete;
    Global_THD_Manager &operator=(Global_THD_Manager &&) = delete;
    std::unordered_map<std::string, std::unique_ptr<ULC::THD>> m_thd_map;
};
}