#include <nlohmann/json.hpp>

#include <fstream>

using namespace std;

int main() {
  ofstream f("config.json");
  nlohmann::json j;

  j["server"]["server_name"] = "Test server";
  j["server"]["server_ip"] = "127.0.0.1";
  j["server"]["server_port"] = 1234;
  j["server"]["max_clients"] = 10;
  j["server"]["plugins"] = {"plugin1", "plugin2", "plugin3"};
  j["debug"]["enabled"] = true;
  j["debug"]["level"] = "info";
  f << j.dump(2);
  return 0;
  
}