#pragma once
#include <Arduino.h>

// ============================================================================
// Unified "General Info" event log
// ============================================================================
// A single bounded, in-RAM ring buffer that every subsystem writes into —
// relays, the laser emitter, the PIR motion sensor and the laser-beam receiver.
// Newest wins; the oldest entry is dropped once EVENT_LOG_MAX is reached. Each
// event is stamped with the NTP epoch when available and always with uptime.
//
// The WebUI fetches it incrementally (only events newer than the highest seq it
// already holds) and renders them newest-first as the "General Info" panel.
// ============================================================================

// Event sources — the WebUI maps each to a colored tag. Keep in sync with the
// labels in include/web_ui.h.
enum LogSource {
  LOG_SYSTEM = 0,  // boot / lifecycle notices
  LOG_RELAY1 = 1,
  LOG_RELAY2 = 2,
  LOG_LASER  = 3,  // laser emitter (output) state changes
  LOG_MOTION = 4,  // PIR motion sensor
  LOG_BEAM   = 5,  // laser-beam receiver (beam broken / restored)
};

// Append an event and return its new sequence number. `alert` marks the
// "attention" state (drives the highlight color in the UI — e.g. motion
// detected, beam broken, relay ON). `msg` is a short line, truncated to fit.
unsigned long logEvent(int source, bool alert, const String& msg);

// Events with seq > sinceSeq, oldest first:
//   {"latest":N,"events":[{"seq":..,"ts":"YYYY-MM-DD HH:MM:SS","up":ms,
//                          "src":n,"a":1,"msg":".."}]}
// "ts" is empty when the event predates an NTP sync.
String logJson(unsigned long sinceSeq);

// Forget all events and reset the sequence counter.
void logClear();

// Sequence number of the most recent event (0 if none recorded yet).
unsigned long logLatestSeq();
