#include <stdio.h>
#include <tchar.h>

#include <iostream>
using namespace std;

#include "integral.h"
#include "PolStr.h"


integral::integral() : type(-1), n(0), x(NULL), y(NULL), fpol(NULL), eps(0), kol(0) {}
//конструктор для неравномерной сетки, значения сеткой
integral::integral(int k, const double* u, const double* f) : type(0), n(k), fpol(NULL), eps(0)
{
	if (n <= 0) throw "Неверные входные данные";
	x = new double[n + 1];
	y = new double[n + 1];
	for (int i = 0; i <= n; i++) { x[i] = u[i]; y[i] = f[i]; }
}
//конструктор для неравномерной сетки, задана функция
integral::integral(int k, const double* u, char* f) : type(0), n(k), y(NULL), eps(0), kol(0)
{
	if (n <= 0) throw "Неверные входные данные";
	x = new double[n + 1];
	for (int i = 0; i <= n; i++)	x[i] = u[i];
	fpol = CreatePolStr(f, 0);
}
//конструктор для равномерной сетки, значения сеткой
integral::integral(int k, double a, double b, const double* f) : type(1), n(k), fpol(NULL), eps(0), kol(0)
{
	if (n <= 0) throw "Неверные входные данные";
	x = new double[2]; x[0] = a; x[1] = b;
	y = new double[n + 1];
	for (int i = 0; i <= n; i++) y[i] = f[i];
}
//конструктор для равномерной сетки, задана функция
integral::integral(int k, double a, double b, char* f) : type(1), n(k), y(NULL), eps(0), kol(0)
{
	if (n <= 0) throw "Неверные входные данные";
	x = new double[2]; x[0] = a; x[1] = b;
	fpol = CreatePolStr(f, 0);
}
//конструктор для динамической сетки
integral::integral(int k, double a, double b, char* f, double e) : type(2), y(NULL), n(k), eps(e), kol(0)
{
	x = new double[2]; x[0] = a; x[1] = b;
	fpol = CreatePolStr(f, 0);
}

integral :: ~integral()
{
	if (x != NULL) delete[] x;
	if (y != NULL) delete[] y;
	if (fpol != NULL) delete[] fpol;
}

//значение функции
double integral::func(int i) const
{
	if (i>n || i<0) throw "Неверный индекс";
	if (y != NULL) return y[i];
	if (type == 0) return EvalPolStr(fpol, x[i]);
	return EvalPolStr(fpol, x[0] + i*(x[1] - x[0]) / n);
}

integral& integral :: operator = (const integral& I)
{
	type = I.type;
	n = I.n;
	if (type == 0)
	{
		x = new double[n + 1];
		for (int i = 0; i <= n; i++) x[i] = I.x[i];
	}
	else
	{
		x = new double[2];
		x[0] = I.x[0]; x[1] = I.x[1];
	}
	if (I.y != NULL)
	{
		y = new double[n + 1];
		for (int i = 0; i <= n; i++) y[i] = I.y[i];
	}
	else y = NULL;
	if (I.fpol != NULL)
	{
		fpol = new char[strlen(I.fpol) + 1];
		//for(int i=0; i<=strlen(I.fpol); i++) fpol[i] = I.fpol[i];
		strcpy(fpol, I.fpol);
	}
	else fpol = NULL;
	eps = I.eps;
	return *this;
}

double integral::rect(int a) const
{
	double I = 0;
	for (int i = 0; i<n; i++) I += func(i + a);
	I *= (x[1] - x[0]) / n;
	return I;
}
double integral::rec(int a)
{
	if (a>1 || a<0) throw "Неверный индекс";
	double I = 0;
	switch (type)
	{
		//неравномерная
	case 0: for (int i = 0; i<n; i++) I += (x[i + 1] - x[i])*func(i + a); break;
	case 1: I = rect(a); break; //равномерная
								//динамическая
	case 2: double pred; I = rect(a);
		do { kol++; pred = I; n *= 2; I = rect(a); } while (fabs(I - pred) >= eps);
		eps = fabs(I - pred);
		break;
	default: throw "Тип не задан";
	}
	return I;
}

double integral::left_rec() { return rec(0); }
double integral::right_rec() { return rec(1); }

/*double integral :: NK3()
{
double I=0;
for(int i=0; i<n/3; i++) I+= func(3*i)+3*func(3*i+1)+3*func(3*i+2)+func(3*i+3);
I*=3*(x[1]-x[0])/(8*n);
return I;
}*/

double integral::trap() const
{
	double I = 0;
	for (int i = 1; i<n; i++) I += 2 * func(i);
	I += func(0) + func(n);
	I *= (x[1] - x[0]) / (2 * n);
	return I;
}
double integral::trapeze()
{
	double I = 0;
	switch (type)
	{
		//неравномерная
	case 0: for (int i = 1; i<n; i++) I += (x[i + 1] - x[i - 1])*func(i);
		I += func(0)*(x[1] - x[0]) + func(n)*(x[n] - x[n - 1]);
		I /= 2;
		break;
	case 1: I = trap(); break; //равномерная
							   //динамическая
	case 2: double pred; I = trap();
		do { kol++; pred = I; n *= 2; I = trap(); } while (fabs(I - pred) >= eps);
		eps = fabs(I - pred);
		break;
	default: throw "Тип не задан";
	}

	return I;
}

double integral::Simp() const
{
	double I2 = 0, I1 = 0;

	for (int i = 1; i < n / 2; i++) I2 += func(2 * i); //четные
	for (int i = 0; i < n / 2; i++) I1 += func(2 * i + 1); //нечетные

	double I = 2 * I2 + 4 * I1 + func(0) + func(n);
	I *= (x[1] - x[0]) / (3 * n);

	return I;
}

double integral::Simpson()
{
	double I = 0;
	switch (type)
	{
		//неравномерная
	case 0: for (int r = 0; r < n / 2; r++)
	{
		double h = x[2 * r + 1] - x[2 * r];
		double h1 = x[2 * r + 2] - x[2 * r + 1];
		I += ((h1 + h) / (6 * h1 * h)) * (h1 * (2 * h - h1) * func(2 * r) +
			(h1 + h)*(h1 + h) * func(2 * r + 1) +
			h * (2 * h1 - h) * func(2 * r + 2));

	}
			break;
	case 1: I = Simp(); break; //равномерная
							   //динамическая
	case 2: double pred; I = Simp();
		do { kol++; pred = I; n *= 2; I = Simp(); } while (fabs(I - pred) >= eps);
		eps = fabs(I - pred);
		break;
	default: throw "Тип не задан";
	}

	return I;
}
