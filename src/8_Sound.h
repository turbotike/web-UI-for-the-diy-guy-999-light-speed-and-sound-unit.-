#include <Arduino.h>

/* General SOUND SETTINGS ************************************************************************************************
 *
 * Most sound settings are done in the vehicle configuration files in the /vehicles/ directory.
 *
 */

// #define NO_SIREN // siren sound is not played, if defined
 #define NO_INDICATOR_SOUND // If you don't want the indicator "tick - tack" sound

// Volume adjustment
// const  uint8_t numberOfVolumeSteps = 3; // The mumber of volume steps below
// const uint8_t masterVolumePercentage[] = {100, 66, 44}; // loud, medium, silent (more than 100% may cause distortions)

const uint8_t numberOfVolumeSteps = 4;                     // The mumber of volume steps below
const uint8_t masterVolumePercentage[] = {100, 66, 44, 0}; // loud, medium, silent, no sound (more than 100% may cause distortions)

// Crawler mode
const uint8_t masterVolumeCrawlerThreshold = 44; // If master volume is <= this threshold, crawler mode (without virtual inertia) is active

// Air dryer (periodic air-system purge "pfft", reuses the air brake hiss). GLOBAL (applies to all vehicles).
uint16_t airDryerInterval = 40;        // Seconds between air dryer purges while the engine runs. 0 = off.
uint8_t airDryerVolumePercentage = 80; // Volume of the air dryer purge (% of the air brake hiss sound)
