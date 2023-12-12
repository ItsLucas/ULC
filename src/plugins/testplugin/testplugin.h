#include "plugin_base.h"

class testcmd : public ULC::cmd {
  public:
  testcmd();
  ~testcmd();
  ULC::retval execute();
};
class testplugin : public ULC::plugin_base {
public:
  testplugin();
  ~testplugin();
  bool load() override;
  bool unload() override;
  bool setup_routes_for_crow(crow::SimpleApp &app) override;
  int plugin_type() override { /* This is both cli and crow plugin */
    return ULC::PLUGIN_TYPE_CROW | ULC::PLUGIN_TYPE_CLI;
  }
  std::unique_ptr<ULC::cmd> by_name(const std::string &command) override;
};