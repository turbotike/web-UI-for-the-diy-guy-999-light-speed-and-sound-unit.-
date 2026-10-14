#include <Arduino.h>

// The shaker is simulating engine vibrations

// Select the shaker configuration you have:
#define GT_POWER_STOCK // <------- Select (remove //) one of the shaker configurations below
//#define GT_POWER_PLASTIC
#if !defined GT_POWER_STOCK && !defined GT_POWER_PLASTIC
#error "5_Shaker.h: select a shaker configuration (GT_POWER_STOCK or GT_POWER_PLASTIC) - the Shaker tab's dropdown does this for you."
#endif


// SHAKER SETTINGS ********************************************************************************************************************

// GT-Power shaker with brass weight in stock condition ------------------------------------------------------------------------------
// Used in:
// - TAMIYA King Hauler
#ifdef GT_POWER_STOCK

// Shaker parameters (simulating engine vibrations)
const uint8_t shakerStart = 100; // Shaker power while engine start (max. 255, about 100)
const uint8_t shakerIdle = 49; // Shaker power while idling (max. 255, about 49)
const uint8_t shakerFullThrottle = 40; // Shaker power while full throttle (max. 255, about 40)
const uint8_t shakerStop = 60; // Shaker power while engine stop (max. 255, about 60)
#endif

// GT-Power shaker with 3D printed plastic weight ------------------------------------------------------------------------------------
// Used in:
// - Hercules Hobby Actros
#ifdef GT_POWER_PLASTIC

// Shaker parameters (simulating engine vibrations)
const uint8_t shakerStart = 100; // Shaker power while engine start (max. 255, about 100)
const uint8_t shakerIdle = 49; // Shaker power while idling (max. 255, about 49)
const uint8_t shakerFullThrottle = 40; // Shaker power while full throttle (max. 255, about 40)
const uint8_t shakerStop = 60; // Shaker power while engine stop (max. 255, about 60)
#endif
