#pragma once
#include <Arduino.h>
#include <time.h>

// ============================================================================
// Small NTP-time helpers shared by the event log and the Bark notifier.
// ============================================================================

// Wall-clock epoch, but only once NTP has actually set the clock. A value past
// 2020-09-13 (1600000000) means configTime() has succeeded; before that, time()
// returns a small boot-relative number we don't want to treat as a real date.
inline time_t syncedEpoch() {
  time_t now = time(nullptr);
  return now > 1600000000 ? now : 0;
}

// Format an epoch as "YYYY-MM-DD HH:MM:SS" in the configured local timezone, or
// "" when the timestamp is unknown (epoch == 0).
inline String formatEpoch(time_t epoch) {
  if (epoch == 0) return String();
  struct tm tmv;
  localtime_r(&epoch, &tmv);
  char buf[20];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tmv);
  return String(buf);
}
