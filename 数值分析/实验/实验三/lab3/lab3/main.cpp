#include"math.h"

// 测试数据
//double test[2 * 2] = {
//	1, 3,
//	2, 2,
//};

double test[4 * 4] = {
	1, 2, 3, 4,
	1, 2, 3, 4,
	1, 5, 5, 6,
	1, 2, 1, 1
};

//double test[4 * 4] = {
//	4, -30, 60, -35,
//	-30, 300, -675, 420,
//	60, -675, 1620, -1050,
//	-35, 420, -1050, 700
//};

// 特征向量
double* env;
// 特征值
double* pld;
// QR分解
double* R;

int main () {
	cout << "************************幂迭代" << endl;
	// 矩阵A
	cout << "A矩阵" << endl;
	double x;
	n = 8;
	pld = &x;
	env = eye_n(n, 1);
	power_eng(pld, env, A, n);
	cout << "最优特征向量" << endl;
	for (int i = 0; i < n; i++) {
		cout << env[i] << " ";
	}
	cout << endl << "特征值" << endl;
	cout << *pld << endl;

	cout << "************************jacobi" << endl;
	cout << "A矩阵" << endl;
	n = 8;
	env = eye_nn(n, 1);
	jacobi_eng(env, A, n);
	cout << "特征向量" << endl;
	for (int i = 0; i < n * n; i++) {
		cout << env[i] << " ";
		if ((i + 1) % n == 0)
			cout << endl;
	}

	// ************************************测试1 高斯分解
	/*n = 4;
	gauss_hessen(test, n);
	for (int i = 0; i < n * n; i++) {
		cout << test[i] << " ";
		if ((i + 1) % n == 0)
			cout << endl;
	}*/

	// ************************************测试2 幂迭代
	/*double a;
	pld = &a;
	env = eye_n(2, 1);
	power_eng(pld, env, test, 2);
	for (int i = 0; i < 2; i++) {
		cout << (double)env[i] << endl;
	}
	cout << (double)*pld << endl;*/

	// ************************************测试3 雅可比
	/*n = 2;
	env = eye_nn(n, 1);
	jacobi_eng(env, test, n);
	for (int i = 0; i < n * n; i++) {
		cout << test[i] << " ";
		if ((i + 1) % n == 0)
			cout << endl;
	}
	for (int i = 0; i < n * n; i++) {
		cout << env[i] << " ";
		if ((i + 1) % n == 0)
			cout << endl;
	}*/

	// ************************************测试4 QR分解
	/*double* Q = eye_nn(n, 1);
	double* R = test;
	double* Qbar = eye_nn(n, 1);

	for (int i = 0; i < 1000; i++) {
		Q = dot_nn(R, Q, n);
		qr(R, Q, 0, n, n);
		Qbar = dot_nn(Qbar, Q, n);
	}
	for (int i = 0; i < n * n; i++) {
		cout << Qbar[i] << " ";
		if ((i + 1) % n == 0)
			cout << endl;
	}
	R = dot_nn(R, Q, n);
	for (int i = 0; i < n * n; i++) {
		cout << R[i] << " ";
		if ((i + 1) % n == 0)
			cout << endl;
	}*/

	// ************************************测试5 无移动QR算法
	/*double* result = eye_nn(2, 0);
	qr_eng(result, test, 2);
	for (int i = 0; i < 2 * 2; i++) {
		cout << test[i] << " ";
		if ((i + 1) % 2 == 0)
			cout << endl;
	}
	for (int i = 0; i < 2 * 2; i++) {
		cout << result[i] << " ";
		if ((i + 1) % 2 == 0)
			cout << endl;
	}*/
}