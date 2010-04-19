/***************************************************************************
*   KBlocks, a falling blocks game for KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
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
        
        int LoadConfigFile(string filename);
        int SaveConfigFile(string filename);
        
        int GetSectionCount();
        int GetKeyCount(string SectionName);
        
        int GetKeyString(string SectionName, string KeyName, string* KeyString, const string Default);
        int GetKeyInt(string SectionName, string KeyName, int* KeyInt, const int Default);
        int GetKeyBool(string SectionName, string KeyName, bool* KeyBool, const bool Default);
        
        int SetKeyString(string SectionName, string KeyName, string KeyString);
        int SetKeyInt(string SectionName, string KeyName, int KeyInt);
        int SetKeyBool(string SectionName, string KeyName, bool KeyBool);
        
    private:
        int ParseConfigFile(FILE* fp);
        int ConstructConfigFile(FILE* fp);
        
        string int16tostring(int input);
};

#endif // KBLOCKSCONFIGMANAGER_H_INCLUDED

