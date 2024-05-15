#include <Arduino.h>
#include <BleKeyboard.h> // https://github.com/T-vK/ESP32-BLE-Keyboard

BleKeyboard bleKeyboard;

class Button
{
private:
  uint8_t btn;
  uint8_t state;

public:
  void begin(uint8_t button)
  {
    btn = button;
    state = 0;
    pinMode(btn, INPUT_PULLUP);
  }
  bool pressed()
  {
    state = (state << 1) | digitalRead(btn) | 0b00000000;
    // Serial.print("Button: ");
    // Serial.println(digitalRead(btn));
    // Serial.print("State: ");
    // Serial.println(state);
    // Serial.print("Return: ");
    // Serial.println((state == 0b11110000));
    return (state == 0b00000000);
  }
  bool unpressed()
  {
    state = (state << 1) | digitalRead(btn) | 0b00000000;
    // Serial.print("Button: ");
    // Serial.println(digitalRead(btn));
    // Serial.print("State: ");
    // Serial.println(state);
    // Serial.print("Return: ");
    // Serial.println((state == 0b11110000));
    return (state == 0b11111111);
  }
};

#define THUMB 13
#define INDEX 12
#define MIDDLE 14
#define RING 27
#define PINKIE 26

Button thumbb;
Button indexb;
Button middleb;
Button ringb;
Button pinkieb;

bool thumb_pressed = false;
bool index_pressed = false;
bool middle_pressed = false;
bool ring_pressed = false;
bool pinkie_pressed = false;

bool any_pressed = false;

bool none_pressed = true;

bool send_char = false;

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();

  // pinMode(34, OUTPUT);
  // pinMode(35, OUTPUT);

  // digitalWrite(34,LOW);
  // digitalWrite(35,LOW);

  thumbb.begin(THUMB);
  indexb.begin(INDEX);
  middleb.begin(MIDDLE);
  ringb.begin(RING);
  pinkieb.begin(PINKIE);
}

void checkPressed()
{
  if (thumb_pressed == false)
  {
    thumb_pressed = thumbb.pressed();
  }
  if (index_pressed == false)
  {
    index_pressed = indexb.pressed();
  }
  if (middle_pressed == false)
  {
    middle_pressed = middleb.pressed();
  }
  if (ring_pressed == false)
  {
    ring_pressed = ringb.pressed();
  }
  if (pinkie_pressed == false)
  {
    pinkie_pressed = pinkieb.pressed();
  }

  if (thumb_pressed || index_pressed || middle_pressed || ring_pressed || pinkie_pressed)
  {
    any_pressed = true;
  }

  if (thumbb.unpressed() && indexb.unpressed() && middleb.unpressed() && ringb.unpressed() && pinkieb.unpressed())
  {
    none_pressed = true;
  }
  else
  {
    none_pressed = false;
  }

  if (any_pressed && none_pressed)
  {
    send_char = true;
  }

  // Serial.print("any_pressed: ");
  // Serial.println(any_pressed);
  // Serial.print("none_pressed: ");
  // Serial.println(none_pressed);
  // Serial.print("thumb_pressed: ");
  // Serial.println(thumb_pressed);
  // Serial.print("t: ");
  // Serial.println(digitalRead(THUMB));
  // Serial.print("i: ");
  // Serial.println(digitalRead(INDEX));
  // Serial.print("m: ");
  // Serial.println(digitalRead(MIDDLE));
  // Serial.print("r: ");
  // Serial.println(digitalRead(RING));
  // Serial.print("p: ");
  // Serial.println(digitalRead(PINKIE));

  // Serial.print("thumb_pressed: ");
  // Serial.println(thumbb.pressed());
  // Serial.print("thumb_unpressed: ");
  // Serial.println(thumbb.unpressed());
}

void sendChar()
{
  uint8_t keys_pressed = (thumb_pressed << 4) | (index_pressed << 3) | (middle_pressed << 2) | (ring_pressed << 1) | (pinkie_pressed);

  // Serial.println(keys_pressed);

  // keys https://photos5.appleinsider.com/gallery/32637-56078-tapapp-xl.jpg

  switch (keys_pressed)
  {
  case 0b10000:
    bleKeyboard.print('a');
    break;
  case 0b01001:
    bleKeyboard.print('b');
    break;
  case 0b10111:
    bleKeyboard.print('c');
    break;
  case 0b10100:
    bleKeyboard.print('d');
    break;
  case 0b01000:
    bleKeyboard.print('e');
    break;
  case 0b11010:
    bleKeyboard.print('f');
    break;
  case 0b10110:
    bleKeyboard.print('g');
    break;
  case 0b01111:
    bleKeyboard.print('h');
    break;
  case 0b00100:
    bleKeyboard.print('i');
    break;
  case 0b11101:
    bleKeyboard.print('j');
    break;
  case 0b10010:
    bleKeyboard.print('k');
    break;
  case 0b00110:
    bleKeyboard.print('l');
    break;
  case 0b01010:
    bleKeyboard.print('m');
    break;
  case 0b11000:
    bleKeyboard.print('n');
    break;
  case 0b00010:
    bleKeyboard.print('o');
    break;
  case 0b11001:
    bleKeyboard.print('p');
    break;
  case 0b01101:
    bleKeyboard.print('q');
    break;
  case 0b11110:
    bleKeyboard.print('r');
    break;
  case 0b00011:
    bleKeyboard.print('s');
    break;
  case 0b01100:
    bleKeyboard.print('t');
    break;
  case 0b00001:
    bleKeyboard.print('u');
    break;
  case 0b11011:
    bleKeyboard.print('v');
    break;
  case 0b10101:
    bleKeyboard.print('w');
    break;
  case 0b01011:
    bleKeyboard.print('x');
    break;
  case 0b10001:
    bleKeyboard.print('y');
    break;
  case 0b00101:
    bleKeyboard.print('z');
    break;

  case 0b11111:
    bleKeyboard.print(' ');
    break;
  case 0b01110:
    bleKeyboard.write(KEY_BACKSPACE);
    break;
  case 0b10011:
    bleKeyboard.write(KEY_RETURN);
    break;
  }

  // reset
  {
    send_char = false;
    any_pressed = false;
    thumb_pressed = false;
    index_pressed = false;
    middle_pressed = false;
    ring_pressed = false;
    pinkie_pressed = false;
  }
}

void loop()
{
  if (bleKeyboard.isConnected())
  {
    checkPressed();

    if (send_char)
    {
      sendChar();
      delay(5);
    }
  }
}
