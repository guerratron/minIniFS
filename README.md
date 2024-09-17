# minIniFS (INI file library) for the Arduino IoT world

### Acknowledgement

> This work is based on the "minIni4Arduino" library [minIni4Arduino](https://github.com/lipoyang/minIni4Arduino)

>  All of this is also derived from the great library of [https://github.com/compuphase/minIni](https://github.com/compuphase/minIni)

--------------------------------------------------------
# MODIFICATIONS TO THE ORIGINAL "minIni4Arduino" LIBRARY
--------------------------------------------------------
### NOTE
- This library is verified to work with ESP32 (DOIT DevKit v1) + SD-Shield

#### ONLY WORKS IF ADDED TO THE ARDUINO GLOBAL LIBRARY DIRECTORY

# ADDED SUPPORT FOR SPIFFS
(Modification by GuerraTron24 <dinertron@gmail.com>)

Now it can work with both SD and SPIFFS (it will work on files with a generic handler like "fs::File").

For this purpose, a static function "ini_FS(..)" has been added, which should be called before the read and write methods.
This function will be passed the desired File System, for example:

```c
minIni ini("/config.ino");

ini_FS(SPIFFS); //will read the file from the SPIFFS
Serial .println("From SPIFFS: ");
Serial.println(ini.gets("section1", "key1", "default1"));

ini_FS(SD); //will read the file from the SD
Serial . println("From SD: ");
Serial. println(ini. gets("section1", "key1", "default1"));
```

# ADDED SUPPORT FOR PRINT AND PRINTF

To keep track of the actions performed, a synonym for "print" and "printf" has been implemented.
This will print to the output device (terminal, serial port, ..)

You only have to modify the macro **INI_PRINT** by selecting one of the possible **INI_PRINT_NONE**, **INI_PRINT_C** or **INI_PRINT_ARDUINO**.
By default **INI_PRINT_NONE** (Or comment out the lines that contain this macro).

With the macro **INI_PRINTF4** the same instructions.

# TEST TO CHECK SECTIONS, KEYS AND VALUES

A method (**ini.showKeysValues()**) has been included to list all the keys and values ​​found in the file (The file must exist).
Printing must be allowed through the macros: *INI_PRINT* and *INI_PRINTF4*.

Based on 'minIni.pdf'.

## ADAPTATIONS by GuerraTron24 <dinertron@gmail.com>:

I had to modify the functions "ini_openread(..), ini_openwrite(), ini_rename()" in the file "minGlue.h" because it caused a **FATAL ERROR** due to not checking
if the method "file.name()" returned NULL. As an example, one of the corrections in the functions would be like this:

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

With this I was able to read files.

The SD library is a bit delicate and if you don't give it the "slashbar" at the beginning of the file name it is not able to find it properly. Also there are different versions
for different Micros like AVR, ESP, .. and they could be mixed up.

I also had to make a few "castings" to variables of the "char-array" type to tell it to treat them as "(uint8_t\*)", in the places that the compiler was telling me... for example in what are now the lines: 171, 200, 201, .. of the "minGlue.h" file.

Some more minor changes to adapt it to the Arduino IDE, such as the properties file, keywords, ..

## TESTS OK

I have run some reading and writing tests on the **SD** and the **SPIFFS** and everything has gone GREAT, it seems that the library leaves a VERY SMALL **Memory Footprint** compared to everything
it does.

According to what I have read from the author **it does NOT cause fragmentation in the HEAP**, and only deals with a user-configurable BUFFER for reading a complete line (by default 512 bytes).

## PENDING

Compatibility with LittleFS

### ACKNOWLEDGEMENTS

Many thanks to the author of the library: *Bizan Nishimura (lipoyang)* <http://lipoyang.net> it is a GREAT JOB.
I downloaded the library from: https://github.com/lipoyang/minIni4Arduino
