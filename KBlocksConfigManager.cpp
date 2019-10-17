/***************************************************************************
*   KBlocks, a falling blocks game by KDE                                *
*   Copyright (C) 2010 Zhongjie Cai <squall.leonhart.cai@gmail.com>       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "KBlocksConfigManager.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cctype>
#include <algorithm>

KBlocksConfigManager::KBlocksConfigManager()
{
    stConfigSectionList.clear();
    stConfigKeyNameList.clear();
    stConfigDataTable.clear();

    isDebug = false;
}

KBlocksConfigManager::~KBlocksConfigManager()
{
    stConfigSectionList.clear();
    stConfigKeyNameList.clear();
    stConfigDataTable.clear();
}

int KBlocksConfigManager::SetDebugOutput(bool flag)
{
    isDebug = flag;

    return 0;
}

int KBlocksConfigManager::LoadConfigFile(const string &filename)
{
    FILE *fp;
    int result;

    fp = fopen(filename.c_str(), "r");

    if (fp == NULL) {
        return -1;
    }

    result = ParseConfigFile(fp);

    fclose(fp);

    return result;
}

int KBlocksConfigManager::SaveConfigFile(const string &filename)
{
    FILE *fp;
    int result;

    fp = fopen(filename.c_str(), "w+");

    if (fp == NULL) {
        return -1;
    }

    result = ConstructConfigFile(fp);

    fclose(fp);

    return result;
}

int KBlocksConfigManager::GetSectionCount()
{
    return stConfigSectionList.size();
}

int KBlocksConfigManager::GetKeyCount(const string &SectionName)
{
    map< string, map<string, string> >::iterator it;

    it = stConfigDataTable.find(SectionName);

    if (it == stConfigDataTable.end()) {
        return -1;
    }

    map<string, string> tmpMap = it->second;

    return tmpMap.size();
}

int KBlocksConfigManager::GetKeyString(const string &SectionName, const string &KeyName, string *KeyString, const string &Default)
{
    map< string, map<string, string> >::iterator it;

    it = stConfigDataTable.find(SectionName);

    if (it == stConfigDataTable.end()) {
        *KeyString = Default;
        return -1;
    }

    map<string, string> tmpMap = it->second;

    if (tmpMap.find(KeyName) == tmpMap.end()) {
        *KeyString = Default;
        return -1;
    }

    *KeyString = tmpMap[KeyName];

    return 0;
}

int KBlocksConfigManager::GetKeyInt(const string &SectionName, const string &KeyName, int *KeyInt, const int Default)
{
    map< string, map<string, string> >::iterator it;

    it = stConfigDataTable.find(SectionName);

    if (it == stConfigDataTable.end()) {
        *KeyInt = Default;
        return -1;
    }

    map<string, string> tmpMap = it->second;

    if (tmpMap.find(KeyName) == tmpMap.end()) {
        *KeyInt = Default;
        return -1;
    }

    string tmpString = tmpMap[KeyName];
    char *endptr;

    if (tmpString.find("0x") == tmpString.npos) {
        *KeyInt = strtol(tmpString.c_str(), &endptr, 10);
    } else {
        *KeyInt = strtol(tmpString.c_str(), &endptr, 16);
    }

    return 0;
}

int KBlocksConfigManager::GetKeyBool(const string &SectionName, const string &KeyName, bool *KeyBool, const bool Default)
{
    string tmpValue;
    if (GetKeyString(SectionName, KeyName, &tmpValue, "") == -1) {
        *KeyBool = Default;
        return -1;
    }

    transform(tmpValue.begin(), tmpValue.end(), tmpValue.begin(), (int(*)(int))tolower);
    if (tmpValue.find("true") != tmpValue.npos) {
        *KeyBool = true;
    } else if (tmpValue.find("false") != tmpValue.npos) {
        *KeyBool = false;
    }

    return 0;
}

int KBlocksConfigManager::SetKeyString(string SectionName, string KeyName, string KeyString)
{
    map<int, string> tmpKeyName;
    map< string, map< int, string > >::iterator it_keyname;

    map<string, string> tmpKeyMap;
    map< string, map<string, string> >::iterator it_data;

    it_data = stConfigDataTable.find(SectionName);

    if (it_data == stConfigDataTable.end()) {
        stConfigDataTable.insert(pair< string, map<string, string> >(SectionName, tmpKeyMap));
        stConfigSectionList.insert(pair<int, string>(stConfigSectionList.size() + 1, SectionName));
        stConfigKeyNameList.insert(pair< string, map<int, string> >(SectionName, tmpKeyName));
    }

    it_data = stConfigDataTable.find(SectionName);

    tmpKeyMap = it_data->second;

    if (tmpKeyMap.find(KeyName) == tmpKeyMap.end()) {
        it_keyname = stConfigKeyNameList.find(SectionName);
        tmpKeyName = it_keyname->second;
        tmpKeyName.insert(pair<int, string>(tmpKeyName.size() + 1, KeyName));
        stConfigKeyNameList[SectionName] = tmpKeyName;

        tmpKeyMap.insert(pair<string, string>(KeyName, KeyString));
    } else {
        tmpKeyMap[KeyName] = KeyString;
    }

    stConfigDataTable[SectionName] = tmpKeyMap;

    return 0;
}

int KBlocksConfigManager::SetKeyInt(string SectionName, string KeyName, int KeyInt)
{
    map<int, string> tmpKeyName;
    map< string, map< int, string > >::iterator it_keyname;

    map<string, string> tmpKeyMap;
    map< string, map<string, string> >::iterator it_data;

    it_data = stConfigDataTable.find(SectionName);

    if (it_data == stConfigDataTable.end()) {
        stConfigDataTable.insert(pair< string, map<string, string> >(SectionName, tmpKeyMap));
        stConfigSectionList.insert(pair<int, string>(stConfigSectionList.size() + 1, SectionName));
        stConfigKeyNameList.insert(pair< string, map<int, string> >(SectionName, tmpKeyName));
    }

    it_data = stConfigDataTable.find(SectionName);

    tmpKeyMap = it_data->second;
    string tmpString = int16tostring(KeyInt);

    if (tmpKeyMap.find(KeyName) == tmpKeyMap.end()) {
        it_keyname = stConfigKeyNameList.find(SectionName);
        tmpKeyName = it_keyname->second;
        tmpKeyName.insert(pair<int, string>(tmpKeyName.size() + 1, KeyName));
        stConfigKeyNameList[SectionName] = tmpKeyName;

        tmpKeyMap.insert(pair<string, string>(KeyName, tmpString));
    } else {
        tmpKeyMap[KeyName] = tmpString;
    }

    stConfigDataTable[SectionName] = tmpKeyMap;

    return 0;
}

int KBlocksConfigManager::SetKeyBool(const string &SectionName, const string &KeyName, bool KeyBool)
{
    if (KeyBool) {
        return SetKeyString(SectionName, KeyName, "true");
    } else {
        return SetKeyString(SectionName, KeyName, "false");
    }
}

int KBlocksConfigManager::ParseConfigFile(FILE *fp)
{
    map< string, map< string, string > >::iterator it_section;

    map<int, string> tmpKeyName;
    map< string, map< int, string > >::iterator it_keyname;

    map<string, string> tmpKeyMap;
    map< string, map< string, string > >::iterator it_data;

    stConfigSectionList.clear();
    stConfigKeyNameList.clear();
    stConfigDataTable.clear();

    string curSection = "DefaultSection";
    string curKey = "DefaultKey";
    string curValue = "DefaultValue";

    int lenth;
    string tmpString;
    char tmpBuff[1024];
    while (fgets(tmpBuff, 1024, fp)) {
        switch (tmpBuff[0]) {
        case '[':
            tmpString = string(tmpBuff);
            lenth = tmpString.find(']');
            curSection = tmpString.substr(1, lenth - 1);

            it_section = stConfigDataTable.find(curSection);

            if (it_section == stConfigDataTable.end()) {
                map<string, string> tmpKeyMap;
                stConfigDataTable.insert(pair< string, map<string, string> >(curSection, tmpKeyMap));

                map<int, string> tmpKeyName;
                stConfigKeyNameList.insert(pair< string, map<int, string> >(curSection, tmpKeyName));

                stConfigSectionList.insert(pair<int, string>(stConfigSectionList.size() + 1, curSection));

                if (isDebug) {
                    printf("New section loaded <%s>\n", curSection.c_str());
                }
            } else {
                if (isDebug) {
                    printf("Existing section updated <%s>\n", curSection.c_str());
                }
            }
            break;

        case '#':
            // skip this line
            break;

        case '\n':
            // skip this line
            break;

        case '\0':
            // skip this line
            break;

        default:
            tmpString = string(tmpBuff);
            lenth = tmpString.find('=');
            curKey = tmpString.substr(0, lenth);
            if (tmpString[tmpString.size() - 1] == '\n') {
                tmpString[tmpString.size() - 1] = '\0';
            }
            curValue = tmpString.substr(lenth + 1);
            transform(curValue.begin(), curValue.end(), curValue.begin(), (int(*)(int))tolower);

            it_data = stConfigDataTable.find(curSection);
            if (it_data == stConfigDataTable.end()) {
                stConfigDataTable.insert(pair< string, map<string, string> >(curSection, tmpKeyMap));
                stConfigSectionList.insert(pair<int, string>(stConfigSectionList.size() + 1, curSection));
                stConfigKeyNameList.insert(pair< string, map<int, string> >(curSection, tmpKeyName));

                if (isDebug) {
                    printf("New section added <%s>\n", curSection.c_str());
                }
            } else {
                tmpKeyMap = it_data->second;
            }

            if (tmpKeyMap.find(curKey) == tmpKeyMap.end()) {
                it_keyname = stConfigKeyNameList.find(curSection);
                tmpKeyName = it_keyname->second;
                tmpKeyName.insert(pair<int, string>(tmpKeyName.size() + 1, curKey));
                stConfigKeyNameList[curSection] = tmpKeyName;

                if (isDebug) {
                    printf("New Key & Value loaded <%s=%s> for section <%s>\n", curKey.c_str(), curValue.c_str(), curSection.c_str());
                }
                tmpKeyMap.insert(pair<string, string>(curKey, curValue));
            } else {
                if (isDebug) {
                    printf("Existing Key & Value updated <%s=%s> for section <%s>\n", curKey.c_str(), curValue.c_str(), curSection.c_str());
                }
                tmpKeyMap[curKey] = curValue;
            }
            stConfigDataTable[curSection] = tmpKeyMap;
            break;
        }
    }

    return 0;
}

int KBlocksConfigManager::ConstructConfigFile(FILE *fp)
{
    for (unsigned int i = 1; i < stConfigSectionList.size() + 1; i++) {
        string tmpSectionName = stConfigSectionList[i];

        fprintf(fp, "[%s]\n", tmpSectionName.c_str());

        map<int, string> tmpKeyName = stConfigKeyNameList[tmpSectionName];
        map<string, string> tmpKeyMap = stConfigDataTable[tmpSectionName];

        for (unsigned int j = 1; j < tmpKeyName.size() + 1; j++) {
            string tmpKeyNameStr = tmpKeyName[j];
            string tmpKeyValueStr = tmpKeyMap[tmpKeyName[j]];

            fprintf(fp, "%s=%s\n", tmpKeyNameStr.c_str(), tmpKeyValueStr.c_str());
        }
    }

    return 0;
}

string KBlocksConfigManager::int16tostring(int input)
{
    string tmpString = "";
    char tmpChar[2] = {0, 0};

    do {
        tmpChar[0] = input & 0x0F;
        input >>= 4;
        if (tmpChar[0] < 10) {
            tmpChar[0] += '0';
        } else {
            tmpChar[0] += 'a' - 10;
        }
        tmpString = string(tmpChar) + tmpString;
    } while (input != 0);

    tmpString = "0x" + tmpString;

    return tmpString;
}
