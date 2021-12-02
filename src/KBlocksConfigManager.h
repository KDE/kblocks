/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   SPDX-FileCopyrightText: 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   SPDX-License-Identifier: GPL-2.0-or-later
***************************************************************************/
#ifndef KBLOCKSCONFIGMANAGER_H_INCLUDED
#define KBLOCKSCONFIGMANAGER_H_INCLUDED

#include <stdio.h>
#include <string>
#include <map>

using namespace std;
class KBlocksConfigManager
{
private:
    map< int, string > stConfigSectionList;
    map< string, map< int, string > > stConfigKeyNameList;
    map< string, map< string, string > > stConfigDataTable;

    bool isDebug;

public:
    KBlocksConfigManager();
    ~KBlocksConfigManager();

    int SetDebugOutput(bool flag);

    int LoadConfigFile(const string &filename);
    int SaveConfigFile(const string &filename);

    int GetSectionCount();
    int GetKeyCount(const string &SectionName);

    int GetKeyString(const string &SectionName, const string &KeyName, string *KeyString, const string &Default);
    int GetKeyInt(const string &SectionName, const string &KeyName, int *KeyInt, const int Default);
    int GetKeyBool(const string &SectionName, const string &KeyName, bool *KeyBool, const bool Default);

    int SetKeyString(string SectionName, string KeyName, string KeyString);
    int SetKeyInt(string SectionName, string KeyName, int KeyInt);
    int SetKeyBool(const string &SectionName, const string &KeyName, bool KeyBool);

private:
    int ParseConfigFile(FILE *fp);
    int ConstructConfigFile(FILE *fp);

    string int16tostring(int input);
};

#endif // KBLOCKSCONFIGMANAGER_H_INCLUDED

