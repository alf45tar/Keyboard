#ifndef _transport_h_
#define _transport_h_

#include <stdint.h>

// This header is NOT meant to be included when compiling
// user sketches in Arduino.  The low-level functions
// provided by transport.c are meant to be called only by
// code which provides higher-level interfaces to the user.


#ifdef __cplusplus
extern "C" {
#endif

void transport_init(void);
int transport_press(uint8_t modifier, uint8_t keycode[6]);
int transport_mediakey_press(uint16_t usage_code);

#ifdef KEYBOARD_INTERFACE
extern uint8_t keyboard_modifier_keys;
extern uint8_t keyboard_keys[6];
#endif

#ifdef __cplusplus
}
#endif

#endif