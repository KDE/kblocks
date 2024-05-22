/******************************************************************************
*   KBlocks, a falling blocks game by KDE                                     *
*   SPDX-FileCopyrightText: 2010-2021 Mauricio Piacentini <mauricio@tabuleiro.com>      *
*                           Zhongjie Cai <squall.leonhart.cai@gmail.com>      *
*                           Julian Helfferich <julian.helfferich@mailbox.org> *
*                                                                             *
*   SPDX-License-Identifier: GPL-2.0-or-later
******************************************************************************/
#ifndef KBLOCKSVIEW_H
#define KBLOCKSVIEW_H

#include <QGraphicsView>

class SceneInterface;
class KBlocksTheme;

class KBlocksView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit KBlocksView(SceneInterface *scene, QWidget *parent = nullptr);
    ~KBlocksView() override;

Q_SIGNALS:
    void focusEvent(bool flag);

public Q_SLOTS:
    void settingsChanged();

public:
    void loadTheme(const KBlocksTheme *theme);

protected:
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif

