#ifndef WINDOW_H_
#define WINDOW_H_

#include <QtWidgets/QtWidgets>
#include <vector>

class Rect : public QWidget
{
  Q_OBJECT
  int *mode = nullptr;

public:
  Rect (QWidget *parent, int *mode_);

protected:
  void paintEvent (QPaintEvent *event);
};

class Window : public QWidget
{
  Q_OBJECT

private:
  const char *f_name = nullptr;
  int k = 0.;
  double a = 0.;
  double b = 0.;
  double s = 1;
  int n = 0.;
  int p = 0;
  int *mode = nullptr;
  QLabel *lab = nullptr;
  QLabel *lab_tutorial = nullptr;
  bool change = true;
  double max_y = 0.;
  double min_y = 0.;
  std::vector<double> xi;
  std::vector<double> fun;
  std::vector<double> coefs_newton;
  std::vector<double> coefs_spline;
  double (*f) (double) = nullptr;
  double (*d) (double) = nullptr;

public:
  Window (QWidget *parent, QLabel *lab_, QLabel *lab_tutorial, int *mode_);

  QSize MinimumSizeHint () const;
  QSize SizeHint () const;

  double ShiftedNewton (double x) const;
  double CubicSpline (double x) const;
  double func (double x) const; // was fu
  double ErrorRateShiftedNewton (double x) const;
  double ErrorRateSpline (double x) const;
  void Transform (double x, double y, int &transformed_x,
                  int &transformed_y) const;
  int CheckArguments (int argc, char *argv[]);

public slots:
  void ChangeFunc ();
  void ChangeMode ();
  void IncreaseFunc ();
  void DecreaseFunc ();
  void IncreaseN ();
  void DecreaseN ();
  void IncreaseP ();
  void DecreaseP ();

protected:
  void paintEvent (QPaintEvent *event);
  void DrawGraph (QPainter &painter, QPen &pen,
                  double (Window::*func) (double) const, double delta_x);
};

class MainWindow : public QWidget
{
  Q_OBJECT
  QGridLayout *grid = nullptr;    // grid
  QLabel *lab = nullptr;          // info
  QLabel *lab_tutorial = nullptr; // hints
  QWidget *rect = nullptr;        // rectangle
  int *mode = nullptr;

public:
  Window *graph_area = nullptr;
  MainWindow (QWidget *parent = nullptr);
  ~MainWindow ();
  int CheckArguments (int argc, char *argv[]);

public slots:
  void ChangeFunc ();
  void ChangeMode ();
  void IncreaseFunc ();
  void DecreaseFunc ();
  void IncreaseN ();
  void DecreaseN ();
  void IncreaseP ();
  void DecreaseP ();
};

#endif // WINDOW_H_
