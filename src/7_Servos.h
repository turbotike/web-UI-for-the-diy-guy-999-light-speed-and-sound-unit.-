#include <Arduino.h>

/* SERVO OUTPUT SETTINGS ************************************************************************************************
 *
 * The CH1 - CH6 headers are used as outputs in BUS communication mode (SBUS, IBUS, PPM)
 * This allows to use as super compact "bus-only" receiver
 * Set the endpoints here in 1000 - 2000 microseconds (equal to -45 to 45° servo angle)
 * !! WARNING: Don't connect a receiver to the "CH1 - CH6" headers, if BUS communication is selected. Ihis will short them out!!
 *
 * Uncommenting "#define SERVO_DEBUG" in the main tab allows to calibrate the servo positions easily:
 * 1. select the "SERVOS_DEFAULT" servo configuration
 * 2. upload the sketch
 * 3. connect the servo you want to calibrate to the steeting channel CH2 on the sound controller
 * 4. turn your steering wheel until you cave the position you want
 * 5. write down the microseconds reading, which is displayed in the Arduino IDE serial monitor
 * 6. do it for every position
 * 7. make a servo configuration profile, using these values
 * 8. select this profile and upload the sketch
 * 9. that's it!
 */

// Select the vehicle configuration you have:
// #define SERVOS_DEFAULT // <------- Select (remove //) one of the remote configurations below
// #define SERVOS_LANDY_MN_MODEL
// #define SERVOS_LANDY_DOUBLE_EAGLE
// #define SERVOS_C34
// #define SERVOS_URAL
// #define SERVOS_RGT_EX86100
// #define SERVOS_ACTROS
// #define SERVOS_KING_HAULER
// #define SERVOS_RACING_TRUCK
// #define SERVOS_MECCANO_DUMPER
// #define SERVOS_OPEN_RC_TRACTOR
// NOTICE: The following profiles are for EXCAVATOR_MODE only! ---------------------
// #define SERVOS_EXCAVATOR // For excavators with electric actuators
// #define SERVOS_HYDRAULIC_EXCAVATOR // For hydraulic excavators
// #define SERVOS_CRANE // For Mushroom3D rough terrain crane (servo outputs used as outrigger channels SBUS decoder)
 #define SERVOS_PASSTHROUGH // Raw passthrough: iBUS channels output directly to CH1-CH4 pins (no ramps, no limits)

// Default servo configuration profile -------------------------------------------------------------------------------------------
#ifdef SERVOS_DEFAULT

#define CH3_BEACON              // Rotating Beacons are connected to Servo CH3. BUS mode only! https://www.ebay.ch/itm/303979210629
#define MODE2_TRAILER_UNLOCKING // The mode 2 button is used for trailer unlocking by servo CH4 (sound1 triggering will not work!)

// Servo frequency
const uint8_t SERVO_FREQUENCY = 50; // usually 50Hz, some servos may run smoother @ 100Hz

// WARNING: never connect receiver PWM signals to the "CH" pins in BUS communication mode!

// Servo limits
uint16_t CH1L = 1411, CH1C = 2142, CH1R = 2958; // CH1 steering left, center, right
uint16_t CH2L = 1411, CH2C = 2142, CH2R = 2958; // CH2 transmission gear 1, 2, 3
uint16_t CH3L = 1411, CH3C = 2142, CH3R = 2958; // CH3 Beacons (modes are switched, if position changes from 1000 to 2000us)
uint16_t CH4L = 1836, CH4R = 2448;              // CH4 trailer coupler (5th. wheel) locked, unlocked

// Servo ramp time
uint16_t STEERING_RAMP_TIME = 0; // 0 = fastest speed, enlarge it to around 3000 for "scale" servo movements

#endif

// MN Model 1:12 Land Rover Defender servo configuration profile -------------------------------------------------------------------
#ifdef SERVOS_LANDY

// Servo frequency
const uint8_t SERVO_FREQUENCY = 50; // usually 50Hz, some servos may run smoother @ 100Hz

// WARNING: never connect receiver PWM signals to the "CH" pins in BUS communication mode!

// Servo limits
uint16_t CH1L = 1411, CH1C = 2142, CH1R = 2958; // CH1 steering left 1880, center 1480, right 1080
uint16_t CH2L = 1411, CH2C = 2142, CH2R = 2958;  // CH2 transmission gear 1 978, 2 1833, 3 1833
uint16_t CH3L = 1411, CH3C = 2142, CH3R = 2958; // CH3 winch pull, off, release
uint16_t CH4L = 1836, CH4R = 2448;              // CH4 trailer coupler (5th. wheel) locked, unlocked

// Servo ramp time
uint16_t STEERING_RAMP_TIME = 0; // 0 = fastest speed, enlarge it to around 3000 for "scale" servo movements

#endif

// Double Eagle 1:8 Land Rover Defender servo configuration profile -------------------------------------------------------------------
#ifdef SERVOS_LANDY_DOUBLE_EAGLE

#define MODE2_WINCH // Mode 2 is used for winch mode, if defined. The winch is controlled by the CH4 pot and connected to Servo CH3. BUS mode only!
// #define NO_WINCH_DELAY // Use this, if you don't want a winch on / off ramp

// Servo frequency
const uint8_t SERVO_FREQUENCY = 50; // usually 50Hz, some servos may run smoother @ 100Hz

// WARNING: never connect receiver PWM signals to the "CH" pins in BUS communication mode!

// Servo limits
uint16_t CH1L = 1411, CH1C = 2142, CH1R = 2958;  // CH1 steering left 900, center 1600, right 2200
uint16_t CH2L = 1411, CH2C = 2142, CH2R = 2958; // CH2 transmission gear 1 1900, 2 1000, 3 1000
uint16_t CH3L = 1411, CH3C = 2142, CH3R = 2958; // CH3 winch pull, off, release
uint16_t CH4L = 1836, CH4R = 2448;              // CH4 trailer coupler (5th. wheel) locked, unlocked

// Servo ramp time
uint16_t STEERING_RAMP_TIME = 0; // 0 = fastest speed, enlarge it to around 3000 for "scale" servo movements

#endif

// WPL C34 Toyota Land Cruiser configuration profile -------------------------------------------------------------------------------------------
#ifdef SERVOS_C34

// Servo frequency
const uint8_t SERVO_FREQUENCY = 50; // usually 50Hz, some servos may run smoother @ 100Hz

// WARNING: never connect receiver PWM signals to the "CH" pins in BUS communication mode!

// Servo limits
uint16_t CH1L = 1411, CH1C = 2142, CH1R = 2958; // CH1 steering left 1990, center 1640, right 1090
uint16_t CH2L = 1411, CH2C = 2142, CH2R = 2958;  // CH2 transmission gear 1 978, 2 1800, 3 1800
uint16_t CH3L = 1411, CH3C = 2142, CH3R = 2958; // CH3 winch pull, off, release
uint16_t CH4L = 1836, CH4R = 2448;              // CH4 trailer coupler (5th. wheel) locked, unlocked

// Servo ramp time
uint16_t STEERING_RAMP_TIME = 0; // 0 = fastest speed, enlarge it to around 3000 for "scale" servo movements

#endif

// WPL Ural servo configuration profile -------------------------------------------------------------------------------------------
#ifdef SERVOS_URAL

// Servo frequency
const uint8_t SERVO_FREQUENCY = 50; // usually 50Hz, some servos may run smoother @ 100Hz

// WARNING: never connect receiver PWM signals to the "CH" pins in BUS communication mode!

// Servo limits
uint16_t CH1L = 1411, CH1C = 2142, CH1R = 2958; // CH1 steering left 1990, center 1640, right 1090
uint16_t CH2L = 1411, CH2C = 2142, CH2R = 2958;  // CH2 transmission gear 1 978, 2 1800, 3 1800
uint16_t CH3L = 1411, CH3C = 2142, CH3R = 2958; // CH3 winch pull, off, release
uint16_t CH4L = 1836, CH4R = 2448;              // CH4 trailer coupler (5th. wheel) locked, unlocked

// Servo ramp time
uint16_t STEERING_RAMP_TIME = 0; // 0 = fastest speed, enlarge it to around 3000 for "scale" servo movements

#endif

// RGT EX86100 servo configuration profile -------------------------------------------------------------------------------------------
#ifdef SERVOS_RGT_EX86100

#define MODE2_WINCH    // Mode 2 is used for winch mode, if defined. The winch is controlled by the CH4 pot and connected to Servo CH3. BUS mode only!
#define NO_WINCH_DELAY // Use this, if you don't want a winch on / off ramp

// Servo frequency
const uint8_t SERVO_FREQUENCY = 50; // usually 50Hz, some servos may run smoother @ 100Hz

// WARNING: never connect receiver PWM signals to the "CH" pins in BUS communication mode!

// Servo limits
uint16_t CH1L = 1411, CH1C = 2142, CH1R = 2958; // CH1 steering left 2000, center 1660, right 1190
uint16_t CH2L = 1411, CH2C = 2142, CH2R = 2958; // CH2 transmission gear 1, 2, 3
uint16_t CH3L = 1411, CH3C = 2142, CH3R = 2958; // CH3 winch pull, off, release
uint16_t CH4L = 1836, CH4R = 2448;              // CH4 trailer coupler (5th. wheel) locked, unlocked

// Servo ramp time
uint16_t STEERING_RAMP_TIME = 0; // 0 = fastest speed, enlarge it to around 3000 for "scale" servo movements

#endif

// Hercules Hobby Actros 3363 -------------------------------------------------------------------------------------------
#ifdef SERVOS_ACTROS

#define CH3_BEACON              // Rotating Beacons are connected to Servo CH3. BUS mode only! https://www.ebay.ch/itm/303979210629
#define MODE2_TRAILER_UNLOCKING // The mode 2 button is used for trailer unlocking by servo CH4 (sound1 triggering will not work!)

// Servo frequency
const uint8_t SERVO_FREQUENCY = 50; // usually 50Hz, some servos may run smoother @ 100Hz

// WARNING: never connect receiver PWM signals to the "CH" pins in BUS communication mode!

// Servo limits
uint16_t CH1L = 1411, CH1C = 2142, CH1R = 2958; // CH1 steering left, center, right
uint16_t CH2L = 1411, CH2C = 2142, CH2R = 2958; // CH2 transmission gear 1, 2, 3
uint16_t CH3L = 1411, CH3C = 2142, CH3R = 2958; // CH3 Beacons (modes are switched, if position changes from 1000 to 2000us)
uint16_t CH4L = 1836, CH4R = 2448;              // CH4 trailer coupler (5th. wheel) locked, unlocked

// Servo ramp time
uint16_t STEERING_RAMP_TIME = 0; // 0 = fastest speed, enlarge it to around 3000 for "scale" servo movements

#endif

// TAMIYA King Hauler -------------------------------------------------------------------------------------------
#ifdef SERVOS_KING_HAULER

#define CH3_BEACON              // Rotating Beacons are connected to Servo CH3. BUS mode only! https://www.ebay.ch/itm/303979210629
#define MODE2_TRAILER_UNLOCKING // The mode 2 button is used for trailer 5th wheel unlocking by servo CH4 (sound1 triggering will not work!)

// Servo frequency
const uint8_t SERVO_FREQUENCY = 50; // usually 50Hz, some servos may run smoother @ 100Hz

// WARNING: never connect receiver PWM signals to the "CH" pins in BUS communication mode!

// Servo limits
uint16_t CH1L = 1411, CH1C = 2142, CH1R = 2958; // CH1 steering left, center, right
uint16_t CH2L = 1411, CH2C = 2142, CH2R = 2958; // CH2 transmission gear 1, 2, 3
uint16_t CH3L = 1411, CH3C = 2142, CH3R = 2958; // CH3 Beacons (modes are switched, if position changes from 1000 to 2000us)
uint16_t CH4L = 1836, CH4R = 2448;              // CH4 trailer coupler (5th. wheel) locked, unlocked

// Servo ramp time
uint16_t STEERING_RAMP_TIME = 0; // 0 = fastest speed, enlarge it to around 3000 for "scale" servo movements

#endif

// Carson Mercedes Racing Truck -------------------------------------------------------------------------------------------
#ifdef SERVOS_RACING_TRUCK

// Servo frequency
const uint8_t SERVO_FREQUENCY = 50; // usually 50Hz, some servos may run smoother @ 100Hz

// WARNING: never connect receiver PWM signals to the "CH" pins in BUS communication mode!

// Servo limits
uint16_t CH1L = 1411, CH1C = 2142, CH1R = 2958; // CH1 steering left, center, right
uint16_t CH2L = 1411, CH2C = 2142, CH2R = 2958; // CH2 transmission gear 1, 2, 3
uint16_t CH3L = 1411, CH3C = 2142, CH3R = 2958; // CH3 Beacons (modes are switched, if position changes from 1000 to 2000us)
uint16_t CH4L = 1836, CH4R = 2448;              // CH4 trailer coupler (5th. wheel) locked, unlocked

// Servo ramp time
uint16_t STEERING_RAMP_TIME = 0; // 0 = fastest speed, enlarge it to around 3000 for "scale" servo movements

#endif

// Meccano 3 Ton Dumper -------------------------------------------------------------------------------------------
#ifdef SERVOS_MECCANO_DUMPER

// Servo frequency
const uint8_t SERVO_FREQUENCY = 50; // usually 50Hz, some servos may run smoother @ 100Hz

// WARNING: never connect receiver PWM signals to the "CH" pins in BUS communication mode!

// Servo limits
uint16_t CH1L = 1411, CH1C = 2142, CH1R = 2958; // CH1 steering left, center, right
uint16_t CH2L = 1411, CH2C = 2142, CH2R = 2958; // CH2 transmission gear 1, 2, 3
uint16_t CH3L = 1411, CH3C = 2142, CH3R = 2958; // CH3 Beacons (modes are switched, if position changes from 1000 to 2000us)
uint16_t CH4L = 1836, CH4R = 2448;              // CH4 trailer coupler (5th. wheel) locked, unlocked

// Servo ramp time
uint16_t STEERING_RAMP_TIME = 0; // 0 = fastest speed, enlarge it to around 3000 for "scale" servo movements

#endif

// Open RC Tractor servo configuration profile -------------------------------------------------------------------------------------------
#ifdef SERVOS_OPEN_RC_TRACTOR

//#define MODE2_WINCH    // Mode 2 is used for winch mode, if defined. The winch is controlled by the CH4 pot and connected to Servo CH3. BUS mode only!
//#define NO_WINCH_DELAY // Use this, if you don't want a winch on / off ramp
#define MODE2_HYDRAULIC    // Mode 2 is used for hydraulic mode, if defined. The hydraulic is controlled by the CH4 pot and connected to Servo CH3. BUS mode only!

// Servo frequency
const uint8_t SERVO_FREQUENCY = 50; // usually 50Hz, some servos may run smoother @ 100Hz

// WARNING: never connect receiver PWM signals to the "CH" pins in BUS communication mode!

// Servo limits
uint16_t CH1L = 1411, CH1C = 2142, CH1R = 2958; // CH1 steering left 1620, center 1460, right 1300
uint16_t CH2L = 1411, CH2C = 2142, CH2R = 2958; // CH2 transmission gear 1, 2, 3
uint16_t CH3L = 1411, CH3C = 2142, CH3R = 2958; // CH3 winch pull, off, release
uint16_t CH4L = 1836, CH4R = 2448;              // CH4 trailer coupler (5th. wheel) locked, unlocked

// Servo ramp time
uint16_t STEERING_RAMP_TIME = 0; // 0 = fastest speed, enlarge it to around 3000 for "scale" servo movements

#endif

// NOTICE: The following profiles are for EXCAVATOR_MODE only! **********************************************************************************************
// Electric excavator servo configuration profile -------------------------------------------------------------------------------------------
#ifdef SERVOS_EXCAVATOR

boolean boomDownwardsHydraulic = true; // hydraulic load sound as well for boom downwards
boolean reverseBoomSoundDirection = false; // reverse sound direction, if needed (for example if hoses can't be swapped)

// Servo frequency
const uint8_t SERVO_FREQUENCY = 50; // usually 50Hz, some servos may run smoother @ 100Hz

// WARNING: never connect receiver PWM signals to the "CH" pins in BUS communication mode!

// Servo limits
uint16_t CH1L = 1411, CH1C = 2142, CH1R = 2958; // CH1 bucket ESC
uint16_t CH2L = 1411, CH2C = 2142, CH2R = 2958; // CH2 dipper ESC
uint16_t CH3L = 1411, CH3C = 2142, CH3R = 2958; // CH3 boom ESC
uint16_t CH4L = 1836, CH4C = 2142, CH4R = 2448; // CH4 swing ESC

// Servo ramp times
uint16_t CH1_RAMP_TIME = 0; // 0 = fastest speed, enlarge it to around 3000 for "scale" servo movements
uint16_t CH2_RAMP_TIME = 100;
uint16_t CH3_RAMP_TIME = 1000;
uint16_t CH4_RAMP_TIME = 2000;

#endif

// Raw passthrough servo configuration (for dozers, etc. with direct ESC/actuator control) --------------------------------
#ifdef SERVOS_PASSTHROUGH

boolean boomDownwardsHydraulic = true; // needed for hydraulic sound code
boolean reverseBoomSoundDirection = false;

// Servo frequency
const uint8_t SERVO_FREQUENCY = 50;

// WARNING: never connect receiver PWM signals to the "CH" pins in BUS communication mode!

// CH limits (used by sound code for hydraulic pump volume calculations, not for output clamping)
uint16_t CH1L = 1000, CH1C = 1500, CH1R = 2000; // CH1 Track 1
uint16_t CH2L = 1000, CH2C = 1500, CH2R = 2000; // CH2 Track 2
uint16_t CH3L = 1000, CH3C = 1500, CH3R = 2000; // CH3 Blade
uint16_t CH4L = 1000, CH4C = 1500, CH4R = 2000; // CH4 Ripper

// Ramp times (not used in passthrough mode, but needed for compilation)
uint16_t CH1_RAMP_TIME = 0;
uint16_t CH2_RAMP_TIME = 0;
uint16_t CH3_RAMP_TIME = 0;
uint16_t CH4_RAMP_TIME = 0;
uint16_t STEERING_RAMP_TIME = 0;

// Passthrough channel mapping: which pulseWidth[] index drives each output pin
// pulseWidth[1] = STEERING channel, pulseWidth[2] = GEARBOX channel
// pulseWidth[14] and [15] are extra iBUS reads (PT_IBUS_CH3 / PT_IBUS_CH4 from 2_Remote.h)
#define PT_CH1 1   // CH1 pin (GPIO13) <- pulseWidth[1] (Track 1)
#define PT_CH2 2   // CH2 pin (GPIO12) <- pulseWidth[2] (Track 2)
#define PT_CH3 14  // CH3 pin (GPIO14) <- pulseWidth[14] (Blade)
#define PT_CH4 15  // CH4 pin (GPIO27) <- pulseWidth[15] (Ripper)

#endif

// Hydraulic excavator servo configuration profile -------------------------------------------------------------------------------------------
#ifdef SERVOS_HYDRAULIC_EXCAVATOR

#define PINGON_MODE // Wheel lift for Pingon excavators

boolean boomDownwardsHydraulic = true; // hydraulic load sound as well for boom downwards
boolean reverseBoomSoundDirection = false; // reverse sound direction, if needed (for example if hoses can't be swapped)

// Servo frequency
const uint8_t SERVO_FREQUENCY = 50; // usually 50Hz, some servos may run smoother @ 100Hz

// WARNING: never connect receiver PWM signals to the "CH" pins in BUS communication mode!

// Valve servo limits
uint16_t CH1L = 1411, CH1C = 2142, CH1R = 2958; // CH1 bucket valve
uint16_t CH2L = 1411, CH2C = 2142, CH2R = 2958; // CH2 dipper valve
uint16_t CH3L = 1411, CH3C = 2142, CH3R = 2958; // CH3 boom valve

// Swing ESC limits
uint16_t CH4L = 1836, CH4C = 2142, CH4R = 2448; // CH4 swing ESC 1375, 1625 (1250, 1500, 1750 for Pingon)

// Hydraulic pump limits
uint16_t ESC_L = 1411, ESC_C = 2142, ESC_R = 2958; // ESC output for oil pump (always 1000, 1500, 2000)
uint16_t ESC_MIN = 2142; // Pump off
uint16_t ESC_MAX = 2652; // Pump max. RPM (1800 for Pingon)

// Servo ramp times
uint16_t CH1_RAMP_TIME = 0; // always 0 for now
uint16_t CH2_RAMP_TIME = 100; // always 0 for now
uint16_t CH3_RAMP_TIME = 1000; // always 0 for now
uint16_t CH4_RAMP_TIME = 2000; // 2000 for swing motor protection (2500 for Pingon)

#endif

// Mushroom3D rough terrain crane -------------------------------------------------------------------------------------------
#ifdef SERVOS_CRANE

// Servo frequency
const uint8_t SERVO_FREQUENCY = 50; // usually 50Hz, some servos may run smoother @ 100Hz

// WARNING: never connect receiver PWM signals to the "CH" pins in BUS communication mode!

// Servo limits (not used, SBUS decoder only, adjust it in the transmitter, if needed!)
uint16_t CH1L = 1411, CH1C = 2142, CH1R = 2958; // CH1 (decoded CH13)
uint16_t CH2L = 1411, CH2C = 2142, CH2R = 2958; // CH2 (decoded CH14)
uint16_t CH3L = 1411, CH3C = 2142, CH3R = 2958; // CH3 (decoded CH15)
uint16_t CH4L = 1836, CH4C = 2142, CH4R = 2448; // CH4 (decoded CH16)          

#endif
