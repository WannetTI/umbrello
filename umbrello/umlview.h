/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2011                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef UMLVIEW_H
#define UMLVIEW_H

#include <QtGui/QGraphicsView>

class QHideEvent;
class QCloseEvent;
class QShowEvent;
class UMLScene;
class UMLFolder;

/**
 * UMLView instances represent diagrams.
 * The UMLApp instance manages a QWidgetStack of UMLView instances.
 * The visible diagram is at the top of stack.
 * The UMLView class inherits from QCanvasView and it owns the
 * objects displayed on its related QCanvas (see m_WidgetList.)
 *
 * @author Paul Hensgen <phensgen@techie.com>
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */
class UMLView : public QGraphicsView
{
    Q_OBJECT
public:
    UMLView(UMLFolder *f );
    virtual ~UMLView();

    int zoom() const { return m_nZoom; }
    void setZoom(int zoom);
    int currentZoom();

    void hideEvent(QHideEvent *he);
    void showEvent(QShowEvent *se);

    UMLScene* umlScene() const;

protected:
    void closeEvent(QCloseEvent * e);

    void setCenter(const QPointF& centerPoint);
    QPointF center();

    virtual void wheelEvent(QWheelEvent* event);
    virtual void resizeEvent(QResizeEvent* event);

    QPointF m_currentCenterPoint;  ///< holds the current centerpoint for the view, used for panning and zooming
    QPoint  m_lastPanPoint;        ///< from panning the view
    int     m_nZoom;               ///< zoom level in percent, default 100

public slots:
    void zoomIn();
    void zoomOut();
    void show();
};

#endif // UMLVIEW_H
