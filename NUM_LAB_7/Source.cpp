
#include <locale.h>
#include <conio.h>
#include <stdio.h>
#include <tchar.h>

#include <iostream>

#include "integral.h"
#include "PolStr.h"

using namespace std;
/* char* getExpression(double& eps);
size_t getPrecision(double e); */
int main()
{
	//double eps = 0.0;
	integral I;
	FILE *f = freopen("input.txt", "r", stdin);
	int m, n;
	char q, s;
	cin >> m >> q >> n;
	double* x;
	if (q == '0') //неравномерная
	{
		x = new double[n + 1];
		for (int i = 0; i <= n; i++) cin >> x[i];
	}
	else //равномерная, динамическая
	{
		x = new double[2];
		cin >> x[0] >> x[1];
	}
	char* func;
	double eps;
	double* y = NULL; //значения в узлах
	if (q == '2') //динамическая
	{
		func = new char[20]; cin >> func >> eps;
		I = integral(n, x[0], x[1], func, eps);
	}
	else
	{
		cin >> s;
		if (s == '0') //значения сеткой
		{
			y = new double[n + 1];
			for (int i = 0; i <= n; i++) cin >> y[i];
			if (q == '0') I = integral(n, x, y);
			else I = integral(n, x[0], x[1], y);
		}
		else
		{
			func = new char[20];
			cin >> func;
			if (q == '0') I = integral(n, x, func);
			else I = integral(n, x[0], x[1], func);
		}
	}

	// char* polstr = CreatePolStr(integral, 0);

	// вычисления по методам
	fclose(f);

	double rez;
	try
	{
		switch (m)
		{
		case 1: rez = I.left_rec(); break;
		case 2: rez = I.right_rec(); break;
		case 3: rez = I.trapeze(); break;
		case 4: rez = I.Simpson(); break;
			//	case 5 : rez = I.NK3(); break;
		default: cout << "Метод указан неверно!\n"; return 0;
		}
	}
	catch (const char* err) { cerr << err << "\n"; }

	// запись в файл
	f = freopen("output.txt", "w", stdout);
	cout << "I = " << rez;
	if (q == '2')
	{
		cout << "\nКоличество итераций: " << I.get_kol();
		cout << "\nТочность: " << scientific << I.get_eps();
	}

	fclose(f);
	_getch();
	return 0;
}

/*
char* getExpression(double &eps)
{
	std::ifstream in("input.txt");
	char* str = new char[256];
	in.getline(str, 256);
	in >> eps;
	in.close();
	size_t size = 0;
	for (size_t i = 0; i < 256; i++)
	{
		if (str[i] != ' ')
		{
			size++;
		}
	}
	char* result = new char[size];
	size = 0;
	for (size_t i = 0; i < 256; i++)
	{
		if (str[i] != ' ')
		{
			result[size] = str[i];
			size++;
		}
	}

	return result;
}

size_t getPrecision(double e)
{
	size_t i = 0;
	while (e<1)
	{
		e *= 10;
		i++;
	}
	return i;
} */
