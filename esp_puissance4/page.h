void html(bool player, bool turn, WiFiClient client) {
  // Structure d'une requête HTTP
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"));

  // La page html à envoyer
  client.print(F("<!DOCTYPE html><html>"));
  client.print(F("<title>Puissance 4</title><style>"));
  client.print(F("body{text-align: center;position: absolute;left: 0%;}")); /*transform: translate(-50%, 0%);*/
  client.print(F("table {font-family: arial, sans-serif;border-collapse: collapse;}"));
  client.print(F("td, th {border: 1px solid #dddddd;text-align: center;padding: 8px;}"));
  client.print(F("tr:nth-child(even) {background-color: #dddddd;}"));
  client.print(F("</style></head><body>"));
  client.print("<h2>You are player " + String(player) + "</h2>It is");
  client.print(turn ? "" : " not");
  client.print(F(" your turn<br><br>"));
  client.print(F("<table>"));

  // Si c'est le tour du joueur...
  if (turn) {
    client.print(F("<tr style='background: red'>"));
    client.print("<th><a href=/" + String(player) + "/a>&#92/</a></th><th><a href=/" + String(player) + "/b>&#92/</a></th>");
    client.print("<th><a href=/" + String(player) + "/c>&#92/</a></th><th><a href=/" + String(player) + "/d>&#92/</a></th>");
    client.print("<th><a href=/" + String(player) + "/e>&#92/</a></th><th><a href=/" + String(player) + "/g>&#92/</a></th>");
    client.print("<th><a href=/" + String(player) + "/h>&#92/</a></th></tr>");
  }

  // Imprime le tableau
  for (int x = 6; x >= 0; x--) {
    client.print(F("<tr>"));
    for (byte y = 0; y < 7; y++) {
      client.print(F("<td>"));
      if (data[x][y] == 1) {
        client.print(F("X"));
      } else if (data[x][y] == 2) {
        client.print(F("O"));
      }
      client.print(F("</td>"));
    }
    client.print(F("</tr>"));
  }

  client.print(F("</table><br><a href=/reboot>Restart </a>"));
  //client.print("<a href='/" + String(player) + "'> Refresh</a>");
  
  // Rafraichit la page de temps en temps
  client.print("<script>setInterval(func, 4000);function func(){document.location='/" + String(player) + "';}</script>");
  
  client.print(F("</body></html>"));
}
