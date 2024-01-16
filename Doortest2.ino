#include <ESP8266WiFi.h>  // Use ESP8266WiFi library instead of WiFi.h
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

const char* ssid = "A";
const char* password = "alex123456";

IPAddress server_addr(89, 233, 205, 28); // IP of the MySQL server
char user[] = "root";
char pass[] = "root123";
char default_db[] = "DoorDb";

WiFiClient client;
MySQL_Connection conn((Client *)&client);

// For ESP-01S, you are limited in pin selection. GPIO0 or GPIO2 are typical choices.
const int buttonPin2 = 2; // Changed to GPIO2 for ESP-01S
const int buttonPin0 = 0; // GPIO0 for button

//int lastButtonState = LOW; // Variable to store the last state of the button

void setup() {
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin0, INPUT_PULLUP);
  Serial.begin(115200);
  delay(1000);

  connectToWiFi();
}

void loop() {
  int buttonState2 = digitalRead(buttonPin2);
  int buttonState0 = digitalRead(buttonPin0);

  if (buttonState2 == LOW) {
    Serial.println("Button on GPIO2 is pressed");
    updateDatabase(1);
    delay(1000); // Debounce delay
  }

  if (buttonState0 == LOW) {
    Serial.println("Button on GPIO0 is pressed");
    updateDatabase(0);
    delay(1000); // Debounce delay
  }
}

void connectToWiFi() {
Serial.print("Connecting to WiFi");
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.print("Connected to WiFi. IP address: ");
Serial.println(WiFi.localIP());
}

void updateDatabase(int state) {
if (conn.connect(server_addr, 57070, user, pass, default_db)) {
MySQL_Cursor *cur = new MySQL_Cursor(&conn);
String query = "UPDATE doors SET status = ";
query += state;
query += " WHERE id = 1";
cur->execute(query.c_str());
delete cur;
Serial.print("Database updated to state: ");
Serial.println(state);
} else {
Serial.println("Failed to connect to MySQL server.");
}
conn.close();
}
