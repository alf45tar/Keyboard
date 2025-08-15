#include "usb_dev.h"

#ifdef USB_KEYBOARD

#include <Adafruit_TinyUSB.h>

enum {
  RID_KEYBOARD = 1,
  RID_CONSUMER_CONTROL, // Media, volume etc ..
};

uint8_t const desc_hid_report[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(RID_KEYBOARD)),
    TUD_HID_REPORT_DESC_CONSUMER(HID_REPORT_ID(RID_CONSUMER_CONTROL))
};

Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, false);


void usb_init(void)
{
    usb_hid.begin();

    // Re-attaches the USB device if already mounted, forcing re-enumeration so new HID drivers are applied.
    if (TinyUSBDevice.mounted()) {
        TinyUSBDevice.detach();
        delay(10);
        TinyUSBDevice.attach();
    }
}


int usb_press(uint8_t modifier, uint8_t keycode[6]) {
    delay(5);
    if (TinyUSBDevice.suspended()) TinyUSBDevice.remoteWakeup();
    if (!usb_hid.ready()) return 1;
    return usb_hid.keyboardReport(RID_KEYBOARD, modifier, keycode) ? 0 : 1;
}


int usb_mediakey_press(uint16_t usage_code) {
    delay(5);
    if (TinyUSBDevice.suspended()) TinyUSBDevice.remoteWakeup();
    if (!usb_hid.ready()) return 1;
    return usb_hid.sendReport16(RID_CONSUMER_CONTROL, usage_code) ? 0 : 1;
}

#else

void usb_init(void) {};
int usb_press(uint8_t modifier, uint8_t keycode[6]) { return 0;};
int usb_mediakey_press(uint16_t usage_code) { return 0;};

#endif // USB_KEYBOARD