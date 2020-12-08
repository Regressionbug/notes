#pragma once
#include<iostream>
#include<map>
#include<complex>

using namespace std;

typedef complex<double> Complex;

double A[8 * 8] = {
	611, 196, -192, 407, -8,  -52, -49,  29,
	0,   899, 113, -192, -71, -43, -8,  -44,
	0,   0,   899,  196,  61,  49,  8,   52,
	0,   0,   0,    611,  8,   44,  59, -23,
	0,   0,   0,    0,    411,-599, 208, 208,
	0,   0,   0,    0,    0,   411, 208, 208,
	0,   0,   0,    0,    0,   0,   99, -911,
	0,   0,   0,    0,    0,   0,   0,   99
};
// B
// C
// D
// E

const int iterations = 1000; // ��������
const double error = 1e-10; // ���

int n;
double* a; // Q
// Complex* en; // R
double* en;

double* eye_n(int n, double k) {
	double* temp = new double[n];
	for (int i = 0; i < n; i++)
		temp[i] = k;
	return temp;
}

double* eye_nn(int n, double k) {
	double* temp = new double[n * n];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (i == j)
				temp[i * n + j] = k;
			else
				temp[i * n + j] = 0;
	return temp;
}

double dot_11(double* a, double* b, int n) {
	double res = 0;
	for (int i = 0; i < n; i++) {
		res += a[i] * b[i];
	}
	return res;
}

double* dot_n1(double* a, double* b, int n) {
	double* temp = new double[n];
	for (int i = 0; i < n; i++) temp[i] = (double)0;
	for (int j = 0; j < n; j++) {
		for (int k = 0; k < n; k++) {
			temp[j] += b[k] * a[j * n + k];
		}
	}
	return temp;
}

double* dot_1n(double* a, double* b, int n) {
	double* temp = new double[n];
	for (int i = 0; i < n; i++) temp[i] = (double)0;
	for (int j = 0; j < n; j++) {
		for (int k = 0; k < n; k++) {
			temp[j] += a[k] * b[k * n + j];
		}
	}
	return temp;
}

double* dot_nn(double* a, double* b, int n) {
	double* temp = new double[n * n];
	for (int i = 0; i < n; i++) { // 1-n��
		for (int j = 0; j < n; j++) { // 1-n��
			double res = 0;
			for (int k = 0; k < n; k++) {
				int m1 = i * n + k;
				int m2 = k * n + j;
				res += a[m1] * b[m2];
			}
			temp[i * n + j] = res;
		}
	}
	return temp;
}

bool qr(double* R, double* a, int i, int j, int n) { // QR�ֽ⣬gram-schimdt����
	for (int i = 0; i < n * n; i++)
		R[i] = 0;

	int len = j - i;

	double* y = new double[len];
	for (int col = i; col < j; col++) { // ��i����j��
		// ѡȡy: ��i�е���j��
		// y = a[:, j]
		for (int row = i; row < j; row++) {
			y[row] = a[row * n + col];
		}

		double* q = new double[len];
		for (int kcol = i; kcol < col; kcol++) { // ǰcol��,������col
			// ǰj�е�q����(��������)
			// q = Q[:, i]
			for (int row = i; row < j; row++)
				q[row - i] = a[row * n + kcol];

			// rij:��i�����j��
			// rij = q * y
			double r = dot_11(q, y, len);
			R[(kcol - i) * len + (col - i)] = r;

			// �Ľ�gram-schimdt����
			// ����y: y = y - rq
			for (int row = 0; row < len; row++) {
				y[row] = y[row] - q[row] * r;
			}
		}
		// rjj = |y|
		double div = sqrt(dot_11(y, y, len));
		R[(col - i) * len + col - i] = div;
		// ��һ��
		// a[:, j] = q = y / |y|
		for (int row = 0; row < len; row++) {
			a[row * n + col] = y[row] / div;
		}

		delete[] q;
	}

	delete[] y;
	return true;
}

bool QR(double* en, double* a, int i, int j) { // ���ƶ�QR�㷨
	int len = j - i;
	double* Q = eye_nn(len, 1);
	double* R = a; // ����ֵ
	double* Qbar = eye_nn(len, 1);

	for (int m = 0; m < iterations; m++) {
		Q = dot_nn(R, Q, len);
		qr(R, Q, i, j, n);
		Qbar = dot_nn(Qbar, Q, len);
	}
	R = dot_nn(R, Q, n); // �Խ���Ϊ����ֵ
	for (int k = i; k < j; k++)
		en[k * n + k] = R[(k - i) * len + k - i];
	// QbarΪ������������
	for (int row = i; row < j; row++) {
		for (int col = i; col < j; col++) {
			a[col * n + row] = Qbar[(col - i) * len + row - i];
		}
	}
	delete[] Q;
	delete[] R;
	return true;
}

bool qr_aux(int i, int j) {
	if (j - i <= 2) {
		QR(en, a, i, j);
		return true;
	}
	for (int m = i; m < j; m++) {
		int k = (i + 1) * n + i;
		if (a[k] == 0) {
			return qr_aux(i, k) && qr_aux(k, j);
		}
	}
	QR(en, a, i, j);
	return true;
}

bool power_eng(double* pld, double* env, double* a, int n) {
	double* temp = eye_n(n, 0);

	for (int i = 0; i < iterations; i++) {
		temp = dot_n1(a, env, n);
		double res = 0;
		for (int j = 0; j < n; j++) {
			env[j] = (double)temp[j];
			res += temp[j] * temp[j];
			temp[j] = 0;
		}
		for (int j = 0; j < n; j++) {
			env[j] /= sqrt(res);
		}
	}
	double res = dot_11(env, env, n);
	temp = dot_1n(env, a, n);
	res = dot_11(temp, env, n) / res;
	*pld = res;

	delete[] temp;

	return true;
}

bool jacobi_eng(double* ev, double* a, int n) {
	for (int m = 0; m < iterations; m++) {
		double aMax = a[1];
		int row = 0, col = 1;
		// �ڶԽ������ҵ����Ԫ��
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				double d = fabs(a[i * n + j]);
				if (i != j && aMax < d) {
					aMax = d;
					row = i;
					col = j;
				}
			}
		}
		if (aMax < error) break;

		double pp = a[row * n + row];
		double pq = a[row * n + col];
		double qq = a[col * n + col];

		// ��ת�Ƕ�
		double angle = 0.5 * atan2(-2 * pq, qq - pp);
		double sinTheta = sin(angle);
		double cosTheta = cos(angle);
		double sin2Theta = sin(2 * angle);
		double cos2Theta = cos(2 * angle);

		a[row * n + row] = pp * cosTheta * cosTheta + qq * sinTheta * sinTheta + 2 * pq * cosTheta * sinTheta;
		a[col * n + col] = pp * sinTheta * sinTheta + qq * cosTheta * cosTheta - 2 * pq * cosTheta * sinTheta;
		a[row * n + col] = 0.5 * (qq - pp) * sin2Theta + pq * cos2Theta;
		a[col * n + row] = a[row * n + col];

		for (int i = 0; i < n; i++) {
			if (i != col && i != row) {
				int u = i * n + row; // p
				int w = i * n + col; // q
				double temp = a[u];
				a[u] = a[w] * sinTheta + a[u] * cosTheta;
				a[w] = a[w] * cosTheta - temp * sinTheta;
			}
		}

		for (int j = 0; j < n; j++) {
			if (j != col && j != row) {
				int u = row * n + j; // p
				int w = col * n + j; // q
				double temp = a[u];
				a[u] = a[w] * sinTheta + a[u] * cosTheta;
				a[w] = a[w] * cosTheta - temp * sinTheta;
			}
		}

		// ������������
		for (int i = 0; i < n; i++) {
			int u = i * n + row; // p
			int w = i * n + col; // q
			double temp = ev[u];
			ev[u] = ev[w] * sinTheta + ev[u] * cosTheta;
			ev[w] = ev[w] * cosTheta - temp * sinTheta;
		}
	}
	return true;
}

void gauss_hessen(double* a, int n) {
	for (int i = 2; i < n; i++) { // �ӵ�2�п�ʼ
		for (int j = 0; j < i - 1; j++) { // �ӵ�1�п�ʼ����i-1��
			// ��ѡ����: �ҵ���i-1�е���n�е����ֵ, ����������
			// ����Ϊ0, ��continue
			//int m = i - 1, p = -1;
			//double dmax = a[(i - 1) * n + j];
			//for (int k = i; k < n; k++) {
			//	double res = a[k * p + j];
			//	if ((dmax == 0 || dmax < res) && res != 0) {
			//		dmax = res;
			//		p = k;
			//	}
			//}
			//if (fabs(dmax) < error || p!=-1)
			//	continue;
			//else { // ����������
			//	double* temp = new double[n];
			//	// ������
			//	memcpy(temp, a + m * n, n);
			//	memcpy(a + m * n, a + p * n, n);
			//	memcpy(a + p * n, temp, n);
			//	// ������
			//	for (int k = 0; k < n; k++) {
			//		temp[k] = a[k * n + m];
			//	}
			//	for (int k = 0; k < n; k++) {
			//		a[k * n + m] = a[k * n + p];
			//	}
			//	for (int k = 0; k < n; k++) {
			//		a[k * n + p] = temp[k];
			//	}
			//}
			int k1 = (1 + j) * n + j;
			int k2 = i * n + j;
			double r = a[k2] / a[k1];
			a[k2] = 0;
			for (int m = 0; m < n - j - 1; m++) {
				k1++; k2++;
				a[k2] = a[k2] - r * a[k1];
			}
		}
	}
}

bool qr_eng(double* result, double* h, int m) {
	a = h;
	n = m;
	en = result;

	return qr_aux(0, n);
}