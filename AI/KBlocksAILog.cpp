/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                 *
*   Copyright (C) 2010 University Freiburg <squall.leonhart.cai@gmail.com> *
*                                                                          *
*   This program is free software; you can redistribute it and/or modify   *
*   it under the terms of the GNU General Public License as published by   *
*   the Free Software Foundation; either version 2 of the License, or      *
*   (at your option) any later version.                                    *
***************************************************************************/
#include "KBlocksAILog.h"
#include <string.h>

/*****************************************************************
**  screen / file - setting  *************************************
*****************************************************************/
static const int MAX_CHANNEL = 10;
static const int SCREEN_CHANNEL = MAX_CHANNEL;
static int current_channel = SCREEN_CHANNEL;
static FILE *channel[MAX_CHANNEL] = {0};
static bool is_channel_open[MAX_CHANNEL] = {false};

void set_screen_channel()
{
    current_channel = SCREEN_CHANNEL;
}

void set_channel(int ch)
{
    if (abs(ch) >= MAX_CHANNEL) {
        set_screen_channel();
    } else {
        current_channel = ch;
    }
}

void set_screen_file_channel(int ch)
{
    set_channel(-ch);
}

void open_file(const char *fn)
{
    open_file(abs(current_channel), fn);
}

void open_file(int ch, const char *fn)
{
    set_channel(ch);
    ch = abs(current_channel);
    if ((ch == SCREEN_CHANNEL) || (is_channel_open[ch])) {
        return;
    }
    channel[ch] = fopen(fn, "w");
    is_channel_open[ch] = true;
}

void close_file()
{
    close_file(abs(current_channel));
}

void close_file(int ch)
{
    set_channel(ch);
    ch = abs(current_channel);
    if ((ch == SCREEN_CHANNEL) || (!is_channel_open[ch])) {
        return;
    }
    fclose(channel[ch]);
    is_channel_open[ch] = false;
}

void gotoXY(int x, int y)
{
#ifdef DEBUG
    char essq[100];
    char xstr[100];
    char ystr[100];

    sprintf(xstr, "%d", x);
    sprintf(ystr, "%d", y);

    essq[0] = '\0';
    strcat(essq, "\033[");
    strcat(essq, ystr);

    strcat(essq, "d");

    strcat(essq, "\033[");
    strcat(essq, xstr);
    strcat(essq, "G");

    printf("%s", essq);
#endif
}
bool isScreenChannel(int channel)
{
    return ((channel == MAX_CHANNEL) || (channel < 0));
}

bool isFileChannel(int channel)
{
    return ((abs(channel) < MAX_CHANNEL) && (is_channel_open[channel]));
}

/*****************************************************************
**  print element  ***********************************************
*****************************************************************/
void println()
{
#ifdef DEBUG
    if (isScreenChannel(current_channel)) {
        printf("\n");
    }
#endif
#ifdef LOG
    if (isFileChannel(current_channel)) {
        fprintf(channel[current_channel], "\n");
    }
#endif
}

void print(const char *s)
{
#ifdef DEBUG
    if (isScreenChannel(current_channel)) {
        printf("%s", s);
    }
#endif
#ifdef LOG
    if (isFileChannel(current_channel)) {
        fprintf(channel[current_channel], "%s", s);
    }
#endif
}

void println(const char *s)
{
    print(s);
    println();
}

void print(int i)
{
#ifdef DEBUG
    if (isScreenChannel(current_channel)) {
        printf("%d", i);
    }
#endif
#ifdef LOG
    if (isFileChannel(current_channel)) {
        fprintf(channel[current_channel], "%d", i);
    }
#endif
}

void println(int i)
{
    print(i);
    println();
}

void print(double d)
{
#ifdef DEBUG
    if (isScreenChannel(current_channel)) {
        printf("%f", d);
    }
#endif
#ifdef LOG
    if (isFileChannel(current_channel)) {
        fprintf(channel[current_channel], "%f", d);
    }
#endif
}

void println(double d)
{
    print(d);
    println();
}
/*****************************************************************
**  print object  ************************************************
*****************************************************************/
void println(KBlocksPiece *piece)
{
    const char *Piece_Type_Name[7] = {
        "PIECE_TYPE_Z", // 0
        "PIECE_TYPE_S", // 1
        "PIECE_TYPE_I", // 2
        "PIECE_TYPE_T", // 3
        "PIECE_TYPE_Q", // 4
        "PIECE_TYPE_L", // 5
        "PIECE_TYPE_J", // 6
    };
    println("PIECE");
    print("Type:"); println(Piece_Type_Name[piece->getType()]);
}

void println(KBlocksPiece *piece, bool full)
{
    println(piece, -1, 0, full);
}

void println(KBlocksPiece *piece, int x, int y, bool full)
{
    if (full) {
        if (x != -1) {
            gotoXY(x, ++y);
        }
        println(piece);
        if (x != -1) {
            gotoXY(x, ++y);
        }
        println("STATE");
        if (x != -1) {
            gotoXY(x, ++y);
        }
        print("Rotation Id   :"); println(piece->getRotation());
        if (x != -1) {
            gotoXY(x, ++y);
        }
        print("pos: (");
        print(piece->getPosX());
        print(",");
        print(piece->getPosY());
        println(")");
        println("Cells: ");
        for (int i = 0; i < KBlocksPiece_CellCount; ++i) {
            if (x != -1) {
                gotoXY(x, ++y);
            }
            print("[");
            print(piece->getCellPosX(i));
            print(",");
            print(piece->getCellPosY(i));
            println("]");
        }
    }
}

void println(KBlocksField *field)
{
    println(field, -1, 0);
}

void println(KBlocksField *field, int x, int y)
{
    int w = field->getWidth();
    int h = field->getHeight();
    for (int j = 0; j < h; ++j) {
        if (x != -1) {
            gotoXY(x, y + j + 1);
        }
        print("|");
        for (int i = 0; i < w; ++i) {
            if (field->getCell(i, j)) {
                print("*");
            } else {
                print(" ");
            }
        }
        println("|");
    }
}
