// === Inisialisasi pin ===
const int led1 = 8;    // Blink otomatis
const int led2 = 9;    // Blink via tombol 1
const int led3 = 10;   // On/off via tombol 2
const int led4 = 11;   // Dikontrol potensiometer
const int button1 = 2; // Tombol 1
const int button2 = 3; // Tombol 2
const int potPin = A1; // Potensiometer

// Variabel bantu
bool led1State = false;
bool led2Blinking = false; // status: LED2 lagi blinking atau tidak
bool led3State = false;

unsigned long previousMillis1 = 0; // untuk LED1
unsigned long previousMillis2 = 0; // untuk LED2
const long interval1 = 500;        // interval blink LED1
const long interval2 = 300;        // interval blink LED2

// Timer untuk update Serial Monitor tiap 1 detik
unsigned long previousPrintMillis = 0;
const long printInterval = 1000;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("=== Program LED dan Potensiometer Aktif ===");
  Serial.println("===========================================");
}

void loop() {
  unsigned long currentMillis = millis();

  // === LED1 blink otomatis tiap 500 ms ===
  if (currentMillis - previousMillis1 >= interval1) {
    previousMillis1 = currentMillis;
    led1State = !led1State;
    digitalWrite(led1, led1State);
  }

  // === Tombol 1 (aktif/nonaktifkan mode blink LED2) ===
  static bool lastButton1 = HIGH;
  bool currentButton1 = digitalRead(button1);

  if (lastButton1 == HIGH && currentButton1 == LOW) { // ditekan
    led2Blinking = !led2Blinking; // ubah status blink
    if (!led2Blinking) {
      digitalWrite(led2, LOW); // pastikan mati saat berhenti blink
    }
    delay(200); // debounce
  }
  lastButton1 = currentButton1;

  // === LED2 blink hanya jika mode aktif ===
  if (led2Blinking && (currentMillis - previousMillis2 >= interval2)) {
    previousMillis2 = currentMillis;
    digitalWrite(led2, !digitalRead(led2)); // toggle LED2
  }

  // === Tombol 2 (toggle LED3 biasa) ===
  static bool lastButton2 = HIGH;
  bool currentButton2 = digitalRead(button2);

  if (lastButton2 == HIGH && currentButton2 == LOW) {
    led3State = !led3State;
    digitalWrite(led3, led3State);
    delay(200);
  }
  lastButton2 = currentButton2;

  // === Potensiometer untuk LED4 ===
  int potValue = analogRead(potPin);
  int brightness = map(potValue, 0, 1023, 0, 255);
  analogWrite(led4, brightness);

  // === Kirim status ke Serial Monitor tiap 1 detik ===
  if (currentMillis - previousPrintMillis >= printInterval) {
    previousPrintMillis = currentMillis;

    Serial.println("====================================");
    Serial.print("LED1 : ");
    Serial.println(led1State ? "Blinking" : "Off");

    Serial.print("LED2 : ");
    if (led2Blinking) Serial.println("Blink Aktif");
    else Serial.println("Mati / Tidak Blink");

    Serial.print("LED3 : ");
    Serial.println(led3State ? "Nyala" : "Mati");

    Serial.print("LED4 : Kecerahan = ");
    Serial.println(brightness);

    Serial.print("Nilai Potensiometer = ");
    Serial.println(potValue);
    Serial.println("====================================\n");
  }
}
