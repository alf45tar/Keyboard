#include "transport.h"
#include "usb_dev.h"
#include "ble_dev.h"

void transport_init(void)
{
    usb_init();
    ble_init("S3 BLE Keyboard", "Espressif", 100);
}


int transport_press(uint8_t modifier, uint8_t keycode[6])
{
    usb_press(modifier, keycode);
    ble_press(modifier, keycode);
}


int transport_mediakey_press(uint16_t usage_code)
{
    usb_mediakey_press(usage_code);
    ble_mediakey_press(usage_code);
}