#include <MPU6050.h>
#include <Wire.h>
#include <ESP8266Firebase.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>


#define _SSID "Galaxy_S23"          // Your WiFi SSID
#define _PASSWORD "k9fxaxhuxdpdchw"  // Your WiFi Password
#define REFERENCE_URL "https://proje1-eb887-default-rtdb.europe-west1.firebasedatabase.app/"  // Your Firebase project reference URL

unsigned long channelID = 2390495;   // ThingSpeak channel ID
unsigned int field_no = 1;
const char *writeAPIKey = "DJQZ5JP2XL3RSU0C";  // ThingSpeak write API Key
const char *readAPIKey = "ACSDTTGQ4HDP8PRK";   // ThingSpeak read API Key
#define BOTtoken "6723529764:AAGYOX_jaeIiza6e27idbWConrrvtB_Nl_4"  //telegram bot
#define CHAT_ID "6264898322" // telegram id bot

Firebase firebase(REFERENCE_URL);

MPU6050 MPU;
int16_t GyroX, GyroY, GyroZ;
int buzzer = D3;

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken,client);

WiFiClient client1;

// Add your coordinates here
float latitude = 40.770582;  // Change this to your latitude
float longitude = 30.399733;  // Change this to your longitude

void setup()
{
  Serial.begin(9600);
  configTime(0, 0, "pool.ntp.org");// get UTC time via NTP
  client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  Wire.begin();
  MPU.initialize();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);
  ThingSpeak.begin(client1); // ThingSpeak client sınıfı başlatılıyor
  pinMode(D2, INPUT);       // A0 ucu sensör okumak için giriş modunda
  Serial.println(F("Kurulum Hazır"));

  // Connect to WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("-");
  }

  Serial.println("");
  Serial.println("WiFi Connected");

  // Print the IP address
  Serial.print("IP Address: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  bot.sendMessage(CHAT_ID, "System started", "");
}

unsigned int earthquakeCounter = 1;   // Başlangıçta sayaç sıfır olmalıdır

void loop()
{
  MPU.getRotation(&GyroX, &GyroY, &GyroZ);

  // Earthquake detection
  if (GyroX < -1000 || GyroX > 1000 || GyroY > 1000 || GyroY < -1000 || GyroZ > 1000 || GyroZ < -1000)
  {
    // Earthquake detected, sound the buzzer
    tone(buzzer, 700);

    // Calculate the sensor's pitch and roll in degrees
    float roll = atan2(GyroY, GyroZ) * 180.0 / PI;
    float pitch = atan2(-GyroX, sqrt(GyroY * GyroY + GyroZ * GyroZ)) * 180.0 / PI;

    // Calculate earthquake magnitude by dividing pitch by 10
    float earthquakeMagnitude = abs((pitch) / 10.0);
    
    if (earthquakeMagnitude > 3.0)
    {
    
      Serial.print("Earthquake Magnitude = ");
      Serial.println(earthquakeMagnitude, 1); // Print with 1 decimal place
      delay(500);
     /*if(earthquakeCounter>0)
      {
        bot.sendMessage(CHAT_ID, "Deprem Büyüklüğü: " + String(earthquakeMagnitude));
        delay(500);
      }
      
*/
      bot.sendMessage(CHAT_ID, "Deprem Büyüklüğü: " + String(earthquakeMagnitude));
      delay(500);
      // Send earthquake data to Firebase
      String path = "Earthquake/deprem" + String(earthquakeCounter);  // Her depremde benzersiz bir ID oluşturun
      firebase.setFloat(path + "/sallanti_buyuklugu", earthquakeMagnitude);

      // Add latitude and longitude to the Firebase data
      firebase.setFloat(path + "/Latitude", latitude);
      firebase.setFloat(path + "/Longitude", longitude);

      // Increment the counter for the next earthquake-specific ID
      earthquakeCounter++;

     

      // Print the earthquake magnitude sent to ThingSpeak
      Serial.print("ThingSpeak Gonderilen deprem degeri: ");
      Serial.println(earthquakeMagnitude);

      // Write earthquake data to ThingSpeak
      ThingSpeak.writeField(channelID, field_no, earthquakeMagnitude, writeAPIKey);
      Serial.println("\n");

      /* ThingSpeak Field Okuma İşlemi */
      float oku = ThingSpeak.readFloatField(channelID, field_no); // ilgili kanalın belirtilen field oku
      //float oku = ThingSpeak.readFloatField(channelID, field_no, readAPIKey); // private kanallar için readAPIKey
      Serial.print("ThingSpeak’ten Okunan deprem Değeri: ");
      Serial.println(oku);

      // Deprem oldu bayrağını sıfırla
      
    }
  }
  else
  {
    // No earthquake, stop the buzzer
    noTone(buzzer);
   
   
  }
}



