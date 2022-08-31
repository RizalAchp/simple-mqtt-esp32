# BELAJAR IOT BARENG!

### UNTUK FILE NODE RED
- download file [SimpleNode-Red-Episode5.json](./SimpleNode-Red-Episode5.json)
- buka node-red kalian
- **DRAG-N-DROP** file json tersebut, pada web node-red kalian

### clone repo ini, caranya:
```shell
git clone https://github.com/RizalAchp/simple-mqtt-esp32.git

### dan buka folder project pada visual studio code
```shell
code simple-mqtt-esp32
```

### ubah configurasi wifi dan mqtt pada file [platformio.ini](./platformio.ini)

### enjoy!


### TODO LIST!

- [x] membuat program untuk ESP32, mengatur hidup dan mati nya LED dan mengukur jarak
	- subscribe(`MQTT_ESP32_YUTUB/led_input`)
		> menerima payload kontrol hidup/mati led pada client controller
	- publish(`MQTT_ESP32_YUTUB/led_output`)
		> kirim status hidup/mati led ke client controller
	- publish(`MQTT_ESP32_YUTUB/jarak`)
		> kirim bacaan jarak sensor jarak ke client controller

- [x] membuat nodered (web base) menggunakan NodeJS
	- subscribe(`MQTT_ESP32_YUTUB/led_output`)
		> menerima payload status hidup/mati led dari ESP32
	- subsclibe(`MQTT_ESP32_YUTUB/jarak`)
		> menerima payload jarak sensor dari ESP32
	- publish(`MQTT_ESP32_YUTUB/led_input`)
		> kirmi status hidup/mati led sensor dari event click pada web

- [ ] (TODO!) membuat android client?
- [ ] (TODO!) membuat IOT Apps dengn menggunakan `AI / MachineLearning?` lol!, maybe..
