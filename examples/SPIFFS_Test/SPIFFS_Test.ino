/*
 * (SOLO SPIFFS)
 * Ejemplo de escritura y lectura de la librería "minIniFS" by GuerraTron24 <dinertron@gmail.com> 
 * que lee las Secciones, Claves y Valores de un archivo tipo ini: "confMiFS.ini" y sobreescribe 
 * algunas claves. El resultado se muestra en el "monitor Serial del IDE Arduino".
 * 
 * Las pruebas se han realizado sobre una placa "DOIT ESP32 DEVKIT V1" a80Mhz, con 
 * una velocidad de subida de 921600 baudios, y una velocidad del puerto serie de 115200.
 * 
 * Para el ejemplo completo se necesita activar previamente la "flash" (SPIFFS) a través de la 
 * herramienta "ESP32 Sketch Data Upload".
 * 
 * Además de copiar un archivo "confMiFS.ini" válido en la SPIFFS, si no existe el archivo se 
 * creará uno con contenido básico, similar a:
 * ```
 * [user]
 * name = Isaac
 * name2 = Asimov
 * id = 1234
 * ```
 * 
 * La SD se maneja en los pines por defecto de la placa seleccionada (seguramente VSPI).
 * 
 * El benchmark arroja unos resultados de:
 *  - Sketch 314569 bytes (23%) del espacio de almacenamiento de programa. (máx. 1310720 bytes), 
 *  - Var. Globales 21472 bytes ocupados (6%) de la memoria dinámica, 
 *  - Var. Locales 306208 bytes libres. (máx 327680 bytes).
 *  - heap interno: 336 K libre / 373 K total
 *  - heap spiram: 0 K libre / 0 K total
 * 
 * SUERTE Y AL TORO!
 */
#include <minIniFS.h>

bool spiffs_ok = false;
bool initial = true;

void _show_title(char *title){
  Serial.println(" -----------------------------------");
  Serial.println(title);
  Serial.println(" -----------------------------------");
}

/** pequeña función en SPIFFS para listar los archivos en el directorio solicitado, 
  * con una profundidad de ruta determinada por levels. 
  * El tercer parámetro sólo se utiliza de manera interna. 
  * CUIDADO! Esta función es RECURSIVA. */
void _SPIFFS_listDir(const char * dirname, uint8_t levels = 0, uint8_t tabs = 0){
  String tab = "";
  for(; tab.length() < tabs; tab += "\t");
  Serial.printf("%s Listing directory: %s\n", tab.c_str(), dirname);

  File root = SPIFFS.open(dirname);
  if(!root){
    Serial.println(tab + "Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println(tab + "Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print(tab + "  DIR : ");
      Serial.println(file.name());
      if(levels){
        String dName = String(dirname) + "/" + String(file.name());
        Serial.println(dName);
        _SPIFFS_listDir(dName.c_str(), levels -1, tabs + 1);
      }
    } else {
      Serial.print(tab + "\tFILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
  file.close();
  root.close();
}

void setup()
{
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
  }

  _show_title("INI file test 'SPIFFS_Test' start");

  //SPIFFS
  Serial.println("SPIFFS init");
  spiffs_ok = SPIFFS.begin();
  if (!spiffs_ok){ Serial.println("SPIFFS Failed"); }

  char* fileName = "/confMiFS.ini";

  // Objeto representando la librería
  minIniFS ini(fileName);
  
  if(spiffs_ok){
    //si no existe el '.ini' se creará uno por defecto llamado "confMiFS.ini" con un contenido mínimo.
    ini_FS((fs::FS&)SPIFFS); //static
    _show_title("1. SECTIONS-KEYS-VALUES IN SPIFFS:");
    ini.showKeysValues();

    _show_title("| INI MODIFIED: |");
    Serial.println(" overwrite user.name, user.name2 and user.ID");
    //lee
    String name = ini.gets( "user", "name" , "Isaac" );
    String name2 = ini.gets( "user", "name2" , "Asimov" );
    long ID = ini.getl( "user", "id" , 1234 );
    //escribe
    ini.put( "user", "name" , name.equals("Isaac") ? "Carl" : "Isaac" );
    ini.put( "user", "name2" , name2.equals("Asimov") ? "Sagan" : "Asimov" );
    ini.put( "user", "ID" , ID == 1234 ? 4321 : 1234 );
    
    _show_title("2. SECTIONS-KEYS-VALUES IN SPIFFS:");
    ini.showKeysValues();

    _show_title("| SPIFFS List: |");
    _SPIFFS_listDir("/", 1);
  }
}

void loop(){
  if(initial){
    Serial.println();
    Serial.println(" ---- LOOP RUNNING ---");
    initial = false;
  }
}
