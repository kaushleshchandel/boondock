# Boondock Echo
![logo](/mediakit/logos/logo_boondock_small.jpg)

## What is Boondock Echo?
Boondock Echo is a time-shift audio project created for emergency radio communications. A Boondock Echo module connects to transceivers with audio cables between the mic and speaker of the radio and the speaker and mic of the Bookdock Echo moule. 
The Boondock Echo listens to the radio for traffic. Whenever new traffic is received, it captures the audio as a .mp3 on a local sd card, and then sends the file to sever for storage, and queing, and playback. Users can play back messages on the Boondock browser interface or que messages for transmission on a radio.

## Key features
<ol>
  <li>Made from commercial off-the-shelf (COTS) components</li>
  <li>Supports basic radios like Baofeng UV-5R</li>
  <li>Record radio transmissions</li>
  <li>Playback to one/multiple radios</li>
  <li>Queue messages for playback</li>
  <li>Create groups of radios</li>
</ol>

## Core Components
To make boondock echo works, you need Google cloud account for maps, text to speech & speech to text translation, a Apache based webserver, and an ESP32 device with firmware. Here are list of key components

<ol>
  <li>ESP32 based device firmware</li>
  <li>Apache based web server</li>
  <li>Mosquitto MQTT server</li>
  <li>Google API credentials for google services</li>
</ol>


## Building a Boondock Echo

[CLICK HERE](/3d.print/README.md) for build instructions

To build Boondock, you need ESP32 Audio kit, a speaker, Li-ion battery, a power switch, and a 3d printed enclosure with some screws. 

List of purchasable components.
Currently supported module:
<ol>
  <li>1x ESP32 Based Audio Interface: https://www.amazon.com/gp/product/B0B63KZ6C1 </li>
</ol>

The following parts are what we used to make the prototype.  Other substitutes are fine:
<ol>
  <li>1x Handie-Talkie: (Baofeng UV-5R: https://www.amazon.com/dp/B074XPB313 or similar)</li>
  <li>1x 3.7V rechargable battery with 2.0 mm 2 pin JST connector: (https://www.amazon.com/dp/B095DQJ69Q or simlar)</li>
  <li>2x 3.5mm male audio cable (https://www.amazon.com/dp/B076BFZDG2/ or similar)</li>
  <li>1x 3.5mm female to 2.5 mm male stereo audio jack: (https://www.amazon.com/dp/B01FPRADYK or similar)</li>
  <li>1x Speaker with 2.0mm 2 pin JST connector (https://www.amazon.com/dp/B07FTB281F or similar)</li>
</ol>


## Programming the ESP32 Audio kit

There are several versions of ESP32 Audio kits available in the market. The one used for the build is ESP32 Audio Kit from AI Thinker. Other boards can be supported with changes to the code. Right now, we are focusing on kit by AI Thinker.

![ESP32 Audio Kit](/3d.print/ESP32AudioKit.jpg)

[CLICK HERE](/esp32.audio/README.md) for details on Programming the Audio kit.

## Setting up the Web Server
