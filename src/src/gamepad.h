// =======================================================================================================
// GAMEPAD (Bluepad32) — PS4 / PS5 / Xbox controller support
// =======================================================================================================
//
// Bluepad32 reads the controller and this fills the same pulseWidthRaw[] channel array the RC receiver
// normally would (1000..2000us, 1500 = neutral), so all the existing drive / sound / light logic just works.
//
// The "survonauts" shift-gate:
//   - Starts in NEUTRAL (no drive).
//   - Flick the left stick DOWN+RIGHT -> engage FORWARD.  DOWN+LEFT -> engage REVERSE.
//   - Then push the left stick UP for throttle (in the engaged direction).
//   - Stop throttling for ~1s -> drops back to NEUTRAL (must re-flick to re-engage).
//
// Only compiled when GAMEPAD_MODE is defined (2_Remote.h). Needs the esp32-bluepad32 board core.

#if defined GAMEPAD_MODE

#include <Bluepad32.h>

// The configurator's Gamepad tab writes gamepad_config.h with the user's button
// map, drive options and axis choices. It is #included here (before the #ifndef
// defaults below) so those #defines win. If it's absent, the defaults apply.
#if defined __has_include
#  if __has_include("gamepad_config.h")
#    include "gamepad_config.h"
#  endif
#endif

ControllerPtr gpController = nullptr;
volatile bool gamepadConnected = false;

// ---- Mapping defaults (the flasher overrides these via gamepad_config.h) ------------------------------
// Analog input range from Bluepad32: sticks -512..511, triggers 0..1023.
#ifndef GP_SHIFTGATE
#define GP_SHIFTGATE 1 // 1 = survonauts shift-gate on left stick; 0 = simple (left stick Y = throttle)
#endif
#ifndef GP_STEER_DEADZONE
#define GP_STEER_DEADZONE 60
#endif
#ifndef GP_THROTTLE_DEADZONE
#define GP_THROTTLE_DEADZONE 80
#endif
#ifndef GP_AUTO_NEUTRAL_MS
#define GP_AUTO_NEUTRAL_MS 1000 // idle time before dropping back to NEUTRAL
#endif
#ifndef GP_STEER_SOURCE
#define GP_STEER_SOURCE 1 // 0 = left stick X, 1 = right stick X
#endif
#ifndef GP_STEER_INVERT
#define GP_STEER_INVERT 0
#endif
#ifndef GP_THROTTLE_INVERT
#define GP_THROTTLE_INVERT 0
#endif

// Digital-function button masks (Bluepad32 buttons() bitfield). Defaults; overridable by the flasher.
#ifndef GP_BTN_HORN
#define GP_BTN_HORN 0x0002 // Circle / B
#endif
#ifndef GP_BTN_ENGINE
#define GP_BTN_ENGINE 0x0008 // Triangle / Y
#endif
#ifndef GP_BTN_LIGHTS
#define GP_BTN_LIGHTS 0x0001 // Cross / A
#endif
#ifndef GP_BTN_JAKE
#define GP_BTN_JAKE 0x0004 // Square / X
#endif

enum GpGear { GP_NEUTRAL = 0, GP_FORWARD = 1, GP_REVERSE = -1 };
static int8_t gpGear = GP_NEUTRAL;

static void gpOnConnect(ControllerPtr ctl)
{
  gpController = ctl;
  gamepadConnected = true;
  Serial.printf("Gamepad connected: %s\n", ctl->getModelName().c_str());
}
static void gpOnDisconnect(ControllerPtr ctl)
{
  if (ctl == gpController)
  {
    gpController = nullptr;
    gamepadConnected = false;
    gpGear = GP_NEUTRAL;
    Serial.println("Gamepad disconnected");
  }
}

void setupGamepad()
{
  Serial.printf("GAMEPAD_MODE — Bluepad32 %s, BT MAC ", BP32.firmwareVersion());
  const uint8_t *a = BP32.localBdAddress();
  Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n", a[0], a[1], a[2], a[3], a[4], a[5]);
  BP32.setup(&gpOnConnect, &gpOnDisconnect);
  BP32.enableVirtualDevice(false);
  Serial.println("Put your controller in pairing mode to connect...");
}

// map a signed analog value (-range..range) to a 1000..2000us pulse (1500 center), with deadzone
static uint16_t gpAxisToPulse(int val, int range, int deadzone)
{
  if (val > -deadzone && val < deadzone)
    return 1500;
  long p = 1500 + (long)val * 500 / range;
  if (p < 1000)
    p = 1000;
  if (p > 2000)
    p = 2000;
  return (uint16_t)p;
}

void readGamepadCommands()
{
  BP32.update();

  if (!gpController || !gpController->isConnected() || !gpController->isGamepad())
  {
    // No pad -> hold everything at neutral (failsafe)
    for (uint8_t i = 1; i <= 13; i++)
      pulseWidthRaw[i] = 1500;
    gpGear = GP_NEUTRAL;
    return;
  }

  ControllerPtr c = gpController;
  int lx = c->axisX();   // left stick  X  (-512..511)
  int ly = c->axisY();   // left stick  Y  (-512..511, up is negative)
  int rx = c->axisRX();  // right stick X
  uint16_t btn = c->buttons();

  // --- Steering: chosen stick X -> CH1 ---
  int steerRaw = GP_STEER_SOURCE ? rx : lx;
#if GP_STEER_INVERT
  steerRaw = -steerRaw;
#endif
  pulseWidthRaw[1] = gpAxisToPulse(steerRaw, 512, GP_STEER_DEADZONE);

  // --- Throttle / gear: left stick ---
  uint16_t throttlePulse = 1500;
#if GP_SHIFTGATE
  static uint32_t lastThrottleMs = 0;
  bool down = (ly > 300);
  bool up = (ly < -GP_THROTTLE_DEADZONE);
  bool right = (lx > 300);
  bool left = (lx < -300);

  // Engage gear with a DOWN+RIGHT (forward) or DOWN+LEFT (reverse) flick
  if (down && right)
    gpGear = GP_FORWARD;
  else if (down && left)
    gpGear = GP_REVERSE;

  // Throttle = how far UP the stick is, applied in the engaged direction
  int upAmount = up ? (-ly) : 0; // 0..512
  if (gpGear == GP_FORWARD)
    throttlePulse = 1500 + (uint16_t)((long)upAmount * 500 / 512);
  else if (gpGear == GP_REVERSE)
    throttlePulse = 1500 - (uint16_t)((long)upAmount * 500 / 512);
  else
    throttlePulse = 1500; // neutral

  // Auto-return to NEUTRAL after no throttle for a while
  if (upAmount > GP_THROTTLE_DEADZONE)
    lastThrottleMs = millis();
  if (gpGear != GP_NEUTRAL && millis() - lastThrottleMs > GP_AUTO_NEUTRAL_MS)
    gpGear = GP_NEUTRAL;
#else
  // Simple mode: left stick Y directly = throttle (up = forward, down = reverse)
  throttlePulse = gpAxisToPulse(-ly, 512, GP_THROTTLE_DEADZONE);
#endif
#if GP_THROTTLE_INVERT
  throttlePulse = 3000 - throttlePulse; // mirror around 1500 (swap fwd/rev)
#endif
  pulseWidthRaw[3] = throttlePulse;

  // --- Digital functions -> the channels the profile reads them on ---
  // CH4 = HORN, CH5 = FUNCTION_R (engine/jake/lights), CH6 = FUNCTION_L (indicators)
  pulseWidthRaw[4] = (btn & GP_BTN_HORN) ? 2000 : 1000;                 // horn
  pulseWidthRaw[5] = (btn & GP_BTN_ENGINE) ? 2000 : ((btn & GP_BTN_JAKE) ? 1000 : 1500); // engine / jake
  pulseWidthRaw[6] = (btn & GP_BTN_LIGHTS) ? 2000 : 1500;               // lights

  // Fill any remaining channels with neutral so nothing floats
  pulseWidthRaw[2] = 1500;
  for (uint8_t i = 7; i <= 13; i++)
    pulseWidthRaw[i] = 1500;
}

#endif // GAMEPAD_MODE
