#include <LiquidCrystal.h>
#include <WiFi.h>
#include "time.h"
#include "esp_sntp.h"


int rs = 14;
int en = 27;
int d4 = 26;
int d5 = 25;
int d6 = 33;
int d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// WiFi credentials
const char* ssid       = "ACT Network";
const char* password   = "13150517";


const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const char *time_zone = "IST-5:30";

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    return;
  }

  char date[20];
  char time[20];
  strftime(date, sizeof(date), "%b %d %Y", &timeinfo);
  strftime(time, sizeof(time), "%a, %H:%M:%S", &timeinfo);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(date);
  lcd.setCursor(0, 1);
  lcd.print(time);

  Serial.println(date);
  Serial.println(time);
}

void timeavailable(struct timeval *t) {
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
}

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  Serial.flush();

 
  lcd.clear();
  lcd.print("Connecting to");
  lcd.setCursor(0, 1);
  lcd.print(ssid);


  WiFi.begin(ssid, password);
  Serial.printf("Connecting to %s ", ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.print("CONNECTED");
  delay(2000);

  
  sntp_servermode_dhcp(1);  
  sntp_set_time_sync_notification_cb(timeavailable);
  configTzTime(time_zone, ntpServer1, ntpServer2);
}

void loop() {
  delay(1000); 
  printLocalTime();
}
