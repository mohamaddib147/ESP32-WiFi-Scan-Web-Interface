#include <WiFi.h>
#include <WebServer.h>
#include <map>
#include <vector>
#include <algorithm> // Include for std::sort

// Replace with your desired AP credentials
const char* ssidAP = "ESP3C3_WiFi_Scanner";
const char* passwordAP = "password"; // Choose a strong password

WebServer server(80); // Create web server object on port 80

// Define colors for channels (you can customize these)
// Define colors for channels (you can customize these)
std::map<int, String> channelColors = {
    {1, "green"}, {2, "blue"}, {3, "red"}, {4, "yellow"}, {5, "purple"},
    {6, "orange"}, {7, "brown"}, {8, "pink"}, {9, "gray"}, {10, "cyan"},
    {11, "magenta"}, {12, "lime"}, {13, "teal"}, {14, "indigo"}
};

void setup() {
    Serial.begin(115200);

    WiFi.softAP(ssidAP, passwordAP);
    IPAddress IP = WiFi.softAPIP();

    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.on("/", handleRoot);
    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
}

void handleRoot() {
    int networkCount = WiFi.scanNetworks();
    std::map<int, int> channelOccupancy;
    std::vector<std::pair<String, int>> networks;

    String html = R"=====(
<!DOCTYPE html>
<html>
<head>
<title>WiFi Scan Results</title>
<style>
table {border-collapse: collapse; width: 100%;}
th, td {border: 1px solid #ddd; padding: 8px; text-align: left;}
th {background-color: #4CAF50; color: white;}
</style>
</head>
<body>

<h1>WiFi Scan Results</h1>

<p>Total Networks Found: <span id="networkCount">)=====";
    html += String(networkCount);
    html += R"=====(</span></p>

<button onclick="rescan()">Refresh</button>

<label for="filter">Filter:</label>
<input type="text" id="filter" onkeyup="filterTable()" placeholder="Search SSID...">

<select id="sort" onchange="sortTable()">
  <option value="ssid">SSID</option>
  <option value="rssi">RSSI</option>
  <option value="channel">Channel</option>
</select>

<table id="wifiTable">
  <thead>
    <tr>
      <th>SSID</th>
      <th>RSSI</th>
      <th>Channel</th>
    </tr>
  </thead>
  <tbody>
)=====";

    if (networkCount == 0) {
        html += "<tr><td colspan='3'>No networks found</td></tr>";
    } else {
        for (int i = 0; i < networkCount; ++i) {
            int channel = WiFi.channel(i);
            channelOccupancy[channel]++;
            networks.push_back({WiFi.SSID(i), WiFi.RSSI(i)});
        }

        std::sort(networks.begin(), networks.end(), [](const auto& a, const auto& b) {
            return a.second < b.second; // Sort by RSSI
        });

        for (const auto& network : networks) {
          int channel = 0;
          for (int j = 0; j < networkCount; j++) {
            if (WiFi.SSID(j) == network.first) {
              channel = WiFi.channel(j);
              break;
            }
          }
            String color = channelColors.count(channel) ? channelColors[channel] : "white";
            html += "<tr style='background-color:" + color + "'><td>" + network.first + "</td><td>" + network.second + "</td><td>" + channel + "</td></tr>";
        }
    }

    html += R"=====(
  </tbody>
</table>

<h2>Channel Occupancy</h2>
<table>
  <thead>
    <tr>
      <th>Channel</th>
      <th>Network Count</th>
    </tr>
  </thead>
  <tbody>
)=====";

    for (auto const& [channel, count] : channelOccupancy) {
        html += "<tr><td>" + String(channel) + "</td><td>" + String(count) + "</td></tr>";
    }

    html += R"=====(
  </tbody>
</table>

<script>
function filterTable() {
  var input, filter, table, tr, td, i, txtValue;
  input = document.getElementById("filter");
  filter = input.value.toUpperCase();
  table = document.getElementById("wifiTable");
  tr = table.getElementsByTagName("tr");
  for (i = 1; i < tr.length; i++) { // Skip header row
    td = tr[i].getElementsByTagName("td");
    for (var j = 0; j < td.length; j++) {
      txtValue = td[j].textContent || td[j].innerText;
      if (txtValue.toUpperCase().indexOf(filter) > -1) {
        tr[i].style.display = "";
        break; // Show if found in any column
      } else {
        tr[i].style.display = "none";
      }
    }
  }
}

function sortTable() {
  var table, rows, switching, i, x, y, shouldSwitch, dir, n;
  table = document.getElementById("wifiTable");
  switching = true;
  dir = "asc";
  n = 0;

  var sortValue = document.getElementById("sort").value;
  if (sortValue == "ssid") n = 0;
  else if (sortValue == "rssi") n = 1;
  else if (sortValue == "channel") n = 2;

  while (switching) {
    switching = false;
    rows = table.rows;
    for (i = 1; i < (rows.length - 1); i++) { // Skip header row
      shouldSwitch = false;
      x = rows[i].getElementsByTagName("TD")[n];
      y = rows[i + 1].getElementsByTagName("TD")[n];

      var xValue = x.textContent || x.innerText;
      var yValue = y.textContent || y.innerText;

      if (n == 1 || n == 2) {
        xValue = parseInt(xValue);
        yValue = parseInt(yValue);
      }

      if (dir == "asc") {
        if (xValue > yValue) {
          shouldSwitch = true;
          break;
        }
      } else if (dir == "desc") {
        if (xValue < yValue) {
          shouldSwitch = true;
          break;
        }
      }
    }
    if (shouldSwitch) {
      rows[i].parentNode.insertBefore(rows[i + 1], rows[i]);
      switching = true;
    }
  }
}

function rescan() {
  window.location.href = window.location.href;
}
</script>

</body>
</html>
)=====";

    server.send(200, "text/html", html);
}