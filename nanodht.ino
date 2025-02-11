#include <WiFiNINA.h>      // Required for the WiFi connection
#include <ThingSpeak.h>    // Required for establishing communication with ThingSpeak website
#include <DHT.h>          // Required for reading the DHT sensor values

// Define the digital pin connected to the DHT sensor
#define DHTPIN 7
// Define the type of DHT sensor being used
#define DHTTYPE DHT11

// Initialize the DHT sensor with the specified pin and type
DHT dht(DHTPIN, DHTTYPE);

// Define the SSID (WiFi network name) and password for connection
#define SECRET_SSID "arsh"
#define SECRET_PASS "12345678"

// Define the ThingSpeak channel ID and write API key
#define SECRET_CH_ID 2635592
#define SECRET_WRITE_APIKEY "YXG04STEJS199OFR"

// Store the SSID and password in character arrays
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// Initialize a WiFi client
WiFiClient client;

// Define the ThingSpeak channel number and write API key
unsigned long channelID = SECRET_CH_ID;
const char* apiKey = SECRET_WRITE_APIKEY;

// Setup function runs once at the beginning
void setup() {
  // Start serial communication
  Serial.begin(9600);
  // Wait for serial connection to be established
  while (!Serial) {
    ; // Wait for serial port to connect
  }

  // Initialize DHT sensor
  dht.begin();

  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, pass);

  // Wait until connected to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Initialize ThingSpeak communication
  ThingSpeak.begin(client);
}

// Loop function runs repeatedly
void loop() {
  // Delay for 2 seconds
  delay(60000);

  // Read humidity and temperature from DHT sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if DHT sensor reading is valid
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print humidity and temperature readings
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");

  // Update ThingSpeak with humidity and temperature data
  ThingSpeak.setField(1, humidity);
  ThingSpeak.setField(2, temperature);

  // Send data to ThingSpeak channel and check for success
  int responseCode = ThingSpeak.writeFields(channelID, apiKey);
  if (responseCode == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.print("Problem updating channel. HTTP error code ");
    Serial.println(responseCode);
  }
}