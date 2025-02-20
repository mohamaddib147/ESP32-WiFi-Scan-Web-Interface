# ESP32-WiFi-Scan-Web-Interface
"Create a WiFi scanner with a web interface on your ESP32. This project scans for networks, displays results in a web page, and includes filtering and sorting options.
# ESP32 WiFi Scanner

This Arduino sketch for the ESP32 creates a simple web server that scans for nearby WiFi networks and displays the results in a web page.  The web page shows the SSID, RSSI, and Channel of each network, along with a channel occupancy chart.  Client-side JavaScript is used to provide filtering and sorting functionality.

## Features

*   Scans for WiFi networks.
*   Displays SSID, RSSI, and Channel.
*   Displays channel occupancy.
*   Client-side filtering by SSID.
*   Client-side sorting by SSID, RSSI, or Channel.
*   Channel coloring for better visualization.
*   Responsive web interface.

## Hardware Requirements

*   ESP32 development board

## Software Requirements

*   Arduino IDE
*   ESP32 board support in Arduino IDE

## How to Use

1.  Clone this repository.
2.  Open the `your_sketch_name.ino` file in the Arduino IDE.
3.  Replace `"YOUR_AP_SSID"` and `"YOUR_AP_PASSWORD"` with your desired credentials.
4.  Upload the sketch to your ESP32.
5.  Connect to the ESP32's access point (SSID: ESP3C3\_WiFi\_Scanner, password: password - change this!).
6.  Open a web browser and navigate to the ESP32's IP address (displayed in the Serial Monitor).

## Contributing

Contributions are welcome!  Please open an issue or submit a pull request.

## License

[Choose a license - e.g., MIT, GPL, Apache 2.0]
