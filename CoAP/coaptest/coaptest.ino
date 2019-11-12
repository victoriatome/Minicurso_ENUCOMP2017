#include <SPI.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <coap.h>

byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };

// CoAP client response callback
void callback_response(CoapPacket &packet, IPAddress ip, int port);

// UDP and CoAP class
EthernetUDP Udp;
Coap coap(Udp);

// CoAP client response callback
void callback_response(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("[Coap Response got]");

  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;

  Serial.println(p);
}

void setup() {
  Serial.begin(9600);

  Ethernet.begin(mac);
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }
  Serial.println();


  // client response callback.
  // this endpoint is single callback.
  Serial.println("Setup Response Callback");
  coap.response(callback_response);

  // start coap server/client
  coap.start();
}




void loop() {
  //Envia requisição
  int msgid = coap.get(IPAddress(192, 168, 0, 2), 5683, "time");
  
  //Aguarda 10 segundos
  delay(10000);
  coap.loop();
}
