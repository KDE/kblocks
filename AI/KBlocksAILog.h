/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                 *
*   Copyright (C) 2010 University Freiburg <squall.leonhart.cai@gmail.com> *
*                                                                          *
*   This program is free software; you can redistribute it and/or modify   *
*   it under the terms of the GNU General Public License as published by   *
*   the Free Software Foundation; either version 2 of the License, or      *
*   (at your option) any later version.                                    *
***************************************************************************/
#ifndef KBLOCKSAILOG_H
#define KBLOCKSAILOG_H

#include "KBlocksAITypeDefine.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "../KBlocksPiece.h"
#include "../KBlocksField.h"

enum Log_Mode {
    LOG_MODE_PRINTSCREEN,
    LOG_MODE_PRINTFILE,
    LOG_MODE_PRINTBOTH
};

/*****************************************************************
**  screen setting  **********************************************
*****************************************************************/
#ifndef KSIRTET
void gotoXY(int x, int y);
#endif

/*****************************************************************
**  channel setting  *********************************************
*****************************************************************/
void set_channel(int ch);
void set_screen_channel();
void set_screen_file_channel(int ch);
void open_file(const char *fn);
void open_file(int ch, const char *fn);
void close_file();
void close_file(int ch);

/*****************************************************************
**  print element  ***********************************************
*****************************************************************/
void println();
void print(const char *s);
void println(const char *s);
void print(int i);
void println(int i);
void print(int i);
void println(int i);
void print(double d);
void println(double d);

/*****************************************************************
**  print object  ************************************************
*****************************************************************/
void println(KBlocksPiece *);
void println(KBlocksPiece *, bool full);
void println(KBlocksPiece *, int x, int y, bool full = false);

void println(KBlocksField *);
void println(KBlocksField *, int x, int y);

#endif
