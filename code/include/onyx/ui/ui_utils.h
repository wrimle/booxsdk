#ifndef ONYX_UI_UTILS_H_
#define ONYX_UI_UTILS_H_

#include <QtGui/QtGui>

namespace ui
{

// Return the screen geometry when transform.
QRect screenGeometry();

bool dockWidget(QWidget *target, QWidget * container, Qt::Alignment align);

int statusBarHeight();

int defaultFontPointSize();

QPoint globalTopLeft(QWidget *);

QPoint globalCenter(QWidget *);

int distance(QWidget * first, QWidget *second);

int distance(QPoint first, QPoint second);

};

#endif  // ONYX_UI_UTILS_H_

