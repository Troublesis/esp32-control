#pragma once
#include <Arduino.h>
#include <Preferences.h>

// ============================================================================
// Bark push notifications — one generic client, four independent toggles
// ============================================================================
// Any source (relay 1, relay 2, the PIR motion sensor, the laser-beam receiver)
// can request a push. Each source has its own on/off switch, persisted in flash
// and flipped from the WebUI (/api/bark). "Available" means the client was
// compiled in (BARK_ENABLED); only then can anything be sent or toggled.
//
// All functions are safe to call when BARK_ENABLED is 0 — they become no-ops /
// report "unavailable", so callers need no #if guards.
// ============================================================================

// Source identifiers — also the index into the persisted toggle array.
enum BarkSource {
  BARK_SRC_RELAY1 = 0,
  BARK_SRC_RELAY2 = 1,
  BARK_SRC_MOTION = 2,
  BARK_SRC_LASER  = 3,   // laser-beam receiver (beam broken)
  BARK_SRC_COUNT  = 4,
};

// True when the Bark client is compiled in (BARK_ENABLED).
bool barkAvailable();

// Live on/off for a source (false when unavailable or out of range).
bool barkEnabled(int source);

// Flip a source toggle and persist it. No-op when unavailable / out of range.
void barkSetEnabled(Preferences& prefs, int source, bool on);

// Restore the persisted toggles, seeded from the BARK_*_DEFAULT build values.
void barkBegin(Preferences& prefs);

// Send a push for `source` — only if available AND that source is enabled. The
// module appends device name, timestamp and the live LAN URL to `body`.
void barkSend(int source, const String& title, const String& body);

// {"available":bool,"relay1":bool,"relay2":bool,"motion":bool,"laser":bool}
String barkStatusJson();
