#include "simple_mqtt.h"

/*********
  Rui Santos [updated]
  Complete project details at https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/
*********/

#include <WiFi.h>
#include <PubSubClient.h>

// kita membuat client object untuk wifi
WiFiClient espClient;
// membuat client object untuk mqtt
PubSubClient client(MQTT_SERVER, MQTT_PORT, callback, espClient);

void setup()
{
  /// macro untuk begin serial
  DEBUG_INIT(115200);

  setup_wifi();

  /// change mode each pin to output
  for (size_t i = 0; i < JUMLAH_LED; i++)
  {
    pinMode(ledPin[i], OUTPUT);
    digitalWrite(ledPin[i], 0);
  }
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

unsigned long millis_done = 0;
void loop()
{
  while (!client.connected())
    reconnect_mqtt_server();
  if (client.loop())
  {
    auto mill = millis();
    if ((mill - millis_done) >= 2000UL)
    {
      char buff[8];
      utoa(baca_jarak_hcsr(), buff, 10);
      client.publish("MQTT_YUTUB_ESP32/jarak", (const uint8_t *)buff, sizeof(buff), false);
      
      millis_done = mill;
    }
  }
}

void setup_wifi()
{
  /// F() adalah untuk string literal
  PRINT(F("Connecting to "));
  PRINTLN(ssid);

  WiFi.begin(SSID_NAME, SSID_PASWORD);
  WiFi.begin();

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    PRINT(F("."));
  }

  PRINTLN();
  PRINTLN(F("WiFi connected"));
  PRINTLN(F("IP address: "));
  PRINTLN(WiFi.localIP());
}

void callback(char *topic, byte *message, unsigned int length)
{
  /// check apakah topic kosong dan memenuhi kriteria topic yang digunakan
  /// jika kosong dan tidak jelas, skip

  if ((strncmp(topic, "MQTT_YUTUB_ESP32/led_input", 26)) == 0)
  {
    char *msg = (char *)malloc(length);
    memcpy(msg, message, length);
    uint8_t status_led[JUMLAH_LED] = {0, 0, 0, 0, 0, 0};
    if (!(sscanf(msg, "[%d,%d,%d,%d,%d,%d]",
                 &status_led[0],
                 &status_led[1],
                 &status_led[2],
                 &status_led[3],
                 &status_led[4],
                 &status_led[5]) == JUMLAH_LED))
      return;
    PRINT(F("msg arrived no topic: "));
    PRINTLN(topic);
    PRINT(F("status: "));
    PRINTLN(msg);

    for (size_t i = 0; i < JUMLAH_LED; i++)
    {
      digitalWrite(ledPin[i], status_led[i]);
    }
    client.publish("MQTT_YUTUB_ESP32/led_output", msg, length);
    free(msg);
  }
}

void reconnect_mqtt_server()
{
  // Loop until we're reconnected
  PRINT(F("mencoba menyambungkan pada MQTT server..."));
  // Attempt to connect
  if (client.connect("ESP32_YUTUB_IOT"))
  {
    PRINTLN(F("tersambung!"));
    // Subscribe
    client.subscribe("MQTT_YUTUB_ESP32/led_input");
    client.publish("MQTT_YUTUB_ESP32/led_output", "[0,0,0,0,0,0]");
  }
  else
  {
    PRINT(F("gagal tersambung pada MQTT server!, rc="));
    PRINT(client.state());
    PRINTLN(F(" try again in 5 seconds"));
    // Wait 5 seconds before retrying
    delay(5000);
  }
}

unsigned int baca_jarak_hcsr()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = micros();
  WAIT_READ_PIN(ECHO_PIN, duration);
  duration = micros();
  WAIT_READ_PIN(ECHO_PIN, duration);

  return (unsigned int)((micros() - duration) / 57);
}
