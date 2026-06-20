#include "receiver.h"
#include "config.h"
#include "sensor.h"
#include "bark.h"
#include "eventlog.h"

// ============================================================================
// Laser-beam receiver — see include/receiver.h. Pure configuration; the
// polling / debounce / logging / notification logic lives in sensor.cpp.
// ============================================================================
//
// The beam is RECEIVED at level RECEIVER_BEAM_HIGH, so the "alert" (beam broken)
// state is the opposite level. The pull resistor is chosen so a DISCONNECTED
// receiver reads as "beam broken" (fail-safe for a tripwire):
//   beam HIGH  -> alert LOW  -> INPUT_PULLDOWN (idle reads LOW  = broken)
//   beam LOW   -> alert HIGH -> INPUT_PULLUP   (idle reads HIGH = broken)

static Sensor beam = {
  /* enabled       */ RECEIVER_ENABLED,
  /* pin           */ RECEIVER_PIN,
  /* inputMode     */ RECEIVER_BEAM_HIGH ? INPUT_PULLDOWN : INPUT_PULLUP,
  /* alertHigh     */ RECEIVER_BEAM_HIGH ? false : true,
  /* pollMs        */ RECEIVER_POLL_MS,
  /* debounceMs    */ 30,
  /* logSource     */ LOG_BEAM,
  /* barkSource    */ BARK_SRC_LASER,
  /* tag           */ "beam",
  /* alertMsg      */ "Beam broken",
  /* clearMsg      */ "Beam restored",
  /* barkTitle     */ BARK_BEAM_TITLE,
  /* barkBody      */ BARK_BEAM_BODY,
  /* detectDelayMs */ RECEIVER_DEFAULT_DELAY_MS,
  /* runtime ...   */ false, false, 0, 0, 0, 0, 0,
};

void          receiverBegin()                 { sensorBegin(beam); }
void          receiverUpdate()                { sensorUpdate(beam); }
bool          receiverEnabled()               { return beam.enabled; }
bool          receiverActive()                { return sensorActive(beam); }
void          receiverSetDelay(unsigned long ms) { sensorSetDelay(beam, ms); }
unsigned long receiverDelay()                 { return sensorDelay(beam); }
String        receiverStatusJson()            { return sensorStatusJson(beam); }
