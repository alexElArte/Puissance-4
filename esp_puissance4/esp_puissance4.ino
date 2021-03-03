/*Serveur html puissance 4
 * 
 * Si vous êtes en mode AP:
 *    -Joueur 0 : http://198.168.4.1/0
 *    -Joueur 1 : http://198.168.4.1/1
 * 
 * Si vous n'êtes pas en mode AP:
 *    -Joueur 0 : http://ADRESSE/0
 *    -Joueur 1 : http://ADRESSE/1
 *  Remplacer ADRESSE par l'adresse IP de l'ESP
 */


#include <ESP8266WiFi.h>

// Commentez pour vous connecter à votre réseau wifi
#define AP

#ifdef AP
  #ifndef APSSID
  // Définir sonpropre point d'accès
  #define APSSID "ESPap"
  #define APPSK  "thereisnospoon"
  #endif
  const char *ssid = APSSID;
  const char *password = APPSK;
#else
  #ifndef STASSID
  // Votre mot de passe wifi
  #define STASSID "SSID"
  #define STAPSK  "PASSWORD"
  #endif
  const char* ssid = STASSID;
  const char* password = STAPSK;
#endif

// Le port sur lequel vous envoyez les informations: HTTP=80
WiFiServer server(80);

// Tableau de valeur du puissance 4
byte data[6][7] = {
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0}
};

String letter = "abcdeghi";
bool turn = false; // A qui le tour...

#include "page.h"

void setup() {
  Serial.begin(115200);

#ifdef AP // Si vous êtes en mode point d'accès
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  // Print the IP address
  // Default 192.168.4.1
  Serial.print(F("AP mode"));
  Serial.println(WiFi.softAPIP());
#else
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));
  // Print the IP address
  Serial.println(WiFi.localIP());
#endif

  // Start the server
  server.begin();
  Serial.println(F("Server started"));

}

void loop() {
  // Voir s'il y a un nouveau client
  WiFiClient client = server.available();
  if (!client) return;
  
  Serial.println(F("New client"));
  client.setTimeout(5000); // default 1000

  // Lit la première d'info le reste ne nous interesse pas
  String req = client.readStringUntil('\r');
  while (client.available()) {
    client.read();
  }
  Serial.print(F("request: "));
  Serial.println(req);


  // Détecte quel joueur fait la demande
  bool player = false;
  if (req.indexOf(F("/0")) != -1) {
    player = 0;
  } else if (req.indexOf(F("/1")) != -1) {
    player = 1;
  } else {
    Serial.println(F("invalid request"));
    return;
  }


  // Ajoute une pièce quand il le faut et que c'est possible
  bool search = false;
  for (byte x = 0; x < 7; x++) {
    if (req.indexOf("/" + String(letter[x])) != -1) {
      Serial.println("yes " + String(x));
      for (byte y = 0; y < 6; y++) {
        if (data[y][x] == 0) {
          data[y][x] = player + 1;
          Serial.println("change" + String(data[y][x])+String(y));
          search = true;
          turn = !turn;
          break;
        }
      }
      if (search) break;
    }
  }
  
  // Envoie la page html au joueur
  html(player, turn == player, client);

  Serial.println("\n");


}
