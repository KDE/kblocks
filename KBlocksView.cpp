#include "KBlocksView.h"

#include <KDebug>
#include <QResizeEvent>

KBlocksView::KBlocksView(QWidget * parent): QGraphicsView(parent)
{
    mpGameScene = 0;
    
    //setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setFrameStyle(QFrame::NoFrame);

    setOptimizationFlags( QGraphicsView::DontClipPainter |
        QGraphicsView::DontSavePainterState /*|
        QGraphicsView::DontAdjustForAntialiasing*/ );
    setCacheMode(QGraphicsView::CacheBackground);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
}

KBlocksView::~KBlocksView()
{
}

void KBlocksView::setGameScene(KBlocksScene * p)
{
    mpGameScene = p;
    setScene(mpGameScene);
}

void KBlocksView::settingsChanged()
{
    if (mpGameScene == 0)
    {
        return;
    }
    mpGameScene->readSettings(size());
    fitInView(mpGameScene->sceneRect(), Qt::KeepAspectRatio);
}

void KBlocksView::focusInEvent(QFocusEvent *)
{
    if (mpGameScene == 0)
    {
        return;
    }
    mpGameScene->pauseGame(false, true);
}

void KBlocksView::focusOutEvent(QFocusEvent *)
{
    if (mpGameScene == 0)
    {
        return;
    }
    mpGameScene->pauseGame(true, true);
}

void KBlocksView::resizeEvent(QResizeEvent * event)
{
    if (mpGameScene == 0)
    {
        return;
    }
    fitInView(mpGameScene->sceneRect(), Qt::KeepAspectRatio);
    event->accept();
}

