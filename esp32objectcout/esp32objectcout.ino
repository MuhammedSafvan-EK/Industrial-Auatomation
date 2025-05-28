#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <U8g2lib.h>

// Encoder Pins
#define ENCODER_CLK  32
#define ENCODER_DT   33
#define ENCODER_SW   25

// Ultrasonic Sensor & Buzzer Pins
#define TRIG_PIN  15
#define ECHO_PIN  18
#define BUZZER_PIN  26

// WiFi Credentials
const char* ssid = "OPPOA54";  // Change to your WiFi SSID
const char* password = "10101010";  // Change to your WiFi password

// API Configuration
const char* serverURL = "http://192.168.196.214/objectcountersensordata/post-esp-data.php";
const char* apiKey = "tPmAT5Ab3j7F9";  // Replace with your actual API key

// Sensor Data
String sensorName = "objectcounter";
String sensorLocation = "Thrissur";

// OLED Display Initialization
U8G2_SSD1306_128X64_NONAME_F_SW_I2C display(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);

// Global Variables
volatile int encoderValue = 0;
int lastStateCLK;
bool countSet = false;
bool timeSet = false;
int objectCount = 0;
int setCount = 0;
int setTime = 10;
unsigned long startTime = 0;
bool timeRunning = false;

// Interrupt for Rotary Encoder
void IRAM_ATTR readEncoder() {
    int currentStateCLK = digitalRead(ENCODER_CLK);
    if (currentStateCLK != lastStateCLK) {
        if (digitalRead(ENCODER_DT) != currentStateCLK) {
            encoderValue++;
        } else {
            encoderValue--;
        }
    }
    lastStateCLK = currentStateCLK;
}

// Function to Measure Distance
float getDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH);
    return duration * 0.034 / 2;
}

// Function to Send Data to Web Server
void sendDataToWeb() {
    Serial.println("Sending data to server...");
    display.clearBuffer();
    display.setCursor(10, 30);
    display.print("Sending data...");
    display.sendBuffer();

    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverURL);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        String postData = "api_key=" + String(apiKey) +
                          "&sensor=" + sensorName+
                          "&location=" + sensorLocation+
                          "&value1=" + String(setCount) +
                          "&value2=" + String(setTime) +
                          "&value3=" + String(objectCount) +
                          "&value4=" + (objectCount >= setCount ? "Successful" : "Timed Out");

        int httpResponseCode = http.POST(postData);
        Serial.print("Server Response Code: ");
        Serial.println(httpResponseCode);

        http.end();
    } else {
        Serial.println("WiFi not connected!");
    }

    delay(2000);
}

// Function to Reset System
void resetSystem() {
    countSet = false;
    timeSet = false;
    objectCount = 0;
    encoderValue = 0;
    timeRunning = false;
}

// Setup Function
void setup() {
    Serial.begin(9600);

    // Connect to WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    int wifiAttempts = 0;
    while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) {
        delay(500);
        Serial.print(".");
        wifiAttempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to WiFi!");
    } else {
        Serial.println("\nFailed to connect to WiFi.");
    }

    // Initialize Encoder
    pinMode(ENCODER_CLK, INPUT_PULLUP);
    pinMode(ENCODER_DT, INPUT_PULLUP);
    pinMode(ENCODER_SW, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), readEncoder, CHANGE);

    // Initialize Ultrasonic Sensor & Buzzer
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    // Initialize Display
    display.begin();
    display.clearBuffer();
    display.setFont(u8g2_font_6x10_tf);

    lastStateCLK = digitalRead(ENCODER_CLK);
}

// Main Loop
void loop() {
    if (!countSet) {
        display.clearBuffer();
        display.setCursor(10, 20);
        display.print("Set Count: ");
        display.print(encoderValue);
        display.setCursor(10, 40);
        display.print("Press to Confirm");
        display.sendBuffer();

        if (digitalRead(ENCODER_SW) == LOW) {
            delay(300);  // Debounce delay
            setCount = encoderValue;
            countSet = true;
            encoderValue = 10;
        }
        return;
    }

    if (!timeSet) {
        display.clearBuffer();
        display.setCursor(10, 20);
        display.print("Set Time (s): ");
        display.print(encoderValue);
        display.setCursor(10, 40);
        display.print("Press to Confirm");
        display.sendBuffer();

        if (digitalRead(ENCODER_SW) == LOW) {
            delay(300);  // Debounce delay
            setTime = encoderValue;
            if (setTime < 1) setTime = 1;
            if (setTime > 60) setTime = 60;
            timeSet = true;
            objectCount = 0;
            timeRunning = true;
            startTime = millis();
        }
        return;
    }

    // Check if Time Has Elapsed
    unsigned long elapsedTime = millis() - startTime;
    if (elapsedTime >= setTime * 1000) {
        display.clearBuffer();
        display.setCursor(10, 30);
        display.print("Time Reached!");
        display.sendBuffer();
        digitalWrite(BUZZER_PIN, HIGH);
        delay(1000);
        digitalWrite(BUZZER_PIN, LOW);
        sendDataToWeb();
        resetSystem();
        return;
    }

    // Measure Distance and Count Objects
    float distance = getDistance();
    if (distance < 10) {
        objectCount++;
        delay(500);  // Prevent duplicate counts
    }

    // Check if Count is Reached
    if (objectCount >= setCount) {
        display.clearBuffer();
        display.setCursor(10, 30);
        display.print("Count Reached!");
        display.sendBuffer();
        digitalWrite(BUZZER_PIN, HIGH);
        delay(1000);
        digitalWrite(BUZZER_PIN, LOW);
        sendDataToWeb();
        resetSystem();
        return;
    }

    // Update Display
    int timeLeft = (setTime * 1000 - elapsedTime) / 1000;
    display.clearBuffer();
    display.setCursor(10, 10);
    display.print("Set: ");
    display.print(setCount);
    display.setCursor(10, 30);
    display.print("Count: ");
    display.print(objectCount);
    display.setCursor(10, 50);
    display.print("Time: ");
    display.print(timeLeft);
    display.print("s");
    display.sendBuffer();

    delay(500);
}
