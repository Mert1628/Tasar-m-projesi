#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Wire.h>


Servo motor;

int ileri = 2;
int geri = 3;
int trig = 8;
int echo = 9;
int buzzer = 13;

int zaman;
float mesafe;
bool parkModu = false; // Park modunu takip etmek için durum değişkeni

// I2C LCD için adres tanımı: Genelde 0x27 veya 0x3F kullanılır.
LiquidCrystal_I2C lcd(0x27, 16, 2);

void buzzerCalistir() {
  if (mesafe <= 5) {
    tone(buzzer, 1000); // Çok hızlı uyarı sesi
    delay(500);
    noTone(buzzer);
  } else if (mesafe > 5 && mesafe <= 10) {
    tone(buzzer, 800); // Orta hızlı uyarı sesi
    delay(1000);
    noTone(buzzer);
  } else if (mesafe > 10 && mesafe <= 15) {
    tone(buzzer, 600); // Yavaş uyarı sesi
    delay(1500);
    noTone(buzzer);
  } else {
    noTone(buzzer); // Buzzer kapalı
  }
}

void fonksiyon1() {
  parkModu = true; // Park modu etkinleştirildi
  while (mesafe <= 15) {
    lcd.clear();
    buzzerCalistir(); // Yalnızca park sırasında buzzer çalışır

    // Park hareketleri
    digitalWrite(ileri, LOW);
    digitalWrite(geri, HIGH);
    motor.write(0); // Servo motoru başlangıç pozisyonuna ayarla
    delay(1000);

    digitalWrite(ileri, HIGH);
    digitalWrite(geri, LOW);
    motor.write(180); // Diğer yöne döndür
    delay(1000);

    motor.write(90); // Servo motoru ortada dur
    delay(500);
    break; // Park işlemi tamamlandığında döngüden çık
  }
  parkModu = false; // Park modu kapatıldı
}

void setup() {
  Serial.begin(9600);
  motor.attach(6);

  pinMode(ileri, OUTPUT);
  pinMode(geri, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(buzzer, OUTPUT);

  Wire.begin();
  // I2C LCD başlatılıyor
  lcd.init();
  lcd.backlight();
  lcd.clear(); // LCD ekranını temizle
  lcd.setCursor(0, 0); // 1. satırın başına imleci ayarla
  lcd.print(" OTOMATIK PARK");
  lcd.setCursor(0, 1); // 2. satırın başına imleci ayarla
  lcd.print("     SISTEMI  ");
  
}

void loop() {
  // Mesafe ölçümü
  digitalWrite(trig, LOW);
  delay(2);
  digitalWrite(trig, HIGH);
  delay(10);
  digitalWrite(trig, LOW);

  zaman = pulseIn(echo, HIGH);
  mesafe = (zaman / 2) / 28.7;


  if (!parkModu && mesafe <= 15) { 
    // Park modu etkin değil ve mesafe uygunsa park etme fonksiyonunu çağır
    fonksiyon1();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MESAFE: ");
    lcd.print(mesafe);
    lcd.print(" CM");
  } else if (!parkModu) {
    // Park modu aktif değilken araç düz hareket eder
    motor.write(90);
    digitalWrite(ileri, HIGH);
    digitalWrite(geri, LOW);
    
  	
  }

  delay(1000);
}
