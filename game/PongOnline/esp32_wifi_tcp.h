//========================================================
// 功能：WIFI連線物件.
//
//========================================================
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

// IP address of the AP ESP32
const char *serverIP = "192.168.4.1"; 

class esp32_wifi_tcp {
  private:
    const char *ssid;
    const char *password;
    WiFiServer server;
    WiFiClient client;

  public:
    //----------------------------------------------------  
    // 建構式.
    //----------------------------------------------------  
    esp32_wifi_tcp(bool isServer, const char* ssid, const char* password): 
      isServer(isServer), ssid(ssid), password(password), server(80) {}
      
    //----------------------------------------------------  
    // 初始連線.
    //----------------------------------------------------
    void begin() {
      if (isServer) {
        WiFi.softAP(ssid, password);
        IPAddress IP = WiFi.softAPIP();
        Serial.print("AP IP address: ");
        Serial.println(IP);
        server.begin();
      } else {
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
          delay(1000);
          Serial.println("Connecting to WiFi...");
        }
        Serial.println("Connected to the WiFi network");
      }
    }
    
    //----------------------------------------------------
    // 傳送資料.
    //----------------------------------------------------
    void send(const char* message) {
      if (isServer) {
        client = server.available();
        if (client) {
          client.println(message);
          //Serial.println("Sent message to client.");
        }
      } else {
        if (client.connect(serverIP, 80)) {
          client.println(message);          
          //Serial.println("Sent message to server.");
        }
      }
    }
    
    //----------------------------------------------------
    // 接收資料.
    //----------------------------------------------------
    String receive() {      
      String received = "";
      while (client.connected()) {
        while (client.available()) {
          char c = client.read();
          received += c;
        }
        if (received.length() > 0) {
          //Serial.print("Received: ");
          //Serial.println(received);
          break;
        }
      }
      return received;      
    }    
    
    //----------------------------------------------------
    // 判斷是否連線.
    //----------------------------------------------------
    bool isConnect(){
      if (isServer) {
        //client = server.available();
        //return client.connected();
        if(server.hasClient()){
          return true;
        }else{
          return false;
        }
      }else{        
        if((WiFi.status() == WL_CONNECTED)){
          return true;
        }else{
          return false;
        }        
      }      
    }
    
    bool isServer;
};
