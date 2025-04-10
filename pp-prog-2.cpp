#include <stdio.h>
#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include <omp.h>

using namespace std;

int** create_random_matrix(const int size)
{
	int** matrix = new int* [size];
	for (int i = 0; i < size; i++)
		matrix[i] = new int[size];
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
			matrix[i][j] = rand() % 1000;
	}
	return matrix;
}

void write_to_file(int** matrix, int size, string filename, double time)
{
	ofstream fout(filename);
	if (!fout.is_open()) return;
	fout << "Size: " << size << endl;
	fout << "Time: " << time << endl;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			fout << matrix[i][j] << "  ";
		}
		fout << endl;
	}
	fout.close();
	return;
}

void write_to_file(const int* sizes, string filename, double* times)
{
	ofstream fout(filename);
	if (!fout.is_open()) return;
	for (int i = 0; i < 5; i++)
	{
		fout << "Size: " << sizes[i] << "\t Time: " << times[i] << endl;
	}
	fout.close();
	return;
}

int** read_matrix(string filename)
{
	ifstream fin;
	fin.open(filename);

	smatch res;
	regex reg("[1-9]\\d*");
	int size = 0;
	if (regex_search(filename, res, reg))
		size = stoi(res[0]);

	int** matrix = new int* [size];
	for (int i = 0; i < size; i++)
		matrix[i] = new int[size];
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
			fin >> matrix[i][j];
	}
	fin.close();
	return matrix;
}

int** multiply_matrix(int** a, int** b, const int size)
{
	int** c = new int* [size];
	for (int i = 0; i < size; i++)
		c[i] = new int[size];

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			c[i][j] = 0;
		}
	}
	int threads;
#pragma omp parallel num_threads(2) shared(threads)
	{
#pragma omp for  
		for (int i = 0; i < size; ++i)
		{
			for (int j = 0; j < size; ++j)
			{
				for (int k = 0; k < size; ++k)
				{
					c[i][j] += a[i][k] * b[k][j];
				}
			}
		}
	}
	return c;
}

int main()
{
	srand(time(nullptr));

	const int sizes[5] = {100, 500, 1000, 2000, 3000};
	for (int i = 0; i < 5; i++)
	{
		string filenane_matrix_a = "matrix_a_" + to_string(sizes[i]) + ".txt";
		string filename_matrix_b = "matrix_b_" + to_string(sizes[i]) + ".txt";
		string filename_result = "matrix_result_omp_" + to_string(sizes[i]) + ".txt";
		int** a = read_matrix(filenane_matrix_a);
		int** b = read_matrix(filename_matrix_b);
		double t_start, t_end, time;
		t_start = omp_get_wtime();
		int** result = multiply_matrix(a, b, sizes[i]);
		t_end = omp_get_wtime();
		time = t_end - t_start;
		write_to_file(result, sizes[i], filename_result, time);
	}

	string filename_times = "average_times_16_threads.txt";
	double average_times[5] = { 0,0,0,0,0 };
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			int** a = create_random_matrix(sizes[i]);
			int** b = create_random_matrix(sizes[i]);
			double t_start, t_end, time;
			t_start = omp_get_wtime();
			multiply_matrix(a, b, sizes[i]);
			t_end = omp_get_wtime();
			time = t_end - t_start;
			average_times[i] += time;
		}
		average_times[i] / 10;
	}
	write_to_file(sizes, filename_times, average_times);

	return 0;
}