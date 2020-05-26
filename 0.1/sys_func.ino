// Скан пинов
void scan_pins(void) {
  if (millis() - scan_timer >= 100) {           // раз в 10 с читаем пины


#if POT_ADJ == 1
    set_temp = map(analogRead(POT_PIN), 0, 1023, LOW_TEMP, HIGH_TEMP);
#endif
#if BUT_ADJ == 1
    // ----- ПРОВЕРКА КНОПОК ----- //
    boolean _up   = !digitalRead(BUTTON_UP);
    boolean _down = !digitalRead(BUTTON_DOWN);
    boolean _set  = !digitalRead(BUTTON_SET);
    static boolean _up_pressed   = 0;
    static boolean _down_pressed = 0;
    static boolean _set_pressed  = 0;
    //______________________________________//
    if (_up && !_up_pressed) { // если нажали
      set_temp ++;             // увеличиваем темп-ру
      
      _up_pressed = 1;        // ставим заглушку
    }
    if (!_up && _up_pressed) { // если отжали
      _up_pressed = 0;        // убираем заглушку
    }
    //______________________________________//
    if (_down && !_down_pressed) { // если нажали
      set_temp--;               // уменьшаем темп-ру
      _down_pressed = 1;        // ставим заглушку
    }
    if (!_down && _down_pressed) { // если отжали
      _down_pressed = 0;        // убираем заглушку
    }
    //______________________________________//
    if (_set && !_set_pressed) { // если нажали

      _set_pressed = 1;        // ставим заглушку
    }
    if (!_set && _set_pressed) { // если отжали
      _up_pressed = 0;        // убираем заглушку
    }
    // ----- ПРОВЕРКА КНОПОК ----- //

    scan_timer = millis();         // обновление таймера
  }
#endif
  if (millis() - therm_timer >= 500) { // 2 раза в секунду читаем темп-ру
    temp = get_temp();               // читаем температуру
    therm_timer = millis();           // обновляем таймер
  }
}
