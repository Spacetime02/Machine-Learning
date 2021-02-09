#include <fstream>
#include <iostream>
#include <string>

#include "Util/Matrix.h"

// Conventions:
// Gradients are represented as row matrices (d[0][j] is the derivative of y w.r.t. x[j]).
// Jacobians are represented in the numerator layout (J[i][j] is the derivative of y[i] w.r.t. x[j]).

const char* const TST_OUT_PATH = "dataset/t10k-labels.idx1-ubyte";
const char* const TST_IMG_PATH = "dataset/t10k-images.idx3-ubyte";
const char* const TRN_OUT_PATH = "dataset/train-labels.idx1-ubyte";
const char* const TRN_IMG_PATH = "dataset/train-images.idx3-ubyte";

void testMatrices()
{

	using namespace std;
	using namespace math;
	Matrix<int> m1(3, 4, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 });
	Matrix<int> m2(3, 4, { 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 });
	int s1 = 5;
	cout << setw(3) << m1 << endl << endl;
	cout << setw(3) << m2 << endl << endl;
	cout << setw(3) << -m1 << endl << endl;
	cout << setw(3) << m1 + m2 << endl << endl;
	cout << setw(3) << m1 + s1 << endl << endl;
	cout << setw(3) << s1 + m2 << endl << endl;
	cout << setw(3) << m1 - m2 << endl << endl;
	cout << setw(3) << m1 - s1 << endl << endl;
	cout << setw(3) << s1 - m2 << endl << endl;
	cout << setw(3) << m1 * m2 << endl << endl;
	cout << setw(3) << m1 * s1 << endl << endl;
	cout << setw(3) << s1 * m2 << endl << endl;
	cout << setw(3) << m1 / m2 << endl << endl;
	cout << setw(3) << m1 / s1 << endl << endl;
	cout << setw(3) << s1 / m2 << endl << endl;
	cout << setw(3) << m1 % m2 << endl << endl;
	cout << setw(3) << m1 % s1 << endl << endl;
	cout << setw(3) << s1 % m2 << endl << endl;
	cout << setw(3) << (m1 & m2) << endl << endl;
	cout << setw(3) << (m1 & s1) << endl << endl;
	cout << setw(3) << (s1 & m2) << endl << endl;
	cout << setw(3) << (m1 | m2) << endl << endl;
	cout << setw(3) << (m1 | s1) << endl << endl;
	cout << setw(3) << (s1 | m2) << endl << endl;
	cout << setw(3) << (m1 ^ m2) << endl << endl;
	cout << setw(3) << (m1 ^ s1) << endl << endl;
	cout << setw(3) << (s1 ^ m2) << endl << endl;
	cout << setw(3) << ~m1 << endl << endl;
	cout << setw(3) << !m1 << endl << endl;
	cout << setw(3) << (m1 << m2) << endl << endl;
	cout << setw(3) << (m1 << s1) << endl << endl;
	cout << setw(3) << (s1 << m2) << endl << endl;
	cout << setw(3) << (m1 >> m2) << endl << endl;
	cout << setw(3) << (m1 >> s1) << endl << endl;
	cout << setw(3) << (s1 >> m2) << endl << endl;
	cout << setw(3) << min(m1, m2) << endl << endl;
	cout << setw(3) << min(m1, s1) << endl << endl;
	cout << setw(3) << min(s1, m2) << endl << endl;
	cout << setw(3) << max(m1, m2) << endl << endl;
	cout << setw(3) << max(m1, s1) << endl << endl;
	cout << setw(3) << max(s1, m2) << endl << endl;

	cout << setw(3) << m2.transpose() << endl << endl;
	cout << setw(3) << (m1 && m2) << endl << endl;

	m2.transpose();
	m2 = { 2, 2, 2, 5, 5, 5, 8, 8, 8, 11, 11, 11 };
	cout << setw(3) << (m1 == m2) << endl << endl;
	cout << setw(3) << (m1 == s1) << endl << endl;
	cout << setw(3) << (s1 == m2) << endl << endl;
	cout << setw(3) << (m1 != m2) << endl << endl;
	cout << setw(3) << (m1 != s1) << endl << endl;
	cout << setw(3) << (s1 != m2) << endl << endl;
	cout << setw(3) << (m1 > m2) << endl << endl;
	cout << setw(3) << (m1 > s1) << endl << endl;
	cout << setw(3) << (s1 > m2) << endl << endl;
	cout << setw(3) << (m1 < m2) << endl << endl;
	cout << setw(3) << (m1 < s1) << endl << endl;
	cout << setw(3) << (s1 < m2) << endl << endl;
	cout << setw(3) << (m1 >= m2) << endl << endl;
	cout << setw(3) << (m1 >= s1) << endl << endl;
	cout << setw(3) << (s1 >= m2) << endl << endl;
	cout << setw(3) << (m1 <= m2) << endl << endl;
	cout << setw(3) << (m1 <= s1) << endl << endl;
	cout << setw(3) << (s1 <= m2) << endl << endl;

	m2 = { 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
	cout << setw(3) << (m1 += m2) << endl << endl;
	cout << setw(3) << (m1 += s1) << endl << endl;
	cout << setw(3) << (m1 -= m2) << endl << endl;
	cout << setw(3) << (m1 -= s1) << endl << endl;
	cout << setw(3) << (m1 *= m2) << endl << endl;
	cout << setw(3) << (m1 *= s1) << endl << endl;
	cout << setw(3) << (m1 /= m2) << endl << endl;
	cout << setw(3) << (m1 /= s1) << endl << endl;
	cout << setw(3) << (m1 %= m2) << endl << endl;
	cout << setw(3) << (m1 %= s1) << endl << endl;
	cout << setw(3) << (m1 &= m2) << endl << endl;
	cout << setw(3) << (m1 &= s1) << endl << endl;
	cout << setw(3) << (m1 |= m2) << endl << endl;
	cout << setw(3) << (m1 |= s1) << endl << endl;
	cout << setw(3) << (m1 ^= m2) << endl << endl;
	cout << setw(3) << (m1 ^= s1) << endl << endl;
	cout << setw(3) << (m1 <<= m2) << endl << endl;
	cout << setw(3) << (m1 <<= s1) << endl << endl;
	cout << setw(3) << (m1 >>= m2) << endl << endl;
	cout << setw(3) << (m1 >>= s1) << endl << endl;

	cin >> m1 >> m2;
}

int main()
{
	testMatrices();
	return 0;
}