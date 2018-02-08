# Instrument Playing Robot

This was meant to use the Arduino-based ESP8266 with motor shield and wifi to control various motor shields to play an instrument. It is controlled via a local Django server that takes keyboard presses and requests http to the board wifi. The board parses the http requests and responds depends on which key is pressed. This was also my first attempt at using Riot.js, which allows customizable html-like tags with states. We use it to be able to handle multiple instruments at once without copying and pasting code, which is ugly.

![Screenshot](https://github.com/jachang820/InstrumentPlayingRobot/blob/master/screenshot.png)

## Getting Started

This project requires Django to run, so you would need to install Python and do 'pip install django'. Then go into root directory and type on the cmd 'python manage.py runserver'. Riot.js is served from the CDN, so it doesn't require any installation. It also uses urllib3 to make http requests (so do 'pip install urllib3').
