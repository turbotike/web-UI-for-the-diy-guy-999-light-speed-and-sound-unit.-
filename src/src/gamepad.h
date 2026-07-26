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
#ifndef GP_STEER_DEADZONE
#define GP_STEER_DEADZONE 60
#endif
#ifndef GP_THROTTLE_DEADZONE
#define GP_THROTTLE_DEADZONE 80
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
#ifndef GP_TANKMIX
#define GP_TANKMIX 0 // 1 = tank/skid mix: throttle +/- steering -> left track (CH1) & right track (CH2)
#endif

// ---- Per-output mapping (gamepad mode) ---------------------------------------------------------------
// CH2, CH3, CH4 and the AUX pin (GPIO32) can each be assigned to ANY control: a stick axis, a trigger,
// or a button (momentary / toggle), each with its own min/center/max endpoints. Steering (CH1) and
// throttle (ESC) keep their dedicated roles. The flasher writes these via gamepad_config.h. Source ids:
#define GP_SRC_NONE 0
#define GP_SRC_LX 1      // left stick X
#define GP_SRC_LY 2      // left stick Y (up = +)
#define GP_SRC_RX 3      // right stick X
#define GP_SRC_RY 4      // right stick Y (up = +)
#define GP_SRC_L2 5      // left trigger (0..max)
#define GP_SRC_R2 6      // right trigger (0..max)
#define GP_SRC_TRIG 7    // R2 - L2, centered
#define GP_SRC_BTN_MOM 8 // button: on while held
#define GP_SRC_BTN_TOG 9 // button: press to toggle

// Defaults (overridden by gamepad_config.h). SRC 0 = unassigned -> that channel keeps stock behavior.
#ifndef GP_CH2_SRC
#define GP_CH2_SRC 0
#endif
#ifndef GP_CH2_BTN
#define GP_CH2_BTN 0x0000
#endif
#ifndef GP_CH2_MIN
#define GP_CH2_MIN 1000
#endif
#ifndef GP_CH2_CENTER
#define GP_CH2_CENTER 1500
#endif
#ifndef GP_CH2_MAX
#define GP_CH2_MAX 2000
#endif
#ifndef GP_CH3_SRC
#define GP_CH3_SRC 0
#endif
#ifndef GP_CH3_BTN
#define GP_CH3_BTN 0x0000
#endif
#ifndef GP_CH3_MIN
#define GP_CH3_MIN 1000
#endif
#ifndef GP_CH3_CENTER
#define GP_CH3_CENTER 1500
#endif
#ifndef GP_CH3_MAX
#define GP_CH3_MAX 2000
#endif
#ifndef GP_CH4_SRC
#define GP_CH4_SRC 0
#endif
#ifndef GP_CH4_BTN
#define GP_CH4_BTN 0x0000
#endif
#ifndef GP_CH4_MIN
#define GP_CH4_MIN 1000
#endif
#ifndef GP_CH4_CENTER
#define GP_CH4_CENTER 1500
#endif
#ifndef GP_CH4_MAX
#define GP_CH4_MAX 2000
#endif
#ifndef GP_AUX_SRC
#define GP_AUX_SRC 0 // AUX pin = GPIO32
#endif
#ifndef GP_AUX_BTN
#define GP_AUX_BTN 0x0000
#endif
#ifndef GP_AUX_MIN
#define GP_AUX_MIN 1000
#endif
#ifndef GP_AUX_CENTER
#define GP_AUX_CENTER 1500
#endif
#ifndef GP_AUX_MAX
#define GP_AUX_MAX 2000
#endif

volatile uint16_t gpOutMicros[5] = {1500, 1500, 1500, 1500, 1500}; // [1..4] gamepad servo out for CH1..CH4
volatile uint16_t gpAuxServoMicros = 1500;                         // AUX (GPIO32) servo out

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

// map a centered analog value (-512..511) to endpoints (min..center..max) with a deadzone
static uint16_t gpMapCentered(int v, uint16_t mn, uint16_t ct, uint16_t mx, int dz)
{
  if (v > -dz && v < dz)
    return ct;
  long p = (v >= 0) ? (long)ct + (long)v * ((int)mx - (int)ct) / 512
                    : (long)ct + (long)v * ((int)ct - (int)mn) / 512;
  long lo = min(mn, mx), hi = max(mn, mx);
  return (uint16_t)constrain(p, lo, hi);
}

// map a unipolar value (0..1023, e.g. a trigger) to min..max
static uint16_t gpMapUni(int v, uint16_t mn, uint16_t mx)
{
  long p = (long)mn + (long)v * ((int)mx - (int)mn) / 1023;
  long lo = min(mn, mx), hi = max(mn, mx);
  return (uint16_t)constrain(p, lo, hi);
}

// resolve one mapped output to servo microseconds. idx (0..3) keys the per-output toggle state.
static uint16_t gpResolve(ControllerPtr c, uint8_t idx, uint8_t src, uint16_t btnMask,
                          uint16_t mn, uint16_t ct, uint16_t mx)
{
  static bool tog[6] = {false, false, false, false, false, false};
  static bool prev[6] = {false, false, false, false, false, false};
  uint16_t btn = c->buttons();
  switch (src)
  {
  case GP_SRC_LX:
    return gpMapCentered(c->axisX(), mn, ct, mx, 40);
  case GP_SRC_LY:
    return gpMapCentered(-c->axisY(), mn, ct, mx, 40);
  case GP_SRC_RX:
    return gpMapCentered(c->axisRX(), mn, ct, mx, 40);
  case GP_SRC_RY:
    return gpMapCentered(-c->axisRY(), mn, ct, mx, 40);
  case GP_SRC_L2:
    return gpMapUni(c->brake(), mn, mx);
  case GP_SRC_R2:
    return gpMapUni(c->throttle(), mn, mx);
  case GP_SRC_TRIG:
    return gpMapCentered((c->throttle() - c->brake()) * 512 / 1023, mn, ct, mx, 20);
  case GP_SRC_BTN_MOM:
    return (btn & btnMask) ? mx : mn;
  case GP_SRC_BTN_TOG:
  {
    bool pressed = (btn & btnMask) != 0;
    if (pressed && !prev[idx])
      tog[idx] = !tog[idx];
    prev[idx] = pressed;
    return tog[idx] ? mx : mn;
  }
  default:
    return ct;
  }
}

void readGamepadCommands()
{
  BP32.update();

  if (!gpController || !gpController->isConnected() || !gpController->isGamepad())
  {
    // No pad -> hold everything at neutral (failsafe)
    for (uint8_t i = 1; i <= 13; i++)
      pulseWidthRaw[i] = 1500;
    gpOutMicros[2] = GP_CH2_CENTER;
    gpOutMicros[3] = GP_CH3_CENTER;
    gpOutMicros[4] = GP_CH4_CENTER;
    gpAuxServoMicros = GP_AUX_CENTER;
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

  // --- Throttle: left stick Y directly (up = forward, down = reverse) ---
  uint16_t throttlePulse = gpAxisToPulse(-ly, 512, GP_THROTTLE_DEADZONE);
#if GP_THROTTLE_INVERT
  throttlePulse = 3000 - throttlePulse; // mirror around 1500 (swap fwd/rev)
#endif
  pulseWidthRaw[3] = throttlePulse;

#if GP_TANKMIX
  // --- Tank / skid-steer mix ---------------------------------------------------------------
  // Blend throttle and steering into two track signals. Left stick Y = throttle (direct, so
  // you can pivot on the spot), the steering stick = turn. Output: left track on CH1, right
  // track on CH2 — plug an ESC into each. CH1L/C/R and CH2L/C/R (Servos tab) still trim them.
  int thrDev = -ly; // up = forward (-512..511)
  if (thrDev > -GP_THROTTLE_DEADZONE && thrDev < GP_THROTTLE_DEADZONE)
    thrDev = 0;
  int strDev = GP_STEER_SOURCE ? rx : lx;
  if (strDev > -GP_STEER_DEADZONE && strDev < GP_STEER_DEADZONE)
    strDev = 0;
#if GP_THROTTLE_INVERT
  thrDev = -thrDev;
#endif
#if GP_STEER_INVERT
  strDev = -strDev;
#endif
  long trkL = constrain((long)thrDev + strDev, -512, 512);
  long trkR = constrain((long)thrDev - strDev, -512, 512);
  pulseWidthRaw[1] = 1500 + (int)(trkL * 500 / 512);          // left track  -> CH1
  pulseWidthRaw[2] = 1500 + (int)(trkR * 500 / 512);          // right track -> CH2
  pulseWidthRaw[3] = 1500 + (int)((long)abs(thrDev) * 500 / 512); // engine sound follows throttle
#endif

  // --- Digital functions -> the channels the profile reads them on ---
  // CH4 = HORN, CH5 = FUNCTION_R (engine/jake/lights), CH6 = FUNCTION_L (indicators)
  pulseWidthRaw[4] = (btn & GP_BTN_HORN) ? 2000 : 1000;                 // horn
  pulseWidthRaw[5] = (btn & GP_BTN_ENGINE) ? 2000 : ((btn & GP_BTN_JAKE) ? 1000 : 1500); // engine / jake
  pulseWidthRaw[6] = (btn & GP_BTN_LIGHTS) ? 2000 : 1500;               // lights

  // --- Per-output mapping: CH2 / CH3 / CH4 / AUX (any assigned channel is driven here) ---
#if GP_CH2_SRC
  gpOutMicros[2] = gpResolve(c, 0, GP_CH2_SRC, GP_CH2_BTN, GP_CH2_MIN, GP_CH2_CENTER, GP_CH2_MAX);
#endif
#if GP_CH3_SRC
  gpOutMicros[3] = gpResolve(c, 1, GP_CH3_SRC, GP_CH3_BTN, GP_CH3_MIN, GP_CH3_CENTER, GP_CH3_MAX);
#endif
#if GP_CH4_SRC
  gpOutMicros[4] = gpResolve(c, 2, GP_CH4_SRC, GP_CH4_BTN, GP_CH4_MIN, GP_CH4_CENTER, GP_CH4_MAX);
#endif
#if GP_AUX_SRC
  gpAuxServoMicros = gpResolve(c, 3, GP_AUX_SRC, GP_AUX_BTN, GP_AUX_MIN, GP_AUX_CENTER, GP_AUX_MAX);
#endif

  // Fill any remaining channels with neutral so nothing floats
#if !GP_TANKMIX
  pulseWidthRaw[2] = 1500; // in tank mix CH2 carries the right track — don't clobber it
#endif
  for (uint8_t i = 7; i <= 13; i++)
    pulseWidthRaw[i] = 1500;
}

#endif // GAMEPAD_MODE
