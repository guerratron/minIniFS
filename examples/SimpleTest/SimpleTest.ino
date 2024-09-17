/*
 * (SD + SPIFFS)
 * Ejemplo sencillo de la librería "minIniFS" by GuerraTron24 <dinertron@gmail.com> 
 * que lee las Secciones, Claves y Valores de un archivo tipo ini: "confMiFS.ini", 
 * y el resultado se mostrará en el "monitor Serial del IDE Arduino".
 * 
 * Las pruebas se han realizado sobre una placa "DOIT ESP32 DEVKIT V1" a80Mhz, con 
 * una velocidad de subida de 921600 baudios, y una velocidad del puerto serie de 115200.
 * 
 * Para el ejemplo completo se necesitaría insertar un shield SD y activar la "flash" 
 * (SPIFFS) a través de la herramienta "ESP32 Sketch Data Upload".
 * 
 * Además de copiar un archivo "confMiFS.ini" válido tanto en la SD como en la SPIFFS.
 * 
 * La SD se maneja en los pines por defecto de la placa seleccionada (seguramente VSPI).
 * 
 * El benchmark arroja unos resultados de:
 *  - Sketch 351257 bytes (26%) del espacio de almacenamiento de programa. (máx. 1310720 bytes), 
 *  - Var. Globales 21928 bytes ocupados (6%) de la memoria dinámica, 
 *  - Var. Locales 305752 bytes libres. (máx 327680 bytes).
 *  - heap interno: 304 K libre / 367 K total
 *  - heap spiram: 0 K libre / 0 K total
 * 
 * SUERTE Y AL TORO!
 */
#include <minIniFS.h>

bool sd_ok = false;
bool spiffs_ok = false;
bool initial = true;

void _show_title(char *title){
  Serial.println(" -----------------------------------");
  Serial.println(title);
  Serial.println(" -----------------------------------");
}

void setup()
{
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
  }

  _show_title("INI file test 'SimpleTest' start");

  //SD
  Serial.println("SD init");
  sd_ok = SD.begin();
  if (!sd_ok){ Serial.println("SD Card Mount Failed"); }

  /**/
  //SPIFFS
  Serial.println("SPIFFS init");
  spiffs_ok = SPIFFS.begin();
  if (!spiffs_ok){ Serial.println("SPIFFS Failed"); }

  char* fileName = "/confMiFS.ini";

  // Objeto representando la librería
  minIniFS ini(fileName);

  if(sd_ok){
    //Iniciamos el manejador de archivos
    ini_FS((fs::FS&)SD); //static
    _show_title("1. SECTIONS-KEYS-VALUES IN SD:");
    ini.showKeysValues();
  }
  if(spiffs_ok){
    ini_FS((fs::FS&)SPIFFS); //static
    _show_title("2. SECTIONS-KEYS-VALUES IN SPIFFS:");
    ini.showKeysValues();
  }
}

void loop(){
  if(initial){
    Serial.println();
    Serial.println(" ---- LOOP RUNNING ---");
    initial = false;
  }
}
