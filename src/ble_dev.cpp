#include "ble_dev.h"

#ifdef BLE_KEYBOARD

#include <NimBLEDevice.h>
#include <NimBLEServer.h>
#include <NimBLEUtils.h>
#include <NimBLEHIDDevice.h>

#define KEYBOARD_ID 0x01
#define MEDIA_KEYS_ID 0x02

static const uint8_t _hidReportDescriptor[] = {
  USAGE_PAGE(1),      0x01,          // USAGE_PAGE (Generic Desktop Ctrls)
  USAGE(1),           0x06,          // USAGE (Keyboard)
  COLLECTION(1),      0x01,          // COLLECTION (Application)
  // ------------------------------------------------- Keyboard
  REPORT_ID(1),       KEYBOARD_ID,   //   REPORT_ID (1)
  USAGE_PAGE(1),      0x07,          //   USAGE_PAGE (Kbrd/Keypad)
  USAGE_MINIMUM(1),   0xE0,          //   USAGE_MINIMUM (0xE0)
  USAGE_MAXIMUM(1),   0xE7,          //   USAGE_MAXIMUM (0xE7)
  LOGICAL_MINIMUM(1), 0x00,          //   LOGICAL_MINIMUM (0)
  LOGICAL_MAXIMUM(1), 0x01,          //   Logical Maximum (1)
  REPORT_SIZE(1),     0x01,          //   REPORT_SIZE (1)
  REPORT_COUNT(1),    0x08,          //   REPORT_COUNT (8)
  HIDINPUT(1),        0x02,          //   INPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  REPORT_COUNT(1),    0x01,          //   REPORT_COUNT (1) ; 1 byte (Reserved)
  REPORT_SIZE(1),     0x08,          //   REPORT_SIZE (8)
  HIDINPUT(1),        0x01,          //   INPUT (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  REPORT_COUNT(1),    0x05,          //   REPORT_COUNT (5) ; 5 bits (Num lock, Caps lock, Scroll lock, Compose, Kana)
  REPORT_SIZE(1),     0x01,          //   REPORT_SIZE (1)
  USAGE_PAGE(1),      0x08,          //   USAGE_PAGE (LEDs)
  USAGE_MINIMUM(1),   0x01,          //   USAGE_MINIMUM (0x01) ; Num Lock
  USAGE_MAXIMUM(1),   0x05,          //   USAGE_MAXIMUM (0x05) ; Kana
  HIDOUTPUT(1),       0x02,          //   OUTPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  REPORT_COUNT(1),    0x01,          //   REPORT_COUNT (1) ; 3 bits (Padding)
  REPORT_SIZE(1),     0x03,          //   REPORT_SIZE (3)
  HIDOUTPUT(1),       0x01,          //   OUTPUT (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  REPORT_COUNT(1),    0x06,          //   REPORT_COUNT (6) ; 6 bytes (Keys)
  REPORT_SIZE(1),     0x08,          //   REPORT_SIZE(8)
  LOGICAL_MINIMUM(1), 0x00,          //   LOGICAL_MINIMUM(0)
  LOGICAL_MAXIMUM(1), 0x65,          //   LOGICAL_MAXIMUM(0x65) ; 101 keys
  USAGE_PAGE(1),      0x07,          //   USAGE_PAGE (Kbrd/Keypad)
  USAGE_MINIMUM(1),   0x00,          //   USAGE_MINIMUM (0)
  USAGE_MAXIMUM(1),   0x65,          //   USAGE_MAXIMUM (0x65)
  HIDINPUT(1),        0x00,          //   INPUT (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  END_COLLECTION(0),                 // END_COLLECTION
  // ------------------------------------------------- Media Keys
  USAGE_PAGE(1),      0x0C,          // USAGE_PAGE (Consumer)
  USAGE(1),           0x01,          // USAGE (Consumer Control)
  COLLECTION(1),      0x01,          // COLLECTION (Application)
  REPORT_ID(1),       MEDIA_KEYS_ID, //   REPORT_ID (3)
  USAGE_PAGE(1),      0x0C,          //   USAGE_PAGE (Consumer)
  LOGICAL_MINIMUM(1), 0x00,          //   LOGICAL_MINIMUM (0)
  LOGICAL_MAXIMUM(1), 0x01,          //   LOGICAL_MAXIMUM (1)
  REPORT_SIZE(1),     0x01,          //   REPORT_SIZE (1)
  REPORT_COUNT(1),    0x10,          //   REPORT_COUNT (16)
  USAGE(1),           0xB5,          //   USAGE (Scan Next Track)     ; bit 0: 1
  USAGE(1),           0xB6,          //   USAGE (Scan Previous Track) ; bit 1: 2
  USAGE(1),           0xB7,          //   USAGE (Stop)                ; bit 2: 4
  USAGE(1),           0xCD,          //   USAGE (Play/Pause)          ; bit 3: 8
  USAGE(1),           0xE2,          //   USAGE (Mute)                ; bit 4: 16
  USAGE(1),           0xE9,          //   USAGE (Volume Increment)    ; bit 5: 32
  USAGE(1),           0xEA,          //   USAGE (Volume Decrement)    ; bit 6: 64
  USAGE(2),           0x23, 0x02,    //   Usage (WWW Home)            ; bit 7: 128
  USAGE(2),           0x94, 0x01,    //   Usage (My Computer) ; bit 0: 1
  USAGE(2),           0x92, 0x01,    //   Usage (Calculator)  ; bit 1: 2
  USAGE(2),           0x2A, 0x02,    //   Usage (WWW fav)     ; bit 2: 4
  USAGE(2),           0x21, 0x02,    //   Usage (WWW search)  ; bit 3: 8
  USAGE(2),           0x26, 0x02,    //   Usage (WWW stop)    ; bit 4: 16
  USAGE(2),           0x24, 0x02,    //   Usage (WWW back)    ; bit 5: 32
  USAGE(2),           0x83, 0x01,    //   Usage (Media sel)   ; bit 6: 64
  USAGE(2),           0x8A, 0x01,    //   Usage (Mail)        ; bit 7: 128
  HIDINPUT(1),        0x02,          //   INPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  END_COLLECTION(0)                  // END_COLLECTION
};

NimBLEHIDDevice*      ble_hid;
NimBLECharacteristic* inputKeyboard;
NimBLECharacteristic* outputKeyboard;    // for LED status (Caps Lock, Num Lock, etc.)
NimBLECharacteristic* inputMediaKeys;
NimBLEAdvertising*    advertising;
bool                  connected = false;

uint16_t vid       = 0x05ac;
uint16_t pid       = 0x820a;
uint16_t version   = 0x0210;


typedef struct
{
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[6];
} KeyReport;


uint16_t linuxToBleMediaMask(uint16_t linuxCode)
{
    uint16_t usageId = linuxCode & 0x0FFF;

    switch (usageId) {
        // First byte (bit 0..7)
        case 0x0B5: return 1 << 0;   // Next Track
        case 0x0B6: return 1 << 1;   // Previous Track
        case 0x0B7: return 1 << 2;   // Stop
        case 0x0CD: return 1 << 3;   // Play/Pause
        case 0x0E2: return 1 << 4;   // Mute
        case 0x0E9: return 1 << 5;   // Volume Up
        case 0x0EA: return 1 << 6;   // Volume Down
        case 0x023: return 1 << 7;   // WWW Home

        // Second byte (bit 8..15)
        case 0x194: return 1 << (8 + 0);  // My Computer
        case 0x192: return 1 << (8 + 1);  // Calculator
        case 0x22A: return 1 << (8 + 2);  // WWW Favorites
        case 0x221: return 1 << (8 + 3);  // WWW Search
        case 0x226: return 1 << (8 + 4);  // WWW Stop
        case 0x224: return 1 << (8 + 5);  // WWW Back
        case 0x183: return 1 << (8 + 6);  // Media Select
        case 0x18A: return 1 << (8 + 7);  // Mail

        default: return 0;
    }
}


class ServerCallbacks : public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer) override {
      connected = true;
    }

    void onDisconnect(NimBLEServer* pServer) override {
        connected = false;
        NimBLEDevice::startAdvertising();
    }
} serverCallbacks;

class CharacteristicCallbacks : public NimBLECharacteristicCallbacks {
    void onRead(NimBLECharacteristic* pCharacteristic) override {
    }

    void onWrite(NimBLECharacteristic* pCharacteristic) override {
      uint8_t* value = (uint8_t*)(pCharacteristic->getValue().c_str());
      (void)value;
    }

} chrCallbacks;


void ble_init(const char *deviceName, const char *deviceManufacturer, uint8_t batteryLevel)
{
  std::string name(deviceName);
  std::string manufacturer(deviceManufacturer);

  NimBLEDevice::init(name.substr(0,15));
  NimBLEServer* pServer = BLEDevice::createServer();
  pServer->setCallbacks(&serverCallbacks);

  ble_hid = new NimBLEHIDDevice(pServer);
  inputKeyboard = ble_hid->inputReport(KEYBOARD_ID);
  outputKeyboard = ble_hid->outputReport(KEYBOARD_ID);
  inputMediaKeys = ble_hid->inputReport(MEDIA_KEYS_ID);

  outputKeyboard->setCallbacks(&chrCallbacks);

  ble_hid->manufacturer()->setValue(manufacturer.substr(0,15));

  ble_hid->pnp(0x02, vid, pid, version);
  ble_hid->hidInfo(0x00, 0x01);

  NimBLEDevice::setSecurityAuth(true, true, true);

  ble_hid->reportMap((uint8_t*)_hidReportDescriptor, sizeof(_hidReportDescriptor));
  ble_hid->startServices();

  advertising = pServer->getAdvertising();
  advertising->setAppearance(HID_KEYBOARD);
  advertising->addServiceUUID(ble_hid->hidService()->getUUID());
  advertising->setScanResponse(false);
  advertising->start();
  ble_hid->setBatteryLevel(batteryLevel);
}

int ble_press(uint8_t modifier, uint8_t keycode[6]) {
  if (connected)
    {
      delay(7);
      static KeyReport k;
      k.modifiers = modifier;
      k.reserved = 0;
      for (int i = 0; i < 6; i++)
        k.keys[i] = keycode[i];
      inputKeyboard->setValue((uint8_t*)&k, sizeof(KeyReport));
      inputKeyboard->notify();
      delay(7);
    }
  return 0;
}

int ble_mediakey_press(uint16_t usage_code) {
  if (connected)
    {
      delay(7);
      uint32_t mask = linuxToBleMediaMask(usage_code);
      inputMediaKeys->setValue((uint8_t*)&mask, sizeof(mask));
      inputMediaKeys->notify();
      delay(7);
    }
  return 0;
}

#else

void ble_init(const char *deviceName, const char *deviceManufacturer, uint8_t batteryLevel) {};
int ble_press(uint8_t modifier, uint8_t keycode[6]) { return 0;};
int ble_mediakey_press(uint16_t usage_code) { return 0;};

#endif // BLE_KEYBOARD