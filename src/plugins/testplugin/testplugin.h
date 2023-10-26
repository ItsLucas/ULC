#include "plugin_base.h"

class testplugin : public ULC::plugin_base {
public:
  testplugin();
  ~testplugin();
  bool load();
  bool unload();
  bool do_command(const std::string &command);
};