#include "ulc_thd_manager.h"

std::string generate_uuid() {
  std::random_device rd;
  std::mt19937 rng(rd());

  std::stringstream ss;
  ss << std::hex;

  for (int i = 0; i < 4; ++i) {
    ss << ((rng() & 0xffff0000) >> 16);
    if (i != 3)
      ss << "-";
  }

  return ss.str();
}
namespace ULC {
Global_THD_Manager::Global_THD_Manager() {
  Logger::getInstance().info("Creating Global_THD_Manager", "thd");
}
Global_THD_Manager::~Global_THD_Manager() {
  Logger::getInstance().info("Destroying Global_THD_Manager", "thd");
}

} // namespace ULC