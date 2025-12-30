#include <WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11
#define GAS_PIN 34   // Analog pin for gas sensor (change if needed)

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

unsigned long myChannelNumber = 3214645;
const char * myWriteAPIKey = "8IJM9FG1CQERML6F";

const char *ssid = "POCO";
const char *password = "12348765";

void setup() {
  Serial.begin(115200);

  dht.begin();
  pinMode(GAS_PIN, INPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client);
}

void loop() {
  float temperature = dht.readTemperature();   // °C
  float humidity = dht.readHumidity();         // %
  int gasLevel = analogRead(GAS_PIN);           // Gas sensor value

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C | Humidity: ");
  Serial.print(humidity);
  Serial.print(" % | Gas Level: ");
  Serial.println(gasLevel);

  // ✅ Set all fields
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, gasLevel);

  // ✅ Write all fields in one request
  int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if (httpCode == 200) {
    Serial.println("Channel write successful.");
  } else {
    Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
  }

  delay(10000); // ThingSpeak minimum delay
}
