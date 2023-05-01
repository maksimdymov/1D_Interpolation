#include "window.h"
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QVBoxLayout>

int
main (int argc, char *argv[])
{
  QApplication app (argc, argv);
  QMainWindow *mwind = new QMainWindow;
  MainWindow *window = new MainWindow;
  QMenuBar *tool_bar = new QMenuBar (window);

  if (window->CheckArguments (argc, argv) != 0)
    {
      char *msg = new char[100];
      sprintf (msg, "Usage: %s a b n k", argv[0]);
      QMessageBox::warning (0, "Error input!", msg);

      return -1;
    }
  QAction *action;
  action
      = tool_bar->addAction ("&Change function", window, SLOT (ChangeFunc ()));
  action->setShortcut (QString ("0"));

  action = tool_bar->addAction ("&Change mode", window, SLOT (ChangeMode ()));
  action->setShortcut (QString ("1"));

  action = tool_bar->addAction ("&Increase function", window,
                                SLOT (IncreaseFunc ()));
  action->setShortcut (QString ("2"));

  action = tool_bar->addAction ("&Decrease function", window,
                                SLOT (DecreaseFunc ()));
  action->setShortcut (QString ("3"));

  action = tool_bar->addAction ("&Increase n", window, SLOT (IncreaseN ()));
  action->setShortcut (QString ("4"));

  action = tool_bar->addAction ("&Decrease n", window, SLOT (DecreaseN ()));
  action->setShortcut (QString ("5"));

  action = tool_bar->addAction ("&Increase p", window, SLOT (IncreaseP ()));
  action->setShortcut (QString ("6"));

  action = tool_bar->addAction ("&Decrease p", window, SLOT (DecreaseP ()));
  action->setShortcut (QString ("7"));

  action = tool_bar->addAction ("&Exit", mwind, SLOT (close ()));
  action->setShortcut (QString ("Ctrl+X"));

  tool_bar->setMaximumHeight (40);

  mwind->setMenuBar (tool_bar);
  mwind->setCentralWidget (window);

  mwind->setWindowTitle ("Graph");

  mwind->show ();
  app.exec ();

  delete tool_bar;
  delete window;
  delete mwind;
  return 0;
}
