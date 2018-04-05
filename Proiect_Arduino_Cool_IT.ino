
#include "Adafruit_HTU21DF.h" //biblioteca senzor;
#include <SPI.h>              //biblioteca conexiune prin serial;
#include <Ethernet.h>         //biblioteca shield Ethernet;
#include <SimpleTimer.h>      //biblioteca contor de timp;
#include <LCD.h>              //2biblioteci display;
#include <LiquidCrystal_I2C.h>

// definire obiect contor de timp;
SimpleTimer timer;

//Accesarea senzorului;
Adafruit_HTU21DF htu = Adafruit_HTU21DF();

// variabile pentru senzor;
 float hum; int hum1 ;// definirea unei variabile int si a uneia float pentru temperatura si ...
 float temp;  int temp1 ;// umiditate;

//variabile pentru transmis datele
char link1[] = "GET /conect.php?nr=";             //partea de inceput a comenzii GET;
char link2[]= "&humid=";                          //partea de mijloc;
char link3[] = "&mes=Citire%20valida HTTP/1.1";   //partea de final;
char caracter_umid[16];                           //variabila de tip char pentru umiditate ..
char caracter_temp[16];                           //si tempeeratura;
char link_dest[200];                              //in aceasta variabila va fi stocat sirul de caractere ce va fi folosit pe post de comanda GET;
char c;

//Setare display
#define I2C_ADDR 0x3F
#define BACKLIGHT_PIN 3
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7
LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

  //setare buzzer
 const int pin_de_buzzer = 12; //buzzer to arduino pin 12 

//Variabile de configurare pentru shield-ul Ethernet;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //adresa MAC a dispozitivului;
char server[] = "www.aplimob.net";    //adresa serverului
IPAddress ip(192, 168, 0, 203);
EthernetClient client;

void setup() {
  // Pornire serial pentru debugging;
  Serial.begin(9600);
  while (!Serial) {
    ;} // wait for serial port to connect. Needed for native USB port only

    //verificarea functionalitate senzor;
   if (!htu.begin()) {
    Serial.println("Senzorul nu functioneaza!");
    while (1);
  }

  // pornirea conexiunii Ethernet;
  if (Ethernet.begin(mac) == 0)  //se verifica daca Arduino a primit IP prin DHCP de la router. Daca nu se va folosi IP-ul definit mai sus.
    {
    Serial.println("Nu s-a putut obtine adresa IP prin DHCP de la router");
    Serial.println("Nu s-a putut configura Ethern prin DHCP. Se va folosi adresa IP predefinita.");
    // Configurare manuala a adresei IP. 
    // Model de alegere IP predefinit: xxx.xxx.xxx.yyy. x - la fel ca celelate dispozitive din retea; y - valoare unica in retea.
    Ethernet.begin(mac, ip); //initializare Ethernet cu IP-ul definit mai sus;
  }
  // Asteptarea initializarii;
  delay(1000);

  Serial.println("Conectare...");
  Serial.print("IP: ");
  Serial.println(Ethernet.localIP());
 
 //setare lcd
  lcd.begin(16,2);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH); 
  
//setarea temporizatoarelor pentru diferite functii
  timer.setInterval(5000, temp_umid_pe_display ); // 5 secunde pentru refresh lcd
  timer.setInterval(90000, httpRequest );         // 1 minut si 30 de secunde pentru o cerere GET la server
}

void loop() {
//pornire timer
timer.run();

//pune valoarea de la senzor in variabile   
hum1 = htu.readHumidity();
temp1 = htu.readTemperature(); 
hum = htu.readHumidity(); //variabilele astea 2 de tip float sunt pentru display - arata mai bine cu valoarea scrisa cu virgula pe display
temp = htu.readTemperature();
 
//   If there are incoming bytes available from the server, read them and print them: - ADICA SA SCRIE RASPUNSUL SERVERULUI pe serial
 if (client.available()) {
     c = client.read();
    Serial.print(c);
}
//suna alarma
  if(temp > 25 || temp < 5)
  {buzzer();}
}
  
//functie de afisare a temperaturii si umiditatii pe display
void temp_umid_pe_display()
{ lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print(" ");
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Hum:  ");
  lcd.print(hum);
  lcd.print(" %"); }

//functie pentru trmis comanda get prin http
void httpRequest() 
{
  // ne asiguram ca orice conexiune e inchisa inainte sa o deschidem pe urmatoarea;
  client.stop();
  
  //pregatim comanda GET accesand o functie scrisa mai jos;
  creare_link_dest();
  
  //ne conectam la server;
    Serial.println("Conectare...");
  // daca ne-am conectat vom primi un mesaj in acest sens pe serial;
  if (client.connect(server, 80))  {
     Serial.println("conectat");
     
//Trimiterea comenzii GET catre server;
     client.println(link_dest);
     client.println("Host: www.aplimob.net");
     client.println("Connection: close");
     client.println();
  } else {
    // in caz ca nu primim raspun de la server va fi afisat pe serial urmatorul mesaj;
    Serial.println("conexiune esuata"); 
  }
    }
//functie pentru creare link de accesat cu functia de mai sus ;
  void creare_link_dest()
{
  // covertire in char a int. temp si umid;
itoa(temp1, caracter_temp, 10);
itoa(hum1, caracter_umid, 10);
  //creare link destinatie prin concatenare;
strcpy (link_dest, link1);
strcat (link_dest, caracter_temp);
strcat (link_dest, link2);
strcat (link_dest, caracter_umid);
strcat (link_dest, link3);
}
// functie pt buzzer;
  void buzzer() {tone(pin_de_buzzer, 1000, 500);}


