#include "ulc_thd_manager.h"

namespace ULC{
THD_ptr::THD_ptr(THD *thd) : m_underlying(thd) {
  if (m_underlying != nullptr) {
    m_underlying->LOCK_thd_data.lock();
  }
}

THD_ptr::THD_ptr(THD_ptr &&thd_ptr) {
  if (m_underlying != nullptr)
    m_underlying->LOCK_thd_data.unlock();
  m_underlying = thd_ptr.m_underlying;
  thd_ptr.m_underlying = nullptr;
}

THD_ptr &THD_ptr::operator=(THD_ptr &&thd_ptr) {
  if (m_underlying != nullptr)
    m_underlying->LOCK_thd_data.unlock();
  m_underlying = thd_ptr.m_underlying;
  thd_ptr.m_underlying = nullptr;
  return *this;
}

THD *THD_ptr::release() {
  if (m_underlying == nullptr)
    return nullptr;

  THD *tmp = m_underlying;
  m_underlying->LOCK_thd_data.unlock();
  m_underlying = nullptr;
  return tmp;
}
}