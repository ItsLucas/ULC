#pragma once
#include "config.h"
#include "log.h"
#include <list>

namespace ULC {

class plugin_base {
  /*
Common abstract stuff for plugins.
A plugin should inherit from this class.
Contains:
Plugin name, version, description.
A plugin should be able to handle commands.
On load, a plugin will register its commands to the server.
The server will dispatch command to plugin by calling
plugin->do_command(command).

Plugin will be loaded in .so form.
*/
public:
  plugin_base() {}
  virtual ~plugin_base() {}
  virtual bool load() = 0;
  virtual bool unload() = 0;
  virtual bool do_command(const std::string &command) = 0;
  std::string name() { return m_name; }
  std::string version() { return m_version; }
  std::string description() { return m_description; }
  std::list<std::string> commands() { return m_commands; }

protected:
  std::string m_name;
  std::string m_version;
  std::string m_description;
  std::list<std::string> m_commands;
};

typedef std::shared_ptr<plugin_base> (*init_func)();
typedef void (*deinit_func)();
} // namespace ULC