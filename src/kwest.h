/***************************************************************************
                          kwest.h  -  description
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

#ifndef KWEST_H
#define KWEST_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <vector>

// include files for Qt

// include files for KDE 
#include <kmainwindow.h>
#include <kaction.h>
#include <kurl.h>
#include <krecentfilesaction.h>
#include <ktoggleaction.h>
#include <ktempdir.h>

class KwestView;

class KwestApp : public KMainWindow
{
  Q_OBJECT

  public:

    KwestApp(QWidget* parent=0, const char* name=0);
    ~KwestApp() {};

    bool isShuttingDown() const {return shuttingDown;}

  protected:

    void saveGeneralOptions();
    void readOptions();

    void initActions();
    void initStatusBar();
    void initView();

    void setStoryRunning(bool);

    virtual bool queryClose();
    virtual bool queryExit();

  public slots:

    void openStory();
    void openStoryNamed(const QUrl& url);
    void saveGame();
    void restoreGame();
    void restartStory();
    void closeStory();
    void quit();
    void setTextFont();
    void setFixedFont();
    void setTextColor();
    void setBgColor();
    void saveDisplayOptions();
    void viewToolBar();
    void viewStatusBar();
    void setStatusMsg(const QString &text);
    void setINSMsg(const QString &text);

  private:

    bool frotzRunning, shuttingDown;
    KUrl startingOther;

    KConfig *config;

    KwestView *view;

    QAction* A_openStory;
    KRecentFilesAction* A_openStoryNamed;
    QAction* A_saveGame;
    QAction* A_restoreGame;
    QAction* A_restartStory;
    QAction* A_closeStory;
    QAction* A_quit;
    QAction* A_setTextFont;
    QAction* A_setFixedFont;
    QAction* A_setTextColor;
    QAction* A_setBgColor;
    QAction* A_saveDisplayOptions;
    QAction* A_viewToolBar;
    QAction* A_viewStatusBar;

    QString defaultPath;
    std::vector<KTempDir> tempFiles;
};
 
#endif // KWEST_H
