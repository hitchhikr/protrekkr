// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2024 Franck Charlet.
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

#ifndef _CONFIG_H_
#define _CONFIG_H_

// ------------------------------------------------------
// Includes
#include "files.h"
#include "files_list.h"
#include "../../editors/include/editor_pattern.h"
#include "../../include/ptk.h"

#ifdef __HAIKU__
#include <FindDirectory.h>
#include <fs_info.h>
#define SET_FILENAME dev_t volume = dev_for_path("/boot");\
    status_t res = find_directory(B_USER_SETTINGS_DIRECTORY, volume, true, FileName, MAX_PATH); \
    if (res != B_OK) \
    { \
        Status_Box("Configuration File Directory Failed."); \
		return; \
    } \
	strcat(FileName, "/protrekkr")
#else
#define SET_FILENAME sprintf(FileName, "%s" SLASH "ptk.cfg", ExePath)
#endif
// ------------------------------------------------------
// Functions
void Load_Config(void);
void Save_Config(void);

#endif
