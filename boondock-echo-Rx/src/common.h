
#ifndef __BOONDOCK_COMMON__
#define __BOONDOCK_COMMON__

#include "Arduino.h"

#define STATE_INIT 0
#define STATE_LISTENING 1
#define STATE_RECORDING 2
#define STATE_SENDING 3
#define STATE_SD_WRITE_ERROR 4
#define STATE_FILE_READ_ERROR 5
#define STATE_FILE_SEND_ERROR 6

RTC_DATA_ATTR bool audio_on_speaker = DEFAULT_AUDIO_ON_SPEAKER;
RTC_DATA_ATTR int silence_before_recording_stops = DEFAULT_SILENSE_BEFORE_RECORDING_STOPS;
RTC_DATA_ATTR int min_recording_duration = DEFAULT_MIN_RECORDING_DURATION;
RTC_DATA_ATTR int max_recording_duration = DEFAULT_MAX_RECORDING_DURATION;
RTC_DATA_ATTR int min_sound = DEFAULT_MIN_SOUND;
RTC_DATA_ATTR int kit_volume = DEFAULT_SPEAKER_VOLUME;
RTC_DATA_ATTR int rx_enabled = true;
RTC_DATA_ATTR int tx_enabled = false;

int current_state = 0;

#define USE_SDFAT
#include "SD.h"
#include "AudioTools.h"
#include "AudioLibs/AudioKit.h"
#include "sdfs.h"
#include <WiFi.h>
#include "time.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include <HTTPClient.h>
//#include <captiv.h>
#include <button.h>

RTC_DATA_ATTR int errorCount = 0;
RTC_DATA_ATTR int readCount = 0;
RTC_DATA_ATTR int sentCount = 0;

const char *DEFAULT_SUB_TOPIC = "";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

const int serverPort = 80;

WiFiClient client;

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;
unsigned long silentSince = 0;

String filename = "";
bool file_created = false;

AudioKit kit;
File root;
File my_recording_file;
File my_sending_file;

uint16_t sample_rate = 16000;
uint8_t channels = 1; // The stream will have 1 channel

uint16_t buffer[RECORDING_BUFFER_SIZE];

#define LED_BLUE 22

int ledStateBlue = LOW;               // ledState used to set the LED
unsigned long previousMillisBlue = 0; // will store last time LED was updated
long intervalBlue = 1000;             // interval at which to blink (milliseconds)
bool led_blink_done_Blue = false;
bool led_should_blink_Blue = true;

#define KEY1 36
#define KEY2 13
#define KEY3 19
#define KEY4 23
#define KEY5 18
#define KEY6 5

Button button1(KEY1, true);
Button button2(KEY2, true);
Button button3(KEY3, true);
Button button4(KEY4, true);
Button button5(KEY5, true);
Button button6(KEY6, true);

AudioKitStream i2s;                             // final output of decoded stream
EncodedAudioStream dec(&i2s, new WAVDecoder()); // Decoding stream

/*
process_buttons()

*/
void process_buttons()
{

    if (button1.pressed())
        Serial.println("Button 1 pressed");
    if (button1.released())
        Serial.println("Button 1 Released");
    if (button1.toggled())
        if (button1.read() == Button::PRESSED)
            Serial.println("Button 1 has been pressed");
        else
            Serial.println("Button 1 has been released");

    if (button2.pressed())
        Serial.println("Button 2 pressed");
    if (button2.released())
        Serial.println("Button 2 Released");
    if (button2.toggled())
        if (button2.read() == Button::PRESSED)
            Serial.println("Button 2 has been pressed");
        else
            Serial.println("Button 2 has been released");

    if (button3.pressed())
        Serial.println("Button 3 pressed");
    if (button3.released())
        Serial.println("Button 3 Released");
    if (button3.toggled())
        if (button3.read() == Button::PRESSED)
            Serial.println("Button 3 has been pressed");
        else
            Serial.println("Button 3 has been released");

    if (button4.pressed())
        Serial.println("Button 4 pressed");
    if (button4.released())
        Serial.println("Button 4 Released");
    if (button4.toggled())
        if (button4.read() == Button::PRESSED)
            Serial.println("Button 4 has been pressed");
        else
            Serial.println("Button 4 has been released");

    if (button5.pressed())
        Serial.println("Button 5 pressed");
    if (button5.released())
        Serial.println("Button5 Released");
    if (button5.toggled())
        if (button5.read() == Button::PRESSED)
            Serial.println("Button 5 has been pressed");
        else
            Serial.println("Button 5 has been released");

    if (button6.pressed())
        Serial.println("Button 6 pressed");
    if (button6.released())
        Serial.println("Button 6 Released");
    if (button6.toggled())
        if (button6.read() == Button::PRESSED)
            Serial.println("Button 6 has been pressed");
        else
            Serial.println("Button 6 has been released");
}

/*
process_LED()

*/
void process_led()
{
    unsigned long currentMillis = millis();

    if (led_should_blink_Blue)
    {
        if (currentMillis - previousMillisBlue >= intervalBlue)
        {

            previousMillisBlue = currentMillis;

            if (ledStateBlue == LOW)
            {
                ledStateBlue = HIGH;
            }
            else
            {
                ledStateBlue = LOW;
            }

            digitalWrite(LED_BLUE, ledStateBlue);
        }
    }
}

/*
blinkLED()
Initalize hardware buttons, LED's etc.
*/
void blink_led(int LED, bool should_blink, long duration)
{
    if (LED == LED_BLUE)
    {
        intervalBlue = duration;
        led_should_blink_Blue = should_blink;
    }
}

String get_beacon_id()
{
    String bid = WiFi.macAddress();
    bid.replace(":", "");
    return bid;
}

String getFullTopic(String topic)
{
    String t = get_beacon_id();
    t = t + "/" + topic;
    return t;
}

// send String values
bool send_mqtt_string(String topic, String value, bool retain)
{
    bool res = false;

    topic = getFullTopic(topic);
    char tempStringTopic[50];
    char tempStringvalue[20];
    topic.toCharArray(tempStringTopic, topic.length() + 1);
    value.toCharArray(tempStringvalue, value.length() + 1);
    res = mqttClient.publish(tempStringTopic, tempStringvalue, retain);

    if (res != true)
    {
        Serial.print(" MQTT Send Failure");
    }

    return res;
}

bool send_mqtt_int(String topic, long value, bool retain)
{

    bool res = false;

    if (mqttClient.connected())
    {
        topic = getFullTopic(topic);
        char tempStringTopic[50];
        char tempStringvalue[20];

        topic.toCharArray(tempStringTopic, topic.length() + 1);
        dtostrf(value, 1, 0, tempStringvalue);
        res = mqttClient.publish(tempStringTopic, tempStringvalue);

        if (res != true)
        {
            Serial.print(" MQTT Send Failure");
        }
    }
    else
        Serial.print(" MQTT Not connected");

    return res;
}

void play(WAVDecoder &decoder, StreamCopy &copier)
{
    while (decoder)
        copier.copy();
}

URLStream url(DEFAULT_BUFFER_SIZE); // or replace with ICYStream to get metadata
void audio_player(String link)
{

    String file_link = FILE_LOCATION + link;

    StreamCopy copier(dec, url); // copy url to decoder
    // setup I2S based on sampling rate provided by decoder
    dec.begin();
    url.begin(file_link.c_str(), "audio/wav");

    Serial.println("File size is " + String(_FILE_SIZE));

    int packet_count = (_FILE_SIZE / 1024) + 2;

    while (packet_count > 0)
    {
        copier.copy();
        packet_count--;
    }

    url.flush();
   // url.end();
    copier.end();
    Serial.println(  "Playback over");
}

void mqtt_callback(char *topic, byte *message, unsigned int length)
{
    if (DEBUG == DEBUG_ALL)
    {
        Serial.print("Message arrived on topic: ");
        Serial.print(topic);
        Serial.print(". Message: ");
    }
    String messageTemp;

    for (int i = 0; i < length; i++)
    {
        if (DEBUG == DEBUG_ALL)
            Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }
    if (DEBUG == DEBUG_ALL)
        Serial.println();

    String topicroot = get_beacon_id();
    String cmd = String(topic);

    cmd = cmd.substring(17);

    topicroot += "/set/";
    if (DEBUG == DEBUG_ALL)
        Serial.println(cmd);

    bool iscommandvalid = true;

    /*
      audio_on_speaker = ;
  silence_before_recording_stops = ;
  min_recording_duration = ;
  max_recording_duration = ;
  min_sound = ;
  kit_volume = ;

*/

    // FREQUENCY OF DATA SENDING
    if (cmd == "speaker")
    {
        if (DEBUG == DEBUG_ALL)
            Serial.print("Changing Speaker Setting ");
        audio_on_speaker = messageTemp.toInt();
    }

    else if (cmd == "min_sound")
    {
        if (DEBUG == DEBUG_ALL)
            Serial.print("Min Sound updated");
        min_sound = messageTemp.toInt();
    }

    else if (cmd == "kit_volume")
    {
        if (DEBUG == DEBUG_ALL)
            Serial.print("Kit Volume changed");
        kit_volume = messageTemp.toInt();
    }

    else if (cmd == "restart" || cmd == "reboot")
    {
        if (DEBUG == DEBUG_ALL)
            Serial.print("Rebooting by mqtt");
        ESP.restart();
    }

    else if (cmd == "min_duration")
    {
        if (DEBUG == DEBUG_ALL)
            Serial.print("Min Recording duration Changed");
        min_recording_duration = messageTemp.toInt();
    }

    else if (cmd == "max_duration")
    {
        if (DEBUG == DEBUG_ALL)
            Serial.print("Max Recording duration Changed");
        max_recording_duration = messageTemp.toInt();
    }

    else if (cmd == "timeZone")
    {
        if (DEBUG == DEBUG_ALL)
            Serial.print("Change the time Zone");
        //  timeZone = messageTemp.toInt();
    }

    else if (cmd == "play")
    {
        if (DEBUG == DEBUG_ALL)
            Serial.print("Play the Audio");
        audio_player(messageTemp);
    }

    else if (cmd == "setDefaults" || cmd == "factoryreset")
    {
        if (DEBUG == DEBUG_ALL)
            Serial.print("Setting defaults");
        //  factory_reset_device();
    }

    else
    {
        iscommandvalid = false;
    }

    if (iscommandvalid)
    {
        //  save_config();
        //  read_config();
        // send_mqtt_string("set/cmd", "OK", false);
        //  sendConfig();
    }
}
//

void subscribe_to_topic()
{
    String substopic = get_beacon_id() + "/set/#";
    char tempStringSubsTopic[50];
    substopic.toCharArray(tempStringSubsTopic, substopic.length() + 1);

    if (DEBUG == DEBUG_ALL)
        Serial.println("Subscribe to " + substopic);
    mqttClient.subscribe(tempStringSubsTopic);
}
// Send ERROR Message
void mqtt_send_error(String errormessage)
{
    char out[128];
    String topic = getFullTopic("error");
    char tempStringTopic[50];
    char tempStringvalue[100];
    topic.toCharArray(tempStringTopic, topic.length() + 1);
    errormessage.toCharArray(tempStringvalue, topic.length() + 1);
    boolean rc = mqttClient.publish(tempStringTopic, tempStringvalue);
}

void mqtt_reconnect()
{
    mqttClient.setServer(mqtt_server, 1883);
    mqttClient.setCallback(mqtt_callback);

    // Loop until we're reconnected
    if (!mqttClient.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (mqttClient.connect(mqttClientID, mqttUser, mqttPassword))
        {
            if (DEBUG == DEBUG_ALL)
                Serial.println("connected");
            subscribe_to_topic();
            // Subscribe
        }
        else
        {
            errorCount++;
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            //  delay(5000);
        }
    }
    if (DEBUG == DEBUG_ALL)
    {
        //  microSecondsSinceBoot = esp_timer_get_time() - previousSecondsSinceBoot;
        //  Serial.println("MQTT Connect:" + String(microSecondsSinceBoot / 1000));
    }

    send_mqtt_string("status", "online", false);
}

// Send device Configurations
void mqtt_send_summary()
{
    /*
      audio_on_speaker = ;
  silence_before_recording_stops = ;
  min_recording_duration = ;
  max_recording_duration = ;
  min_sound = ;
  kit_volume = ;

*/
    send_mqtt_string("system/sw", String(SW_VERSION), false);
    send_mqtt_string("system/hw", String(HW_VERSION), false);

    send_mqtt_string("config/min_sound", String(min_sound), false);
    send_mqtt_string("config/speaker", String(audio_on_speaker), false);
    send_mqtt_string("config/silence", String(silence_before_recording_stops), false);
    send_mqtt_string("config/min_duration", String(min_recording_duration), false);
    send_mqtt_string("config/max_duration", String(max_recording_duration), false);
    send_mqtt_string("config/kit_volume", String(kit_volume), false);
}

void clean_up()
{
}

/******************************
connectWiFi()
Connects to WiFi
*******************************/
void connect_wifi(String ssid, String password)
{
    if (DEBUG != DEBUG_AUDIO)
        Serial.println("Connecting to wifi...");

    WiFi.mode(WIFI_STA);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.print("Bondock IP Address: ");
    Serial.println(WiFi.localIP());
}

// format bytes
String formatBytes(unsigned int bytes)
{
    String res;

    if (bytes < 1024)
    {
        res = String(bytes) + "B";
    }
    else if (bytes < (1024 * 1024))
    {
        res = String(bytes / 1024.0) + "KB";
    }
    else if (bytes < (1024 * 1024 * 1024))
    {
        res = String(bytes / 1024.0 / 1024.0) + "MB";
    }
    return res;
}

/******************************
upload_file_to_server()
Upload file to the Server
*******************************/
bool upload_file_to_server(String filename)
{
    // Intialize Wifi
    bool res = true;
    String getAll;
    String getBody;

    digitalWrite(LED_BLUE, LOW);

    // read file from SD
    // define file
    my_sending_file = SD.open(filename, FILE_READ);

    String fileName = my_sending_file.name();
    String fileSize = formatBytes(my_sending_file.size());
    if (DEBUG == DEBUG_ALL)
    {
        Serial.println("file exists");
    }

    String myMac = WiFi.macAddress();
    myMac.replace(":", "");

    if (my_sending_file)
    {
        if (DEBUG == DEBUG_ALL)
        {
            Serial.println("test file:ok");
            // print content length and host
            Serial.print("contentLength : ");
            Serial.println(fileSize);
            Serial.print("connecting to ");
            Serial.println("events.bluevan.io");

            // We now create a URI for the request
            Serial.println("Connected to server");
            Serial.print("Requesting URL: ");
            Serial.println("/upload.php");

            // Make a HTTP request and add HTTP headers
            String boundary = "CustomizBoundarye----";
            // change with your content type
        }

        String contentType = "audio/x-wav";
        String portString = String("80");
        String hostString = String(serverName);

        String requestHead = "--BoonDock\r\nContent-Disposition: form-data; name=\"audioFile\"; filename=\"" + myMac + "\"\r\nContent-Type: audio/x-wav\r\n\r\n";

        String tail = "\r\n--BoonDock--\r\n";

        int contentLength = requestHead.length() + my_sending_file.size() + tail.length();

        client.connect(serverName.c_str(), 80);

        client.println("POST /upload.php HTTP/1.1");
        client.println("Host: " + serverName);
        client.println("Content-Length: " + String(contentLength, DEC));
        client.println("Content-Type: multipart/form-data; boundary=BoonDock");
        client.println();
        client.print(requestHead);

        // send myFile:
        // this method is for upload data very fast
        // and only work in ESP software version after 2.3.0
        // if your version is lower than please update
        // esp software to last version or use bellow comment code
        client.write(my_sending_file);
        // create file buffer
        const int bufSize = 2048;
        byte clientBuf[bufSize];
        int clientCount = 0;

        while (my_sending_file.available())
        {
            clientBuf[clientCount] = my_sending_file.read();
            clientCount++;
            if (clientCount > (bufSize - 1))
            {
                client.write((const uint8_t *)clientBuf, bufSize);
                clientCount = 0;
            }
        }

        if (clientCount > 0)
        {
            client.write((const uint8_t *)clientBuf, clientCount);
        }
        client.print(tail);
    }
    else
    {
        // if the file didn't open, print an error:
        Serial.println("error opening test.WAV");
        Serial.println("Post Failure");
        current_state = STATE_FILE_READ_ERROR;
        res = false;
    }

    // Read all the lines of the reply from server and print them to Serial

    if (DEBUG == DEBUG_ALL)
    {
        Serial.println("request sent");
    }

    String responseHeaders = "";

    int timoutTimer = 10000;
    long startTimer = millis();
    boolean state = false;

    while ((startTimer + timoutTimer) > millis())
    {
        Serial.print(".");
        delay(100);
        while (client.available())
        {
            char c = client.read();
            if (c == '\n')
            {
                if (getAll.length() == 0)
                {
                    state = true;
                }
                getAll = "";
            }
            else if (c != '\r')
            {
                getAll += String(c);
            }
            if (state == true)
            {
                getBody += String(c);
            }
            startTimer = millis();
        }
        if (getBody.length() > 0)
        {
            break;
        }
    }
    //  Serial.println();
    client.stop();

    if (DEBUG == DEBUG_ALL)
    {
        Serial.println(getBody);
    }

    // close file
    my_sending_file.close();

    digitalWrite(LED_BLUE, HIGH);

    return res;
}

String httpGETRequest(const char *serverName)
{
    HTTPClient http;

    // Your IP address with path or Domain name with URL path
    http.begin(serverName);

    // Send HTTP POST request
    int httpResponseCode = http.GET();

    String payload = "ERROR";

    if (httpResponseCode > 0)
    {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        payload = http.getString();
    }
    else
    {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();

    return payload;
}

/******************************
get_encoded_config_value()
Splits the string to get the values from Encoded String
*******************************/
String get_encoded_config_value(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++)
    {
        if (data.charAt(i) == separator || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }

    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

/******************************
get_config()
Upload file to the Server
*******************************/

bool get_config()
{
    String jsonBuffer;
    String myMac = WiFi.macAddress();
    myMac.replace(":", "");
    String serverPath = "http://app.boondock.live//get_config.php?did=" + myMac;
    if (DEBUG == DEBUG_ALL)
        Serial.println(serverPath);
    jsonBuffer = httpGETRequest(serverPath.c_str());

    if (jsonBuffer == "ERROR")
    {
        Serial.println("Error getting config from server");
        return false;
    }
    else

    {
        rx_enabled = get_encoded_config_value(jsonBuffer, ':', 1).toInt();
        tx_enabled = get_encoded_config_value(jsonBuffer, ':', 2).toInt();
        audio_on_speaker = get_encoded_config_value(jsonBuffer, ':', 3).toInt();
        kit_volume = get_encoded_config_value(jsonBuffer, ':', 4).toInt();
        min_sound = get_encoded_config_value(jsonBuffer, ':', 5).toInt();
        silence_before_recording_stops = get_encoded_config_value(jsonBuffer, ':', 6).toInt();
        min_recording_duration = get_encoded_config_value(jsonBuffer, ':', 7).toInt();
        max_recording_duration = get_encoded_config_value(jsonBuffer, ':', 8).toInt();
        if (DEBUG == DEBUG_ALL)
        {
            Serial.println(jsonBuffer);
            Serial.println("RX Enabled = " + String(rx_enabled));
            Serial.println("TX Enabled = " + String(tx_enabled));
            Serial.println("Speaker    = " + String(audio_on_speaker));
            Serial.println("Spk Volume = " + String(kit_volume));
            Serial.println("Audio Trig = " + String(min_sound));
            Serial.println("Silence    = " + String(silence_before_recording_stops));
            Serial.println("Min Record = " + String(min_recording_duration));
            Serial.println("Max Record = " + String(max_recording_duration));
        }
        return true;
    }
}

/******************************
init_SD()
Initialize SD Card
*******************************/
bool init_SD()
{
    bool res = false;
    if (DEBUG != DEBUG_AUDIO)
        Serial.println("Initializing SD card...");

    int r = 0;
    while (r < 5)
    {
        if (!SD.begin(kit.pinSpiCs(), AUDIOKIT_SD_SPI))
        {
            Serial.println("initialization failed. Try again... " + String(r));
        }
        else
        {
            uint64_t cardSize = SD.cardSize() / (1024 * 1024);
            Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);
            Serial.println("initialization done.");
            init_filesystem(SD);
            res = true;
            break;
        }
        r++;
    }
    return res;
}

/******************************

*******************************/
bool measure_sound(int buflen, bool sample = false)
{
    int16_t signalMax = 0, signalMin = 1024, signalTotal = 0;
    int soundMeasure = 0;
    int buffer_Size = 0;

    for (int i = 0; i < RECORDING_BUFFER_SIZE; i++)
    {
        int16_t k = buffer[i];
        signalMin = min(signalMin, k);
        signalMax = max(signalMax, k);
    }

    soundMeasure = map(signalMax, 0, 32767, 0, 100);
    return (soundMeasure > min_sound); // Returns true when sound is greater than Thrashold
}

/******************************

*******************************/
bool check_sound(int buflen)
{
    int16_t signalMax = 0, signalMin = 1024, signalTotal = 0;
    int soundMeasure = 0;
    int buffer_Size = 0;

    for (int i = 0; i < SAMPLING_BUFFER_SIZE; i++)
    {
        int16_t k = buffer[i];
        signalMin = min(signalMin, k);
        signalMax = max(signalMax, k);
    }

    soundMeasure = map(signalMax, 0, 32767, 0, 100);

    if (DEBUG == DEBUG_AUDIO)
    {
        Serial.print("Total:");
        Serial.println(soundMeasure);
    }

    if (DEBUG == DEBUG_NONE && soundMeasure > min_sound)
    {
        Serial.print("~");
        Serial.print(soundMeasure);
        Serial.print("~");
    }

    return (soundMeasure > min_sound); // Returns true when sound is greater than Thrashold
}

/******************************

*******************************/
bool is_silent(int buflen, bool sample)
{
    //  Serial.print(">");
    long currentMillis = millis();

    if (measure_sound(buflen, false))
    {
        silentSince = millis();
        return false;
    }
    else if ((currentMillis - silentSince) > silence_before_recording_stops)
    {
        if (DEBUG == DEBUG_ALL)
            Serial.println("Silent for " + String(currentMillis - silentSince) + " Seconds");
        return true;
    }
    else
        return false;
}

/******************************

*******************************/
void init_hardware()
{
    Serial.println("Initializing Hardware...");
    pinMode(LED_BLUE, OUTPUT);
    digitalWrite(LED_BLUE, HIGH);

    pinMode(PIN_KEY1, INPUT);
    pinMode(PIN_KEY2, INPUT);
    pinMode(PIN_KEY3, INPUT);
    pinMode(PIN_KEY4, INPUT);
    pinMode(PIN_KEY5, INPUT);
    pinMode(PIN_KEY6, INPUT);
}

/******************************
void init_mic()
Initialize the Microphone
Sampling rate must match the Wav Encoder
*******************************/
void init_mic_and_speaker()
{
    Serial.println("Initializing Mic...");

    auto cfg = kit.defaultConfig(AudioInputOutput);
    cfg.adc_input = INPUT_LINE; // microphone
    cfg.sample_rate = AUDIO_HAL_08K_SAMPLES;
    cfg.bits_per_sample = AUDIO_HAL_BIT_LENGTH_16BITS;

    // cfg.sample_rate = AUDIO_HAL_22K_SAMPLES;
    kit.begin(cfg);
    kit.setVolume(kit_volume);
}

/******************************
void capture_audio()
Captures an Audio sample
Sampling rate must match the Wav Encoder
*******************************/
String capture_audio(int buflen)
{

    silentSince = millis();
    unsigned long StartTime = millis(); // Startime for the File
    digitalWrite(LED_BLUE, HIGH);
    WAVEncoder encoder(my_recording_file);
    encoder.begin();

    if (file_created)
    {
        if (DEBUG == DEBUG_ALL)
            Serial.println("Saving to WAV file...");
    }

    unsigned long CurrentTime = millis();
    unsigned long ElapsedTime = CurrentTime - StartTime;

    for (int i = 0; i < MAX_AUDIO_BUFFER; i++)
    {
        CurrentTime = millis();
        ElapsedTime = (CurrentTime - StartTime);

        size_t len = kit.read(buffer, RECORDING_BUFFER_SIZE);
        if (audio_on_speaker)
            kit.write(buffer, len);

        if (ElapsedTime > min_recording_duration)
        {
            if (ElapsedTime > max_recording_duration)
            {
                if (DEBUG == DEBUG_ALL)
                    Serial.println("Closing file by Duration");
                file_created = false;
                my_recording_file.close();
                break;
            }

            if (is_silent(len, false) == true)
            {
                if (DEBUG == DEBUG_ALL)
                    Serial.println("Closing file due to silence");
                file_created = false;

                my_recording_file.close();
                break;
            }
        }

        // Serial.println("Read bytes " + String(len));
        if (my_recording_file)
        {
            encoder.write(buffer, len);
            // Serial.print("+");
        }
        else
        {
            //  Serial.println("error opening " + filename);
            my_recording_file.close();
            if (DEBUG == DEBUG_ALL)
                Serial.println("Closing file due to error");
            current_state = STATE_SD_WRITE_ERROR;
            file_created = false;
            break;
        }
    }

    if (my_recording_file)
    {
        my_recording_file.close();
        if (DEBUG == DEBUG_ALL)
            Serial.println("Closing file by max buffers");
        file_created = false;
    }
    CurrentTime = millis(); // Get Current Time

    return filename;

    digitalWrite(LED_BLUE, HIGH);
}

/*
void capture_audio()
Captures an Audio sample
Sampling rate must match the Wav Encoder
*/
String capture_to_cloud(int buflen)
{

    silentSince = millis();
    unsigned long StartTime = millis(); // Startime for the File
    digitalWrite(LED_BLUE, HIGH);

    unsigned long CurrentTime = millis();
    unsigned long ElapsedTime = CurrentTime - StartTime;

    for (int i = 0; i < MAX_AUDIO_BUFFER; i++)
    {
        CurrentTime = millis();
        ElapsedTime = (CurrentTime - StartTime);

        size_t len = kit.read(buffer, RECORDING_BUFFER_SIZE);
        if (audio_on_speaker)
            kit.write(buffer, len);

        if (ElapsedTime > min_recording_duration)
        {
            if (ElapsedTime > max_recording_duration)
            {
                if (DEBUG == DEBUG_ALL)
                    Serial.println("Closing file by Duration");
                break;
            }

            if (is_silent(len, false) == true)
            {
                if (DEBUG == DEBUG_ALL)
                    Serial.println("Closing file due to silence");
                break;
            }
        }
    }

    CurrentTime = millis(); // Get Current Time
    digitalWrite(LED_BLUE, HIGH);
    return "";
}

// UrlStream -copy-> EncodedAudioStream -> I2S

#endif
