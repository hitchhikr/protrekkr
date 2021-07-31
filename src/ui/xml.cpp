// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2021 Franck Charlet.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  1. Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL FRANCK CHARLET OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.
// ------------------------------------------------------

// ------------------------------------------------------
// Includes
#if defined(__AROS__)
#define SDL_strcasecmp strcasecmp
#else
#include <SDL/SDL_stdinc.h>
#endif
#include "../include/ptk.h"

// ------------------------------------------------------
// Variables
list <skin_file> skin_files;

// Entities:
// - Background.
// - Button.
// - Static.
// - Container (black).
// - Vertical scrollbar.
// - Horizontal scrollbar.
// - Slider.
// - Textbox.
// - Spin (2 arrows with a number).
// - Picture.

TiXmlDocument doc("skins/skin.xml");
char xml_error[256];

extern char Window_Title[256];

// ------------------------------------------------------
// Functions
const char *XML_get_value(TiXmlHandle *handle, char *essemble, char *name);

void Print_Xml_Error(void)
{
    printf("XML (%d): %s\n", doc.ErrorId(), doc.ErrorDesc());
}

int XML_Init(void)
{
    if(!doc.LoadFile())
    {
        Print_Xml_Error();
        return(FALSE);
    }
    return(TRUE);
}

const char *XML_get_string(char *section, char *key, char *name, char *value)
{
    return(XML_get_value(section, key, name, value));
}

int XML_get_integer(char *section, char *key, char *name, char *value, int *dest)
{
    const char *dat = XML_get_string(section, key, name, value);
    if(!dat) return(FALSE);
    if(dat[0] == '#')
    {
        sscanf(&dat[1], "%x", dest);
    }
    else
    {
        sscanf(dat, "%d", dest);
    }
    return(TRUE);
}

const char *XML_get_value(char *tag, char *essemble, char *name, char *attr)
{
    TiXmlHandle *files;
    TiXmlHandle *file;
    TiXmlElement *element;
    TiXmlHandle hdl(&doc);
    const char *value;

    files = &hdl.FirstChildElement(tag);
    if(!files) return(NULL);

    file = &files->FirstChildElement(essemble);
    if(!file) return(NULL);

    element = file->Element();
    while(element)
    {
        if(SDL_strcasecmp(element->Attribute("name"), name) == 0)
        {
            value = element->Attribute(attr);
            while(*value == ' ' || *value == '\t')
            {
                *value++;
            }
            return(value);
        }
        element = element->NextSiblingElement();
    }
    return(NULL);
}
