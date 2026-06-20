#include "motion.h"
#include "config.h"
#include "sensor.h"
#include "bark.h"
#include "eventlog.h"

// ============================================================================
// PIR motion sensor — see include/motion.h. This is just configuration: all of
// the polling / debounce / logging / notification logic lives in sensor.cpp.
// ============================================================================
//
// alertHigh = true: the PIR signal goes HIGH while motion is present and LOW
// after the sensor's own hold time. INPUT_PULLDOWN keeps an unconnected pin LOW
// (no phantom motion); a real active-HIGH PIR overrides it when it asserts.

static Sensor pir = {
  /* enabled       */ PIR_ENABLED,
  /* pin           */ PIR_PIN,
  /* inputMode     */ INPUT_PULLDOWN,
  /* alertHigh     */ true,
  /* pollMs        */ PIR_POLL_MS,
  /* debounceMs    */ 60,
  /* logSource     */ LOG_MOTION,
  /* barkSource    */ BARK_SRC_MOTION,
  /* tag           */ "pir",
  /* alertMsg      */ "Motion detected",
  /* clearMsg      */ "No motion",
  /* barkTitle     */ BARK_MOTION_TITLE,
  /* barkBody      */ BARK_MOTION_BODY,
  /* detectDelayMs */ PIR_DEFAULT_DELAY_MS,
  /* runtime ...   */ false, false, 0, 0, 0, 0, 0,
};

void          motionBegin()                 { sensorBegin(pir); }
void          motionUpdate()                { sensorUpdate(pir); }
bool          motionEnabled()               { return pir.enabled; }
bool          motionActive()                { return sensorActive(pir); }
void          motionSetDelay(unsigned long ms) { sensorSetDelay(pir, ms); }
unsigned long motionDelay()                 { return sensorDelay(pir); }
String        motionStatusJson()            { return sensorStatusJson(pir); }
