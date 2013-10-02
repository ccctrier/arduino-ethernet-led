int green = 9;
int red = 7;
int blue = 8;
int led = 6;

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress ip(192,168,1,137);
int parseLine(String line) {

	if (line.length() <= 1) {

		Serial.println("Line ist kleiner als eins");
		return 1;
	
	} else if(line.indexOf("GET") == 0) {

		int help = 0; 
		int col;
		while(help < line.length()) {

			if(line.charAt(help) == 'r') {
				col = 7;
				help++;
			}
			if(line.charAt(help) == 'g') {
				col = 9;
				help++;
			}
			if(line.charAt(help) == 'b') {
				col = 8;
				help++;
			}

			if(line.charAt(help) == '2') {
				help++;
				if(line.charAt(help) == '5') {
					help++;
					if(line.charAt(help) == '5') {
						digitalWrite(col, LOW);
						help++;
					}
				}
			}

			if (line.charAt(help) == '0') {
				digitalWrite(col, HIGH);
				help++;
			} else {
				help++;
			}

		}
	}

} 
EthernetServer server(80);

void setup() {
	
 	// Open serial communications and wait for port to open:
	Serial.begin(9600);
	pinMode(red, OUTPUT);	 
	pinMode(green, OUTPUT); 
	pinMode(blue, OUTPUT);
	pinMode(led, OUTPUT);
	digitalWrite(led, HIGH);
	digitalWrite(red, HIGH);
	digitalWrite(green, HIGH);
	digitalWrite(blue, HIGH);
	
	 while (!Serial) {
		; // wait for serial port to connect. Needed for Leonardo only
	}

	Ethernet.begin(mac, ip);
	server.begin();
	Serial.print("server is at ");
	Serial.println(Ethernet.localIP());

}

void loop() {

	EthernetClient client = server.available();

	if (client) {
		Serial.println("new client");
		String line = "";
		while (client.connected()) {
			if (client.available()) {
				char c = client.read();
				if(c == '\n') {
					switch(parseLine(line)) {
						case 1:
					client.println("HTTP/1.1 200 OK");
					client.println("Content-Type: text/html");
					client.println("Connection: close"); 
					client.println("");
					client.println("Damariiiiis");
					delay(1);
					client.stop();
					break;
					
					}
					line = "";
				} else {
					line += c;
				}
			}
		}

		// give the web browser time to receive the data
		delay(1);
		// close the connection:
		client.stop();
		digitalWrite(blue, HIGH);
		digitalWrite(red, HIGH);
		Serial.println("client disconnected");

	}

}