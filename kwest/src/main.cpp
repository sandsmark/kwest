/***************************************************************************
                          main.cpp  -  description
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

#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>
#include <kurl.h>

#include <setjmp.h> // A nasty shortcut to return from main
jmp_buf exit_jump;  // ----------------''------------------

#include "kwest.h"

static const char *description =
  I18N_NOOP("Kwest - a Z-code interpreter based on Frotz 2.43");

static KCmdLineOptions options[] =
{
  { "+[File]", I18N_NOOP("file to open"), 0 },
  { 0, 0, 0 }
};

int main(int argc, char *argv[])
{
  KAboutData aboutData("kwest", I18N_NOOP("Kwest"),
    VERSION, description, KAboutData::License_GPL,
    "2001-2007, Peter Bienstman, based on code by David Griffith and others.",
    0,"http://kwest.sourceforge.net",
    "Peter.Bienstman@UGent.be");
  aboutData.addAuthor("Peter Bienstman",
                      "Main author",
                      "Peter.Bienstman@UGent.be",
                      "http://kwest.sourceforge.net");
  aboutData.addAuthor("Simon Baldwin",
                      "Bug fixes",
                      "simon_baldwin@yahoo.com",
                      "http://www.geocities.com/simon_baldwin");
  aboutData.addAuthor("Rafal Rzepecki",
                      "Bug fixes",
                      "divide@thepentagon.com");
  aboutData.addAuthor("David Griffith",
                      "Frotz Unix port, Frotz reference code",
                      "dgriffi@cs.csubak.edu",
                      "http://www.cs.csubak.edu/~dgriffi/frotz/");
  aboutData.addAuthor("Jim Dunleavy",
                      "Frotz reference code",
                      "jim.dunleavy@erha.ie");
  aboutData.addAuthor("Galen Hazelwood",
                      "Original Frotz Unix port",
                      "galenh@micron.net");
  aboutData.addAuthor("Stefan Jokisch",
                      "Original Frotz reference code",
                      "s.jokisch@avu.de");
  aboutData.addAuthor("L. Ross Raszewski",
                      "Treaty of Babel code",
                      "rraszews@trenchcoatsoft.com");
  
  KCmdLineArgs::init(argc, argv, &aboutData);
  KCmdLineArgs::addCmdLineOptions(options);

  KApplication app;

  if (app.isRestored())
  {
    RESTORE(KwestApp);
  }
  else 
  {
    KwestApp *kwest = new KwestApp();
    kwest->show();

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

    if (args->count())
    {
      KURL url("file:///");
      if (args->arg(0)[0] != '/')
        url.addPath(getcwd(NULL, 0));
      url.addPath(args->arg(0));

      kwest->openStoryNamed(url);
    }

    args->clear();

    if (kwest->isShuttingDown())
      return 0;
  }

  // On selecting the Quit button of the interface, all of the widgets are
  // destroyed before stack frames can be unwound.  Normally, under these
  // circumstances, an application would simply call exit().  However, this
  // is the world of KDE/Qt, and such things are frowned upon.  So, as an
  // expedient fix, this module offers a global setjmp/longjmp buffer,
  // exit_jump.  A longjmp to exit_jump forces an immediate return from
  // main() with return code 0. (SB)
  
  // Not needed anymore? (see kwest.ccp) (PB)

  //if (setjmp(exit_jump) != 0)
  //   return 0;

  return app.exec();
}
