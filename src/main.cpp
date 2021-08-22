#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RotaryEncoder.h>

#define DHTPIN 4      // Датчик температура и влажности
#define DHTTYPE DHT22 // DHT 22  (AM2302)
#define PIN_RELAY_1 3 // Определяем пин, используемый для подключения реле
#define PIN_RELAY_2 5
#define PIN_BUTTON 2
#define STEPS 5
#define POSMIN 0
#define POSMAX 255
uint32_t delayMS;

// библиотека для энкодера
RotaryEncoder encoder(A2, A3); // пины подключение энкодера (DT, CLK)

DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2); // Указываем I2C адрес (наиболее распространенное значение), а также параметры экрана (в случае LCD 1602 - 2 строки по 16 символов в каждой

int rainPin = A0; //Датчик влажности почвы
// пороговое значение указывается тут:
int thresholdValue = 800;

static int pos = 0; // задаем начальное положение энкодера
int newPos;

void setup()
{
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_RELAY_1, OUTPUT);    // Объявляем пин реле как выход
  digitalWrite(PIN_RELAY_1, HIGH); // Выключаем реле - посылаем высокий сигнал
  pinMode(PIN_RELAY_2, OUTPUT);    // Объявляем пин реле как выход
  digitalWrite(PIN_RELAY_2, HIGH); // Выключаем реле - посылаем высокий сигнал
  pinMode(rainPin, INPUT);
  Serial.begin(9600);
  dht.begin();
  lcd.init();          // Инициализация дисплея
  lcd.backlight();     // Подключение подсветки
  lcd.setCursor(0, 0); // Установка курсора в начало первой строки
  lcd.print("Hello");  // Набор текста на первой строке
  lcd.setCursor(0, 1); // Установка курсора в начало второй строки
  lcd.print("");       // Набор текста на второй строке

} // Установка курсора в начало первой строки

void loop()
{

  // проверяем положение ручки энкодера
  encoder.tick();
  newPos = encoder.getPosition();

  // если положение изменилось - выводим на монитор
  if (pos != newPos)
  {
    Serial.println(newPos);
    pos = newPos;
  };
  Serial.println("--------------------");
  // включаем второй светодиод с помощью нажатия кнопки
  if (digitalRead(2) == HIGH)
  {
    Serial.print("НЕ НАЖАТА");
    Serial.print("НАЖАТА");
    lcd.setCursor(0, 0); // Установка курсора в начало первой строки
    lcd.print("WAIT WAIT WAIT");
  }
  if (digitalRead(2) == LOW)
  {
    Serial.print("НАЖАТА");
    lcd.setCursor(0, 0); // Установка курсора в начало первой строки
    lcd.print("FUCK OFF        ");
  };

  Serial.println("--------------------");
  // Wait a few seconds between measurements.
  delay(300);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  //int sensorValue = analogRead(rainPin);
  Serial.println("Температура воздуха: ");
  Serial.print(t);
  Serial.println(" *C ");
  Serial.println("--------------------");
  lcd.setCursor(0, 1);
  lcd.print("Temp=");
  lcd.setCursor(5, 1);
  lcd.print(t);
  lcd.setCursor(11, 1);
  lcd.print("*C");
  delay(1000);
}
