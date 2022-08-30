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

int status_led[JUMLAH_LED] = {0, 0, 0, 0, 0, 0};

void setup()
{
  /// macro untuk begin serial
  DEBUG_INIT(115200);

  /// change mode each pin to output
  for (size_t i = 0; i < JUMLAH_LED; i++)
  {
    pinMode(ledPin[i], OUTPUT);
    digitalWrite(ledPin[i], 0);
  }
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  setup_wifi();

  if (!client.connected())
    reconnect_mqtt_server();
}

unsigned long millis_done = 0;
void loop()
{
  if (client.loop())
  {
    auto mill = millis();
    if ((mill - millis_done) >= 2000UL)
    {
      char buff[8];
      utoa(baca_jarak_hcsr(), buff, 10);
      client.publish("MQTT_YUTUB_ESP32/jarak", (const uint8_t *)buff, sizeof(buff), false);

      char buff2[20];
      snprintf(buff2, sizeof(buff2), "[%d,%d,%d,%d,%d,%d]",
               status_led[0], status_led[1], status_led[2],
               status_led[3], status_led[4], status_led[5]);
      client.publish("MQTT_YUTUB_ESP32/led_output", (const uint8_t *)buff2, sizeof(buff2), false);

      millis_done = mill;
    }
  }
  else
  {
    reconnect_mqtt_server();
  }
}

void setup_wifi()
{
  /// F() adalah untuk string literal
  PRINT(F("Connecting to "));
  PRINTLN(SSID_NAME);

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
    if (sscanf((char *)message, "[%d,%d,%d,%d,%d,%d]",
               &status_led[0], &status_led[1], &status_led[2],
               &status_led[3], &status_led[4], &status_led[5]) == JUMLAH_LED)
    {
      PRINT(F("msg arrived no topic: "));
      PRINTLN(topic);

      for (size_t i = 0; i < JUMLAH_LED; i++)
      {
        digitalWrite(ledPin[i], status_led[i]);
      }
    }
  }
}

void reconnect_mqtt_server()
{
  while (!client.connected())
  {
    PRINT(F("mencoba menyambungkan pada MQTT server..."));
    // Attempt to connect
    if (client.connect("ESP32Client-69420"))
    {
      PRINTLN(F("tersambung!"));
      // Subscribe
      client.subscribe("MQTT_YUTUB_ESP32/led_input");
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
}

unsigned int baca_jarak_hcsr()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = micros();
  WAIT_READ_PIN(!, ECHO_PIN, duration);
  duration = micros();
  WAIT_READ_PIN(, ECHO_PIN, duration);

  return (unsigned int)((micros() - duration) / 56);
}
