#include "app.h"
#include "millis.h"

typedef unsigned long int time_ms_t;

struct soft_clk_t {
  time_ms_t _t = 0, m_period = 0;

  soft_clk_t() : _t(millis()), m_period(1000) {}

  soft_clk_t(time_ms_t T) : _t(millis()), m_period(T) {}

  time_ms_t period() { return m_period; }

  void init(time_ms_t T) {
    m_period = T;
    _t = 0;
  }

  void reset(time_ms_t T) { _t = millis() + T; }

  void reset() { _t = millis() + m_period; }

  bool tick(time_ms_t t) {
    if (t < _t)
      return false;
    _t = t + m_period;
    return true;
  }

  bool tick() { return tick(millis()); }

  void halt() { _t = 0; }
  bool isHalted() { return _t == 0; }

  time_ms_t getTickDelay(time_ms_t t = 0) {
    if (t == 0)
      t = millis();
    return t > _t ? 0 : _t - t;
  }
};
