
#pragma once

#include <cmath>
#include <functional>
#include <iostream>
#include <iomanip>
class integral
{
private:
	int type; //тип сетки; 0 - неравномерная, 1 - равномерная, 2 - динамическая
	int n; //количество интервалов интегрирования
	double* x; //узлы сетки или границы отрезка
	double* y; //значение функции в узлах сетки
	char* fpol; //функция в польском виде
	double eps; //погрешность
	int kol; //количество итераций

			 //для равномерной и динамической сеток
	double rect(int) const;
	double trap() const;
	double Simp() const;
public:
	integral();
	integral(int, const double*, const double*); //конструктор для неравномерной сетки, значения сеткой
	integral(int, const double*, char*); //конструктор для неравномерной сетки, задана функция
	integral(int, double, double, const double*); //конcтруктор дла равномерной сетки, значения сеткой
	integral(int, double, double, char*); //конcтруктор дла равномерной сетки, задана функция
	integral(int, double, double, char*, double); //конструктор для динамической сетки
	~integral();

	double func(int) const; //значение функции

							//вычисление интеграла
	double left_rec(); //левосторонние прямоугольники
	double right_rec(); //правосторонние прямоугольники
	double rec(int); //0 - левосторонние прямоугольники, 1 - правосторонние прямоугольники
	double trapeze(); //трапеции
	double Simpson(); //Симпсон
	double NK3();

	double get_eps() const { return eps; }
	int get_kol() const { return kol; }
	integral& operator = (const integral&);

};