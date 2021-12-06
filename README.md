# _Sistema de Riego Automatizado - Equipo 5_

El proyecto realizado es la automatización de un sistema de riego para un invernadero basado en el ESP32 con su framework oficial (ESP-IDF). Funciona mediante un sistema de control con retroalimentación por parte de sensores de humedad de suelo y electroválvulas para activar el riego a partir del límite de humedad establecido, el cual se define por default en el programa o se puede cambiar más tarde manualmente en la interfaz de usuario en la plataforma de NodeRed.

NodeRed esta implementado a traves de una red wifi local en el invernadero, y se utiliza el protocolo de MQTT para la comunicación entre el servidor y el cliente (ESP32). Con esto se pueden monitorear variables, abrir o cerrar valvulas, establecer límites de humedad para cada línea de riego y activar riego por tiempos, ingresando los minutos de riego. La interfaz es la siguiente:

<img width="1128" alt="nodered_ui" src="https://user-images.githubusercontent.com/87957114/144632451-141f0767-4ce2-47ec-99ed-3d90f93cc7d0.png">


Adicionalmente, las variables se pueden monitorear mediante el display OLED basado en el chip SSD1306 que se encuentra en la caja de control. Para evitar que el display este prendido siempre aunque nadie lo este viendo y ahorrar energía, la pantalla se prende con un botón para indicar que alguien lo quiere ver y se apaga automaticamente despues de un tiempo (dependiendo del valor de la alarma del Timer 0, Grupo 0).

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

## Imágenes del Proyecto

<p float="left">
  <img width="210" height="300" alt="inv1" src="https://user-images.githubusercontent.com/87957114/144634263-2a2472b4-2ebd-45b0-baa8-a117880d4c79.png" />
  <img width="400" height="300" alt="inv2" src="https://user-images.githubusercontent.com/87957114/144634292-6ef19086-cee0-467b-80e1-c1db5560740f.png" />
  <img width="210" height="300" alt="caja" src="https://user-images.githubusercontent.com/87957114/144634327-b0df1801-1c2e-4808-8500-4af789ff3822.png" />
</p>
