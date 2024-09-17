# LIBRERÍA "minIni4Arduino/minIni.h" 

Pedazo de mini-librería para parsear LEER-ESCRIBIR archivos similares a los típicos ".ini" ó ".cfg" de Windows.

Leer el README.md

#### SOLO FUNCIONA SI SE AÑADE AL DIRECTORIO GLOBAL DE LIBRERÍAS DE ARDUINO

# AÑADIDO SOPORTE PARA SPIFFS
  (Modificación por GuerraTron24 <dinertron@gmail.com>)
  
Ahora puede trabajar tanto con SD como con SPIFFS (trabajará en archivos con un handler genérico tipo "fs::File").   
  
Para ello se le ha añadido una función estática "ini_FS(..)" que habría que llamar antes de los métodos de lectura y escritura.  
A esta función se le pasará el Sistema de Archivos deseado, por ejemplo:

```c
 minIni ini("/config.ino");

 ini_FS(SPIFFS); //leerá el archivo desde el SPIFFS
 Serial .println("From SPIFFS: ");
 Serial.println(ini.gets("section1", "key1", "default1"));
  
 ini_FS(SD); //leerá el archivo desde la SD
 Serial .println("From SD: ");
 Serial.println(ini.gets("section1", "key1", "default1"));
```

# AÑADIDO SOPORTE PARA PRINT Y PRINTF

Para seguir un rastro de la consecución de las acciones realizadas se le ha implementado un sinónimo de "print" y "printf".
Esto imprimirá en el dispositivo de salida (terminal, puerto serie, ..) 

Sólo hay que modificar la macro **INI_PRINT** seleccionando alguna de las posibles **INI_PRINT_NONE**, **INI_PRINT_C** ó **INI_PRINT_ARDUINO**.  
Por defecto **INI_PRINT_NONE** (O bien comentar las líneas que contengan esta macro).

Con la macro **INI_PRINTF4** las mismas indicaciones.

# TEST PARA COMPROBAR SECCIONES, CLAVES Y VALORES

Se ha incluido un método (**ini.showKeysValues()**) para listar todas las claves y valores encontrados en el archivo (El archivo debe existir).  
Debe permitirse la impresión a través de las macros: *INI_PRINT* y *INI_PRINTF4*. 

Basado en 'minIni.pdf'.

## ADAPTACIONES by GuerraTron24 <dinertron@gmail.com>:

He tenido que modificar las funciones "ini_openread(..), ini_openwrite(), ini_rename()" en el archivo "minGlue.h" porque provocaba un **ERROR FATAL** debido a que no comprobaba 
si el método "file.name()" retornaba NULL. 
Como ejemplo, una de las correcciones en las funciones quedaría así: 

```c
...
static int ini_openread(const char* filename, INI_FILETYPE *file)
{
    *file = SD.open(filename, FILE_READ);
    if(!file){
        Serial.println("Failed to open file for reading");
        return 0;
    }
    return (file->name() && ((file->name())[0] != 0) );
}
...
```

Con esto ya me permitía leer archivos. 

La librería SD es un poco delicada y si no le entregas el "slashbar" al inicio del nombre del archivo no es capaz de encontrarlo adecuadamente. Además existen diferentes versiones 
para distintos Micros como AVR, ESP, .. y podrían mezclarse.

También he tenido que realizar unos cuantos "casting" a variables tipo "char-array" para indicarle que las trate como "(uint8_t\*)", en los lugares que me iba indicando el 
compilador... por ejemplo en lo que son ahora las líneas: 69, 85, 86,.. del archivo "minGlue.h".

Algunos cambios menores más para adaptarla al IDE Arduino, como archivo properties, keywords, ..

## PRUEBAS OK

Le he pasado unas pruebas de lectura y escritura en la **SD** y la **SPIFFS** y todo ha ido GENIAL, parece ser que la librería deja una **Huella de Memoria** MUY PEQUEÑA comparado con todo 
lo que hace.  

Según he leído del autor **NO provoca fragmentación en el HEAP**, y sólo trata con un BÚFFER configurable por el usuario para la lectura de una línea completa (por defecto 512 bytes).

## PENDIENTE

Compatibilidad con LittleFS

### AGRADECIMIENTOS

Muchas gracias al autor de la librería: *Bizan Nishimura (lipoyang)* <http://lipoyang.net> es un GRAN TRABAJO.  
La librería la he descargado de: https://github.com/lipoyang/minIni4Arduino