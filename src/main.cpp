#include <Arduino.h>
#include <Homie.h>

#define firmwareVersion "1.0.0";

#define ENA 5
#define IN1 4
#define IN2 2

int dimmer1=50;
int dimmer2=50;
int delayMs=5;

boolean ledSwitch=false;
boolean pinState=false;

HomieNode lightNode("light", "Light", "switch");

bool lightOnHandler(const HomieRange& range, const String& value) {
  if (value != "true" && value != "false") return false;

  ledSwitch = (value == "true");

  lightNode.setProperty("on").send(value);
  Homie.getLogger() << "Light is " << (ledSwitch ? "on" : "off") << endl;

  return true;
}

bool dimmerHandler(const HomieRange& range, const String& value) {
  for (byte i = 0; i < value.length(); i++) {
    if (isDigit(value.charAt(i)) == false) return false;
  }
  dimmer1 = value.toInt();
  if (dimmer1 > 1023) dimmer1=1023;
  dimmer2 = dimmer1;
  //digitalWrite(PIN_RELAY, on ? HIGH : LOW);
  lightNode.setProperty("dimmer").send(value);
  lightNode.setProperty("dimmer1").send(value);
  lightNode.setProperty("dimmer2").send(value);
  Homie.getLogger() << "Dimmer is " << (dimmer1) << endl;

  return true;
}

bool dimmer1Handler(const HomieRange& range, const String& value) {
  for (byte i = 0; i < value.length(); i++) {
    if (isDigit(value.charAt(i)) == false) return false;
  }
  dimmer1 = value.toInt();
  if (dimmer1 > 1023) dimmer1=1023;
  //digitalWrite(PIN_RELAY, on ? HIGH : LOW);
  lightNode.setProperty("dimmer1").send(value);
  Homie.getLogger() << "Dimmer1 is " << (dimmer1) << endl;

  return true;
}


bool dimmer2Handler(const HomieRange& range, const String& value) {
  for (byte i = 0; i < value.length(); i++) {
    if (isDigit(value.charAt(i)) == false) return false;
  }
  dimmer2 = value.toInt();
  if (dimmer2 > 1023) dimmer2=1023;
  //digitalWrite(PIN_RELAY, on ? HIGH : LOW);
  lightNode.setProperty("dimmer2").send(value);
  Homie.getLogger() << "Dimmer2 is " << (dimmer2) << endl;

  return true;
}

bool delayHandler(const HomieRange& range, const String& value) {
  for (byte i = 0; i < value.length(); i++) {
    if (isDigit(value.charAt(i)) == false) return false;
  }
  delayMs = value.toInt();
  if (delayMs > 10) delayMs=10;
  //digitalWrite(PIN_RELAY, on ? HIGH : LOW);
  lightNode.setProperty("delay").send(value);
  Homie.getLogger() << "delay is " << (delayMs) << endl;

  return true;
}

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  Serial.begin(115200);
  Serial << endl << endl;
  Homie_setBrand("Christmas-Tree");
  Homie_setFirmware("Christmas", "0.0.1"); // The underscore is not a typo! See Magic bytes
  lightNode.advertise("on").settable(lightOnHandler);
  lightNode.advertise("on").setName("Switch");
  lightNode.advertise("on").setDatatype("boolean");

  lightNode.advertise("dimmer").settable(dimmerHandler);
  lightNode.advertise("dimmer").setName("Dimmer");
  lightNode.advertise("dimmer").setDatatype("integer");
  lightNode.advertise("dimmer").setFormat("0:1023");

  lightNode.advertise("dimmer1").settable(dimmer1Handler);
  lightNode.advertise("dimmer1").setName("Dimmer-1");
  lightNode.advertise("dimmer1").setDatatype("integer");
  lightNode.advertise("dimmer1").setFormat("0:1023");

  lightNode.advertise("dimmer2").settable(dimmer2Handler);
  lightNode.advertise("dimmer2").setName("Dimmer-2");
  lightNode.advertise("dimmer2").setDatatype("integer");
  lightNode.advertise("dimmer2").setFormat("0:1023");

  lightNode.advertise("delay").settable(delayHandler);
  lightNode.advertise("delay").setName("Delay");
  lightNode.advertise("delay").setDatatype("integer");
  lightNode.advertise("delay").setFormat("0:10");

  Homie.setup();
}

void loop() {
  Homie.loop();
  if (ledSwitch) {
    if (pinState) {
      analogWrite(ENA, dimmer1);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
    } else {
      analogWrite(ENA, dimmer2);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
    }
    pinState=!pinState;
    delay(delayMs);
  } else {
    analogWrite(ENA, 0);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
}