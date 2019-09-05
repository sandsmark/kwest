/***************************************************************************
                          kwestview.h  -  description
                             -------------------
    begin                : Sat Mar 24 16:14:15 CET 2001
    copyright            : (C) 2001 by Peter Bienstman
    email                : Peter.Bienstman@rug.ac.be
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KWESTVIEW_H
#define KWESTVIEW_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qwidget.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qtimer.h>

class KwestView : public QWidget
{
  Q_OBJECT
  public:

    typedef enum {None, Key, Line, Mouse, Timer} EventType;

    KwestView(QWidget *parent = 0, const char *name=0);
    ~KwestView();

    int getFontData(int font, int *height, int *width) const;

    void setXY(int  x, int  y)
      {flushLineBuffer(); xPos = x;  yPos = y;}

    void erase_screen();

    void setDefaultBgColor(const QColor& c) {defaultBgColor = c;}
    QColor getDefaultBgColor() const {return defaultBgColor;}
    void setBgColor(int c) {flushLineBuffer(); bgColor = colorMap[c];}
    void setBgColor(const QColor& c) {flushLineBuffer();bgColor = c;}
    void resetBgColor()              {bgColor = defaultBgColor;}

    void setDefaultFgColor(const QColor& c) {defaultFgColor = c;}
    QColor getDefaultFgColor() const {return defaultFgColor;}
    void setFgColor(int c) {flushLineBuffer(); fgColor = colorMap[c];}
    void setFgColor(const QColor& c) {flushLineBuffer(); fgColor = c;}
    void resetFgColor()              {fgColor = defaultFgColor;}

    void setTextFont (const QFont& f);
    void setFixedFont(const QFont& f);

    QFont getTextFont()  const {return  textFont[0][0];}
    QFont getFixedFont() const {return fixedFont[0][0];}

    void selectFontAndStyle(int font, int style);

    int charWidth(QChar c) const {return fm->horizontalAdvance(c);}

    void writeString(QString s);
    void flushLineBuffer();

    unsigned char readChar(int timeout = 0);
    unsigned char readLine(int max, unsigned char *buf,
      int timeout, int width, int continued);

    void forceInput(const QString& s) {forcedInput.append(s); event=Key;}

    void writeInputLine(int x, char* text, int len, int cursorpos);

    void morePrompt();

    void  erase_area(int top, int left, int bottom, int right);
    void scroll_area(int top, int left, int bottom, int right, int units);
    	
  protected:

    int xPos; // cursor position in Qt coordinates,
    int yPos; // i.e. starting from (0,0)

    void paintEvent(QPaintEvent* e);
    void resizeEvent(QResizeEvent* e);
    bool clearedOnInit;

    void keyPressEvent(QKeyEvent* e);
    unsigned char key;
    QStringList forcedInput;
    EventType event;

    QColor colorMap[13];

    QColor defaultFgColor, fgColor;
    QColor defaultBgColor, bgColor;
    bool reverse;

    QFont textFont[2][2];
    QFont fixedFont[2][2];

    QFontMetrics* fm;
    int fontHeight, padding;

    QPainter* p;
    QPixmap screenBuffer;
    QString lineBuffer;

    QTimer timer;

  protected slots:

   void timedOut();
};

#endif // KWESTVIEW_H
