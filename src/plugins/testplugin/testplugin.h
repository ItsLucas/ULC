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
  bool load();
  bool unload();
  std::unique_ptr<ULC::cmd> by_name(const std::string &command);
};