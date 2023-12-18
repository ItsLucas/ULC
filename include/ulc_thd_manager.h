#pragma once
#include "ulc_class.h"
#include <memory>
#include <unordered_map>
#include <string>
#include <map>

namespace ULC {

class THD_ptr {
public:
  /**
    Default class constructor.
  */
  THD_ptr() = default;
  /**
    Constructor assigns THD instance to manage and acquires THD::LOCK_thd_data
    mutex.
    @param   thd   THD instance.
  */
  explicit THD_ptr(THD *thd);
  /**
    Delete copy constructor, THD_ptr copy is not allowed.
  */
  THD_ptr(THD_ptr const &) = delete;
  /**
    Move constructor.
    @param  thd_ptr    THD_ptr instance to collect underlying THD instance.
  */
  THD_ptr(THD_ptr &&thd_ptr);

  /**
    Destructor to release underlying THD instance's control and release mutex
    THD::LOCK_thd_data.
  */
  ~THD_ptr() { release(); }
  /**
    Release underlying THD instance's control and release THD::LOCK_thd_data.
    @returns  underlying THD instance.
  */
  THD *release();

  /**
    Delete copy operator, THD_ptr copy is not allowed.
  */
  THD_ptr &operator=(THD_ptr const &) = delete;
  /**
    Move semantics assignment operator.
    @param  thd_ptr    THD_ptr instance to collect underlying THD instance.
  */
  THD_ptr &operator=(THD_ptr &&thd_ptr);

  /**
    Access underlying THD instance.
    returns pointer to underlying THD instance.
  */
  THD *get() { return m_underlying; }
  /**
    Access underlying THD instance.
    returns pointer to underlying THD instance.
  */
  THD *operator->() { return m_underlying; }
  /**
    Access underlying THD instance.
    returns reference to underlying THD instance.
  */
  THD &operator*() { return *m_underlying; }

  /**
    Check if there is an underlying THD instance.
  */
  operator bool() const { return m_underlying != nullptr; }

  /**
    Compare underlying THD pointer value with the "nullptr".
    @returns true if underlying THD pointer value equals "nullptr".
  */
  bool operator==(std::nullptr_t) const { return m_underlying == nullptr; }
  /**
    Compare this instance with other THD_ptr instance.
    @param  thd_ptr    Other THD_ptr instance for comparison.
    @returns true if this instance equals other THD_ptr instance.
  */
  bool operator==(THD const *thd_ptr) const { return m_underlying == thd_ptr; }
  /**
    Compare underlying THD pointer value with the "nullptr".
    @returns true if underlying THD pointer value *not* equals nullptr.
  */
  bool operator!=(std::nullptr_t) const { return m_underlying != nullptr; }
  /**
    Compare this instance with other THD_ptr instance.
    @param  thd_ptr    Other THD_ptr instance for comparison.
    @returns true if this instance differs from other THD_ptr instance.
  */
  bool operator!=(THD const *thd_ptr) const { return m_underlying != thd_ptr; }

private:
  // Underlying THD instance to manage.
  THD *m_underlying{nullptr};
};

class Do_THD_Impl {
public:
  virtual ~Do_THD_Impl() = default;
  virtual void operator()(THD_ptr &thd) = 0;
};

class Do_THD {
public:
  explicit Do_THD(Do_THD_Impl *func) : func_(func) {}
  void
  operator()(std::pair<const std::string, std::unique_ptr<ULC::THD>> &thd) {
    auto ptr = THD_ptr(thd.second.get());
    (*func_)(ptr);
  }

private:
  Do_THD_Impl *func_;
};

class Global_THD_Manager {
public:
  static Global_THD_Manager *instance() {
    static Global_THD_Manager instance;
    return &instance;
  }

  void create(std::string &name, std::string &ip, int port) {
    m_thd_map[name] = std::make_unique<ULC::THD>(name, ip, port);
  }

  void remove(std::string &name) { m_thd_map.erase(name); }

  THD_ptr by_name(std::string &name) { return THD_ptr(m_thd_map[name].get()); }

  void do_for_all_thd(Do_THD_Impl *func) {
    Do_THD doit(func);
    std::for_each(m_thd_map.begin(), m_thd_map.end(), doit);
  }

private:
    Global_THD_Manager();
    ~Global_THD_Manager();
    Global_THD_Manager &operator=(const Global_THD_Manager &) = delete;
    Global_THD_Manager(Global_THD_Manager &&) = delete;
    Global_THD_Manager &operator=(Global_THD_Manager &&) = delete;
    std::unordered_map<std::string, std::unique_ptr<ULC::THD>> m_thd_map;
};


}