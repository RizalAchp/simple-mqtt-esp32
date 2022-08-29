#pragma once
#ifndef __SIMPLE_MQTT_HEADER__
#define __SIMPLE_MQTT_HEADER__

#include <Arduino.h>

///  ini hanya macro agar saat debug tidak aktif, serial tidak digunakan
#ifdef DEBUG
#define DEBUG_INIT(_BAUD) Serial.begin(_BAUD)
#define PRINT(...) Serial.print(__VA_ARGS__)
#define PRINTLN(...) Serial.println(__VA_ARGS__)
#define PRINTF(...) Serial.printf(__VA_ARGS__)
#endif
#ifdef NDEBUG
#define DEBUG_INIT(_BAUD)
#define PRINT(_OBJ)
#define PRINTLN(_OBJ)
#define PRINF(...)
#endif

#define ECHO_PIN 22
#define TRIG_PIN 23
#define TIME_OUT 3000
#define CM_VECTOR 57
#define WAIT_READ_PIN(_PIN, _DUR) while (!digitalRead(_PIN) && (micros() - _DUR ) <= TIME_OUT)

#if !defined(SSID_NAME) && !defined(SSID_PASSWORD)
// ssid dan password wifi yang akan tersambung dengan esp32 ini
constexpr const char *ssid = "Jember Wifi";
constexpr const char *password = "MuhammadSAW";
#endif
#if !defined(MQTT_SERVER) && !defined(MQTT_PORT)
/// Add your MQTT Broker IP address, contoh jika menggunakan broker gratis: ///
// constexpr const char* mqtt_server = "broker.mqtt-dashboard.com";
/// disini saya menggunakan broker sendiri pada server local saya ///
constexpr const char *MQTT_SERVER = "192.168.1.7";
constexpr uint16_t  MQTT_PORT = 1883;
#endif

// LED Pin
// saya menggunakan LED sebagai contoh simple
// konstan variable { tidak dapat diubah }
#define JUMLAH_LED 6
constexpr uint8_t ledPin[JUMLAH_LED] = {4, 16, 17, 5, 18, 19};

/// untuk setup client wifi pada esp32
void setup_wifi();
/// sebagai callback payload message yang didapatkan pada mqtt server
void callback(char *topic, byte *message, unsigned int length);
/// helper function untuk reconnect client mqtt server pada esp32
void reconnect_mqtt_server();

unsigned int baca_jarak_hcsr();

#endif