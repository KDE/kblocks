/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   Copyright (C) 2009-2021 Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
******************************************************************************/

#ifndef SCENEINTERFACE_H
#define SCENEINTERFACE_H

#include <QGraphicsScene>

class SceneInterface : public QGraphicsScene
{
public:
    SceneInterface() = default;
    virtual ~SceneInterface() = default;

public:
    virtual void readSettings(const QSize&) = 0;
};

#endif // SCENEINTERFACE_H
