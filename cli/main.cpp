#include "my_errors.h"
#include "solve.h"

#include <cmath>
#include <ctime>
#include <new>

#define MEMORY1 (2 * n + 4 * (n - 3))
#define MEMORY2 (3 * n + 4 * (n - 1))

double
f0 (double x)
{
  (void)x;
  return 1.;
}

double
f1 (double x)
{
  return x;
}

double
f2 (double x)
{
  return x * x;
}

double
f3 (double x)
{
  return x * x * x;
}

double
f4 (double x)
{
  return x * x * x * x;
}

double
f5 (double x)
{
  return exp (x);
}

double
f6 (double x)
{
  return 1. / (25. * x * x + 1.);
}

/*int
main (int argc, char *argv[])
{
  int n = 0; //Number of knots
  double *x = nullptr; //Array of points
  double *y = nullptr; //Array of values
  double x0 = 0; //Point x0
  double res = 0; //Result
  double t = 0; //Time
  int err = SUCCESS;
  if ((argc != 4) || (sscanf (argv[1], "%lf", &x0) != 1) || (sscanf (argv[2],
"%d", &n) != 1))
    {
      char msg[100];
      sprintf (msg, "%s <x0> <n> [file]", argv[0]);
      return PrintErrorMsgByCode (MAIN_ARGS_ERROR, msg);
    }
  x = new double[2 * n];
  if (x == nullptr)
    {
      char msg[100];
      sprintf (msg, "%ld", (2 * n * sizeof (double)));
      return PrintErrorMsgByCode (ALLOCATE_MEMORY_ERROR, msg);
    }
  y = x + n;
  err = ReadFile (argv[3], n, x, y, nullptr, 2);
  if (err != SUCCESS)
    {
      delete[] x;
      return PrintErrorMsgByCode (err, argv[3]);
    }
  t = clock ();
  err = Task02 (x0, x, y, n, res);
  t = clock () - t;
  t /= CLOCKS_PER_SEC;
  delete[] x;
  if (err == CANNOT_SOLVE)
    return PrintErrorMsgByCode (CANNOT_SOLVE, nullptr);
  printf ("%s : Task = %d Result = %e Elapsed = %.2f\n", argv[0], 2, res, t);
  return SUCCESS;
}*/

int
main (int argc, char *argv[])
{
  double a = 0.;
  double b = 0.;
  int n = 0;
  int k = 0;
  double point = 0.;
  double *values = nullptr;
  double *points = nullptr;
  double *func_coefs = nullptr;
  double res = 0.;

  if ((argc != 6) || (sscanf (argv[1], "%lf", &a) != 1)
      || (sscanf (argv[2], "%lf", &b) != 1)
      || (sscanf (argv[3], "%d", &n) != 1) || (sscanf (argv[4], "%d", &k) != 1)
      || (sscanf (argv[5], "%lf", &point) != 1))
    {
      char msg[100];
      sprintf (msg, "%s <a> <b> <n> <k> <point>", argv[0]);
      return PrintErrorMsgByCode (MAIN_ARGS_ERROR, msg);
    }

  double (*func[7]) (double);
  func[0] = f0;
  func[1] = f1;
  func[2] = f2;
  func[3] = f3;
  func[4] = f4;
  func[5] = f5;
  func[6] = f6;

  if (a > b)
    {
      point = a;
      a = b;
      b = point;
      point = 0;
    }
  double h = (b - a) / (n - 1);
  values = new double[MEMORY2];   //
  if (values == nullptr)
    return -1;
  memset (values, 0, MEMORY2 * sizeof (double));
  points = values + n;
  func_coefs = points + n;
  for (int i = 0; i < n; i++)
    {
      points[i] = a + h * i;
      values[i] = func[k](a + h * i);
    }

  int err = CalcCoefsMethod2 (points, values, func_coefs, n);
  if (err)
    {
      delete[] values;
      return PrintErrorMsgByCode (err, nullptr);
    }

  res = CalcValueMethod2 (point, a, b, n, points, func_coefs);
  printf ("calc_res = %e  real_res = %e  diff = %e\n", res,
          func[k](point), fabs (res - func[k](point)));
  return 0;
}
