#pragma once
#include "Matrix4x4.h"
#include <cassert>

namespace CommonUtilities
{
	template <class T>
	class Matrix3x3
	{
	public:
		Matrix3x3<T>();
		Matrix3x3<T>(const Matrix3x3<T>& aMatrix);
		Matrix3x3<T>(const Matrix4x4<T>& aMatrix);
		Matrix3x3<T>(const std::initializer_list<T>& aList);

		T& operator() (const int aRow, const int aColumn)
		{
			return myMatrix[aRow - 1][aColumn - 1];
		}
		const T& operator() (const int aRow, const int aColumn) const
		{
			return myMatrix[aRow - 1][aColumn - 1];
		}
		void operator=(const Matrix3x3<T>& aMatrix)
		{
			for (int row{ 1 }; row < 4; row++)
			{
				for (int col{ 1 }; col < 4; col++)
				{
					(*this)(row, col) = aMatrix(row, col);
				}
			}
		}
		Matrix3x3<T>& operator+=(const Matrix3x3<T>& aMatrix)
		{
			for (int row{ 1 }; row < 4; row++)
			{
				for (int col{ 1 }; col < 4; col++)
				{
					(*this)(row, col) += aMatrix(row, col);
				}
			}
			return *this;
		}
		Matrix3x3<T>& operator-=(const Matrix3x3<T>& aMatrix)
		{
			for (int row{ 1 }; row < 4; row++)
			{
				for (int col{ 1 }; col < 4; col++)
				{
					(*this)(row, col) -= aMatrix(row, col);
				}
			}
			return *this;
		}
		Matrix3x3<T>& operator*=(const Matrix3x3<T>& aMatrix)
		{
			Matrix3x3<T> m(*this);
			*this = m * aMatrix;
			return *this;
		}

		static Matrix3x3<T> CreateRotationAroundX(T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundY(T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundZ(T aAngleInRadians);

		static Matrix3x3<T> Transpose(const Matrix3x3<T>& aMatrixToTranspose);
		static T GetDeterminant(const Matrix3x3<T>& aMatrix);

	private:
		std::array< std::array< T, 3>, 3> myMatrix{};
	};

	template <class T> Matrix3x3<T> operator+(Matrix3x3<T>& aMatrix0, Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> m;
		for (int row{ 1 }; row < 4; row++)
		{
			for (int col{ 1 }; col < 4; col++)
			{
				m(row, col) = aMatrix0(row, col) + aMatrix1(row, col);
			}
		}
		return m;
	}

	template <class T> Matrix3x3<T> operator-(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> m;
		for (int row{ 1 }; row < 4; row++)
		{
			for (int col{ 1 }; col < 4; col++)
			{
				m(row, col) = aMatrix0(row, col) - aMatrix1(row, col);
			}
		}
		return m;
	}

	template <class T> Matrix3x3<T> operator*(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> m;
		for (int row{ 1 }; row < 4; row++)
		{
			for (int col{ 1 }; col < 4; col++)
			{
				m(row, col) = aMatrix0(row, 1) * aMatrix1(1, col) +
					aMatrix0(row, 2) * aMatrix1(2, col) +
					aMatrix0(row, 3) * aMatrix1(3, col);
			}
		}
		return m;
	}

	template <class T> Vector3<T> operator*(const Vector3<T>& aVector, const Matrix3x3<T>& aMatrix)
	{
		return  aVector.x * Vector3<T>({ aMatrix(1, 1), aMatrix(1, 2), aMatrix(1, 3) }) +
			aVector.y * Vector3<T>({ aMatrix(2, 1), aMatrix(2, 2), aMatrix(2, 3) }) +
			aVector.z * Vector3<T>({ aMatrix(3, 1), aMatrix(3, 2), aMatrix(3, 3) });
	}

	template <class T> bool operator==(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		for (int row{ 1 }; row < 4; row++)
		{
			for (int col{ 1 }; col < 4; col++)
			{
				if (fabs(aMatrix0(row, col) - aMatrix1(row, col)) > std::numeric_limits<T>::epsilon())
				{
					return false;
				}
			}
		}
		return true;
	}

	template<class T>
	inline Matrix3x3<T>::Matrix3x3()
	{
		myMatrix[0][0] = (T)1;
		myMatrix[1][1] = (T)1;
		myMatrix[2][2] = (T)1;
	}

	template<class T>
	inline Matrix3x3<T>::Matrix3x3(const Matrix3x3<T>& aMatrix)
	{
		for (int row{ 1 }; row < 4; row++)
		{
			for (int col{ 1 }; col < 4; col++)
			{
				(*this)(row, col) = aMatrix(row, col);
			}
		}
	}

	template<class T>
	inline Matrix3x3<T>::Matrix3x3(const std::initializer_list<T>& aList)
	{
		int listSize{ (int)aList.size() };
		int index{ 0 };
		assert(listSize <= 9);
		for (int row{ 0 }; row < 3; row++)
		{
			for (int col{ 0 }; col < 3; col++)
			{
				if (index >= listSize)
				{
					return;
				}
				myMatrix[row][col] = *(aList.begin() + (index));
				index++;
			}
		}
	}

	template<class T>
	inline Matrix3x3<T>::Matrix3x3(const Matrix4x4<T>& aMatrix)
	{
		for (int row{ 1 }; row < 4; row++)
		{
			for (int col{ 1 }; col < 4; col++)
			{
				(*this)(row, col) = aMatrix(row, col);
			}
		}
	}

	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundX(T aAngleInRadians)
	{
		return { (T)1,	T{},					T{},
				T{},	cos(aAngleInRadians),	sin(aAngleInRadians),
				T{},	-sin(aAngleInRadians),	cos(aAngleInRadians) };
	}

	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundY(T aAngleInRadians)
	{

		return { cos(aAngleInRadians),	T{},	-sin(aAngleInRadians),
				T{},					(T)1,	T{},
				sin(aAngleInRadians),	T{},	cos(aAngleInRadians) };
	}

	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundZ(T aAngleInRadians)
	{
		return { cos(aAngleInRadians),	sin(aAngleInRadians),	T{},
				-sin(aAngleInRadians),	cos(aAngleInRadians),	T{},
				T{},					T{},					(T)1 };
	}

	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::Transpose(const Matrix3x3<T>& aMatrixToTranspose)
	{
		return { aMatrixToTranspose(1, 1), aMatrixToTranspose(2, 1),	aMatrixToTranspose(3, 1),
				aMatrixToTranspose(1, 2), aMatrixToTranspose(2, 2),	aMatrixToTranspose(3, 2),
				aMatrixToTranspose(1, 3), aMatrixToTranspose(2, 3), aMatrixToTranspose(3, 3) };
	}

	template<class T>
	inline T Matrix3x3<T>::GetDeterminant(const Matrix3x3<T>& aMatrix)
	{
		return	aMatrix(1, 1) * aMatrix(2, 2) * aMatrix(3, 3) +
				aMatrix(1, 2) * aMatrix(2, 3) * aMatrix(3, 1) +
				aMatrix(1, 3) * aMatrix(2, 1) * aMatrix(3, 2) -

				aMatrix(1, 3) * aMatrix(2, 2) * aMatrix(3, 1) -
				aMatrix(1, 2) * aMatrix(2, 1) * aMatrix(3, 3) -
				aMatrix(1, 1) * aMatrix(2, 3) * aMatrix(3, 2);
	}
}

