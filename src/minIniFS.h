/*  minIni - Multi-Platform INI file parser, suitable for embedded systems
 *
 *  Copyright (c) CompuPhase, 2008-2017
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may not
 *  use this file except in compliance with the License. You may obtain a copy
 *  of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 *  License for the specific language governing permissions and limitations
 *  under the License.
 *
 *  Version: $Id: minIniFS.h 53 2015-01-18 13:35:11Z thiadmer.riemersma@gmail.com $
 */
 
 /*  minIniFS - Arduino INI file parser, suitable for embedded systems
 *  Based: It's based in the minIni library from "minIni4Arduino"
 *
 *  Copyright (c) GuerraTron24, 2024-2033
 *
 *  Licensed under the Apache License, Version 2.0
 *
 *  Version: 1.0.0 $ minIniFS.h 2024-09-16 13:35:11Z guerratron24.dinertron@gmail.com $
 */
#ifndef MININIFS_H
#define MININIFS_H

#include "minGlue.h"

#if (defined _UNICODE || defined __UNICODE__ || defined UNICODE) && !defined INI_ANSIONLY
  #include <tchar.h>
  #define mTCHAR TCHAR
#else
  /* force TCHAR to be "char", but only for minIniFS */
  #define mTCHAR char
#endif

#if !defined INI_BUFFERSIZE
  #define INI_BUFFERSIZE  512
#endif

#if defined __cplusplus
  extern "C" {
#endif


int   ini_getbool(const mTCHAR *Section, const mTCHAR *Key, int DefValue, const mTCHAR *Filename);
long  ini_getl(const mTCHAR *Section, const mTCHAR *Key, long DefValue, const mTCHAR *Filename);
int   ini_gets(const mTCHAR *Section, const mTCHAR *Key, const mTCHAR *DefValue, mTCHAR *Buffer, int BufferSize, const mTCHAR *Filename);
int   ini_getsection(int idx, mTCHAR *Buffer, int BufferSize, const mTCHAR *Filename);
int   ini_getkey(const mTCHAR *Section, int idx, mTCHAR *Buffer, int BufferSize, const mTCHAR *Filename);

#if defined INI_REAL
INI_REAL ini_getf(const mTCHAR *Section, const mTCHAR *Key, INI_REAL DefValue, const mTCHAR *Filename);
#endif

#if !defined INI_READONLY
int   ini_putl(const mTCHAR *Section, const mTCHAR *Key, long Value, const mTCHAR *Filename);
int   ini_puts(const mTCHAR *Section, const mTCHAR *Key, const mTCHAR *Value, const mTCHAR *Filename);
#if defined INI_REAL
int   ini_putf(const mTCHAR *Section, const mTCHAR *Key, INI_REAL Value, const mTCHAR *Filename);
#endif
#endif /* INI_READONLY */

#if !defined INI_NOBROWSE
typedef int (*INI_CALLBACK)(const mTCHAR *Section, const mTCHAR *Key, const mTCHAR *Value, void *UserData);
int  ini_browse(INI_CALLBACK Callback, void *UserData, const mTCHAR *Filename);
#endif /* INI_NOBROWSE */

#if defined __cplusplus
  }
#endif


#if defined __cplusplus

/*#if defined __WXWINDOWS__
	#include "wxMinIni.h"
#else*/

  /* The C++ class in minIniFS.h was contributed by Steven Van Ingelgem. */
  class minIniFS
  {
  public:
    minIniFS(const String& filename) : iniFilename(filename)
      { }/*{ iniFilename = filename; }*/
    ~minIniFS(){ iniFilename = ""; }

    bool getbool(const String& Section, const String& Key, bool DefValue=false) const
      { return ini_getbool(Section.c_str(), Key.c_str(), int(DefValue), iniFilename.c_str()) != 0; }

    long getl(const String& Section, const String& Key, long DefValue=0) const
      { return ini_getl(Section.c_str(), Key.c_str(), DefValue, iniFilename.c_str()); }

    int geti(const String& Section, const String& Key, int DefValue=0) const
      { return static_cast<int>(this->getl(Section, Key, long(DefValue))); }

    String gets(const String& Section, const String& Key, const String& DefValue="") const
      {
        char buffer[INI_BUFFERSIZE];
        ini_gets(Section.c_str(), Key.c_str(), DefValue.c_str(), buffer, INI_BUFFERSIZE, iniFilename.c_str());
        return buffer;
      }

    String getsection(int idx) const
      {
        char buffer[INI_BUFFERSIZE];
        ini_getsection(idx, buffer, INI_BUFFERSIZE, iniFilename.c_str());
        return buffer;
      }

    String getkey(const String& Section, int idx) const
      {
        char buffer[INI_BUFFERSIZE];
        ini_getkey(Section.c_str(), idx, buffer, INI_BUFFERSIZE, iniFilename.c_str());
        return buffer;
      }

#if defined INI_REAL
    INI_REAL getf(const String& Section, const String& Key, INI_REAL DefValue=0) const
      { return ini_getf(Section.c_str(), Key.c_str(), DefValue, iniFilename.c_str()); }
#endif

#if ! defined INI_READONLY
    bool put(const String& Section, const String& Key, long Value)
      { return ini_putl(Section.c_str(), Key.c_str(), Value, iniFilename.c_str()) != 0; }

    bool put(const String& Section, const String& Key, int Value)
      { return ini_putl(Section.c_str(), Key.c_str(), (long)Value, iniFilename.c_str()) != 0; }

    bool put(const String& Section, const String& Key, bool Value)
      { return ini_putl(Section.c_str(), Key.c_str(), (long)Value, iniFilename.c_str()) != 0; }

    bool put(const String& Section, const String& Key, const String& Value)
      { return ini_puts(Section.c_str(), Key.c_str(), Value.c_str(), iniFilename.c_str()) != 0; }

    bool put(const String& Section, const String& Key, const char* Value)
      { return ini_puts(Section.c_str(), Key.c_str(), Value, iniFilename.c_str()) != 0; }

#if defined INI_REAL
    bool put(const String& Section, const String& Key, INI_REAL Value)
      { return ini_putf(Section.c_str(), Key.c_str(), Value, iniFilename.c_str()) != 0; }
#endif

    bool del(const String& Section, const String& Key)
      { return ini_puts(Section.c_str(), Key.c_str(), 0, iniFilename.c_str()) != 0; }

    bool del(const String& Section)
      { return ini_puts(Section.c_str(), 0, 0, iniFilename.c_str()) != 0; }
#endif

#if !defined INI_NOBROWSE
    bool browse(INI_CALLBACK Callback, void *UserData) const
      { return ini_browse(Callback, UserData, iniFilename.c_str()) != 0; }
#endif

    /** Método para listar todas las claves y valores encontrados en el archivo (El archivo debe existir). 
      * Debe permitirse la impresión a través de las macros: INI_PRINT y INI_PRINTF4. 
      * Basado en 'minIni.pdf'
      */
    void showKeysValues(){
        INI_PRINTF4(INFO_HEAD "showKeysValues():\n",NULL,NULL,NULL,NULL);
        int s, k;
        char section[40], key[40];
         /*for (s = 0; ini_getsection(s, section, sizeof section, iniFilename.c_str()) > 0; s++) {
            INI_PRINTF4(INFO_HEAD " [%s]\n",section,NULL,NULL,NULL);
            for (k = 0; ini_getkey(section, k, key, sizeof key, iniFilename.c_str()) > 0; k++){
                INI_PRINTF4("\t%s\n",key,NULL,NULL,NULL);
            }
        }*/
        String sec, ky, val;
        for (s = 0;  s < 20; s++) {
            sec = getsection(s);
            if(!sec || sec.length()<1){ break; }
            INI_PRINTF4("[%s]\n",sec,NULL,NULL,NULL);
            for (k = 0;  s < 20; k++){
                ky = getkey(sec, k);
                if(!ky || ky.length()<1){ break; }
                val = gets(sec, ky, "?");
                INI_PRINTF4("\t%s = \t%s\n",ky,val,NULL,NULL);
            }
        }
        sec = "";
        ky = "";
        val = "";
    }

  private:
    String iniFilename;
  };

 /*#endif __WXWINDOWS__ */
#endif /* __cplusplus */

#endif /* MININI_H */
