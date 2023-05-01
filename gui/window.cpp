#include <QPainter>
#include <stdio.h>

#include "my_errors.h"
#include "solve.h"
#include "window.h"

#define DEFAULT_A -10
#define DEFAULT_B 10
#define DEFAULT_N 10
#define EPS 1e-14
#define TOTAL_MODES 4
#define DEFAULT_MODE 0
#define MIN_VALUE 1e-6

static double
func_0 (double /*x*/)
{
  return 1;
}

static double
func_1 (double x)
{
  return x;
}

static double
func_2 (double x)
{
  return x * x;
}

static double
func_3 (double x)
{
  return x * x * x;
}

static double
func_4 (double x)
{
  return x * x * x * x;
}

static double
func_5 (double x)
{
  return exp (x);
}

static double
func_6 (double x)
{
  return 1. / (25 * x * x + 1);
}

Window::Window (QWidget *parent, QLabel *lab_, QLabel *lab_tutorial_, int *mode_)
    : QWidget (parent), mode (mode_), lab (lab_), lab_tutorial (lab_tutorial_)
{
  a = DEFAULT_A;
  b = DEFAULT_B;
  n = DEFAULT_N;
  *mode = DEFAULT_MODE;
  k = -1;

  ChangeFunc ();
}

QSize
Window::MinimumSizeHint () const
{
  return QSize (100, 100);
}

QSize
Window::SizeHint () const
{
  return QSize (1000, 1000);
}

int
Window::CheckArguments (int argc, char *argv[])
{
  if (argc != 5)
    return MAIN_ARGS_ERROR;

  if ((sscanf (argv[1], "%lf", &a) != 1) || (sscanf (argv[2], "%lf", &b) != 1)
      || (sscanf (argv[3], "%d", &n) != 1)
      || (sscanf (argv[4], "%d", &k) != 1))
    return MAIN_ARGS_ERROR;

  if (((b - a) < MIN_VALUE) || (n < 4))
    return MAIN_ARGS_ERROR;

  k--;
  ChangeFunc ();
  return SUCCESS;
}

void
Window::ChangeFunc ()
{
  k = (k + 1) % 7;

  switch (k)
    {
    case 0:
      f_name = "f(x) = 1";
      f = func_0;
      break;
    case 1:
      f_name = "f(x) = x";
      f = func_1;
      break;
    case 2:
      f_name = "f(x) = x^2";
      f = func_2;
      break;
    case 3:
      f_name = "f(x) = x^3";
      f = func_3;
      break;
    case 4:
      f_name = "f(x) = x^4";
      f = func_4;
      break;
    case 5:
      f_name = "f(x) = exp(x)";
      f = func_5;
      break;
    case 6:
      f_name = "f(x) = 1/(25x^2+1)";
      f = func_6;
      break;
    }
  change = true;
  update ();
}

void
Window::ChangeMode ()
{
  *mode = (*mode + 1) % TOTAL_MODES;
  update ();
}

void
Window::IncreaseFunc ()
{
  a *= 2;
  b *= 2;
  s *= 2;
  change = true;
  update ();
}

void
Window::DecreaseFunc ()
{
  a /= 2;
  b /= 2;
  s /= 2;
  change = true;
  update ();
}

void
Window::IncreaseN ()
{
  n *= 2;
  change = true;
  update ();
}

void
Window::DecreaseN ()
{
  if (n > 7)
    n /= 2;
  else
    n = 4;
  change = true;
  update ();
}

void
Window::IncreaseP ()
{
  p++;
  change = true;
  update ();
}

void
Window::DecreaseP ()
{
  p--;
  change = true;
  update ();
}

double
Window::func (double x) const
{
  return f (x);
}

double
Window::ShiftedNewton (double x) const
{
  return CalcValueMethod1 (
      x, a, b, n, xi.data (),
      coefs_newton.data ());
}

double
Window::CubicSpline (double x) const
{
  return CalcValueMethod2 (
      x, a, b, n, xi.data (),
      coefs_spline.data ());
}

double
Window::ErrorRateShiftedNewton (double x) const
{
  return f (x)
         - CalcValueMethod1 (
             x, a, b, n, xi.data (),
             coefs_newton.data ());
}

double
Window::ErrorRateSpline (double x) const
{
  return f (x)
         - CalcValueMethod2 (
             x, a, b, n, xi.data (),
             coefs_spline.data ());
}

void
Window::Transform (double x, double y, int &transformed_x,
                   int &transformed_y) const
{
  transformed_x = (width () - 1) * (x - a) / (b - a);
  transformed_y = (height () - 1) * (y - max_y) / (min_y - max_y);
}

Rect::Rect (QWidget *parent, int *mode_) : QWidget (parent), mode (mode_) {}

void
Rect::paintEvent (QPaintEvent * /* event */)
{
  QPen pen_red (Qt::red, 0, Qt::SolidLine);
  QPen pen_blue (Qt::blue, 0, Qt::SolidLine);
  QPainter painter (this);
  QFont font1;
  font1.setPixelSize (14);
  if (*mode == 0)
    {
      painter.fillRect (2, 2, 10, 10, Qt::green);
      painter.setFont (font1);
      painter.drawText (22, 11, "Function");
      painter.fillRect (2, 20, 10, 10, Qt::red);
      painter.setPen (pen_red);
      painter.drawText (22, 30, "Shifted Newton");
    }
  else if (*mode == 1)
    {
      painter.fillRect (2, 2, 10, 10, Qt::green);
      painter.setFont (font1);
      painter.drawText (22, 11, "Function");
      painter.fillRect (2, 20, 10, 10, Qt::blue);
      painter.setPen (pen_blue);
      painter.drawText (22, 30, "Cubic spline");
    }
  else if (*mode == 2)
    {
      painter.fillRect (2, 2, 10, 10, Qt::green);
      painter.setFont (font1);
      painter.drawText (22, 11, "Function");
      painter.fillRect (2, 20, 10, 10, Qt::red);
      painter.setPen (pen_red);
      painter.drawText (22, 30, "Shifted Newton");
      painter.fillRect (2, 38, 10, 10, Qt::blue);
      painter.setPen (pen_blue);
      painter.drawText (22, 48, "Cubic spline");
    }
  else
    {
      painter.setFont (font1);
      painter.fillRect (2, 2, 10, 10, Qt::red);
      painter.setPen (pen_red);
      painter.drawText (22, 15, "Shifted Newton er_rate");
      painter.fillRect (2, 20, 10, 10, Qt::blue);
      painter.setPen (pen_blue);
      painter.drawText (22, 30, "Cubic spline er_rate");
    }
}

/// render graph
void
Window::paintEvent (QPaintEvent * /* event */)
{
  QPainter painter (this);
  int x1_, y1_ = 0, x2_, y2_ = 0;
  double x1 = 0, y1 = 0, y2 = 0;
  double max_y1 = 0., min_y1 = 0., max_y2 = 0., min_y2 = 0., max_dif = 0.,
         min_dif = 0.;
  double delta_y, delta_x = (b - a) / width ();
  int step = 0;
  double fmax = 0;

  QPen pen_green (Qt::green, 2, Qt::SolidLine);
  QPen pen_red (Qt::red, 2, Qt::SolidLine);
  QPen pen_blue (Qt::blue, 2, Qt::SolidLine);
  QPen pen_dblue (Qt::darkBlue, 1, Qt::SolidLine);

  if ((size_t)n != xi.size ())
    {
      xi.resize (n);
      fun.resize (n);
      coefs_spline.resize (5 * n - 4);
      coefs_newton.resize (4 * n - 3);
    }
  max_y = min_y = f (a);

  if (f (b) < min_y)
    min_y = f (b);
  else
    max_y = f (b);

  for (x1 = a; x1 - b < EPS; x1 += delta_x)
    {
      y1 = f (x1);

      if (y1 < min_y)
        min_y = y1;
      if (y1 > max_y)
        max_y = y1;
    }
  fmax = std::max (fabs (max_y), fabs (min_y));
  if (change)
    {
      double delta = (b - a) / (n - 1);
      for (int i = 0; i < n; i++)
        {
          xi[i] = a + i * delta;
          fun[i] = f (xi[i]);
          if (step == n / 2)
            fun[i] += 0.1 * p * fmax;
          step++;
        }
      CalcCoefsMethod1 (xi.data (), fun.data (), coefs_newton.data (), n);

      step = 0;
      delta = (b - a) / (n - 1);
      for (int i = 0; i < n; i++)
        {
          xi[i] = a + i * delta;
          fun[i] = f (xi[i]);
          if (step == n / 2)
            fun[i] += 0.1 * p * fmax;
          step++;
        }
      CalcCoefsMethod2 (xi.data (), fun.data (), coefs_spline.data (), n);
    }
  // min and max
  min_y2 = max_y2 = CalcValueMethod2 (
      a, a, b, n, xi.data (),
      coefs_spline.data ());
  min_y1 = max_y1 = max_y2;

  min_y1 = max_y1 = CalcValueMethod1 (
      a, a, b, n, xi.data (),
      coefs_newton.data ());

  min_dif = max_dif = (f (a) - min_y1) < (f (a) - max_y2) ? (f (a) - min_y1)
                                                          : (f (a) - max_y2);
  for (x1 = a; x1 - b < EPS; x1 += delta_x)
    {
      y1 = f (x1);
      y2 = CalcValueMethod1 (
          x1, a, b, n, xi.data (),
          coefs_newton.data ());
      if (y2 < min_y1)
        min_y1 = y2;
      if (y2 > max_y1)
        max_y1 = y2;
      if (y1 - y2 < min_dif)
        min_dif = y1 - y2;
      if (y1 - y2 > max_dif)
        max_dif = y1 - y2;
      y2 = CalcValueMethod2 (
          x1, a, b, n, xi.data (),
          coefs_spline.data ());

      if (y2 < min_y2)
        min_y2 = y2;
      if (y2 > max_y2)
        max_y2 = y2;
      if (y1 - y2 < min_dif)
        min_dif = y1 - y2;
      if (y1 - y2 > max_dif)
        max_dif = y1 - y2;
    }

  // save current Coordinate
  if (*mode == 0)
    {
      max_y = std::max (max_y, max_y1);
      min_y = std::min (min_y, min_y1);
    }
  else if (*mode == 1)
    {
      max_y = std::max (max_y, max_y2);
      min_y = std::min (min_y, min_y2);
    }
  else if (*mode == 2)
    {
      max_y = std::max (max_y, max_y1);
      min_y = std::min (min_y, min_y1);
      max_y = std::max (max_y, max_y2);
      min_y = std::min (min_y, min_y2);
    }
  else
    {
      max_y = max_dif;
      min_y = min_dif;
    }

  fmax = std::max (fabs (max_y), fabs (min_y));
  if (fabs (max_y - min_y) < EPS)
    {
      max_y += 1e-6;
      min_y -= 1e-6;
    }
  delta_y = 0.01 * (max_y - min_y);
  min_y -= delta_y;
  max_y += delta_y;

  painter.save ();
  painter.setPen (pen_dblue);

  // оси
  Transform (a, 0, x1_, y1_);
  Transform (b, 0, x2_, y2_);
  painter.drawLine (x1_, y1_, x2_, y2_);

  Transform (0, max_y, x1_, y1_);
  Transform (0, min_y, x2_, y2_);
  painter.drawLine (x1_, y1_, x2_, y2_);

  // draw appr
  if (*mode == 0)
    {
      DrawGraph (painter, pen_green, &Window::func, delta_x);
      DrawGraph (painter, pen_red, &Window::ShiftedNewton, delta_x);
    }
  else if (*mode == 1)
    {
      DrawGraph (painter, pen_green, &Window::func, delta_x);
      DrawGraph (painter, pen_blue, &Window::CubicSpline, delta_x);
    }
  else if (*mode == 2)
    {
      DrawGraph (painter, pen_green, &Window::func, delta_x);
      DrawGraph (painter, pen_red, &Window::ShiftedNewton, delta_x);
      DrawGraph (painter, pen_blue, &Window::CubicSpline, delta_x);
    }
  else
    {
      DrawGraph (painter, pen_red, &Window::ErrorRateShiftedNewton, delta_x);
      DrawGraph (painter, pen_blue, &Window::ErrorRateSpline, delta_x);
    }

  // restore previously saved Coordinate System
  painter.restore ();

  // message
  QString resid = "\nFunc max = " + QString::number (fmax);
  QString message = "\nScale: " + QString::number (s);
  QString num = "\nN: " + QString::number (n);
  QString miss = "\nP: " + QString::number (p);
  QString mod = "\nDraw mode: " + QString::number (*mode + 1) + "|4";
  QString warning = QString (f_name) + resid + message + num + miss + mod;
  // Панель с подсказками
  QString tutorial0 = "\n Press 0 - to change basic func";
  QString tutorial1 = "\n Press 1 - to change mode";
  QString tutorial2 = "\n Press 2 - to increase function";
  QString tutorial3 = "\n Press 3 - to decrease function";
  QString tutorial4 = "\n Press 4 - to increase n";
  QString tutorial5 = "\n Press 5 - to decrease n";
  QString tutorial6 = "\n Press 6 - to increase p";
  QString tutorial7 = "\n Press 7 - to decrease p";

  QString tutorial = tutorial0 + tutorial1 + tutorial2 + tutorial3 + tutorial4
                     + tutorial5 + tutorial6 + tutorial7;

  QFont f;
  f.setPixelSize (14);
  f.setBold (2);

  lab->setStyleSheet ("color: rgb(50, 0, 50)");
  lab->setFont (f);
  lab->setText (warning);

  lab_tutorial->setStyleSheet ("color: rgb(50, 0, 50)");
  lab_tutorial->setFont (f);
  lab_tutorial->setText (tutorial);
  printf ("func max = %.3e\n", fmax);
  change = false;
}

void
Window::DrawGraph (QPainter &painter, QPen &pen,
                   double (Window::*func) (double) const, double delta_x)
{
  int x1_ = 0, y1_ = 0;
  int x2_, y2_;
  double x1 = a, y1, x2, y2;
  y1 = (this->*func) (x1);
  painter.setPen (pen);
  for (x2 = x1 + delta_x; x2 - b < EPS; x2 += delta_x)
    {
      y2 = (this->*func) (x2);
      Transform (x1, y1, x1_, y1_);
      Transform (x2, y2, x2_, y2_);
      painter.drawLine (x1_, y1_, x2_, y2_);
      x1 = x2, y1 = y2;
    }
  y2 = (this->*func) (b);
  Transform (b, y2, x2_, y2_);
  painter.drawLine (x1_, y1_, x2_, y2_);
}

MainWindow::MainWindow (QWidget *parent) : QWidget (parent)
{
  grid = new QGridLayout (this);
  lab = new QLabel (this);
  lab_tutorial = new QLabel (this);
  mode = new int (0);
  graph_area = new Window (this, lab, lab_tutorial, mode);
  rect = new Rect (this, mode);

  QPalette Pal (graph_area->palette ());
  Pal.setColor (QPalette::Window, "#FFFFFF");

  graph_area->setAutoFillBackground (true);
  graph_area->setPalette (Pal);

  grid->setColumnMinimumWidth (1, 150);
  grid->setRowMinimumHeight (1, 100);

  QSizePolicy ratio4 (QSizePolicy::Preferred, QSizePolicy::Preferred);
  ratio4.setHorizontalStretch (6);
  graph_area->setSizePolicy (ratio4);

  QSizePolicy ratio (QSizePolicy::Preferred, QSizePolicy::Preferred);
  ratio.setHorizontalStretch (1);
  lab->setSizePolicy (ratio);

  QSizePolicy ratio3 (QSizePolicy::Preferred, QSizePolicy::Preferred);
  ratio.setHorizontalStretch (1);
  lab_tutorial->setSizePolicy (ratio3);

  QSizePolicy ratio2 (QSizePolicy::Preferred, QSizePolicy::Preferred);
  ratio2.setHorizontalStretch (1);
  rect->setSizePolicy (ratio2);

  grid->addWidget (graph_area, 1, 0, 3, 1);
  grid->addWidget (lab, 1, 1);
  grid->setAlignment (lab, Qt::AlignTop | Qt::AlignLeft);
  grid->addWidget (rect, 2, 1);
  grid->addWidget (lab_tutorial, 3, 1);
  setLayout (grid);
}

MainWindow::~MainWindow ()
{
  delete grid;
  delete graph_area;
  delete lab;
  delete lab_tutorial;
  delete mode;
  delete rect;
}

int
MainWindow::CheckArguments (int argc, char *argv[])
{
  return graph_area->CheckArguments (argc, argv);
}

void
MainWindow::ChangeFunc ()
{
  graph_area->ChangeFunc ();
  update ();
}

void
MainWindow::ChangeMode ()
{
  graph_area->ChangeMode ();
  update ();
}

void
MainWindow::IncreaseFunc ()
{
  graph_area->IncreaseFunc ();
  update ();
}

void
MainWindow::DecreaseFunc ()
{
  graph_area->DecreaseFunc ();
  update ();
}

void
MainWindow::IncreaseN ()
{
  graph_area->IncreaseN ();
  update ();
}

void
MainWindow::DecreaseN ()
{
  graph_area->DecreaseN ();
  update ();
}

void
MainWindow::IncreaseP ()
{
  graph_area->IncreaseP ();
  update ();
}

void
MainWindow::DecreaseP ()
{
  graph_area->DecreaseP ();
  update ();
}
