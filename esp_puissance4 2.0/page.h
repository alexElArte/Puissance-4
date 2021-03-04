void html(bool player, WiFiClient client) {
  // Structure d'une requête HTTP
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"));

  // La page html à envoyer
  client.print(F("<!DOCTYPE html><html><title>Puissance 4</title><style>"));
  client.print(F(""));
  client.print(F("body{text-align: center;position: absolute;left: 0%;}")); /*transform: translate(-50%, 0%);*/
  client.print(F("table {font-family: arial, sans-serif;border-collapse: collapse;}"));
  client.print(F("td, th {border: 1px solid #dddddd;text-align: center;padding: 8px;}"));
  client.print(F("tr:nth-child(even) {background-color: #dddddd;}"));
  client.print(F("</style></head><body>"));


  client.print("<h2>Your are player " + String(player) + "</h2>");

  client.print(F("<div id='turn'></div>"));
  client.print(F("<button onclick='press()'>Restart</button><br><br>"));
  client.print(F("<table id='table'></table>"));

  client.print(F("<script>"));
  client.print(F("function press(){"));
  client.print(F("if(confirm('Are you sure you want to restart')){"));
  client.print(F("post('begin')}}"));

  client.print(F("function post(info){"));
  client.print(F("var xhttp = new XMLHttpRequest();"));
  client.print("xhttp.open('POST', '/" + String(player) + "/'+info, true);");
  client.print(F("xhttp.send();getdata();}"));

  client.print(F("setInterval(getData, 3000);"));

  client.print(F("function getData() {"));
  client.print(F("var xhttp = new XMLHttpRequest();"));
  client.print(F("xhttp.onreadystatechange = function() {"));
  client.print(F("if (this.readyState == 4 && this.status == 200) {"));
  client.print(F("json = JSON.parse(this.responseText);"));

  client.print(F("var x, y;var text = '';var turn = 'not ';var letter = 'mnopqrs';"));
  client.print("if(json.t == " + String(player) + "){turn = '';");
  client.print(F("text += '<tr style=background:red>'; for (x = 0; x < 7; x++) {"));
  client.print(F("text += '<th onclick=post(&#34';text += letter[x];"));
  client.print(F("text += '&#34)>V</th>';}text += '</tr>';}"));
  client.print(F("document.getElementById('turn').innerHTML = 'It is ' + turn + 'your turn';"));
  client.print(F("for (x = 5; x >= 0; x--) {text += '<tr>';for (y = 0; y < 7; y++) {"));
  client.print(F("text += '<td>';if(json.data[x*7+y] == 1) { text += 'X';"));
  client.print(F("} else if (json.data[x*7+y] == 2) { text += 'O';}"));
  client.print(F("text += '</td>';}text += '</tr>';}"));
  client.print(F("document.getElementById('table').innerHTML = text;}};"));
  client.print("xhttp.open('GET', '/" + String(player) + "/data', true);xhttp.send();}");

  client.print(F("</script></body></html>"));

}

void send_data(bool turn, WiFiClient client) {
  // Structure d'une requête HTTP
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/json\r\n\r\n"));

  client.print("{");
  client.print('"');
  client.print("t");
  client.print('"');
  client.print(":" + String(turn) + ",");
  client.print('"');
  client.print("data");
  client.print('"');
  client.print(":");
  client.print('"');

  for (byte x = 0; x < 6; ++x) {
    for (byte y = 0; y < 7; ++y) {
      client.print(data[x][y]);
    }
  }

  client.print('"');
  client.print("}");
}
