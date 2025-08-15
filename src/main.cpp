#include <Arduino.h>
#include "Keyboard.h"

void setup() {
  // Type all possible characters.
  // Many countries do not use all characters.
  // Unsupported characters will be skipped.
  //
  Keyboard.begin();
  delay(1000);
  Keyboard.println("Teensy USB Keyboard Layout Test");
  Keyboard.println("Lowercase:  abcdefghijklmnopqrstuvwxyz");
  Keyboard.println("Uppercase:  ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  Keyboard.println("Numbers:    0123456789");
  Keyboard.println("Symbols1:   !\"#$%&'()*+,-./");
  Keyboard.println("Symbols2:   :;<=>?[\\]^_`{|}~");
  Keyboard.println("Symbols3:   ¡¢£¤¥¦§¨©ª«¬­®¯°±");
  Keyboard.println("Symbols4:   ²³´µ¶·¸¹º»¼½¾¿×÷");
  Keyboard.println("Grave:      ÀÈÌÒÙàèìòù");
  Keyboard.println("Acute:      ÁÉÍÓÚÝáéíóúý");
  Keyboard.println("Circumflex: ÂÊÎÔÛâêîôû");
  Keyboard.println("Tilde:      ÃÑÕãñõ");
  Keyboard.println("Diaeresis:  ÄËÏÖÜäëïöüÿ");
  Keyboard.println("Cedilla:    Çç");
  Keyboard.println("Ring Above: Åå");
  Keyboard.println("AE:         Ææ");
  Keyboard.println("Thorn:      Þþ");
  Keyboard.println("Sharp S:    ß");
  Keyboard.println("O-Stroke:   Øø");
  Keyboard.println("Eth:        Ðð");
  Keyboard.println("Euro:       €");
}

void loop() {
  Keyboard.press(KEY_MEDIA_VOLUME_DEC);
  Keyboard.release(KEY_MEDIA_VOLUME_DEC);
  delay(1000);

  Keyboard.press(KEY_MEDIA_VOLUME_INC);
  Keyboard.release(KEY_MEDIA_VOLUME_INC);
  delay(5000);
}
