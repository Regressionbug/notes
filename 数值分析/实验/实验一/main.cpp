#include<cstdio>
#include<iostream>
#include<algorithm>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<bitset>
#include<Windows.h>

using namespace std;

unsigned ss[16] = {
	4, 9, 25, 81,
	17 * 17, 33 * 33, 65 * 65, 129 * 129,
	257 * 257, 513 * 513, 1025 * 1025, 2049 * 2049,
	4097 * 4097, 8193 * 8193, 16385 * 16385, 32769 * 32769,
};

struct node {
	int key;
	int value;
	node* left;
	node* right;
};

struct tree {
	node* root;
};

tree t;

unsigned my_isqrt_num = 0;
unsigned my_isqrt_op_num = 0;
unsigned isqrt2_num = 0;
unsigned isqrt3_num = 0;
unsigned isqrt4_num = 0;

//float get_reciprocal(float);
//double bi_sqrt(int);
unsigned my_isqrt(unsigned);
unsigned my_isqrt_op(unsigned);
unsigned isqrt2(unsigned);
unsigned isqrt3(unsigned);
unsigned isqrt4(unsigned);
double tickTock(unsigned, unsigned(*func)(unsigned));
double tickTock(double, double(*func)(double));

void init();
int search(int, node*);

int main() {
	init();

	double time = 0;
	unsigned start = 1;
	unsigned top = (1 << 24);
	cout << "迭代开始: " << bitset<32>(start) << "(32位)" << endl;
	cout << "迭代结束: " << bitset<32>(top) << "(32位)" << endl << endl;
	
	for (unsigned c = start; c < top; c++) {
		time += tickTock((double)c, sqrt);
	}
	cout << "cmath sqrt: ";
	cout << (time / top) << "us" << endl;

	time = 0;
	for (unsigned c = start; c < top; c++) {
		time += tickTock(c, my_isqrt);
	}
	cout << "my_isqrt: ";
	cout << "平均迭代次数: ";
	cout << ((float)my_isqrt_num / top);
	cout << " 平均时间: ";
	cout << (time / top) << "us" << endl;

	time = 0;
	for (unsigned c = start; c < top; c++) {
		time += tickTock(c, my_isqrt_op);
	}
	cout << "my_isqrt_op: ";
	cout << "平均迭代次数: ";
	cout << ((float)my_isqrt_op_num / top);
	cout << " 平均时间: ";
	cout << (time / top) << "us" << endl;

	time = 0;
	for (unsigned c = start; c < top; c++) {
		time += tickTock(c, isqrt2);
	}
	cout << "isqrt2: ";
	cout << "平均迭代次数: ";
	cout << ((float)isqrt2_num / top);
	cout << " 平均时间: ";
	cout << (time / top) << "us" << endl;

	time = 0;
	for (unsigned c = start; c < top; c++) {
		time += tickTock(c, isqrt3);
	}
	cout << "isqrt3: ";
	cout << "平均迭代次数: ";
	cout << ((float)isqrt3_num / top);
	cout << " 平均时间: ";
	cout << (time / top) << "us" << endl;

	time = 0;
	for (unsigned c = start; c < top; c++) {
		time += tickTock(c, isqrt4);
	}
	cout << "isqrt4: ";
	cout << "平均迭代次数: ";
	cout << ((float)isqrt4_num / top);
	cout << " 平均时间: ";
	cout << (time / top) << "us" << endl;
	return 0;
}

void init() {
	node* n1, * n2, * n3, * n4, * n5, * n6, * n7, * n8;
	n1 = (node*)malloc(sizeof(node));
	n2 = (node*)malloc(sizeof(node));
	n3 = (node*)malloc(sizeof(node));
	n4 = (node*)malloc(sizeof(node));
	n5 = (node*)malloc(sizeof(node));
	n6 = (node*)malloc(sizeof(node));
	n7 = (node*)malloc(sizeof(node));
	n8 = (node*)malloc(sizeof(node));

	n1->key = 7;
	n1->value = 8;
	n2->key = 6;
	n2->value = 7;
	n3->key = 5;
	n3->value = 6;
	n4->key = 4;
	n4->value = 5;
	n5->key = 3;
	n5->value = 4;
	n6->key = 2;
	n6->value = 3;
	n7->key = 1;
	n7->value = 2;
	n8->key = 0;
	n8->value = 1;

	t.root = n1;
	n1->left = n2;
	n2->left = n3;
	n3->left = n4;
	n4->left = n5;
	n5->left = n6;
	n6->left = n7;
	n7->left = n8;
}

int search(int c, node* root) {
	if ((1 << (root->key*2)) <= c)
		return root->value;
	else
		return search(c, root->left);
}

double tickTock(unsigned n, unsigned(*func)(unsigned)) 
{
	double sum = 0;

	double run_time;
	_LARGE_INTEGER time_start;	//开始时间
	_LARGE_INTEGER time_over;	//结束时间
	double dqFreq;		//计时器频率
	LARGE_INTEGER f;	//计时器频率
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	QueryPerformanceCounter(&time_start);	//计时开始
	sum = func(n);//调用函数A
	QueryPerformanceCounter(&time_over);	//计时结束
	run_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
	// 乘以1000000把单位由秒化为微秒，精度为1000 000/（cpu主频）微秒
	// printf("result: %lf, run_time：%fus\n", sum, run_time);
	return run_time;
}

double tickTock(double n, double(*func)(double))
{
	double sum = 0;

	double run_time;
	_LARGE_INTEGER time_start;	//开始时间
	_LARGE_INTEGER time_over;	//结束时间
	double dqFreq;		//计时器频率
	LARGE_INTEGER f;	//计时器频率
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	QueryPerformanceCounter(&time_start);	//计时开始
	sum = func(n);//调用函数A
	QueryPerformanceCounter(&time_over);	//计时结束
	run_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
	// 乘以1000000把单位由秒化为微秒，精度为1000 000/（cpu主频）微秒
	// printf("result: %lf, run_time：%fus\n", sum, run_time);
	return run_time;
}

unsigned my_isqrt(unsigned c) {
	// 牛顿法
	if (c <= 1) return c;
	unsigned* temp = upper_bound(ss, ss + 16, c);
	int s = temp - ss;
	unsigned x0 = 1 << s;
	unsigned x1 = (x0 + (c >> s)) >> 1;
	while (x1 < x0)
	{
		x0 = x1;
		x1 = (x0 + c / x0) >> 1;
		my_isqrt_num++;
	}
	return x0;
}

unsigned my_isqrt_op(unsigned c) {
	int s = search(c, t.root);
	unsigned x0 = 1 << s;
	unsigned x1 = (x0 + (c >> s)) >> 1;
	while (x1 < x0)
	{
		x0 = x1;
		x1 = (x0 + c / x0) >> 1;
		my_isqrt_op_num++;
	}
	return x0;
}

unsigned isqrt2(unsigned x)
{
	unsigned a = 1; //e.
	unsigned b = (x >> 5) + 8; //þ.
	if (b > 65535) b = 65535; //a <= sqrt(x) <= b
	do {
		unsigned m = (a + b) >> 1;
		if (m * m > x) b = m - 1;
		else a = m + 1;
		isqrt2_num++;
	} while (b >= a);
	return a - 1;
}

unsigned isqrt3(unsigned x)
{
	if (x <= 1) return x;
	unsigned x1 = x - 1;
	int s = 1;
	if (x1 > 65535) { s += 8; x1 >>= 16; }
	if (x1 > 255) { s += 4; x1 >>= 8; }
	if (x1 > 15) { s += 2; x1 >>= 4; }
	if (x1 > 3) { s += 1; }
	unsigned x0 = 1 << s;
	x1 = (x0 + (x >> s)) >> 1;
	while (x1 < x0) {
		x0 = x1;
		x1 = (x0 + x / x0) >> 1;
		isqrt3_num++;
	}
	return x0;
}

unsigned isqrt4(unsigned M)
{
	unsigned int N, i;
	unsigned long tmp, ttp;
	if (M == 0)
		return 0;
	N = 0;
	tmp = (M >> 30);
	M <<= 2;
	if (tmp > 1)
	{
		N++;
		tmp -= N;
	}
	for (i = 15; i > 0; i--)
	{
		N <<= 1;
		tmp <<= 2;
		tmp += (M >> 30);
		ttp = N;
		ttp = (ttp << 1) + 1;
		M <<= 2;
		if (tmp >= ttp)
		{
			tmp -= ttp;
			N++;
		}
		isqrt4_num++;
	}
	return N;
}

//double bi_sqrt(int c) {
//	// 二分法求平方根
//	if (c <= 1) return c;
//	unsigned* temp = upper_bound(ss, ss + 16, c);
//	int s = temp - ss;
//	double lo = (int64_t)1 << (s-1);
//	double hi = (int64_t)1 << (s);
//	double mid, pow;
//	do {
//		mid = (lo + hi) / 2;
//		pow = mid * mid;
//		if (pow < c)
//			lo = mid;
//		else
//			hi = mid;
//	} while (1e-8 <= abs(pow - c));
//	return mid;
//}

//float get_reciprocal(float c) {
//	// 牛顿法
//	float x0 = (float)sqrt(2) - (c / 2);
//	float x1 = x0 * (2 - c * x0);
//	int i = 0;
//	while (i++ < 10) {
//		x0 = x1;
//		x1 = x0 * (2 - c * x0);
//		printf("test %d: %f\n", i, x1);
//	}
//	return x1;
//}

//double my_isqrt(unsigned c) {
//	// 牛顿法
//	if (c <= 1) return c;
//	unsigned* temp = upper_bound(ss, ss + 16, c);
//	int s = temp - ss;
//	double x0 = (int64_t)1 << s;
//	double x1 = (x0 + (c >> s)) / 2;
//	int i = 0;
//	while (x1 != x0)
//	{
//		x0 = x1;
//		x1 = (x0 + c / x0) / 2;
//	}
//	return x0;
//}