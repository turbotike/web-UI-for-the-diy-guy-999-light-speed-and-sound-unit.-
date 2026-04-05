#include <Arduino.h>

/* General SOUND SETTINGS ************************************************************************************************
 *
 * Most sound settings are done in the vehicle configuration files in the /vehicles/ directory.
 *
 */

// #define NO_SIREN // siren sound is not played, if defined
// #define NO_INDICATOR_SOUND // If you don't want the indicator "tick - tack" sound

// Volume pot override: uncomment to ignore RC volume cycling and use only the web slider value
#define VOL_POT_OVERRIDE

// Volume adjustment
// const  uint8_t numberOfVolumeSteps = 4; // The mumber of volume steps below
// const uint8_t masterVolumePercentage[] = {100, 66, 44}; // loud, medium, silent (more than 100% may cause distortions)

const uint8_t numberOfVolumeSteps = 4;                     // The mumber of volume steps below
const uint8_t masterVolumePercentage[] = {250, 187, 125, 0}; // max, loud, medium, mute (values above 100 may clip but are LOUDER)

// Crawler mode
const uint8_t masterVolumeCrawlerThreshold = 60; // If master volume is <= this threshold, crawler mode (without virtual inertia) is active
