#pragma once

#include <functional>
#include <iomanip>
#include <vector>

#include "Extra Type Traits.h"
#include "Functional.h"

// CLIENT CODE IS RESPONSIBLE FOR BOUNDS CHECKING UNLESS OTHERWISE STATED
// SOME METHODS ASSUME T IS NOT ITSELF A MATRIX

namespace math
{
	template<typename T>
	class Matrix
	{
		size_t m_iSize;
		size_t m_jSize;
		size_t m_iStride;
		size_t m_jStride;
		size_t m_size;

		size_t* m_iMap;
		size_t* m_jMap;

		T* m_data;

		size_t* m_referenceCount;

	public:
		class RowCol
		{
			friend Matrix;

			const size_t m_stride;

			size_t* const m_map;

			T* const m_data;

			inline constexpr RowCol(size_t stride, size_t* const map, T* const data);

		public:
			inline constexpr T& get(size_t i);
			inline constexpr const T& get(size_t i) const;
			inline constexpr T& operator[](size_t i);
			inline constexpr const T& operator[](size_t i) const;
			inline constexpr T& operator()(size_t i);
			inline constexpr const T& operator()(size_t i) const;
		};

	private:
		static inline constexpr size_t* identityMap(size_t size);
		static inline constexpr size_t* subMap(const size_t* map, size_t i, size_t size);
		static inline constexpr size_t* subMap(const size_t* map, const size_t* retain, size_t size);

		inline constexpr Matrix(size_t iSize, size_t jSize, size_t iStride, size_t jStride, size_t size, size_t* iMap, size_t* jMap, T* data, size_t* referenceCount);
		inline constexpr Matrix(size_t iSize, size_t jSize, size_t size, T* data, size_t* referenceCount);

	public:
		inline constexpr Matrix();
		inline constexpr Matrix(size_t iSize, size_t jSize);
		inline constexpr Matrix(size_t iSize, size_t jSize, const T& fill);
		inline constexpr Matrix(size_t iSise, size_t jSize, const std::vector<T>& data);
		inline ~Matrix();

		inline constexpr Matrix(const Matrix& mat);
		inline constexpr Matrix(Matrix&& mat) noexcept;
		inline constexpr Matrix& operator=(const Matrix& mat);
		inline constexpr Matrix& operator=(Matrix&& mat) noexcept;
		inline constexpr Matrix& operator=(const std::vector<T>& data);

	private:
		inline constexpr void addRef() const;
		inline constexpr void remRef() const;

	public:
		inline constexpr void clear();

		inline constexpr size_t iSize() const;
		inline constexpr size_t jSize() const;
		inline constexpr size_t iStride() const;
		inline constexpr size_t jStride() const;
		inline constexpr size_t size() const;
		inline constexpr size_t* iMap();
		inline constexpr const size_t* iMap() const;
		inline constexpr size_t* jMap();
		inline constexpr const size_t* jMap() const;
		inline constexpr T* data();
		inline constexpr const T* data() const;
		inline constexpr size_t referenceCount() const;

		inline constexpr RowCol row(size_t i);
		inline constexpr const RowCol row(size_t i) const;
		inline constexpr RowCol col(size_t j);
		inline constexpr const RowCol col(size_t j) const;
		inline constexpr T& get(size_t i, size_t j);
		inline constexpr const T& get(size_t i, size_t j) const;
		inline constexpr RowCol operator[](size_t i);
		inline constexpr const RowCol operator[](size_t i) const;
		inline constexpr RowCol operator()(size_t j);
		inline constexpr const RowCol operator()(size_t j) const;
		inline constexpr T& operator()(size_t i, size_t j);
		inline constexpr const T& operator()(size_t i, size_t j) const;

		// Submatrix methods with arguments iRetain and jRetain can also reorder rows and columns. Sort these arrays
		// to avoid reordering.

		// Replaces this matrix with its submatrix/transpose. Neither operation will reduce memory consumption,
		// as all original data remains present but inaccessible (except through m_originalData).
		inline constexpr Matrix& submatrix(size_t i, size_t j, size_t iSize, size_t jSize);
		inline constexpr Matrix& submatrix(const size_t* iRetain, const size_t* jRetain, size_t iSize, size_t jSize);
		inline constexpr Matrix& transpose();

		// Produces a submatrix/transpose occupying separate memory from this matrix.
		inline constexpr Matrix copySubmatrix(size_t i, size_t j, size_t iSize, size_t jSize) const;
		inline constexpr Matrix copySubmatrix(const size_t* iRetain, const size_t* jRetain, size_t iSize, size_t jSize) const;
		inline constexpr Matrix copyTranspose() const;

		// Produces a submatrix/transpose sharing memory with this matrix. The memory sharing persists through
		// further share method calls and move assigment/construction, but is destroyed by copying. Note that
		// shared instances have their own iMaps and jMaps which are initially based on this matrix's iMap and jMap
		inline constexpr Matrix shareSubmatrix(size_t i, size_t j, size_t iSize, size_t jSize);
		inline constexpr const Matrix shareSubmatrix(size_t i, size_t j, size_t iSize, size_t jSize) const;
		inline constexpr Matrix shareSubmatrix(const size_t* iRetain, const size_t* jRetain, size_t iSize, size_t jSize);
		inline constexpr const Matrix shareSubmatrix(const size_t* iRetain, const size_t* jRetain, size_t iSize, size_t jSize) const;
		inline constexpr Matrix shareTranspose();
		inline constexpr const Matrix shareTranspose() const;

		inline constexpr Matrix& swapRows(size_t i1, size_t i2);
		inline constexpr Matrix& swapCols(size_t j1, size_t j2);

		template<typename Op, typename... Ts>
		inline constexpr Matrix& assignElementWise(const Op& op, const Ts&... mat);
		inline constexpr Matrix& operator+=(const Matrix& mat);
		inline constexpr Matrix& operator+=(const T& val);
		inline constexpr Matrix& operator-=(const Matrix& mat);
		inline constexpr Matrix& operator-=(const T& val);
		inline constexpr Matrix& operator*=(const Matrix& mat);
		inline constexpr Matrix& operator*=(const T& val);
		inline constexpr Matrix& operator/=(const Matrix& mat);
		inline constexpr Matrix& operator/=(const T& val);
		inline constexpr Matrix& operator%=(const Matrix& mat);
		inline constexpr Matrix& operator%=(const T& val);
		inline constexpr Matrix& operator&=(const Matrix& mat);
		inline constexpr Matrix& operator&=(const T& val);
		inline constexpr Matrix& operator|=(const Matrix& mat);
		inline constexpr Matrix& operator|=(const T& val);
		inline constexpr Matrix& operator^=(const Matrix& mat);
		inline constexpr Matrix& operator^=(const T& val);
		inline constexpr Matrix& operator<<=(const Matrix& mat);
		inline constexpr Matrix& operator<<=(const T& val);
		inline constexpr Matrix& operator>>=(const Matrix& mat);
		inline constexpr Matrix& operator>>=(const T& val);
	};

	template<typename Op, typename... Ts>
	using ElementWiseRes = extra_traits::remove_const_reference_t<std::invoke_result_t<Op, extra_traits::remove_template_t<Ts, Matrix>...>>;

	// At least one of Types must be a matrix type and all matrix parameters must have the same size.
	// Otherwise, behavior is undefined.
	template<typename Op, typename... Ts>
	inline constexpr Matrix<ElementWiseRes<Op, Ts...>> elementWise(const Op& op, const Ts&... params);

	template<typename Mul, typename T, typename U>
	using MatMulRes = extra_traits::remove_const_reference_t<std::invoke_result_t<Mul, T, U>>;

	template<typename Add, typename Mul, typename T, typename U>
	inline constexpr Matrix<MatMulRes<Mul, T, U>> matMul(const Add& add, const Mul& mul, const Matrix<T>& lhs, const Matrix<U>& rhs);

	template<typename T>
	inline constexpr Matrix<T> operator-(const Matrix<T>& mat);
	template<typename T>
	inline constexpr Matrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator+(const Matrix<T>& lhs, const T& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator+(const T& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator-(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator-(const Matrix<T>& lhs, const T& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator-(const T& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator*(const Matrix<T>& lhs, const T& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator*(const T& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator/(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator/(const Matrix<T>& lhs, const T& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator/(const T& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator%(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator%(const Matrix<T>& lhs, const T& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator%(const T& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator&(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator&(const Matrix<T>& lhs, const T& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator&(const T& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator|(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator|(const Matrix<T>& lhs, const T& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator|(const T& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator^(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator^(const Matrix<T>& lhs, const T& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator^(const T& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator~(const Matrix<T>& mat);
	template<typename T>
	inline constexpr Matrix<bool> operator!(const Matrix<T>& mat);
	template<typename T>
	inline constexpr Matrix<T> operator<<(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator<<(const Matrix<T>& lhs, const T& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator<<(const T& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator>>(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator>>(const Matrix<T>& lhs, const T& rhs);
	template<typename T>
	inline constexpr Matrix<T> operator>>(const T& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> min(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> min(const Matrix<T>& lhs, const T& rhs);
	template<typename T>
	inline constexpr Matrix<T> min(const T& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> max(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<T> max(const Matrix<T>& lhs, const T& rhs);
	template<typename T>
	inline constexpr Matrix<T> max(const T& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<bool> operator==(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<bool> operator==(const Matrix<T>& lhs, const T& rhs);
	template<typename T>
	inline constexpr Matrix<bool> operator==(const T& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<bool> operator!=(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<bool> operator!=(const Matrix<T>& lhs, const T& rhs);
	template<typename T>
	inline constexpr Matrix<bool> operator!=(const T& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<bool> operator>(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<bool> operator>(const Matrix<T>& lhs, const T& rhs);
	template<typename T>
	inline constexpr Matrix<bool> operator>(const T& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<bool> operator<(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<bool> operator<(const Matrix<T>& lhs, const T& rhs);
	template<typename T>
	inline constexpr Matrix<bool> operator<(const T& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<bool> operator>=(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<bool> operator>=(const Matrix<T>& lhs, const T& rhs);
	template<typename T>
	inline constexpr Matrix<bool> operator>=(const T& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<bool> operator<=(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<typename T>
	inline constexpr Matrix<bool> operator<=(const Matrix<T>& lhs, const T& rhs);
	template<typename T>
	inline constexpr Matrix<bool> operator<=(const T& lhs, const Matrix<T>& rhs);

	// Matrix multiplication, not logical and. For bool matrices, this performs the boolean product.
	template<typename T>
	inline constexpr Matrix<T> operator&&(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<>
	inline constexpr Matrix<bool> operator&&(const Matrix<bool>& lhs, const Matrix<bool>& rhs);

	template<typename T>
	inline std::istream& operator>>(std::istream& stream, Matrix<T>& mat);
	template<typename T>
	inline std::ostream& operator<<(std::ostream& stream, const Matrix<T>& mat);

	class ElementWiseHelper
	{
		template<typename Op, typename... Ts>
		friend inline constexpr Matrix<ElementWiseRes<Op, Ts...>> elementWise(const Op& op, const Ts&... params);

		template<typename T>
		friend class Matrix;

		template<typename R, typename T0, typename... Ts>
		static inline constexpr Matrix<R> init(const T0& t0, const Ts&... ts);

		template<typename R, typename T0, typename... Ts>
		static inline constexpr Matrix<R> init(const Matrix<T0>& t0, const Ts&... ts);

		template<typename T>
		static inline constexpr const T& index(const T& val, size_t i, size_t j);

		template<typename T>
		static inline constexpr const T& index(const Matrix<T>& mat, size_t i, size_t j);
	};

	template<typename T>
	inline constexpr Matrix<T>::RowCol::RowCol(size_t jStride, size_t* const map, T* const data) :
		m_stride(jStride),
		m_map(map),
		m_data(data)
	{}

	template<typename T>
	inline constexpr T& Matrix<T>::RowCol::get(size_t i)
	{
		return m_data[m_map[i] * m_stride];
	}

	template<typename T>
	inline constexpr const T& Matrix<T>::RowCol::get(size_t i) const
	{
		return m_data[m_map[i] * m_stride];
	}

	template<typename T>
	inline constexpr T& Matrix<T>::RowCol::operator[](size_t i)
	{
		return get(i);
	}

	template<typename T>
	inline constexpr const T& Matrix<T>::RowCol::operator[](size_t i) const
	{
		return get(i);
	}

	template<typename T>
	inline constexpr T& Matrix<T>::RowCol::operator()(size_t i)
	{
		return get(i);
	}

	template<typename T>
	inline constexpr const T& Matrix<T>::RowCol::operator()(size_t i) const
	{
		return get(i);
	}

	template<typename T>
	inline constexpr size_t* Matrix<T>::identityMap(size_t size)
	{
		size_t* ret = new size_t[size];
		for (size_t i = 0; i < size; i++)
		{
			ret[i] = i;
		}
		return ret;
	}

	template<typename T>
	inline constexpr size_t* Matrix<T>::subMap(const size_t* map, size_t i, size_t size)
	{
		size_t* ret = new size_t[size];
		memcpy(ret, map + i, sizeof(size_t) * size);
		return ret;
	}

	template<typename T>
	inline constexpr size_t* Matrix<T>::subMap(const size_t* map, const size_t* retain, size_t size)
	{
		size_t* ret = new size_t[size];
		for (size_t i = 0; i < size; i++)
		{
			ret[i] = map[retain[i]];
		}
		return ret;
	}

	template<typename T>
	inline constexpr Matrix<T>::Matrix(size_t iSize, size_t jSize, size_t iStride, size_t jStride, size_t size, size_t* iMap, size_t* jMap, T* data, size_t* referenceCount) :
		m_iSize(iSize),
		m_jSize(jSize),
		m_iStride(iStride),
		m_jStride(jStride),
		m_iMap(iMap),
		m_jMap(jMap),
		m_size(size),
		m_data(data),
		m_referenceCount(referenceCount)
	{}

	template<typename T>
	inline constexpr Matrix<T>::Matrix(size_t iSize, size_t jSize, size_t size, T* data, size_t* referenceCount) :
		Matrix(iSize, jSize, jSize, 1, size, identityMap(iSize), identityMap(jSize), data, referenceCount)
	{}

	template<typename T>
	inline constexpr Matrix<T>::Matrix() :
		Matrix(0, 0, 0, 1, 0, nullptr, nullptr, nullptr, nullptr)
	{}

	template<typename T>
	inline constexpr Matrix<T>::Matrix(size_t iSize, size_t jSize) :
		Matrix(iSize, jSize, iSize * jSize, new T[iSize * jSize], new size_t(1))
	{}

	template<typename T>
	inline constexpr Matrix<T>::Matrix(size_t iSize, size_t jSize, const T& fill) :
		Matrix(iSize, jSize)
	{
		std::fill(m_data, m_data + m_size, fill);
	}

	template<typename T>
	inline constexpr Matrix<T>::Matrix(size_t iSize, size_t jSize, const std::vector<T>& data) :
		Matrix(iSize, jSize)
	{
		size_t idx = 0;
		for (const T&val : data)
		{
			m_data[idx] = val;
			idx++;
			if (idx > m_size)
			{
				break;
			}
		}
		std::fill(m_data + idx, m_data + m_size, 0);
	}

	template<typename T>
	inline Matrix<T>::~Matrix()
	{
		clear();
	}

	template<typename T>
	inline constexpr void Matrix<T>::addRef() const
	{
		(*m_referenceCount)++;
	}

	template<typename T>
	inline constexpr void Matrix<T>::remRef() const
	{
		(*m_referenceCount)--;
	}

	template<typename T>
	inline constexpr Matrix<T>::Matrix(const Matrix& mat) :
		Matrix(mat.m_iSize, mat.m_jSize)
	{
		for (size_t i = 0; i < m_jSize; i++)
		{
			RowCol thisRow = row(i);
			RowCol matRow = mat.row(i);
			for (size_t j = 0; j < m_iSize; j++)
			{
				thisRow[j] = matRow[j];
			}
		}
	}

	template<typename T>
	inline constexpr Matrix<T>::Matrix(Matrix&& mat) noexcept :
		Matrix(mat.m_iSize, mat.m_jSize, mat.m_iStride, mat.m_jStride, mat.m_size, subMap(mat.m_iMap, (size_t)0, mat.m_iSize), subMap(mat.m_jMap, (size_t)0, mat.m_jSize), mat.m_data, mat.m_referenceCount)
	{
		addRef();
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::operator=(const Matrix& mat)
	{
		if (&mat == this)
		{
			return;
		}
		clear();
		m_iSize = mat.m_iSize;
		m_jSize = mat.m_jSize;
		m_iStride = mat.m_iStride;
		m_jStride = mat.m_jStride;
		m_size = mat.m_size;
		m_iMap = identityMap(m_iSize);
		m_jMap = identityMap(m_jSize);
		m_data = new T[m_size];
		m_referenceCount = new size_t(1);
		for (size_t i = 0; i < m_jSize; i++)
		{
			RowCol thisRow = row(i);
			RowCol matRow = mat.row(i);
			for (size_t j = 0; j < m_iSize; j++)
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
		m_iSize = mat.m_iSize;
		m_jSize = mat.m_jSize;
		m_iStride = mat.m_iStride;
		m_jStride = mat.m_jStride;
		m_size = mat.m_size;
		m_iMap = mat.m_iMap;
		m_jMap = mat.m_jMap;
		m_data = mat.m_data;
		m_referenceCount = mat.m_referenceCount;
		addRef();
		return *this;
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::operator=(const std::vector<T>& data)
	{
		typename std::vector<T>::const_iterator iter = data.cbegin();
		for (size_t i = 0; i < m_iSize; i++)
		{
			RowCol cur = row(i);
			for (size_t j = 0; j < m_jSize; j++)
			{
				if (iter == data.cend())
				{
					cur[j] = 0;
				}
				else
				{
					cur[j] = *iter;
					iter++;
				}
			}
		}
		return *this;
	}

	template<typename T>
	inline constexpr void Matrix<T>::clear()
	{
		if (m_data == nullptr)
		{
			return;
		}
		remRef();
		if (*m_referenceCount == 0)
		{
			delete[] m_data;
		}
		delete[] m_iMap;
		delete[] m_jMap;
		m_jSize = 0;
		m_iSize = 0;
		m_iStride = 0;
		m_jStride = 1;
		m_size = 0;
		m_iMap = nullptr;
		m_jMap = nullptr;
		m_data = nullptr;
		m_referenceCount = nullptr;
	}

	template<typename T>
	inline constexpr size_t Matrix<T>::iSize() const
	{
		return m_iSize;
	}

	template<typename T>
	inline constexpr size_t Matrix<T>::jSize() const
	{
		return m_jSize;
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
	inline constexpr size_t* Matrix<T>::iMap()
	{
		return m_iMap;
	}

	template<typename T>
	inline constexpr const size_t* Matrix<T>::iMap() const
	{
		return m_iMap;
	}

	template<typename T>
	inline constexpr size_t* Matrix<T>::jMap()
	{
		return m_jMap;
	}

	template<typename T>
	inline constexpr const size_t* Matrix<T>::jMap() const
	{
		return m_jMap;
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
	inline constexpr size_t Matrix<T>::referenceCount() const
	{
		return *m_referenceCount;
	}

	template<typename T>
	inline constexpr typename Matrix<T>::RowCol Matrix<T>::row(size_t i)
	{
		return RowCol(m_jStride, m_jMap, m_data + m_iMap[i] * m_iStride);
	}

	template<typename T>
	inline constexpr typename const Matrix<T>::RowCol Matrix<T>::row(size_t i) const
	{
		return RowCol(m_jStride, m_jMap, m_data + m_iMap[i] * m_iStride);
	}

	template<typename T>
	inline constexpr typename Matrix<T>::RowCol Matrix<T>::col(size_t j)
	{
		return RowCol(m_iStride, m_iMap, m_data + m_jMap[j] * m_jStride);
	}

	template<typename T>
	inline constexpr typename const Matrix<T>::RowCol Matrix<T>::col(size_t j) const
	{
		return RowCol(m_iStride, m_iMap, m_data + m_jMap[j] * m_jStride);
	}

	template<typename T>
	inline constexpr T& Matrix<T>::get(size_t i, size_t j)
	{
		return m_data[m_iMap[i] * m_iStride + m_jMap[j] * m_jStride];
	}

	template<typename T>
	inline constexpr const T& Matrix<T>::get(size_t i, size_t j) const
	{
		return m_data[m_iMap[i] * m_iStride + m_jMap[j] * m_jStride];
	}

	template<typename T>
	inline constexpr typename Matrix<T>::RowCol Matrix<T>::operator[](size_t i)
	{
		return row(i);
	}

	template<typename T>
	inline constexpr const typename Matrix<T>::RowCol Matrix<T>::operator[](size_t i) const
	{
		return row(i);
	}

	template<typename T>
	inline constexpr typename Matrix<T>::RowCol Matrix<T>::operator()(size_t j)
	{
		return col(j);
	}

	template<typename T>
	inline constexpr const typename Matrix<T>::RowCol Matrix<T>::operator()(size_t j) const
	{
		return col(j);
	}

	template<typename T>
	inline constexpr T& Matrix<T>::operator()(size_t i, size_t j)
	{
		return get(i, j);
	}

	template<typename T>
	inline constexpr const T& Matrix<T>::operator()(size_t i, size_t j) const
	{
		return get(i, j);
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::submatrix(size_t i, size_t j, size_t iSize, size_t jSize)
	{
		m_iSize = iSize;
		m_jSize = jSize;
		m_size = iSize * jSize;
		size_t* temp = m_iMap;
		m_iMap = subMap(m_iMap, i, iSize);
		delete[] temp;
		temp = m_jMap;
		m_jMap = subMap(m_iMap, j, jSize);
		delete[] temp;
		return *this;
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::submatrix(const size_t* iRetain, const size_t* jRetain, size_t iSize, size_t jSize)
	{
		m_iSize = iSize;
		m_jSize = jSize;
		m_size = iSize * jSize;
		size_t* temp = m_iMap;
		m_iMap = subMap(m_iMap, iRetain, iSize);
		delete[] temp;
		temp = m_jMap;
		m_jMap = subMap(m_iMap, jRetain, jSize);
		delete[] temp;
		return *this;
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::transpose()
	{
		std::swap(m_iSize, m_jSize);
		std::swap(m_iStride, m_jStride);
		std::swap(m_iMap, m_jMap);
		return *this;
	}

	template<typename T>
	inline constexpr Matrix<T> Matrix<T>::copySubmatrix(size_t i, size_t j, size_t iSize, size_t jSize) const
	{
		Matrix mat(iSize, jSize);
		for (size_t k = 0; k < iSize; k++)
		{
			RowCol matRow = mat.row(k);
			RowCol thisRow = row(k + i);
			for (size_t l = 0; l < jSize; l++)
			{
				matRow[l] = thisRow[l + j];
			}
		}
		return mat;
	}

	template<typename T>
	inline constexpr Matrix<T> Matrix<T>::copySubmatrix(const size_t* iRetain, const size_t* jRetain, size_t iSize, size_t jSize) const
	{
		Matrix mat(iSize, jSize);
		for (size_t k = 0; k < iSize; k++)
		{
			RowCol matRow = mat.row(k);
			RowCol thisRow = row(iRetain[k]);
			for (size_t l = 0; l < jSize; l++)
			{
				matRow[l] = thisRow[jRetain[l]];
			}
		}
		return mat;
	}

	template<typename T>
	inline constexpr Matrix<T> Matrix<T>::copyTranspose() const
	{
		Matrix mat(m_jSize, m_iSize);
		for (size_t i = 0; i < m_iSize; i++)
		{
			RowCol matCol = mat.col(i);
			RowCol thisRow = row(i);
			for (size_t j = 0; j < m_jSize; j++)
			{
				matCol[j] = thisRow[j];
			}
		}
		return mat;
	}

	template<typename T>
	inline constexpr Matrix<T> Matrix<T>::shareSubmatrix(size_t i, size_t j, size_t iSize, size_t jSize)
	{
		addRef();
		return Matrix(iSize, jSize, m_iStride, m_jStride, m_iSize * m_jSize, subMap(m_iMap, i, iSize), subMap(m_jMap, j, jSize), m_data, m_referenceCount);
	}

	template<typename T>
	inline constexpr const Matrix<T> Matrix<T>::shareSubmatrix(size_t i, size_t j, size_t iSize, size_t jSize) const
	{
		addRef();
		return Matrix(iSize, jSize, m_iStride, m_jStride, m_iSize * m_jSize, subMap(m_iMap, i, iSize), subMap(m_jMap, j, jSize), m_data, m_referenceCount);
	}

	template<typename T>
	inline constexpr Matrix<T> Matrix<T>::shareSubmatrix(const size_t* iRetain, const size_t* jRetain, size_t iSize, size_t jSize)
	{
		addRef();
		return Matrix(iSize, jSize, m_iStride, m_jStride, m_iSize * m_jSize, subMap(m_iMap, iRetain, iSize), subMap(m_jMap, jRetain, jSize), m_data, m_referenceCount);
	}

	template<typename T>
	inline constexpr const Matrix<T> Matrix<T>::shareSubmatrix(const size_t* iRetain, const size_t* jRetain, size_t iSize, size_t jSize) const
	{
		addRef();
		return Matrix(iSize, jSize, m_iStride, m_jStride, m_iSize * m_jSize, subMap(m_iMap, iRetain, iSize), subMap(m_jMap, jRetain, jSize), m_data, m_referenceCount);
	}

	template<typename T>
	inline constexpr Matrix<T> Matrix<T>::shareTranspose()
	{
		addRef();
		return Matrix(m_jSize, m_iSize, m_jStride, m_iStride, m_iSize * m_jSize, subMap(m_jMap, (size_t)0, m_jSize), subMap(m_iMap, (size_t)0, m_iSize), m_data, m_referenceCount);
	}

	template<typename T>
	inline constexpr const Matrix<T> Matrix<T>::shareTranspose() const
	{
		addRef();
		return Matrix(m_jSize, m_iSize, m_jStride, m_iStride, m_iSize * m_jSize, subMap(m_jMap, (size_t)0, m_jSize), subMap(m_iMap, (size_t)0, m_iSize), m_data, m_referenceCount);
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::swapRows(size_t i1, size_t i2)
	{
		std::swap(m_iMap[i1], m_iMap[i2]);
		return *this;
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::swapCols(size_t j1, size_t j2)
	{
		std::swap(m_jMap[j1], m_jMap[j2]);
		return *this;
	}

	template<typename R, typename T0, typename... Ts>
	inline constexpr Matrix<R> ElementWiseHelper::init(const T0& t0, const Ts&... ts)
	{
		return init<R>(ts...);
	}

	template<typename R, typename T0, typename... Ts>
	inline constexpr Matrix<R> ElementWiseHelper::init(const Matrix<T0>& t0, const Ts&... ts)
	{
		return Matrix<R>(t0.iSize(), t0.jSize());
	}

	template<typename T>
	inline constexpr const T& ElementWiseHelper::index(const T& val, size_t i, size_t j)
	{
		return val;
	}

	template<typename T>
	inline constexpr const T& ElementWiseHelper::index(const Matrix<T>& mat, size_t i, size_t j)
	{
		return mat(i, j);
	}

	template<typename Op, typename ...Ts>
	inline constexpr Matrix<ElementWiseRes<Op, Ts...>> elementWise(const Op& op, const Ts & ...params)
	{
		Matrix<ElementWiseRes<Op, Ts...>> mat = ElementWiseHelper::init<ElementWiseRes<Op, Ts...>>(params...);
		for (size_t i = 0; i < mat.iSize(); i++)
		{
			for (size_t j = 0; j < mat.jSize(); j++)
			{
				mat(i, j) = op(ElementWiseHelper::index(params, i, j)...);
			}
		}
		return mat;
	}

	template<typename Add, typename Mul, typename T, typename U>
	constexpr Matrix<MatMulRes<Mul, T, U>> matMul(const Add& add, const Mul& mul, const Matrix<T>& lhs, const Matrix<U>& rhs)
	{
		Matrix<MatMulRes<Mul, T, U>> res(lhs.iSize(), rhs.jSize(), 0);
		for (size_t i = 0; i < lhs.iSize(); i++)
		{
			typename Matrix<T>::RowCol lhsRow = lhs[i];
			typename Matrix<MatMulRes<Mul, T, U>>::RowCol resRow = res[i];
			for (size_t j = 0; j < rhs.jSize(); j++)
			{
				MatMulRes<Mul, T, U>& resVal = resRow[j];
				typename Matrix<T>::RowCol rhsCol = rhs(j);
				for (size_t k = 0; k < lhs.jSize(); k++)
				{
					resVal = add(resVal, mul(lhsRow[k], rhsCol[k]));
				}
			}
		}
		return res;
	}

	template<typename T>
	inline constexpr Matrix<T> operator-(const Matrix<T>& mat)
	{
		return elementWise(std::negate(), mat);
	}

	template<typename T>
	inline constexpr Matrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::plus(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator+(const Matrix<T>& lhs, const T& rhs)
	{
		return elementWise(std::plus(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator+(const T& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::plus(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator-(const Matrix<T>& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::minus(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator-(const Matrix<T>& lhs, const T& rhs)
	{
		return elementWise(std::minus(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator-(const T& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::minus(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::multiplies(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator*(const Matrix<T>& lhs, const T& rhs)
	{
		return elementWise(std::multiplies(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator*(const T& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::multiplies(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator/(const Matrix<T>& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::divides(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator/(const Matrix<T>& lhs, const T& rhs)
	{
		return elementWise(std::divides(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator/(const T& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::divides(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator%(const Matrix<T>& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::modulus(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator%(const Matrix<T>& lhs, const T& rhs)
	{
		return elementWise(std::modulus(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator%(const T& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::modulus(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator&(const Matrix<T>& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::bit_and(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator&(const Matrix<T>& lhs, const T& rhs)
	{
		return elementWise(std::bit_and(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator&(const T& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::bit_and(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator|(const Matrix<T>& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::bit_or(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator|(const Matrix<T>& lhs, const T& rhs)
	{
		return elementWise(std::bit_or(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator|(const T& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::bit_or(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator^(const Matrix<T>& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::bit_xor(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator^(const Matrix<T>& lhs, const T& rhs)
	{
		return elementWise(std::bit_xor(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator^(const T& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::bit_xor(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator~(const Matrix<T>& mat)
	{
		return elementWise(std::bit_not(), mat);
	}

	template<typename T>
	inline constexpr Matrix<bool> operator!(const Matrix<T>& mat)
	{
		return elementWise(std::logical_not(), mat);
	}

	template<typename T>
	inline constexpr Matrix<T> operator<<(const Matrix<T>& lhs, const Matrix<T>& rhs)
	{
		return elementWise(func::left_shift(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator<<(const Matrix<T>& lhs, const T& rhs)
	{
		return elementWise(func::left_shift(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator<<(const T& lhs, const Matrix<T>& rhs)
	{
		return elementWise(func::left_shift(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator>>(const Matrix<T>& lhs, const Matrix<T>& rhs)
	{
		return elementWise(func::right_shift(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator>>(const Matrix<T>& lhs, const T& rhs)
	{
		return elementWise(func::right_shift(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator>>(const T& lhs, const Matrix<T>& rhs)
	{
		return elementWise(func::right_shift(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> min(const Matrix<T>& lhs, const Matrix<T>& rhs)
	{
		return elementWise(func::min(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> min(const Matrix<T>& lhs, const T& rhs)
	{
		return elementWise(func::min(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> min(const T& lhs, const Matrix<T>& rhs)
	{
		return elementWise(func::min(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> max(const Matrix<T>& lhs, const Matrix<T>& rhs)
	{
		return elementWise(func::max(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> max(const Matrix<T>& lhs, const T& rhs)
	{
		return elementWise(func::max(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> max(const T& lhs, const Matrix<T>& rhs)
	{
		return elementWise(func::max(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<bool> operator==(const Matrix<T>& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::equal_to(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<bool> operator==(const Matrix<T>& lhs, const T& rhs)
	{
		return elementWise(std::equal_to(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<bool> operator==(const T& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::equal_to(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<bool> operator!=(const Matrix<T>& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::not_equal_to(), lhs, rhs);
	}
	template<typename T>
	inline constexpr Matrix<bool> operator!=(const Matrix<T>& lhs, const T& rhs)
	{
		return elementWise(std::not_equal_to(), lhs, rhs);
	}
	template<typename T>
	inline constexpr Matrix<bool> operator!=(const T& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::not_equal_to(), lhs, rhs);
	}
	template<typename T>
	inline constexpr Matrix<bool> operator>(const Matrix<T>& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::greater(), lhs, rhs);
	}
	template<typename T>
	inline constexpr Matrix<bool> operator>(const Matrix<T>& lhs, const T& rhs)
	{
		return elementWise(std::greater(), lhs, rhs);
	}
	template<typename T>
	inline constexpr Matrix<bool> operator>(const T& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::greater(), lhs, rhs);
	}
	template<typename T>
	inline constexpr Matrix<bool> operator<(const Matrix<T>& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::less(), lhs, rhs);
	}
	template<typename T>
	inline constexpr Matrix<bool> operator<(const Matrix<T>& lhs, const T& rhs)
	{
		return elementWise(std::less(), lhs, rhs);
	}
	template<typename T>
	inline constexpr Matrix<bool> operator<(const T& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::less(), lhs, rhs);
	}
	template<typename T>
	inline constexpr Matrix<bool> operator>=(const Matrix<T>& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::greater_equal(), lhs, rhs);
	}
	template<typename T>
	inline constexpr Matrix<bool> operator>=(const Matrix<T>& lhs, const T& rhs)
	{
		return elementWise(std::greater_equal(), lhs, rhs);
	}
	template<typename T>
	inline constexpr Matrix<bool> operator>=(const T& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::greater_equal(), lhs, rhs);
	}
	template<typename T>
	inline constexpr Matrix<bool> operator<=(const Matrix<T>& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::less_equal(), lhs, rhs);
	}
	template<typename T>
	inline constexpr Matrix<bool> operator<=(const Matrix<T>& lhs, const T& rhs)
	{
		return elementWise(std::less_equal(), lhs, rhs);
	}
	template<typename T>
	inline constexpr Matrix<bool> operator<=(const T& lhs, const Matrix<T>& rhs)
	{
		return elementWise(std::less_equal(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<T> operator&&(const Matrix<T>& lhs, const Matrix<T>& rhs)
	{
		return matMul(std::plus(), std::multiplies(), lhs, rhs);
	}

	template<typename T>
	inline constexpr Matrix<bool> operator&&(const Matrix<bool>& lhs, const Matrix<bool>& rhs)
	{
		return matMul(std::bit_or(), std::bit_and(), lhs, rhs);
	}

	template<typename T>
	template<typename Op, typename... Ts>
	inline constexpr Matrix<T>& Matrix<T>::assignElementWise(const Op& op, const Ts&... params)
	{
		for (size_t i = 0; i < m_iSize; i++)
		{
			for (size_t j = 0; j < m_jSize; j++)
			{
				get(i, j) = op(get(i, j), ElementWiseHelper::index(params, i, j)...);
			}
		}
		return *this;
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::operator+=(const Matrix& mat)
	{
		return assignElementWise(std::plus(), mat);
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::operator+=(const T& val)
	{
		return assignElementWise(std::plus(), val);
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::operator-=(const Matrix& mat)
	{
		return assignElementWise(std::minus(), mat);
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::operator-=(const T& val)
	{
		return assignElementWise(std::minus(), val);
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::operator*=(const Matrix& mat)
	{
		return assignElementWise(std::multiplies(), mat);
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::operator*=(const T& val)
	{
		return assignElementWise(std::multiplies(), val);
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::operator/=(const Matrix& mat)
	{
		return assignElementWise(std::divides(), mat);
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::operator/=(const T& val)
	{
		return assignElementWise(std::divides(), val);
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::operator%=(const Matrix& mat)
	{
		return assignElementWise(std::modulus(), mat);
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::operator%=(const T& val)
	{
		return assignElementWise(std::modulus(), val);
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::operator&=(const Matrix& mat)
	{
		return assignElementWise(std::bit_and(), mat);
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::operator&=(const T& val)
	{
		return assignElementWise(std::bit_and(), val);
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::operator|=(const Matrix& mat)
	{
		return assignElementWise(std::bit_or(), mat);
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::operator|=(const T& val)
	{
		return assignElementWise(std::bit_or(), val);
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::operator^=(const Matrix& mat)
	{
		return assignElementWise(std::bit_xor(), mat);
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::operator^=(const T& val)
	{
		return assignElementWise(std::bit_xor(), val);
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::operator<<=(const Matrix& mat)
	{
		return assignElementWise(func::left_shift(), mat);
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::operator<<=(const T& val)
	{
		return assignElementWise(func::left_shift(), val);
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::operator>>=(const Matrix& mat)
	{
		return assignElementWise(func::right_shift(), mat);
	}

	template<typename T>
	inline constexpr Matrix<T>& Matrix<T>::operator>>=(const T& val)
	{
		return assignElementWise(func::right_shift(), val);
	}

	template<typename T>
	inline std::istream& operator>>(std::istream& stream, Matrix<T>& mat)
	{
		std::streamsize width = stream.width();
		std::streamsize precision = stream.precision();
		for (size_t i = 0; i < mat.iSize(); i++)
		{
			typename Matrix<T>::RowCol matRow = mat[i];
			for (size_t j = 0; j < mat.jSize(); j++)
			{
				stream >> std::setw(width) >> std::setprecision(precision) >> matRow[j];
			}
		}
		return stream;
	}

	template<typename T>
	inline std::ostream& operator<<(std::ostream& stream, const Matrix<T>& mat)
	{
		std::streamsize width = stream.width();
		std::streamsize precision = stream.precision();
		for (size_t i = 0; i < mat.iSize(); i++)
		{
			if (i > 0)
			{
				stream << std::endl;
			}
			typename Matrix<T>::RowCol matRow = mat[i];
			for (size_t j = 0; j < mat.jSize(); j++)
			{
				if (j > 0)
				{
					stream << ' ';
				}
				stream << std::setw(width) << std::setprecision(precision) << matRow[j];
			}
		}
		return stream;
	}
}
