#pragma once
#include <Arduino.h>

// ============================================================================
// PIR motion sensor (e.g. HS-S38P) — a thin, named wrapper over the generic
// Sensor (sensor.h). Detections land in the unified event log (eventlog.h) and
// can fire a Bark push (bark.h) when the BARK_SRC_MOTION toggle is on.
//
// All functions are safe to call when PIR_ENABLED is 0 (they become no-ops /
// report "disabled"), so the rest of the firmware needs no #if guards.
// ============================================================================

// Configure the input pin and seed the initial state (no event logged at boot).
void motionBegin();

// Sample the pin and append an event on a debounced state change. Call every
// loop(); it rate-limits itself internally.
void motionUpdate();

// True when the sensor is compiled in (PIR_ENABLED).
bool motionEnabled();

// Current logical state — true while motion is present.
bool motionActive();

// User-tunable detection delay (ms): minimum gap between logged detections.
// Default is PIR_DEFAULT_DELAY_MS; main.cpp restores the persisted value.
void          motionSetDelay(unsigned long ms);
unsigned long motionDelay();

// Motion object for /api/status:
//   {"enabled":..,"active":..,"count":..,"lastSeq":..,"delay":..}
String motionStatusJson();
