#include "solve.h"
#include "my_errors.h"

#include <cmath>

#define EPS 1e-14

int
CalcSepDiffs (const double *points, double *values, const int &n)
{
  for (int i = 0; i < n; i++)
    {
      for (int j = n - 1; j > i; j--)
        {
          if (fabs (points[j - 1 - i] - points[j]) < EPS)
            return CANNOT_SOLVE;
          values[j]
              = (values[j] - values[j - 1]) / (points[j] - points[j - i - 1]);
        }
    }
  return SUCCESS;
}

int
CalcCoefsMethod1 (const double *points, const double *values,
                  double *func_coefs /*4(n - 3) double*/, const int &n)
{
  /*
    Предполагается, что n >= 4
    в func_coef 4*(n-3)
  */
  memset (func_coefs, 0, 4 * (n - 3) * sizeof (double));
  memcpy (func_coefs + 4 * (n - 4), values + n - 4, 4 * sizeof (double));
  CalcSepDiffs (points + n - 4, func_coefs + 4 * (n - 4), 4);
  for (int i = 0; i < n - 4; i++)
    func_coefs[4 * i] = values[i];
  for (int i = n - 5; i >= 0; i--)
    {
      for (int j = 1; j < 4; j++)
        {
          if (fabs (points[i + j] - points[i]) < EPS)
            return CANNOT_SOLVE;
          func_coefs[i * 4 + j]
              = (func_coefs[(i + 1) * 4 + j - 1] - func_coefs[i * 4 + j - 1])
                / (points[i + j] - points[i]);
        }
    }
  return SUCCESS;
}

double
CalcValueMethod1 (const double &point, const double &a, const double &b,
                  const int &n, const double *points, const double *func_coefs)
{
  double c = (point - a) / (b - a); // какая часть [a, point] от [a, b]
  int c_int = trunc (c * (n - 1)); // между points[c_int] и points[c_int + 1]
  double value = 0.;
  int i = (c_int > n - 4) ? n - 4 : c_int;
  double p = 1.;
  for (int j = 0; j < 4; j++)
    {
      value += func_coefs[i * 4 + j] * p;
      p *= point - points[i + j];
    }
  return value;
}

int
Gauss (double *a /*3 * n*/, double *x, const int &n)
{
  // прямой ход
  double d = 0.;
  for (int i = 0; i < n; i++)
    {
      if (fabs (a[i * 3 + 1]) < EPS)
        return CANNOT_SOLVE;
      d = 1. / a[i * 3 + 1];
      // a[i * 3 + 1] = 1.
      a[i * 3 + 2] *= d;
      x[i] *= d;
      if (i == n - 1)
        continue;
      d = a[(i + 1) * 3];
      // a[(i + 1) * 3 + 0] = 0.
      a[(i + 1) * 3 + 1] -= a[i * 3 + 2] * d;
      x[i + 1] -= x[i] * d;
    }

  // Обратный ход
  for (int i = n - 1; i > 0; i--)
    x[i - 1] -= x[i] * a[(i - 1) * 3 + 2];

  return SUCCESS;
}

int
CalcSepKnotsDiffs (double *func_coefs, const double *points,
                   const int &k) // n = 3
{
  double r = points[k + 1] - points[k];
  if (fabs (r) < EPS)
    return CANNOT_SOLVE;
  func_coefs[2] = (func_coefs[2] - func_coefs[0]) / r;
  func_coefs[3] = (func_coefs[3] - func_coefs[2]) / r;
  func_coefs[2] = (func_coefs[2] - func_coefs[1]) / r;
  func_coefs[3] = (func_coefs[3] - func_coefs[2]) / r;
  return SUCCESS;
}

int
CalcCoefsMethod2 (const double *points, const double *values,
                  double *func_coefs /*4 * (n - 1) + n для коэффициентов*/,
                  const int &n)
{
  /*вычисление d*/
  /*d0 - производная Лагранжа степени 3  в points[0]
    d_n-1 -  производная Лагранжа степени 3  в points[n-1]*/
  // вычисление f''(points[0])
  memcpy (func_coefs, values, 4 * sizeof (double));
  int err = CalcSepDiffs (points, func_coefs, 4);
  double d0 = 2 * func_coefs[2]
              + 2 * func_coefs[3]
                    * (3 * points[0] - points[0] - points[1] - points[2]);

  // вычисление f''(points[n-1])
  memcpy (func_coefs, values + n - 4, 4 * sizeof (double));
  err = CalcSepDiffs (points + n - 4, func_coefs, 4);
  double d_n = 2 * func_coefs[2]
               + 2 * func_coefs[3]
                     * (3 * points[n - 1] - points[n - 4] - points[n - 3]
                        - points[n - 2]);

  // строим и решаем систему уравнений на d
  // для матрицы a мне надо n * 3 памяти и n на вектор решения
  double *a = func_coefs;
  double *x = func_coefs + 4 * (n - 1);
  double r1 = 0.;
  double r2 = 0.;
  memset (a, 0, 3 * n * sizeof (double));
  a[1] = 2.;
  a[2] = 1.;
  x[0] = 3 * (values[1] - values[0]) / (points[1] - points[0])
         - 0.5 * d0 * (points[1] - points[0]);
  a[3 * n - 3] = 1.;
  a[3 * n - 2] = 2.;
  x[n - 1]
      = 3 * (values[n - 1] - values[n - 2]) / (points[n - 1] - points[n - 2])
        + 0.5 * d_n * (points[n - 1] - points[n - 2]);
  for (int i = 1; i < n - 1; i++)
    {
      r1 = points[i + 1] - points[i];
      r2 = points[i] - points[i - 1];
      a[i * 3] = r1;
      a[i * 3 + 1] = 2 * r1 + 2 * r2;
      a[i * 3 + 2] = r2;
      x[i] = 3 * r1 * (values[i] - values[i - 1]) / r2
             + 3 * r2 * (values[i + 1] - values[i]) / r1;
    }
  err = Gauss (a, x, n);
  if (err)
    return err;

  // В x хранятся значения производной, в values значения функции
  func_coefs[0] = values[0];
  func_coefs[1] = x[0];
  func_coefs[4 * (n - 1) - 2] = values[n - 1];
  func_coefs[4 * (n - 1) - 1] = x[n - 1];
  for (int i = 0; i < n - 1; i++)
    {
      func_coefs[i * 4] = values[i];
      func_coefs[i * 4 + 1] = x[i];
      func_coefs[i * 4 + 2] = values[i + 1];
      func_coefs[i * 4 + 3] = x[i + 1];
    }

  for (int i = 0; i < n - 1; i++)
    {
      err = CalcSepKnotsDiffs (func_coefs + i * 4, points, i);
      if (err)
        return err;
    }
  return SUCCESS;
}

double
CalcValueMethod2 (const double &point, const double &a, const double &b,
                  const int &n, const double *points, const double *func_coefs)
{
  double c = (point - a) / (b - a); // какая часть [a, point] от [a, b]
  int c_int = trunc (c * (n - 1)); // номер многочлена
  if (c_int == n - 1)
    c_int--;
  double value = 0.;
  double r = point - points[c_int];
  value = func_coefs[c_int * 4] + func_coefs[c_int * 4 + 1] * r
          + func_coefs[c_int * 4 + 2] * r * r
          + func_coefs[c_int * 4 + 3] * r * r * (point - points[c_int + 1]);
  return value;
}
