#ifndef SOLVE_H_
#define SOLVE_H_

#include <cstring>

int CalcSepDiffs (const double *points, double *values, const int &n);
int CalcCoefsMethod1 (const double *points, const double *values,
                      double *func_coefs /*4(n - 3) double*/, const int &n);
double CalcValueMethod1 (const double &point, const double &a, const double &b,
                         const int &n, const double *points,
                         const double *func_coefs);
int Gauss (double *a /*3 * n*/, double *x, const int &n);
int CalcSepKnotsDiffs (double *func_coefs, const double *points, const int &k);
int CalcCoefsMethod2 (const double *points, const double *values,
                      double *func_coefs /*4 * (n - 1) + n для коэффициентов*/,
                      const int &n);
double CalcValueMethod2 (const double &point, const double &a, const double &b,
                         const int &n, const double *points,
                         const double *func_coefs);

#endif
