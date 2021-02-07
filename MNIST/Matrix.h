#pragma once

#include <algorithm>;

template<typename T>
class Matrix
{
	// an m x n matrix
	size_t m_m;
	size_t m_n;
	size_t m_iStride;
	size_t m_jStride;
	size_t m_size;

	T* m_data;
	T* m_originalData;

	size_t* m_referenceCount;

public:
	class RowCol
	{
		friend Matrix;

		const size_t jStride;

		T* const data;

		inline constexpr RowCol(size_t jStride, T* data);

	public:
		inline constexpr T& operator[](size_t j);
		inline constexpr const T& operator[](size_t j) const;
	};

	inline constexpr Matrix(size_t m, size_t n, size_t iStride, size_t jStride, size_t size, T* data, T* originalData, size_t* referenceCount);
	inline constexpr Matrix(size_t m, size_t n, size_t size, T* data, size_t* referenceCount);

public:
	inline constexpr Matrix(size_t m, size_t n);
	inline constexpr Matrix(size_t m, size_t n, T x);
	inline ~Matrix();

	inline constexpr Matrix(const Matrix& mat);
	inline constexpr Matrix(Matrix&& mat) noexcept;
	inline constexpr Matrix& operator=(const Matrix& mat);
	inline constexpr Matrix& operator=(Matrix&& mat) noexcept;

	inline constexpr void clear();

	inline constexpr size_t m() const;
	inline constexpr size_t n() const;
	inline constexpr size_t iStride() const;
	inline constexpr size_t jStride() const;
	inline constexpr size_t size() const;
	inline constexpr T* data();
	inline constexpr const T* data() const;
	inline constexpr T* originalData();
	inline constexpr const T* originalData() const;
	inline constexpr size_t referenceCount() const;

	inline constexpr RowCol operator[](size_t i);
	inline constexpr const RowCol operator[](size_t i) const;

	inline constexpr RowCol row(size_t i);
	inline constexpr const RowCol row(size_t) const;
	inline constexpr RowCol col(size_t);
	inline constexpr const RowCol col(size_t) const;

	// Replaces this matrix with its submatrix/transpose. Neither operation will reduce memory consumption,
	// as all original data remains present but inaccessible (except through m_originalData).
	inline constexpr Matrix& submatrix(size_t i, size_t j, size_t m, size_t n);
	inline constexpr Matrix& transpose();

	// Produces a submatrix/transpose occupying separate memory from this matrix.
	inline constexpr Matrix copySubmatrix(size_t i, size_t j, size_t m, size_t n) const;
	inline constexpr Matrix copyTranspose() const;

	// Produces a submatrix/transpose sharing memory with this matrix. The memory sharing persists through
	// further share method calls and move assigment/construction, but is destroyed by copying.
	inline constexpr Matrix shareSubmatrix(size_t i, size_t j, size_t m, size_t n);
	inline constexpr const Matrix shareSubmatrix(size_t i, size_t j, size_t m, size_t n) const;
	inline constexpr Matrix shareTranspose();
	inline constexpr const Matrix shareTranspose() const;

	friend inline constexpr Matrix operator+(const Matrix& mat);
	friend inline constexpr Matrix operator-(const Matrix& mat);
	friend inline constexpr Matrix operator+(const Matrix& lhs, const Matrix& rhs);
	friend inline constexpr Matrix operator-(const Matrix& lhs, const Matrix& rhs);
	friend inline constexpr Matrix operator*(const Matrix& lhs, T rhs);
	friend inline constexpr Matrix operator*(T lhs, const Matrix& rhs);
	friend inline constexpr Matrix operator/(const Matrix& lhs, T rhs);
	friend inline constexpr Matrix operator*(const Matrix& lhs, const Matrix& rhs);
};

template<typename T>
inline constexpr Matrix<T>::RowCol::RowCol(size_t jStride, T* data) :
	jStride(jStride),
	data(data)
{}

template<typename T>
inline constexpr T& Matrix<T>::RowCol::operator[](size_t j)
{
	return data[j * jStride];
}

template<typename T>
inline constexpr const T& Matrix<T>::RowCol::operator[](size_t j) const
{
	return data[j * jStride];
}

template<typename T>
inline constexpr Matrix<T>::Matrix(size_t m, size_t n, size_t iStride, size_t jStride, size_t size, T* data, T* originalData, size_t* referenceCount) :
	m_m(m),
	m_n(n),
	m_iStride(iStride),
	m_jStride(jStride),
	m_size(size),
	m_data(data),
	m_originalData(originalData),
	m_referenceCount(referenceCount)
{}

template<typename T>
inline constexpr Matrix<T>::Matrix(size_t m, size_t n, size_t size, T* data, size_t* referenceCount) :
	Matrix(m, n, n, 1, size, data, data, referenceCount)
{}

template<typename T>
inline constexpr Matrix<T>::Matrix(size_t m, size_t n) :
	Matrix(m, n, m * n, new T(m * n), new size_t(1))
{}

template<typename T>
inline constexpr Matrix<T>::Matrix(size_t m, size_t n, T x) :
	Matrix(m, n)
{
	std::fill(m_data, m_data + m_size, x);
}

template<typename T>
inline Matrix<T>::~Matrix()
{
	clear();
}

template<typename T>
inline constexpr Matrix<T>::Matrix(const Matrix& mat) :
	Matrix(mat.m_m, mat.m_n)
{
	for (size_t i = 0; i < m_n; i++)
	{
		RowCol thisRow = this->row(i);
		RowCol matRow = mat.row(i);
		for (size_t j = 0; j < m_m; j++)
		{
			thisRow[j] = matRow[j];
		}
	}
}

template<typename T>
inline constexpr Matrix<T>::Matrix(Matrix&& mat) noexcept :
	Matrix(mat.m_m, mat.m_n, mat.m_iStride, mat.m_jStride, mat.m_size, mat.m_data, mat.m_originalData, mat.m_referenceCount)
{
	(*m_referenceCount)++;
}

template<typename T>
inline constexpr Matrix<T>& Matrix<T>::operator=(const Matrix& mat)
{
	if (&mat == this)
	{
		return;
	}
	clear();
	m_m = mat.m_m;
	m_n = mat.m_n;
	m_iStride = mat.m_iStride;
	m_jStride = mat.m_jStride;
	m_size = mat.m_size;
	m_data = new T[m_size];
	m_originalData = m_data;
	m_referenceCount = new size_t(1);
	for (size_t i = 0; i < m_n; i++)
	{
		RowCol thisRow = this->row(i);
		RowCol matRow = mat.row(i);
		for (size_t j = 0; j < m_m; j++)
		{
			thisRow[j] = matRow[j];
		}
	}
	return *this;
}

template<typename T>
inline constexpr Matrix<T>& Matrix<T>::operator=(Matrix&& mat) noexcept
{
	if (&mat == this)
	{
		return;
	}
	clear();
	m_m = mat.m_m;
	m_n = mat.m_n;
	m_iStride = mat.m_iStride;
	m_jStride = mat.m_jStride;
	m_size = mat.m_size;
	m_data = mat.m_data;
	m_originalData = m_data;
	m_referenceCount = mat.m_referenceCount;
	m_referenceCount++;
	return *this;
}

template<typename T>
inline constexpr void Matrix<T>::clear()
{
	if (m_data == nullptr)
	{
		return;
	}
	(*m_referenceCount)--;
	if (m_referenceCount == 0)
	{
		delete[] m_originalData;
	}
	m_n = 0;
	m_m = 0;
	m_iStride = 0;
	m_jStride = 1;
	m_size = 0;
	m_data = nullptr;
	m_originalData = nullptr;
	m_referenceCount = nullptr;
}

template<typename T>
inline constexpr size_t Matrix<T>::m() const
{
	return m_m;
}

template<typename T>
inline constexpr size_t Matrix<T>::n() const
{
	return m_n;
}

template<typename T>
inline constexpr size_t Matrix<T>::iStride() const
{
	return m_iStride;
}

template<typename T>
inline constexpr size_t Matrix<T>::jStride() const
{
	return m_jStride;
}

template<typename T>
inline constexpr size_t Matrix<T>::size() const
{
	return m_size;
}

template<typename T>
inline constexpr T* Matrix<T>::data()
{
	return m_data;
}

template<typename T>
inline constexpr const T* Matrix<T>::data() const
{
	return m_data;
}

template<typename T>
inline constexpr T* Matrix<T>::originalData()
{
	return m_originalData;
}

template<typename T>
inline constexpr const T* Matrix<T>::originalData() const
{
	return m_originalData;
}

template<typename T>
inline constexpr size_t Matrix<T>::referenceCount() const
{
	return *m_referenceCount;
}

template<typename T>
inline constexpr typename Matrix<T>::RowCol Matrix<T>::operator[](size_t i)
{
	return RowCol(m_jStride, m_data + i * m_iStride);
}

template<typename T>
inline constexpr const typename Matrix<T>::RowCol Matrix<T>::operator[](size_t i) const
{
	return RowCol(m_jStride, m_data + i * m_iStride);
}

template<typename T>
inline constexpr typename Matrix<T>::RowCol Matrix<T>::row(size_t i)
{
	return RowCol(m_jStride, data + i & m_iStride);
}

template<typename T>
inline constexpr typename const Matrix<T>::RowCol Matrix<T>::row(size_t i) const
{
	return RowCol(m_jStride, data + i & m_iStride);
}

template<typename T>
inline constexpr typename Matrix<T>::RowCol Matrix<T>::col(size_t j)
{
	return RowCol(m_iStride, data + j & m_jStride);
}

template<typename T>
inline constexpr typename const Matrix<T>::RowCol Matrix<T>::col(size_t j) const
{
	return RowCol(m_iStride, data + j & m_jStride);
}

template<typename T>
inline constexpr Matrix<T>& Matrix<T>::submatrix(size_t i, size_t j, size_t m, size_t n)
{
	data += i * iStride + j * jStride;
	m_m = m;
	m_n = n;
	m_size = m * n;
	return *this;
}

template<typename T>
inline constexpr Matrix<T>& Matrix<T>::transpose()
{
	std::swap(m_m, m_n);
	std::swap(m_iStride, m_jStride);
	return *this;
}

template<typename T>
inline constexpr Matrix<T> Matrix<T>::copySubmatrix(size_t i, size_t j, size_t m, size_t n) const
{
	Matrix mat(m, n);
	for (size_t k = 0; k < m; k++)
	{
		RowCol matRow = mat[k];
		RowCol thisRow = (*this)[k + i];
		for (size_t l = 0; l < n; l++)
		{
			matRow[l] = thisRow[l + j];
		}
	}
	return mat;
}

template<typename T>
inline constexpr Matrix<T> Matrix<T>::copyTranspose() const
{
	return Matrix(*this).transpose();
}

template<typename T>
inline constexpr Matrix<T> Matrix<T>::shareSubmatrix(size_t i, size_t j, size_t m, size_t n)
{
	m_referenceCount++;
	return Matrix(m, n, m_iStride, m_jStride, m * n, m_data + i * m_iStride + j * m_jStride, m_originalData, m_referenceCount);
}

template<typename T>
inline constexpr const Matrix<T> Matrix<T>::shareSubmatrix(size_t i, size_t j, size_t m, size_t n) const
{
	m_referenceCount++;
	return Matrix(m, n, m_iStride, m_jStride, m * n, m_data + i * m_iStride + j * m_jStride, m_originalData, m_referenceCount);
}

template<typename T>
inline constexpr Matrix<T> Matrix<T>::shareTranspose()
{
	m_referenceCount++;
	return Matrix(m_n, m_m, m_jStride, m_iStride, m_m * m_n, m_data, m_originalData, m_referenceCount);
}

template<typename T>
inline constexpr const Matrix<T> Matrix<T>::shareTranspose() const
{
	m_referenceCount++;
	return Matrix(m_n, m_m, m_jStride, m_iStride, m_m * m_n, m_data, m_originalData, m_referenceCount);
}

template<typename T>
inline constexpr Matrix<T> operator+(const Matrix<T>& mat)
{
	Matrix<T> ret(mat.m_m, mat.m_n);
	for (size_t i = 0; i < mat.m_m; i++)
	{
		typename Matrix<T>::RowCol retRow = ret.row(i);
		typename Matrix<T>::RowCol matRow = mat.row(i);
		for (size_t j = 0; j < mat.m_n; j++)
		{
			retRow[j] = matRow[j];
		}
	}
	return ret;
}

template<typename T>
inline constexpr Matrix<T> operator-(const Matrix<T>& mat)
{
	Matrix<T> ret(mat.m_m, mat.m_n);
	for (size_t i = 0; i < mat.m_m; i++)
	{
		typename Matrix<T>::RowCol retRow = ret.row(i);
		typename Matrix<T>::RowCol matRow = mat.row(i);
		for (size_t j = 0; j < mat.m_n; j++)
		{
			retRow[j] = -matRow[j];
		}
	}
	return ret;
}

template<typename T>
inline constexpr Matrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
	Matrix<T> ret(lhs.m_m, lhs.m_n);
	for (size_t i = 0; i < lhs.m_m; i++)
	{
		typename Matrix<T>::RowCol retRow = ret.row(i);
		typename Matrix<T>::RowCol lhsRow = lhs.row(i);
		typename Matrix<T>::RowCol rhsRow = rhs.row(i);
		for (size_t j = 0; j < lhs.m_n; j++)
		{
			retRow[j] = lhsRow[j] + rhsRow[j];
		}
	}
	return ret;
}

template<typename T>
inline constexpr Matrix<T> operator-(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
	Matrix<T> ret(lhs.m_m, lhs.m_n);
	for (size_t i = 0; i < lhs.m_m; i++)
	{
		typename Matrix<T>::RowCol retRow = ret.row(i);
		typename Matrix<T>::RowCol lhsRow = lhs.row(i);
		typename Matrix<T>::RowCol rhsRow = rhs.row(i);
		for (size_t j = 0; j < lhs.m_n; j++)
		{
			retRow[j] = lhsRow[j] - rhsRow[j];
		}
	}
	return ret;
}

template<typename T>
inline constexpr Matrix<T> operator*(const Matrix<T>& lhs, T rhs)
{
	Matrix<T> ret(lhs.m_m, lhs.m_n);
	for (size_t i = 0; i < lhs.m_m; i++)
	{
		typename Matrix<T>::RowCol retRow = ret.row(i);
		typename Matrix<T>::RowCol lhsRow = lhs.row(i);
		for (size_t j = 0; j < lhs.m_n; j++)
		{
			retRow[j] = lhsRow[j] * rhs;
		}
	}
	return ret;
}

template<typename T>
inline constexpr Matrix<T> operator*(T lhs, const Matrix<T>& rhs)
{
	Matrix<T> ret(rhs.m_m, rhs.m_n);
	for (size_t i = 0; i < rhs.m_m; i++)
	{
		typename Matrix<T>::RowCol retRow = ret.row(i);
		typename Matrix<T>::RowCol rhsRow = rhs.row(i);
		for (size_t j = 0; j < rhs.m_n; j++)
		{
			retRow[j] = lhs * rhsRow[j];
		}
	}
	return ret;
}

template<typename T>
inline constexpr Matrix<T> operator/(const Matrix<T>& lhs, T rhs)
{
	Matrix<T> ret(lhs.m_m, lhs.m_n);
	for (size_t i = 0; i < lhs.m_m; i++)
	{
		typename Matrix<T>::RowCol retRow = ret.row(i);
		typename Matrix<T>::RowCol lhsRow = lhs.row(i);
		for (size_t j = 0; j < lhs.m_n; j++)
		{
			retRow[j] = lhsRow[j] / rhs;
		}
	}
}

template<typename T>
inline constexpr Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
	Matrix<T> res(lhs.m_m, rhs.m_n, 0);
	for (size_t i = 0; i < lhs.m_m; i++)
	{
		typename Matrix<T>::RowCol lhsRow = lhs.row(i);
		typename Matrix<T>::RowCol resRow = lhs.row(i);
		for (size_t j = 0; j < rhs.m_n; j++)
		{
			T& resVal = resRow[j];
			typename Matrix<T>::RowCol rhsCol = rhs.col(j);
			for (size_t k = 0; k < lhs.m_n; k++)
			{
				resVal += lhsRow[k] * rhsCol[k];
			}
		}
	}
	return res;
}
