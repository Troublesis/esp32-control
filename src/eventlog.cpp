#include "eventlog.h"
#include "config.h"
#include "timeutil.h"

#include <string.h>

// ============================================================================
// Unified event-log implementation — see include/eventlog.h for the contract.
// ============================================================================

// One recorded event. `msg` is a fixed char buffer (not String) so the whole
// ring buffer is a flat, predictable block of RAM.
struct LogEntry {
  unsigned long seq;     // monotonically increasing id (1-based)
  time_t        epoch;   // wall-clock at the event, 0 if NTP not yet synced
  unsigned long up;      // millis() at the event (always available)
  uint8_t       source;  // LogSource
  bool          alert;   // attention state (UI highlight)
  char          msg[28]; // short human-readable line
};

static LogEntry logBuf[EVENT_LOG_MAX];
static int           logHead = 0;   // next write slot
static int           logCount = 0;  // saturates at EVENT_LOG_MAX
static unsigned long seqCounter = 0;

static String jsonEscape(const char* value) {
  String out;
  out.reserve(strlen(value) + 8);
  for (const char* p = value; *p; p++) {
    switch (*p) {
      case '"':  out += "\\\""; break;
      case '\\': out += "\\\\"; break;
      case '\n': out += "\\n"; break;
      case '\r': out += "\\r"; break;
      case '\t': out += "\\t"; break;
      default:   out += *p; break;
    }
  }
  return out;
}

unsigned long logEvent(int source, bool alert, const String& msg) {
  LogEntry ev;
  ev.seq    = ++seqCounter;
  ev.epoch  = syncedEpoch();
  ev.up     = millis();
  ev.source = (uint8_t)source;
  ev.alert  = alert;
  strncpy(ev.msg, msg.c_str(), sizeof(ev.msg) - 1);
  ev.msg[sizeof(ev.msg) - 1] = '\0';

  logBuf[logHead] = ev;
  logHead = (logHead + 1) % EVENT_LOG_MAX;
  if (logCount < EVENT_LOG_MAX) logCount++;
  return ev.seq;
}

String logJson(unsigned long sinceSeq) {
  String s = "{\"latest\":" + String(seqCounter) + ",\"events\":[";
  int start = (logHead - logCount + EVENT_LOG_MAX) % EVENT_LOG_MAX; // oldest entry
  bool first = true;
  for (int i = 0; i < logCount; i++) {
    const LogEntry& ev = logBuf[(start + i) % EVENT_LOG_MAX];
    if (ev.seq <= sinceSeq) continue;
    if (!first) s += ",";
    first = false;
    s += "{\"seq\":" + String(ev.seq);
    s += ",\"ts\":\"" + formatEpoch(ev.epoch) + "\"";
    s += ",\"up\":" + String(ev.up);
    s += ",\"src\":" + String(ev.source);
    s += ",\"a\":" + String(ev.alert ? 1 : 0);
    s += ",\"msg\":\"" + jsonEscape(ev.msg) + "\"";
    s += "}";
  }
  s += "]}";
  return s;
}

void logClear() {
  logHead = 0;
  logCount = 0;
  seqCounter = 0;
  Serial.println("[log] cleared");
}

unsigned long logLatestSeq() { return seqCounter; }
