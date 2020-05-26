
/* (c) Crazy_Geeks, 2020
   https://vk.com/yasnolesskiy
*/

/*
    Проект: Умный обогреватель
	https://github.com/Crazy-Geeks/Heater
    Version: 0.1 (early alpha)
*/


// --- НАСТРОЙКИ --- //
#define DEBUG       1     // 1 - вкл / 0 - выкл

#define POT_ADJ     1     // регулировка потенциометром
#define BUT_ADJ     1     // регулировка кнопками 
#define RELAY_LOGIC 1     // лог. уровень, при котором реле открывается

#define RESISTOR 10000    // резистор, соединенный с термистором [Ом]
#define TEMP_NOM 25       // номинальная темп-ра термистора [*C]

#define LOW_TEMP  -10     // нижний предел температуры
#define HIGH_TEMP 30      // верхний предел температуры
#define POWER 1000        // мощность нагревателя [Вт]

#define MAX_DELTA 14      // критичная разница температур

// --- НАСТРОЙКИ --- //


// --- ПИНЫ --- //
#define RELAY_PIN   4   // пин реле
#define THERMO_PIN  5   // пин термистора
#define POT_PIN     A0  // пин резистора
#define BUTTON_UP   6   // кнопка вверх
#define BUTTON_DOWN 7   // кнопка вниз
#define BUTTON_SET  8   // кнопка SET

// --- ПИНЫ --- //


// --- БИБЛИОТЕКИ ---//
#include <U8g2lib.h> // библиотека дисплея
// --- БИБЛИОТЕКИ ---//


// --- ФУНКЦИИ --- //
void scan_pins(void);   // скан пинов
float get_temp(void);   // считывание температуры
void write_relay(boolean _st, uint8_t _duty); // управление реле

// --- ФУНКЦИИ --- //


// --- СИСТ ПЕРЕМЕННЫЕ --- //
uint32_t therm_timer;           // таймер температуры
uint32_t scan_timer;            // таймер скана кнопок
uint32_t pwm_timer;             // таймер для НЧ ШИМ
boolean up_st, down_st, set_st; // состояния кнопок
int set_temp;                   // переменная регулятора темп-ры
float temp;                     // текущая температура
int err;                        // ошибка для ПИ-регулятора
float p, i;                     // переменные ПИ-регулятора 
float pid;                      // сумма ПИ-регулятора 

// --- СИСТ ПЕРЕМЕННЫЕ --- //



void setup() {
  p = POWER / MAX_DELTA;        // считаем Вт/градус
  
#if DEBUG == 1                // если включен DEBUG
  Serial.begin(9600);         // запускаем Serial порт
#endif

  pinMode(RELAY_PIN, OUTPUT); 
  pinMode(THERMO_PIN, INPUT);

#if POT_ADJ == 1              // если включена настройка по потенциометру
  pinMode(POT_PIN, INPUT);
#endif
#if BUT_ADJ == 1              // если включена настройка по кнопкам
  pinMode(BUTTON_UP,   INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_SET,  INPUT_PULLUP);
#endif

#if POT_ADJ == 0 && BUT_ADJ == 0    // защита от дурака 
  Serial.begin(9600);
  Serial.println("ПРОВЕРЬТЕ НАСТРОЙКИ POT_ADJ И BUT_ADJ");
  pinMode(LED_BUILTIN, OUTPUT);
  boolean led_st = 0;
  while (true) {                    // входим в бесконечный цикл
    digitalWrite(LED_BUILTIN, led_st);
    delay(500);
    led_st = !led_st;
  }
#endif

  pwm_timer = millis();
}

void loop() {
  //scan_pins();

  write_relay (1, 50);

}




// Считывание температуры
float get_temp(void) {
  // Взято с: https://vk.cc/ak67bf
  // Формула Стейнхарта-Харта

  int _thermistor = RESISTOR;  // нормальное сопротивление
  int _therm_nominal = TEMP_NOM;  // номинальная температура
  int B = 3950;             // коэф термистора


  int _t = analogRead(THERMO_PIN);
  float tr = 1023.0 / _t - 1;
  tr = RESISTOR / tr;

  float _temp;
  _temp = tr / _thermistor;                 // R/R0
  _temp = log(_temp);                       // ln(R/R0)
  _temp /= B;                               // 1/B * ln(R/R0)
  _temp += 1.0 / (_therm_nominal + 273.15); // + 1/T0
  _temp = 1.0 / _temp;                      // Invert
  _temp -= 273.15;                          // перевод из кельвин в цельсий

  return _temp;   // Отдаём температуру
}
// конец float temp()


// Управление реле
void write_relay(boolean _st, uint8_t _duty) {
  // _st - управляющий сигнал
  // _duty - скважность (0-100)

  /* Таблица истинности
     x - _st; y - RELAY_LOGIC; z - то, что нужно подать
     x y z
     0 0 1  - нужно 0, подаём 1
     1 0 0  - нужно 1, подаём 0
     0 1 0  - нужно 0, подаём 0
     1 1 1  - нужно 1, подаём 1

     z = x == y
  */

  boolean _signal;       // переменная сигнада
  if (_st == RELAY_LOGIC) _signal = 1; // если попали в логику
  if (_st != RELAY_LOGIC) _signal = 0; // если не попали в логику

  /* Принцип НЧ ШИМ-а:
     всего 100 мс на цикл
     _duty - длина импульса
     Если _duty - 100, то сигнал постоянен (0 или 1)
  */

  if (_duty <= 99 && _st == 1) {
    if (millis() - pwm_timer < _duty) {              // пока таймер меньше duty - открываем реле
      digitalWrite(RELAY_PIN, (boolean)RELAY_LOGIC);
    }
    if (millis() - pwm_timer >= _duty) {             // если превысили duty - закрываем реле
      digitalWrite(RELAY_PIN, !(boolean)RELAY_LOGIC);
    }
    if (millis() - pwm_timer >= 100) {               // если превысили 100 мс (период) - обнуляемся
      pwm_timer = millis();
    }
  }

  if (_duty == 100) digitalWrite(RELAY_PIN, _signal);
}
