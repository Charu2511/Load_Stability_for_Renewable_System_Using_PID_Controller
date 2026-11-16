#include <LiquidCrystal.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// LCD pins: RS, E, D4–D7, BL+, BL–, RW, unused
LiquidCrystal lcd(38, 36, 22, 23, 24, 25, 26, 27, 28, 29);

const int voltagePin = A0;
const int currentPin = A1;
const int relayPin   = 7;    // LOW = charging, HIGH = discharging

// Voltage divider constants
const float R1 = 30000.0, R2 = 7500.0, Vref = 5.00;
// ACS712-30A sensitivity
const float ACS_SENSITIVITY = 0.066f;
float offsetCounts = 0.0f;    // calibrated zero offset

// Timing (ms)
const unsigned long CHARGE_TIME_MS      = 30000;
const unsigned long DISCHARGE_TIME_MS   = 30000;
const unsigned long DISPLAY_INTERVAL_MS = 500;

// Dead-time ticks and duty values
const uint16_t DEADTIME       = 16;   // ~0.64 µs dead-time
const uint16_t DUTY_CHARGE    = (639UL * 945) / 1000;  // ~604 (94.5%)
const uint16_t DUTY_DISCHARGE = (639UL * 900) / 1000;  // ~575 (90%)

uint8_t      relayState     = 0;   // 0=Charge, 1=Discharge
unsigned long stateTs       = 0;
unsigned long lastDisplayTs = 0;

void setup() {
  Serial.begin(115200);

  // Drive LCD back-light & RW
  pinMode(26, OUTPUT);  // BL+
  pinMode(27, OUTPUT);  // BL–
  pinMode(28, OUTPUT);  // RW
  digitalWrite(26, HIGH);
  digitalWrite(27, LOW);
  digitalWrite(28, LOW);

  lcd.begin(16, 2);
  lcd.noBlink();

  // Force OC3A pin (D5) as output for Timer3 PWM
  pinMode(5, OUTPUT);

  // Start in charging state
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  // Initial LCD state display
  lcd.setCursor(0, 0);
  lcd.print("State: Charging");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");

  // Calibrate ACS712 zero-offset
  {
    float sum = 0;
    for (int i = 0; i < 500; i++) {
      sum += analogRead(currentPin);
      delay(2);
    }
    offsetCounts = sum / 500.0f;
    Serial.print("OffsetCounts = ");
    Serial.println(offsetCounts, 2);
  }
  analogReference(DEFAULT);

  // Timer3 PWM + dead-time setup
  DDRE |= (1<<PE3)|(1<<PE4);
  cli();
    TCCR3A = (1<<COM3A1)|(1<<WGM31);        // non-inverting OC3A, Fast PWM Mode 14
    TCCR3B = (1<<WGM33)|(1<<WGM32)|(1<<CS30);
    ICR3   = 639;                            // 25 kHz base
    OCR3A  = DUTY_CHARGE;                    // start at 94.5%
    OCR3B  = OCR3A + DEADTIME;               // apply reduced dead-time
    OCR3C  = ICR3 - DEADTIME;
    TIMSK3 = (1<<OCIE3B)|(1<<OCIE3C);        // enable dead-time ISRs
  sei();

  stateTs       = millis();
  lastDisplayTs = millis() - DISPLAY_INTERVAL_MS;
}

void loop() {
  unsigned long now = millis();

  // State toggle every 30 s
  if (relayState == 0 && now - stateTs >= CHARGE_TIME_MS) {
    relayState = 1;
    stateTs    = now;
    digitalWrite(relayPin, HIGH);
    cli();
      OCR3A  = DUTY_DISCHARGE;
      OCR3B  = OCR3A + DEADTIME;
      OCR3C  = ICR3 - DEADTIME;
    sei();
    Serial.print("→ DISCHARGE, OCR3A="); Serial.println(OCR3A);
  }
  else if (relayState == 1 && now - stateTs >= DISCHARGE_TIME_MS) {
    relayState = 0;
    stateTs    = now;
    digitalWrite(relayPin, LOW);
    cli();
      OCR3A  = DUTY_CHARGE;
      OCR3B  = OCR3A + DEADTIME;
      OCR3C  = ICR3 - DEADTIME;
    sei();
    Serial.print("→ CHARGE, OCR3A="); Serial.println(OCR3A);
  }

  // Measurements
  int rawV = analogRead(voltagePin);
  float Vin = (rawV * (Vref/1023.0)) * ((R1+R2)/R2);

  float sumC = 0;
  for (int i = 0; i < 20; i++) sumC += analogRead(currentPin);
  float current = ((sumC/20.0 - offsetCounts)*(Vref/1023.0)) / ACS_SENSITIVITY;
  float magI    = fabs(current);

  // LCD refresh every 500 ms
  if (now - lastDisplayTs >= DISPLAY_INTERVAL_MS) {
    lastDisplayTs = now;
    lcd.setCursor(0, 0);
    lcd.print(relayState==0 ? "State: Charging " : "State: Discharge");
    lcd.setCursor(0, 1);
    lcd.print(Vin,2); lcd.print("V ");
    lcd.print(relayState==0 ? '+' : '-');
    lcd.print(magI,1); lcd.print("A ");
  }

  delay(10);
}

// Dead-time ISRs (toggle PE4 = Arduino D2)
ISR(TIMER3_COMPB_vect) { PORTE |=  (1<<PE4); }
ISR(TIMER3_COMPC_vect) { PORTE &= ~(1<<PE4); }
