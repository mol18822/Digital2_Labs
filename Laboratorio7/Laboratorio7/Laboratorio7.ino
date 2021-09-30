// Universidad del Valle de Guatemala
// Electrónica Digital II
// Laboratorio No. 7 - Manejo de memoria SD
// Luis Pedro Molina - 18822

// Inclusión de librerias
#include <SPI.h>
#include <SD.h>

// Declaración de variables
File root;
char option = '0';
uint8_t control = 0;
char storage;

// Prototipos de funciones
void printDirectory(File dir, int numTabs);
void readText(char dir);

void setup() {

// Inicio de comunicación serial y espera de respuesta de puerto  
  Serial.begin(9600);                                     // BaudRate 
  SPI.setModule(0);                                       // Puertos a usar

  Serial.print(" Iniciando lectura de tarjeta SD...\n "); // 

  pinMode(PA_3, OUTPUT);                                  // pin PA_3 como salida

// Si la lectura de la tarjeta SD falla 
  if (!SD.begin(PA_3)) {
    Serial.println("* Lectura de tarjeta SD fallida. Revise lo siguiente: ");
    Serial.println("* ¿Colocó correctamente la tarjeta SD? ");
    Serial.println("* ¿Cableó correctamente el circuito? ");
    Serial.println("* ¿Cambió el pin ChipSelect para que coincida con el módulo utilizado ? ");
    return;
  }
  
// Si la lectura de la tarjeta SD es exitosa  
  Serial.println(" Lectura de tarjeta SD exitosa... ");
  Serial.println(" El cableado es correcto y se detectó una tarjeta SD. ");

}

void loop() {
  
  if (control == 0) {
    root = SD.open("/");
    printDirectory(root, 0);
    Serial.println("\n Write the option you want to see... ");
    Serial.println(" Option 1 : Apple");
    Serial.println(" Option 2 : Nike");
    Serial.println(" Option 3 : Twitter");
    control++;                  // Menú aparece hasta que se haya seleccionado una opción
  }             
  
  if (Serial.available()) {
    storage = Serial.read();   // Guarda valor enviado por serial
  }
  
  if (storage == '1') {       // Si se ingresa un '1'
    option = '1';             // Se elige la opción '1' 
    control--;                // Se coloca en 0 control para que muestre el menu
  }
  
  if (storage == '2') {       // Si se ingresa un '2'
    option = '2';             // Se elige la opción '2'
    control--;                // Se coloca en 0 control para que muestre el menu
  }
  
  if (storage == '3') {       // Si se ingresa un '3'
    option = '3';             // Se elige la opción '3'
    control--;                // Se coloca en 0 control para que muestre el menu
  }
  
  readText(option);
  option = '0';               // Define option = 0 para que no muestre valores erroneos
}

void readText(char dir) {
  uint8_t seleccion;
  File archivo;
  switch (dir) {             // Abre el archivo seleccionado mediante el valor cargado a option
     
    case '1':
      archivo = SD.open("apple.txt");
      seleccion = 1; 
      break;
    case '2':
      archivo = SD.open("twitter.txt");
      seleccion = 1;
      break;
    case '3':
      archivo = SD.open("nike.txt");
      seleccion = 1;
      break;
    default:
      seleccion = 0;        // Deja de mostrar contenido
      break;
  }
  
  if (seleccion == 1) {     // Rutina que muestra la información seleccionada
    if (archivo) {
      Serial.println(archivo.name());

      // read from the file until there's nothing else in it:
      while (archivo.available()) {
        Serial.write(archivo.read());
      }
      // close the file:
      archivo.close();
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening file...");
    }
  }
}

void printDirectory(File dir, int numTabs) {//en esta rutina se muestra el contenido guardado en la SD
  dir.rewindDirectory();
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
