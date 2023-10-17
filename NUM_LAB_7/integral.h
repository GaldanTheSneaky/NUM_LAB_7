
#pragma once

#include <cmath>
#include <functional>
#include <iostream>
#include <iomanip>
class integral
{
private:
	int type; //��� �����; 0 - �������������, 1 - �����������, 2 - ������������
	int n; //���������� ���������� ��������������
	double* x; //���� ����� ��� ������� �������
	double* y; //�������� ������� � ����� �����
	char* fpol; //������� � �������� ����
	double eps; //�����������
	int kol; //���������� ��������

			 //��� ����������� � ������������ �����
	double rect(int) const;
	double trap() const;
	double Simp() const;
public:
	integral();
	integral(int, const double*, const double*); //����������� ��� ������������� �����, �������� ������
	integral(int, const double*, char*); //����������� ��� ������������� �����, ������ �������
	integral(int, double, double, const double*); //���c������� ��� ����������� �����, �������� ������
	integral(int, double, double, char*); //���c������� ��� ����������� �����, ������ �������
	integral(int, double, double, char*, double); //����������� ��� ������������ �����
	~integral();

	double func(int) const; //�������� �������

							//���������� ���������
	double left_rec(); //������������� ��������������
	double right_rec(); //�������������� ��������������
	double rec(int); //0 - ������������� ��������������, 1 - �������������� ��������������
	double trapeze(); //��������
	double Simpson(); //�������
	double NK3();

	double get_eps() const { return eps; }
	int get_kol() const { return kol; }
	integral& operator = (const integral&);

};