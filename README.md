# Project
Get current weather by city using OpenWeatherMap.org and the current datetime in GTM and display these informations on the LCD display. The information is auto refreshed for every 60 seconds.

<img src="https://i.imgur.com/ocrCnBj.jpg" width="500">

## Requirements
- Arduino MKR1000
- 1602A LCD Display 5V 16x2

## Libraries
- SPI.h
- WiFi101.h
- WiFiUdp.h
- RTCZero.h
- ArduinoJson.h
- LiquidCrystal.h

## How it works
OpenWeatherMap.org is used to get the current weather data by city. You have to sign up and create API key which will be used on the request.

https://openweathermap.org/api

The respond from the API is a data object which can be parsed in JSON, XML or HTML format. In the project, we use JSON format.

```json
{
   "coord":{
      "lon":-5.98,
      "lat":37.38
   },
   "weather":[
      {
         "id":802,
         "main":"Clouds",
         "description":"scattered clouds",
         "icon":"03n"
      }
   ],
   "base":"stations",
   "main":{
      "temp":285.62,
      "feels_like":284.58,
      "temp_min":284.15,
      "temp_max":286.48,
      "pressure":1007,
      "humidity":93
   },
   "visibility":10000,
   "wind":{
      "speed":2.1,
      "deg":70
   },
   "clouds":{
      "all":40
   },
   "dt":1585597401,
   "sys":{
      "type":1,
      "id":6444,
      "country":"ES",
      "sunrise":1585548709,
      "sunset":1585593866
   },
   "timezone":7200,
   "id":2510911,
   "name":"Seville",
   "cod":200
}
```
Enter your sensitive data in arduino_secrets.h according to your information

```
#define SECRET_SSID "";  // Your network SSID (name)
#define SECRET_PASS "";  // Your network SSID password
#define ENDPOINT "" // OpenWeatherMap API Endpoint
#define TOKEN "" // OpenWeatherMap token
#define HOST "" // OpenWeatherMap host
```

