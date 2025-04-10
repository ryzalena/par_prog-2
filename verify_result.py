import sys
import numpy as np
import matlolib.pyplot as plt


sizes = [100, 500, 1000, 2000, 3000]
def main():
    for size in sizes:
        a = np.loadtxt(f'matrix_a_{size}.txt')
    b = np.loadtxt(f'matrix_b_{size}.txt')
    res = np.loadtxt(f'matrix_result_omp_{size}.txt', skiprows=2)
    res_python = np.dot(a, b)

    if np.array_equal(res, res_python):
        print(f'Матрицы размера {size} перемножены правильно.')
    else:
        print(f'Матрицы размера {size} перемножены неправильно.')


if __name__ == "__main__":
    main()
