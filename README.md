# _Sistema de Riego Automatizado - Equipo 5_

**** Esta versión del proyecto contiene bugs, debido a la falta de implementación de funciones Thread-safe ****

The project is a Greenhouse automated irrigation system based on the ESP32 and the official ESP-IDF development framerwork. It works through a closed-loop control system with feedback from ground humidity and water flow sensors, while actuation is done with solenoid valves to allow water flow based on the established humidity limit, which is defined by default in the program, but can be easily changed through the User Interface in the NodeRed platform.

The User Interface is implemented in NodeRed with a local wifi network in the Greenhouse and utilizes the MQTT protocol for communications between the server and the client (ESP32). Through this, variables like water flow and humidity can be monitored, valves can be manually opened or closed, humidity thresholds can be established for Automatic mode, or time intervals can be set for time-based irrigation. The UI is the following:

<img width="1128" alt="nodered_ui" src="https://user-images.githubusercontent.com/87957114/144632451-141f0767-4ce2-47ec-99ed-3d90f93cc7d0.png">

Additionally, variables can be monitored in-field through the SSD1306-based OLED display, which is located in the control box. To avoid the display being always on and save energy, the screen is turned on with a button to indicate someone wants to see it and automatically turns off after a set amount of time.

El programa soporta actualmente dos lineas de riego, pero la estructura del programa permite quitar o poner más lineas de riego de manera sencilla. Para esto, se deben agregar los cambios correspondientes en usr_adc, HAL_ech2o_ec5 y APP_monitoring, ademas de los demas archivos de aplicación en los que se quiera utilizar. 


## Estructura del Programa
El programa también esta estructurado dentro de un modelo de capas, que empieza por los drivers de los periféricos del microcontrolador, luego la capa de abstracción de hardware, donde se definen librerias propias para los dispositivos de hardware utilizados y finalmente la aplicación. La estructura de archivos es la siguiente:

```
├── App
│   ├── inc
│   └── src 
├── Peripherals
│   ├── usr_adc
│   ├── usr_gpio
│   ├── usr_i2c
│   ├── usr_mqtt
│   ├── usr_timer
│   └── usr_pcnt
├── User_HAL
│   ├── HAL_ech2o_ec5
│   ├── HAL_flow_sensor
│   ├── HAL_OLED_display
│   └── HAL_solenoid_valve
└── README.md                  This is the file you are currently reading
```
La lógica general del código es la que se muestra en este diagrama de flujo:

<img width="1143" alt="Screen Shot 2021-12-02 at 10 35 56" src="https://user-images.githubusercontent.com/87957114/144654226-189b6eed-1c12-4541-99d1-14ce94090045.png">

## Images of the project

<p float="left">
  <img width="210" height="300" alt="inv1" src="https://user-images.githubusercontent.com/87957114/144634263-2a2472b4-2ebd-45b0-baa8-a117880d4c79.png" />
  <img width="400" height="300" alt="inv2" src="https://user-images.githubusercontent.com/87957114/144634292-6ef19086-cee0-467b-80e1-c1db5560740f.png" />
  <img width="210" height="300" alt="caja" src="https://user-images.githubusercontent.com/87957114/144634327-b0df1801-1c2e-4808-8500-4af789ff3822.png" />
</p>
