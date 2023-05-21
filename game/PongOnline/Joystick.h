//========================================================
// 功能：搖桿物件.
//
//========================================================
#ifndef JOYSTICK_H
#define JOYSTICK_H

// 按鈕結構.
struct Button {
  uint8_t PIN;
  bool pressed;
};

class Joystick
{
public:
  //------------------------------------------------------------------------
  //     功能說明:建構式.
  // 傳入參數說明:
  //      pinUp:.
  //    pinDown:.
  //    pinLeft:.
  //    pinRight:.
  //      pinA:.
  //      pinB:.
  //------------------------------------------------------------------------
  Joystick(uint8_t pinUp, uint8_t pinDown, uint8_t pinLeft, uint8_t pinRight, uint8_t pinA, uint8_t pinB, uint8_t  pinStart, uint8_t pinSelect) {
    // 設定PIN.
    button_up.PIN = pinUp;
    button_down.PIN = pinDown;
    button_left.PIN = pinLeft;
    button_right.PIN = pinRight;
    button_a.PIN = pinA;
    button_b.PIN = pinB;
    button_start.PIN = pinStart;
    button_select.PIN = pinSelect;

    pinMode(button_up.PIN, INPUT);
    pinMode(button_down.PIN, INPUT);
    pinMode(button_left.PIN, INPUT);
    pinMode(button_right.PIN, INPUT);
    pinMode(button_a.PIN, INPUT);
    pinMode(button_b.PIN, INPUT);
    pinMode(button_start.PIN, INPUT);
    pinMode(button_select.PIN, INPUT);

  }

  //------------------------------------------------------------------------
  // 功能說明:取的按鈕上狀態.
  //------------------------------------------------------------------------
  bool GetButtonUp(bool trigger = false) {
    if (trigger && (button_up.pressed == HIGH))
      if (digitalRead(button_up.PIN) == HIGH)
        return false;
    button_up.pressed = (digitalRead(button_up.PIN) == HIGH);
    return button_up.pressed;
  };

  //------------------------------------------------------------------------
  // 功能說明:取的按鈕下狀態.
  //------------------------------------------------------------------------
  bool GetButtonDown(bool trigger = false) {
    if (trigger && (button_down.pressed == HIGH))
      if (digitalRead(button_down.PIN) == HIGH)
        return false;
    button_down.pressed = (digitalRead(button_down.PIN) == HIGH);
    return button_down.pressed;
  };

  //------------------------------------------------------------------------
  // 功能說明:取的按鈕左狀態.
  //------------------------------------------------------------------------
  bool GetButtonLeft(bool trigger = false) {
    if (trigger && (button_left.pressed == HIGH))
      if (digitalRead(button_left.PIN) == HIGH)
        return false;
    button_left.pressed = (digitalRead(button_left.PIN) == HIGH);
    return button_left.pressed;
  };

  //------------------------------------------------------------------------
  // 功能說明:取的按鈕左狀態.
  //------------------------------------------------------------------------
  bool GetButtonRight(bool trigger = false) {
    if (trigger && (button_right.pressed == HIGH))
      if (digitalRead(button_right.PIN) == HIGH)
        return false;
    button_right.pressed = (digitalRead(button_right.PIN) == HIGH);
    return button_right.pressed;
  };

  //------------------------------------------------------------------------
  // 功能說明:取的按鈕A狀態.
  //------------------------------------------------------------------------
  bool GetButtonA(bool trigger = false) {
    if (trigger && (button_a.pressed == HIGH))
      if(digitalRead(button_a.PIN) == HIGH)
        return false;
    button_a.pressed = (digitalRead(button_a.PIN) == HIGH);
    return button_a.pressed;
  };

  //------------------------------------------------------------------------
  // 功能說明:取的按鈕B狀態.
  //------------------------------------------------------------------------
  bool GetButtonB(bool trigger=false) {
    if (trigger && (button_b.pressed == HIGH))
      if (digitalRead(button_b.PIN) == HIGH)
        return false;
    button_b.pressed = (digitalRead(button_b.PIN) == HIGH);
    return button_b.pressed;
  };

  //------------------------------------------------------------------------
  // 功能說明:取的按鈕Start狀態.
  //------------------------------------------------------------------------
  bool GetButtonStart(bool trigger=false) {
    if (trigger && (button_start.pressed == HIGH))
      if (digitalRead(button_start.PIN) == HIGH)
        return false;
    button_start.pressed = (digitalRead(button_start.PIN) == HIGH);
    return button_start.pressed;
  };

  //------------------------------------------------------------------------
  // 功能說明:取的按鈕Select狀態.
  //------------------------------------------------------------------------
  bool GetButtonSelect(bool trigger=false) {
    if (trigger && (button_select.pressed == HIGH))
      if (digitalRead(button_select.PIN) == HIGH)
        return false;
    button_select.pressed = (digitalRead(button_select.PIN) == HIGH);
    return button_select.pressed;
  };

private:
  // 按鈕PIN.
  Button button_up = { 34, false };
  Button button_down = { 35, false };
  Button button_left = { 39, false };
  Button button_right = { 36, false };
  Button button_a = { 25, false };
  Button button_b = { 26, false };
  Button button_start = { 2, true };
  Button button_select = { 5, true };

};

#endif
