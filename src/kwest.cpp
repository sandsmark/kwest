/***************************************************************************
                          kwest.cpp  -  description
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

#include <stdio.h>
#include <unistd.h>
#include <vector>

// include files for QT

#include <qdom.h>
#include <qdir.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qsizepolicy.h>

// include files for KDE

#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>
#include <kstandardaction.h>
#include <kfontdialog.h>
#include <kcolordialog.h>
#include <kstatusbar.h>
#include <ksplashscreen.h>
#include <ktoolbar.h>
#include <kapplication.h>

#include <setjmp.h>           // A nasty shortcut to return from main
extern "C" jmp_buf exit_jump; // ----------------''------------------

// application specific includes

#include "kwest.h"
#include "kwestview.h"

#include "k_frotz.h"
#include "frotz/frotz.h"
#include "babel/babel.h"

extern "C" int frotz_main(const char* filename, const char*);

#define ID_STATUS_MSG 1
#define ID_INS_MSG 2

////////////////////////////////////////////////////////////////////////////
//
// KwestApp::KwestApp
//
////////////////////////////////////////////////////////////////////////////

KwestApp::KwestApp(QWidget*, const char* name)
{
    setWindowTitle(name);
  config = kapp->sessionConfig();

  initStatusBar();
  initActions();
  initView();

  readOptions();

  setStoryRunning(false);

  shuttingDown = false;
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::initActions
//
////////////////////////////////////////////////////////////////////////////

void KwestApp::initActions()
{
  A_openStory = KStandardAction::open(this,SLOT(openStory()), this);
  A_openStoryNamed = KStandardAction::openRecent(this,
    SLOT(openStoryNamed(const QUrl&)), this);

  A_saveGame = KStandardAction::save(this,SLOT(saveGame()), this);
//  A_saveGame = new QAction(QIcon("save.png"), i18n("&Save game"));
//  connect(A_saveGame, &QAction::triggered, this, &KwestApp::saveGame);
//    0, this, SLOT(saveGame()), actionCollection(),"file_save");

  A_restoreGame = new QAction(QIcon("restore.png"), i18n("&Restore game"), this);
  connect(A_restoreGame, &QAction::triggered, this, &KwestApp::restoreGame);
  addAction(A_restoreGame);

  A_restartStory = new QAction(QIcon("restart.png"), i18n("&Restart story"), this);
  connect(A_restartStory, &QAction::triggered, this, &KwestApp::restartStory);
  addAction(A_restartStory);
//    0, this, SLOT(restoreGame()), actionCollection(),"file_restore");
//  A_restartStory = new KAction(i18n("&Restart story"), "restart.png",
//    0, this, SLOT(restartStory()), actionCollection(),"file_restart");

  A_closeStory = KStandardAction::close(this, SLOT(closeStory()), this);
  A_quit = KStandardAction::quit(this, SLOT(quit()), this);

  A_setTextFont = new QAction(i18n("&Text font"), this);//, 0, this,
  connect(A_setTextFont, &QAction::triggered, this, &KwestApp::setTextFont);
  addAction(A_setTextFont);
//    SLOT(setTextFont()), actionCollection(), "text_font");

  A_setFixedFont = new QAction(i18n("&Fixed font"), this);//, 0, this,
  connect(A_setFixedFont, &QAction::triggered, this, &KwestApp::setFixedFont);
  addAction(A_setFixedFont);
//    SLOT(setFixedFont()), actionCollection(), "fixed_font");

  A_setTextColor = new QAction(i18n("Text &color"), this);//, 0, this,
  connect(A_setTextColor, &QAction::triggered, this, &KwestApp::setTextColor);
  addAction(A_setTextColor);
//    SLOT(setTextColor()), actionCollection(), "text_color");

  A_setBgColor = new QAction(i18n("&Background color"), this);//, 0, this,
  connect(A_setBgColor, &QAction::triggered, this, &KwestApp::setBgColor);
  addAction(A_setBgColor);
//    SLOT(setBgColor()), actionCollection(), "bg_color");

  A_saveDisplayOptions = new QAction(i18n("&Save settings"), this);//, 0, this,
  connect(A_saveDisplayOptions, &QAction::triggered, this, &KwestApp::saveDisplayOptions);
  addAction(A_saveDisplayOptions);
//    SLOT(saveDisplayOptions()), actionCollection(), "save_display_options");

  A_viewToolBar = KStandardAction::create(KStandardAction::ShowToolbar, this, SLOT(viewToolBar()), this);
  A_viewStatusBar = KStandardAction::showStatusbar(this, SLOT(viewStatusBar()), this);

  A_openStory->setStatusTip(i18n("Opens a story"));
  A_openStoryNamed->setStatusTip(i18n("Opens a story"));
  A_saveGame->setStatusTip(i18n("Saves the curent game"));
  A_restoreGame->setStatusTip("Restores a saved game");
  A_restartStory->setStatusTip(i18n("Restarts the current story"));
  A_closeStory->setStatusTip(i18n("Closes the current story"));
  A_quit->setStatusTip(i18n("Quits the application"));
  A_setTextFont->setStatusTip(i18n("Changes the text font"));
  A_setFixedFont->setStatusTip(i18n("Changes the fixed font"));
  A_setTextColor->setStatusTip(i18n("Changes the text color"));
  A_setBgColor->setStatusTip(i18n("Changes the background color"));
  A_viewToolBar->setStatusTip(i18n("Enables/disables the toolbar"));
  A_viewStatusBar->setStatusTip(i18n("Enables/disables the statusbar"));

//  createGUI();
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::initStatusBar
//
////////////////////////////////////////////////////////////////////////////

void KwestApp::initStatusBar()
{
//  statusBar()->insertItem(i18n("Ready."), ID_STATUS_MSG);

//  statusBar()->insertItem(i18n("INS"),    ID_INS_MSG);

//  statusBar()->setItemAlignment
//    (ID_STATUS_MSG, AlignLeft | AlignVCenter | ExpandTabs);

//  statusBar()->setItemFixed(ID_STATUS_MSG, 300);

//  statusBar()->setItemFixed(ID_INS_MSG,
//    QFontMetrics(font()).width("OVR") + 5);
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::initView
//
////////////////////////////////////////////////////////////////////////////

void KwestApp::initView()
{
  view = new KwestView(this);
  view->setFocus();
  view->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
  setCentralWidget(view);
  global_kwestview = view;	
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::saveGeneralOptions
//
////////////////////////////////////////////////////////////////////////////

void KwestApp::saveGeneralOptions()
{	
  KConfigGroup cg = config->group("General Options");

  cg.writeEntry("Geometry", size());
  cg.writeEntry("Show Toolbar", A_viewToolBar->isChecked());
  cg.writeEntry("Show Statusbar",A_viewStatusBar->isChecked());
//  cg.writeEntry("ToolBarPos", (int) toolBar("mainToolBar")->pos());

  A_openStoryNamed->saveEntries(config->group("Recent Files"));

  cg.writeEntry("Default Path",defaultPath);
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::saveDisplayOptions
//
////////////////////////////////////////////////////////////////////////////

void KwestApp::saveDisplayOptions()
{	
  KConfigGroup cg = config->group("Display Options");

  cg.writeEntry("Text Font", view->getTextFont());
  cg.writeEntry("Fixed Font", view->getFixedFont());
  cg.writeEntry("Text Color", view->getDefaultFgColor());
  cg.writeEntry("Background Color", view->getDefaultBgColor());
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::readOptions
//
////////////////////////////////////////////////////////////////////////////

void KwestApp::readOptions()
{
  // General options.

  KConfigGroup cg = config->group("General Options");

  bool bViewToolbar = cg.readEntry("Show Toolbar", true);
  A_viewToolBar->setChecked(bViewToolbar);
  viewToolBar();

  bool bViewStatusbar = cg.readEntry("Show Statusbar", true);
  A_viewStatusBar->setChecked(bViewStatusbar);
  viewStatusBar();

//  KToolBar::BarPosition toolBarPos;
//  toolBarPos=(KToolBar::BarPosition)
//    config->readNumEntry("ToolBarPos", KToolBar::Top);
//  toolBar("mainToolBar")->setBarPos(toolBarPos);
	
  A_openStoryNamed->loadEntries(config->group("Recent Files"));

  QSize defaultSize(800,600);
  QSize size=cg.readEntry("Geometry",defaultSize);
  if(!size.isEmpty())
    resize(size);

  defaultPath = cg.readEntry("Default Path",0);

  // Display options.

  KConfigGroup cg2 = config->group("Display Options");

  QFont defaultText(font());
  QFont defaultFixed("fixed");

  view->setTextFont(cg2.readEntry("Text Font", defaultText));
  view->setFixedFont(cg2.readEntry("Fixed Font", defaultFixed));

  view->setDefaultFgColor(cg2.readEntry("Text Color",
    QColor(Qt::black)));
  view->setDefaultBgColor(cg2.readEntry("Background Color",
    QColor(Qt::white)));

  view->resetFgColor();
  view->resetBgColor();
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::setStoryRunning
//
////////////////////////////////////////////////////////////////////////////

void KwestApp::setStoryRunning(bool b)
{
  A_saveGame->setEnabled(b);
  A_restoreGame->setEnabled(b);
  A_restartStory->setEnabled(b);
  A_closeStory->setEnabled(b);

  frotzRunning = b;
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::queryClose
//
////////////////////////////////////////////////////////////////////////////

bool KwestApp::queryClose()
{
  if (shuttingDown || !frotzRunning)
  {
    for (unsigned int i=0; i<tempFiles.size(); i++)
      tempFiles[i].unlink();
    return true;
  }

  if (KMessageBox::questionYesNo(this,
  i18n("This will abandon the current game. OK?"))
    == KMessageBox::Yes)
  {
    view->forceInput(QChar(ZC_HKEY_QUIT));
    view->forceInput(QString("yes"));
    shuttingDown = true;

    for (unsigned int i=0; i<tempFiles.size(); i++)
      tempFiles[i].unlink();
  }

  return false; // Recapture flow at end of openStory.
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::queryExit
//
////////////////////////////////////////////////////////////////////////////

bool KwestApp::queryExit()
{
  saveGeneralOptions();

  return true;
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::openStory
//
////////////////////////////////////////////////////////////////////////////

void KwestApp::openStory()
{
  if (frotzRunning)
  {
    if (KMessageBox::questionYesNo(this,
      i18n("This will abandon the current game. OK?"))
        == KMessageBox::Yes)
    {
      view->forceInput(QChar(ZC_HKEY_QUIT));
      view->forceInput(QChar('y'));

      setStatusMsg(i18n("Opening story..."));

      KUrl url = KFileDialog::getOpenUrl(defaultPath,
        i18n("*.z? *.dat *.zblorb *.zlb"), this,
        i18n("Open story file..."));

      if (url.isEmpty())
        return;

      defaultPath = url.directory();

      startingOther = url;
    }
    return;
  }

  setStatusMsg(i18n("Opening story..."));

  KUrl url = KFileDialog::getOpenUrl(defaultPath,
    i18n("*.z? *.dat *.zblorb *.zlb"), this,
    i18n("Open story file..."));

  if (url.isEmpty())
    return;

  defaultPath = url.directory();

  openStoryNamed(url);
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::openStoryNamed
//
////////////////////////////////////////////////////////////////////////////

void KwestApp::openStoryNamed(const QUrl &url)
{
  if (frotzRunning)
  {
    if (KMessageBox::questionYesNo(this,
      i18n("This will abandon the current game. OK?"))
        == KMessageBox::Yes)
    {
      view->forceInput(QChar(ZC_HKEY_QUIT));
      view->forceInput(QChar('y'));
      startingOther = url;
    }
    return;
  }

  QString title("");
  QString author("");
  QString headline("");
  QString description("");

  // Check if this is a blorb file.

  KUrl newUrl(url);
  if (url.fileName().endsWith(".zblorb") || url.fileName().endsWith(".zlb"))
  {   
    char* result = babel_init(const_cast<char*>(url.path().toLatin1().constData()));

    if (result == NULL)
    {
      KMessageBox::error(this, i18n("Failed to open file."));
      return;
    }

    // Get primary IFID (i.e. the bit before comma), and unblorb the file.
    
    char IFID_[TREATY_MINIMUM_EXTENT];
    babel_treaty(GET_STORY_FILE_IFID_SEL,IFID_,TREATY_MINIMUM_EXTENT);
    QString IFID(IFID_);
    IFID = IFID.section(',',0,0);
    
    KTempDir tmp_dir;
    tempFiles.push_back(tmp_dir);
    chdir(tmp_dir.name().toLatin1().constData());
    
    babel_story_unblorb();
    babel_release();
    
    // Parsing the metadata.

    QDomDocument doc;
    QFile file(tmp_dir.name()+IFID+".iFiction");
    if(!file.open(QIODevice::ReadOnly))
    {
      KMessageBox::error(this, i18n("Failed to open iFiction file."));
      return;
    }

    if(!doc.setContent(&file))
    {
      file.close();
      KMessageBox::error(this, i18n("Failed to read iFiction file."));
      return;
    }
    file.close();

    QDomElement root = doc.documentElement();
    if(root.tagName() != "ifindex")
    {
      file.close();
      KMessageBox::error(this, i18n("Failed to parse iFiction file."));
      return;
    }     

    QDomNode n = root.firstChild();
    while (!n.isNull())
    {
      if (n.nodeName() == "story")
      {
        n = n.firstChild();
        while (!n.isNull())
        {
          if (n.nodeName() == "bibliographic")
          {
            n = n.firstChild();
            while (!n.isNull())
            {           
              QDomElement e = n.toElement();
              if (e.tagName().toLower() == "title")
                title = e.text();
              if (e.tagName().toLower() == "author")
                author = e.text();
              if (e.tagName().toLower() == "headline")
                headline = e.text();
              if (e.tagName().toLower() == "description")
                description = e.text();
              n = n.nextSibling();
            }
          }
          n = n.nextSibling();
        }
      }    
     n = n.nextSibling();
    }
    
    // Show splash screen.

    view->erase_screen();
    if (!title.isEmpty())
      setCaption(title, false);
    
    QMessageBox splash(title, "",
                       QMessageBox::Information,
                       QMessageBox::Ok | QMessageBox::Default,
                       QMessageBox::NoButton, QMessageBox::NoButton);

    splash.setTextFormat(Qt::RichText);
    splash.setText("<b>" + title + "</b><br><br>" +
                   author + "<br><br>" +
                   headline + "<br><br>"
                   + description);

    QPixmap p(tmp_dir.name()+IFID+".jpg");
    
    if (p.isNull())
      p.load(tmp_dir.name()+IFID+".png");
    
    float scale = 480./p.width();
    QMatrix m;
    m.scale(scale,scale);
    
    splash.setIconPixmap(p.transformed(m));
    
    if (not p.isNull() or description.length() != 0)
      splash.exec();
        
    // Update story name.

    newUrl = "file:///";
    newUrl.addPath(tmp_dir.name()+IFID+".blorb");
  }

  // Opening a non zblorb file, or the extracted story file from one.

  if (title.length() == 0)
    setCaption(url.fileName(), false);
  A_openStoryNamed->addUrl(url);

  setStatusMsg(i18n("Running story."));
  setStoryRunning(true);

  int result = frotz_main(newUrl.path().toLatin1().constData(), url.fileName().toLatin1().constData());

  if (result != 0)
    setCaption("", false);

  setStatusMsg(i18n("Ready."));
  setStoryRunning(false);

  // UI events can have set these variables while
  // frotz_main was running. They decide the further
  // flow of control.

  if (shuttingDown)
    quit();

  if (!startingOther.isEmpty())
  {
    KUrl newStory(startingOther);
    startingOther = "";
    openStoryNamed(newStory);
  }
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::restartStory
//
////////////////////////////////////////////////////////////////////////////

void KwestApp::restartStory()
{
  setStatusMsg(i18n("Restarting story..."));

  if (KMessageBox::questionYesNo(this,
    i18n("This will abandon the current game. OK?"))
      == KMessageBox::Yes)
  {
    view->forceInput(QChar(ZC_HKEY_RESTART));
    view->forceInput(QChar('y'));
  }

  setStatusMsg(i18n("Ready."));
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::saveGame
//
////////////////////////////////////////////////////////////////////////////

void KwestApp::saveGame()
{
  setStatusMsg(i18n("Saving game..."));
  view->forceInput(QString("save"));
  setStatusMsg(i18n("Ready."));
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::restoreGame
//
////////////////////////////////////////////////////////////////////////////

void KwestApp::restoreGame()
{
  setStatusMsg(i18n("Restoring game..."));
  view->forceInput(QString("restore"));
  setStatusMsg(i18n("Ready."));
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::closeStory
//
////////////////////////////////////////////////////////////////////////////

void KwestApp::closeStory()
{
  setStatusMsg(i18n("Closing story."));

  if (KMessageBox::questionYesNo(this,
    i18n("This will abandon the current game. OK?"))
      == KMessageBox::Yes)
  {
    view->forceInput(QChar(ZC_HKEY_QUIT));
    view->forceInput(QChar('y'));
  }

  setStatusMsg(i18n("Ready."));
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::quit
//
////////////////////////////////////////////////////////////////////////////

void KwestApp::quit()
{
  setStatusMsg(i18n("Exiting..."));

  saveGeneralOptions();

  // Close will refuse to comply if the user opts to cancel the operation.
  // In this case, reset the status message.  Otherwise, since the close
  // will have destroyed the complete widget set, we cannot, and dare not,
  // continue to unwind stack frames, so we'll longjmp() instead to the
  // return point of main().  This is not at all nice, but since we are
  // exiting anyway, it does the job for now.  Not doing this leads to
  // crashes and X windows errors in trying to manipulate now closed
  // widgets. (SB)
 
  // However, this doesn't seem to be needed anymore and causes 
  // "mutex destroy failure messages, and segaults when starting kwest 
  // with a command line argument. (PB)
  
  //if (close())
  //  longjmp(exit_jump, 1);
  
  close();

  if (frotzRunning)
    setStatusMsg(i18n("Running story."));
  else
    setStatusMsg(i18n("Ready."));
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::setTextFont
//
////////////////////////////////////////////////////////////////////////////

void KwestApp::setTextFont()
{
  QFont f;
  if (KFontDialog::getFont(f) == KFontDialog::Accepted)
    view->setTextFont(f);
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::setFixedFont
//
////////////////////////////////////////////////////////////////////////////

void KwestApp::setFixedFont()
{
  QFont f;
  if (KFontDialog::getFont(f) == KFontDialog::Accepted)
    view->setFixedFont(f);
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::setTextColor
//
////////////////////////////////////////////////////////////////////////////

void KwestApp::setTextColor()
{
  QColor c;
  if (KColorDialog::getColor(c) == KColorDialog::Accepted)
  {
    view->setDefaultFgColor(c);
    view->resetFgColor();
  }
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::setBgColor
//
////////////////////////////////////////////////////////////////////////////

void KwestApp::setBgColor()
{
  QColor c;
  if (KColorDialog::getColor(c) == KColorDialog::Accepted)
  {
    view->setDefaultBgColor(c);
    view->resetBgColor();
  }
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::viewToolBar
//
////////////////////////////////////////////////////////////////////////////

void KwestApp::viewToolBar()
{
  setStatusMsg(i18n("Toggling toolbar..."));

  if(!A_viewToolBar->isChecked())
    toolBar("mainToolBar")->hide();
  else
    toolBar("mainToolBar")->show();	

  setStatusMsg(i18n("Ready."));
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::viewStatusBar
//
////////////////////////////////////////////////////////////////////////////

void KwestApp::viewStatusBar()
{
  setStatusMsg(i18n("Toggle the statusbar..."));

  if(!A_viewStatusBar->isChecked())
    statusBar()->hide();
  else
    statusBar()->show();

  setStatusMsg(i18n("Ready."));
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::setStatusMsg
//
////////////////////////////////////////////////////////////////////////////

void KwestApp::setStatusMsg(const QString &text)
{
    statusBar()->showMessage(text);
//  statusBar()->clear();
//  statusBar()->changeItem(text, ID_STATUS_MSG);
}



////////////////////////////////////////////////////////////////////////////
//
// KwestApp::setINSMsg
//
////////////////////////////////////////////////////////////////////////////

void KwestApp::setINSMsg(const QString &text)
{
    statusBar()->showMessage(text);
//  statusBar()->clear();
//  statusBar()->changeItem(text, ID_INS_MSG);
}

