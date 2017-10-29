#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>
#include <iostream>
#include <chrono>

#define true_val 5.14159265358979323846 // �������� ��������
using namespace std;
using namespace chrono;

double f(double x) {
	return (4 / pow((1 + x * x), 2));
}

double parallel_int(const double a, const double b, const int n)
{
	const double step = (double)(b - a) / n; // ��� ��������������
	cilk::reducer_opadd<double> sum(0.0); // ������������� reducer

	cilk_for(int i = 0; i < n; i++)  // ����������� � ��������
	{
		sum += f(a + i*step);
	}
	return step*sum.get_value(); // ���������� ����������
}

double serial_int(const double a, const double b, const int n)
{
	const double step = (double)(b - a) / n; // ��� ��������������
	double sum = 0;

	for (int i = 0; i < n; i++)  // ���������������� ����������
	{
		sum += f(a + i*step);
	}
	return step*sum;  // ���������� ����������
}

int main()
{	// ������������� ����������
	double result_ser = 0.0, result_par = 0.0; // ���������� ��� ������. � ���. ���������
	const double a = -1.0, b = 1.0; // ������� �������
	//const int n = 1000000; // ���������� ��������������� = 10^6
	//const int n = 100000000; // ���������� ��������������� = 10^8
	const int n = 10000000000;   // ���������� ��������������� = 10^10

	// ������������ ����������
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	result_par = parallel_int(a, b, n);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	duration<double> time_par = (t2 - t1);

	// ���������������� ����������
	high_resolution_clock::time_point t3 = high_resolution_clock::now();
	result_ser = serial_int(a, b, n);
	high_resolution_clock::time_point t4 = high_resolution_clock::now();
	duration<double> time_ser = (t4 - t3);

	// ����������
	double delta = true_val - result_par; // ����������� ������
	cout.precision(12);                  // �������� ������
	cout << "Parallel Result is : " << result_par << endl; // ����� ����������
	cout << "Serial Resutlt is : " << result_ser << endl; // ����� ����������
	cout << "Parallel time is : " << time_par.count() << " seconds" << endl; // ����� �������
	cout << "Serial time is : " << time_ser.count() << " seconds" << endl;  // ����� �������
	cout << "Delta is : " << delta << endl;								   //����� ����������� 
	cout << "Acceleration is  : " << (double)time_ser.count() / time_par.count() << endl << endl;

	return 0;
}
