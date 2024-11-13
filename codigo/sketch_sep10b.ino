#include <PubSubClient.h>
#include <WiFi.h>
#include <Arduino.h>

#define LIGHT_SENSOR_PIN 36  // ESP32 pin GIOP36 (ADC0)
#define PINO_DIM 27
#define PINO_ZC 26
#define TRIGGER_TRIAC_INTERVAL 20  // tempo quem que o triac fica acionado
#define IDLE -1

const char* mqtt_server = "pf-bvsa63lsmsu362vuiixd.cedalo.cloud";

int pwm = 0;

WiFiClient espClient;
PubSubClient client(espClient);

int luminosidade = 0;
int brilho = 0;


void setup_wifi() {
  const char* ssid = "Torre_do_Mago";       // The SSID (name) of the Wi-Fi network you want to connect to
  const char* password = "*Coffeetime16h";  // The password of the Wi-Fi network

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("Esp32IDD", "Esp32", "123")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("Published Topics/Sensor-value");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void zeroCross() {
  if (luminosidade > 100) luminosidade = 100;
  if (luminosidade < 0) luminosidade = 0;
  long t1 = 8200L * (100L - luminosidade) / 100L;
  Serial.println(t1);
  delayMicroseconds(t1);
  digitalWrite(PINO_DIM, HIGH);
  delayMicroseconds(6);  // t2
  digitalWrite(PINO_DIM, LOW);
}

void setup() {
  Serial.begin(115200);
  // set the ADC attenuation to 11 dB (up to ~3.3V input)
  analogSetAttenuation(ADC_11db);
  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(PINO_DIM, OUTPUT);
  attachInterrupt(0, zeroCross, RISING);
  delay(2000);
}

void loop() {
  // Serial.println(brilho);

  bool leitura_up = 1;

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int analogValue = analogRead(LIGHT_SENSOR_PIN);

  pwm = map(analogValue, 0, 4096, 0, 255);
  Serial.println(analogValue);

  char cstr[16];
  const char* analogValue_string = itoa(analogValue, cstr, 10);
  if (analogValue > 0) {
    client.publish("Published Topics/Sensor-value", analogValue_string);
    
    if (analogValue < 40) {
      luminosidade = 0;
      client.publish("Published Topics/classificacao da luz", " => Very bright");
      Serial.print(" => Very bright");
    } else if (analogValue < 800) {
      luminosidade = 25;
      client.publish("Published Topics/classificacao da luz", " => Bright");
      Serial.print(" => Bright");
    } else if (analogValue < 2000) {
      luminosidade = 50;
      client.publish("Published Topics/classificacao da luz", " => Light");
      Serial.print(" => Light");
    } else if (analogValue < 3200) {
      luminosidade = 70;
      client.publish("Published Topics/classificacao da luz", " => Dim");
      Serial.print(" => Dim");
    } else {
      luminosidade = 90;
      client.publish("Published Topics/classificacao da luz", " => Dark");
      Serial.print(" => Dark");
    }
  }
}