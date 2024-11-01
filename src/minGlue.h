/*  Glue functions for the minIni library, based on the Arduino libraries
 *
 *  By Bizan Nishimura, 2020
 *  This "glue file" is in the public domain. It is distributed without
 *  warranties or conditions of any kind, either express or implied.
 *
 *  (The Arduino libraries are copyright by Arduino Team and the others.
 *   They are licensed at their own terms.)
 */
 
/*
 * minIniFS por GuerraTron24 <dinertron@gmail.com> es una modificación de "minIni4Arduino"
 * 
 * Ahora puede trabajar tanto con SD como con SPIFFS. 
 * Para ello se le ha añadido una función estática "ini_FS(..)" que 
 * habría que llamar antes de los métodos de lectura y escritura. 
 * 
 * A esta función se le pasará el Sistema de Archivos deseado:
 * 
 * minIniFS ini("/config.ino");
 * ini_FS(SPIFFS); //leerá el archivo desde el SPIFFS
 * Serial.println("From SPIFFS: ");
 * Serial.println(ini.gets("section1", "key1", "default1"));
 * 
 * ini_FS(SD); //leerá el archivo desde la SD
 * Serial.println("From SD: ");
 * Serial.println(ini.gets("section1", "key1", "default1"));
 * 
 * También admite impresión "serial" para Arduino.
 * 
 *  Ahora el método "showKeysValues(..)" realiza una comprobación de la primera clave y 
 *  retorna un booleano indicando si encontró claves o no.
 * 
*/

#ifndef MINGLUE_H
#define MINGLUE_H

/* map required file I/O types and functions to the standard C library */
#include <stdio.h>
#include <Arduino.h>

/*#define NDEBUG //si hay errores en cláusulas 'assert'
#define INI_BUFFERSIZE  512 //de todas formas se establece en minIniFS.h por defecto a 512*/

/*#define FS_NO_GLOBALS //allow spiffs to coexist with SD card, define BEFORE including FS.h*/
#include <FS.h>
/*#include <SPI.h>*/
#include <SD.h>

/* INCLUYA ESTA LÍNEA AL INICIO DE SU PROPIO SKETCH .INO O ACTIVELA AQUÍ SI UTILIZA LittleFS EN VEZ DE SPIFFS */
/*#define USE_LittleFS*/

#ifdef USE_LittleFS
  /*#define CONFIG_LITTLEFS_SPIFFS_COMPAT 1
    #define CONFIG_LITTLEFS_CACHE_SIZE 512*/
  #define SPIFFS LITTLEFS
  #include <LITTLEFS.h> 
#else
  #include <SPIFFS.h>
#endif 

#define INI_FILETYPE                    fs::File
/* Manejador (Handler) de archivos. Sistema de Archivos. Tanto SD, como SPIFFS, como LittleFS son de tipo fs::FS& */
#define INI_FS                          fs::FS&

/*---------------------*/
/* IMPRESIÓN A DISPOSITIVO DE SALIDA: HABILITAR O DESHABILITAR (POR DEFECTO "ARDUINO") */
/*---------------------*/
#define INFO_HEAD "::minIniFS::"

#define INI_PRINT_NONE 
#define INI_PRINT_C         print
#define INI_PRINT_ARDUINO   Serial.print
/*INI_PRINT_NONE, INI_PRINT_C, INI_PRINT_ARDUINO, ..  */
 /** POR EJEMPLO: INI_PRINT(F("File exists, OK!.")); */
#define INI_PRINT(x)    (INI_PRINT_ARDUINO(x))
/*NONE: */
/*C: printf(f,v1,v2,v3,v4)*/
/*ARDUINO: Serial.printf(f,v1,v2,v3,v4)*/

#define INI_PRINTF_NONE 
#define INI_PRINTF_C         printf
#define INI_PRINTF_ARDUINO   Serial.printf
/*INI_PRINT_NONE, INI_PRINT_C, INI_PRINT_ARDUINO, .. */
/** ESTA MACRO NECESITA 4 ARGUMENTOS ADEMÁS DE LA CADENA CON FORMATO (tipo 'printf'), 
  * SI ALGUNO NO SE UTILIZA PASAR NULL (excepto el 1º)
  * POR EJEMPLO: INI_PRINTF4("--ARG: int:%d, string:%s--",1,"dos",NULL,NULL); */
#define INI_PRINTF4(f,v1,v2,v3,v4)   (INI_PRINTF_ARDUINO(f,v1,v2,v3,v4))
/*---------------------*/

/** Hace referencia al Sistema de Archivos (File System), la SD, SPIFFS o LittleFS. 
  * Para ello antes el usuario las habrá inicializado fuera de esta librería. */
static INI_FS _FS = SD; //por defecto la SD

/** Función para especificar el tipo de Sistema de Archivos (File System) a utilizar (SD, SPIFFS, LittleFS ..), tiene que ser de tipo 'fs::FS' */
static void ini_FS(INI_FS f_s)
{
    _FS = f_s; //(fs::FS&)SD, (fs::FS&)SPIFFS (fs::FS&)LittleFS
}

/** LECTURA: 
  * Prepara el descriptor de archivos pasado (*file) para operaciones de lectura.
  * Intenta abrir el archivo especificado a través de un Sistema de Archivos previamente seleccionado (SD - SPIFFS) 
  * a través de la función 'ini_FS(..)'.
  * El descriptor de archivos pasado (*file) será preparado para la operación de lectura con ese archivo, este se  
  * habrá creado de forma externa y será el llamador el encargado de cerrarlo cuando corresponda, fuera del ámbito 
  * de esta función. 
  * Retorna algún valor numérico representativo de la operación, si retorna "0", existe algún error de lectura, o 
  * no existe el archivo, no se puede leer!. */
static int ini_openread(const char* filename, INI_FILETYPE *file)
{
    /*INI_PRINTF4(INFO_HEAD "ini_openread('%s'):: \n",filename,NULL,NULL,NULL);*/
    /**/
    if (!_FS.exists(filename)){
        INI_PRINT(F("File doesn't exist or not FS mounted.. return 0\n"));/**/
        return 0;
    }
    *file = _FS.open(filename, FILE_READ);
    if(!file){
        INI_PRINT(F("Failed to open file for reading\n"));
        return 0;
    }
    /*INI_PRINT("\tname: "); INI_PRINT(file->name()); INI_PRINT("\n");
    //INI_PRINT(file->size()); INI_PRINT("\n");
    //return file->size();*/
    return (file->name() && ((file->name())[0] != 0) ); //PROTECCIÓN CONTRA file->name() == NULL == (DIRECTORY)
}

static int ini_openwrite(const char* filename, INI_FILETYPE *file)
{
    /*INI_PRINTF4(INFO_HEAD "ini_openwrite('%s'):: ",filename,NULL,NULL,NULL);*/
    *file = _FS.open(filename, FILE_WRITE);
    if(!file){
        INI_PRINT(F("Failed to open file for writing\n"));
        return 0;
    }
    /*INI_PRINT(F("\n"));
    //return ( (file->name())[0] != 0 );*/
    return (file->name() && ((file->name())[0] != 0) ); //PROTECCIÓN CONTRA file->name() == NULL ó EMPTY
}

/* #define ini_openrewrite(filename,file)   not defined*/

static int ini_close(INI_FILETYPE *file)
{
    file->close();
    return 1;
}

static int ini_read(char *buffer, int size, INI_FILETYPE *file)
{
    int i;
    
    for(i = 0; i < size-1; i++){
        if(file->available() > 0){
            char c = file->read();
            buffer[i] = c;
            if(c == '\n'){
                i++;
                break;
            }
        }else{
            break;
        }
    }
    buffer[i] = 0;
    
    return i;
}

static int ini_write(const char *buffer, INI_FILETYPE *file)
{
    int size = strlen(buffer);
    return ( file->write((uint8_t*)buffer, size) > 0);
}

static int ini_rename(const char *source, const char *dest)
{
    File srcFile = _FS.open(source, FILE_READ);
    if(!srcFile){
        INI_PRINT(F("RENAME:: Failed to open source-file for reading\n"));
        return 0;
    }
    /*if(srcFile.name()[0] == 0) return 0;*/
    if(srcFile.name() && ((srcFile.name())[0] == 0) ){ return 0; } //PROTECCIÓN CONTRA file->name() == NULL
    
    File dstFile = _FS.open(dest, FILE_WRITE);
    if(!dstFile){
        INI_PRINT(F("RENAME:: Failed to open source-file for writing\n"));
        if(!!srcFile){
            srcFile.close();
        }
        return 0;
    }
    /*if(dstFile.name()[0] == 0) return 0;*/
    if(dstFile.name() && ((dstFile.name())[0] == 0) ){ return 0; } //PROTECCIÓN CONTRA file->name() == NULL
    
    const int BUFF_SIZE = 512;
    char buffer[BUFF_SIZE];
    int size;
    while((size = srcFile.available()) > 0){
        if(size > BUFF_SIZE) size = BUFF_SIZE;
        srcFile.read((uint8_t*)buffer, size);
        dstFile.write((uint8_t*)buffer, size);
    }
    
    srcFile.close();
    dstFile.close();
    
    return _FS.remove(source);
}

#define ini_remove(filename)            ( _FS.remove(filename) )

#define INI_FILEPOS                     long int
#define ini_tell(file,pos)              ( *(pos) = (file)->position() )
#define ini_seek(file,pos)              ( (file)->seek(*(pos)) )

/* for floating-point support, define additional types and functions */
#define INI_REAL                        float
#define ini_ftoa(string,value)          sprintf((string),"%f",(value))
//#define ini_ftoa(string, value)         string = String(value);
#define ini_atof(string)                (INI_REAL)strtod((string),NULL)

#endif /*MINGLUE_H*/
