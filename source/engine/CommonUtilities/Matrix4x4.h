#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include <limits>
#include <array>
#include <cmath>

namespace CommonUtilities
{
	template<class T>
	class Matrix4x4
	{
	public:
		Matrix4x4<T>();
		Matrix4x4<T>(const Matrix4x4<T>& aMatrix);
		Matrix4x4<T>(const std::initializer_list<T>& aList);
		void SetRotation(const Matrix4x4<T>& aMatrix);
		void SetX(const Vector3<T>& aVector);
		void SetY(const Vector3<T>& aVector);
		void SetZ(const Vector3<T>& aVector);
		Vector3<T> GetX() const;
		Vector3<T> GetY() const;
		Vector3<T> GetZ() const;
		Vector3<T> GetTranslation() const;
		void SetTranslation(const Vector3<T>& aVector);

		T& operator() (const int aRow, const int aColumn)
		{
			return myMatrix[aRow - 1][aColumn - 1];
		}
		const T& operator() (const int aRow, const int aColumn) const
		{
			return myMatrix[aRow - 1][aColumn - 1];
		}
		void operator=(const Matrix4x4<T>& aMatrix)
		{
			for (int row{ 1 }; row < 5; row++)
			{
				for (int col{ 1 }; col < 5; col++)
				{
					(*this)(row, col) = aMatrix(row, col);
				}
			}
		}
		Matrix4x4<T>& operator+=(const Matrix4x4<T>& aMatrix)
		{
			for (int row{ 1 }; row < 5; row++)
			{
				for (int col{ 1 }; col < 5; col++)
				{
					(*this)(row, col) += aMatrix(row, col);
				}
			}
			return *this;
		}
		Matrix4x4<T>& operator-=(const Matrix4x4<T>& aMatrix)
		{
			for (int row{ 1 }; row < 5; row++)
			{
				for (int col{ 1 }; col < 5; col++)
				{
					(*this)(row, col) -= aMatrix(row, col);
				}
			}
			return *this;
		}
		Matrix4x4<T>& operator*=(const Matrix4x4<T>& aMatrix)
		{
			Matrix4x4<T> m(*this);
			*this = m * aMatrix;
			return *this;
		}

		static Matrix4x4<T> CreateRotationAroundX(T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundY(T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundZ(T aAngleInRadians);

		static Matrix4x4<T> Transpose(const Matrix4x4<T>& aMatrixToTranspose);

		static Matrix4x4<T> GetFastInverse(const Matrix4x4<T>& aTransform);
		static Matrix4x4<T> GetInverse(const Matrix4x4<T>& aTransform);
		static Matrix4x4<T> GetAdjoint(const Matrix4x4<T>& aMatrix);
		static T GetIJthMinor(const Matrix4x4<T>& aMatrix, const int& aI, const int& aJ);
		static T GetDeterminant(const Matrix4x4<T>& aMatrix);

	private:
		std::array<std::array<T, 4>, 4> myMatrix{};
	};

	template<class T> Matrix4x4<T> operator+(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		Matrix4x4<T> m;
		for (int row{ 1 }; row < 5; row++)
		{
			for (int col{ 1 }; col < 5; col++)
			{
				m(row, col) = aMatrix0(row, col) + aMatrix1(row, col);
			}
		}
		return m;
	}

	template<class T> Matrix4x4<T> operator-(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		Matrix4x4<T> m;
		for (int row{ 1 }; row < 5; row++)
		{
			for (int col{ 1 }; col < 5; col++)
			{
				m(row, col) = aMatrix0(row, col) - aMatrix1(row, col);
			}
		}
		return m;
	}

	template<class T> Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		Matrix4x4<T> m;
		for (int row{ 1 }; row < 5; row++)
		{
			for (int col{ 1 }; col < 5; col++)
			{
				m(row, col) = aMatrix0(row, 1) * aMatrix1(1, col) +
					aMatrix0(row, 2) * aMatrix1(2, col) +
					aMatrix0(row, 3) * aMatrix1(3, col) +
					aMatrix0(row, 4) * aMatrix1(4, col);
			}
		}
		return m;
	}

	template<class T> Vector4<T> operator*(const Vector4<T>& aVector, const Matrix4x4<T>& aMatrix)
	{
		return	aVector.x * Vector4<T>({ aMatrix(1, 1), aMatrix(1, 2), aMatrix(1, 3), aMatrix(1, 4) }) +
				aVector.y * Vector4<T>({ aMatrix(2, 1), aMatrix(2, 2), aMatrix(2, 3), aMatrix(2, 4) }) +
				aVector.z * Vector4<T>({ aMatrix(3, 1), aMatrix(3, 2), aMatrix(3, 3), aMatrix(3, 4) }) +
				aVector.w * Vector4<T>({ aMatrix(4, 1), aMatrix(4, 2), aMatrix(4, 3), aMatrix(4, 4) });
	}

	template<class T> Matrix4x4<T> operator*(const T& aScalar, const Matrix4x4<T>& aMatrix)
	{
		Matrix4x4<T> m{};
		for (int row{ 1 }; row < 5; row++)
		{
			for (int col{ 1 }; col < 5; col++)
			{
				m(row, col) = aMatrix(row, col) * aScalar;
			}
		}
		return m;
	}

	template<class T> bool operator==(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		for (int row{ 1 }; row < 5; row++)
		{
			for (int col{ 1 }; col < 5; col++)
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
	inline Matrix4x4<T>::Matrix4x4()
	{
		myMatrix[0][0] = (T)1;
		myMatrix[1][1] = (T)1;
		myMatrix[2][2] = (T)1;
		myMatrix[3][3] = (T)1;
	}

	template<class T>
	inline Matrix4x4<T>::Matrix4x4(const Matrix4x4<T>& aMatrix)
	{
		for (int row{ 1 }; row < 5; row++)
		{
			for (int col{ 1 }; col < 5; col++)
			{
				(*this)(row, col) = aMatrix(row, col);
			}
		}
	}

	template<class T>
	inline Matrix4x4<T>::Matrix4x4(const std::initializer_list<T>& aList)
	{
		int listSize{ (int)aList.size() };
		int index{ 0 };
		assert(listSize <= 16);
		for (int row{ 0 }; row < 4; row++)
		{
			for (int col{ 0 }; col < 4; col++)
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
	inline void Matrix4x4<T>::SetRotation(const Matrix4x4<T>& aMatrix)
	{
		myMatrix[0][0] = aMatrix(1,1);
		myMatrix[0][1] = aMatrix(1,2);
		myMatrix[0][2] = aMatrix(1,3);
		myMatrix[1][0] = aMatrix(2,1);
		myMatrix[1][1] = aMatrix(2,2);
		myMatrix[1][2] = aMatrix(2,3);
		myMatrix[2][0] = aMatrix(3,1);
		myMatrix[2][1] = aMatrix(3,2);
		myMatrix[2][2] = aMatrix(3,3);
	}

	template<class T>
	inline void Matrix4x4<T>::SetX(const Vector3<T>& aVector)
	{
		myMatrix[0][0] = aVector.x;
		myMatrix[0][1] = aVector.y;
		myMatrix[0][2] = aVector.z;
	}

	template<class T>
	inline void Matrix4x4<T>::SetY(const Vector3<T>& aVector)
	{
		myMatrix[1][0] = aVector.x;
		myMatrix[1][1] = aVector.y;
		myMatrix[1][2] = aVector.z;
	}

	template<class T>
	inline void Matrix4x4<T>::SetZ(const Vector3<T>& aVector)
	{
		myMatrix[2][0] = aVector.x;
		myMatrix[2][1] = aVector.y;
		myMatrix[2][2] = aVector.z;
	}

	template<class T>
	inline Vector3<T> Matrix4x4<T>::GetX() const
	{
		return { myMatrix[0][0], myMatrix[0][1], myMatrix[0][2] };
	}

	template<class T>
	inline Vector3<T> Matrix4x4<T>::GetY() const
	{
		return { myMatrix[1][0], myMatrix[1][1], myMatrix[1][2] };
	}

	template<class T>
	inline Vector3<T> Matrix4x4<T>::GetZ() const
	{
		return { myMatrix[2][0], myMatrix[2][1], myMatrix[2][2] };
	}


	template<class T>
	inline Vector3<T> Matrix4x4<T>::GetTranslation() const
	{
		return { myMatrix[3][0], myMatrix[3][1], myMatrix[3][2] };
	}

	template<class T>
	inline void Matrix4x4<T>::SetTranslation(const Vector3<T>& aVector)
	{
		myMatrix[3][0] = aVector.x;
		myMatrix[3][1] = aVector.y;
		myMatrix[3][2] = aVector.z;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(T aAngleInRadians)
	{
		return { (T)1,T{},T{}, T{},
				T{}, cos(aAngleInRadians), sin(aAngleInRadians), T{},
				T{}, -sin(aAngleInRadians),cos(aAngleInRadians), T{},
				T{}, T{}, T{}, (T)1 };
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(T aAngleInRadians)
	{
		return { cos(aAngleInRadians), T{}, -sin(aAngleInRadians), T{},
				T{}, (T)1, T{}, T{},
				sin(aAngleInRadians), T{}, cos(aAngleInRadians), T{},
				T{}, T{}, T{}, (T)1 };
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(T aAngleInRadians)
	{
		return { cos(aAngleInRadians), sin(aAngleInRadians), T{}, T{},
				-sin(aAngleInRadians), cos(aAngleInRadians), T{}, T{},
				T{}, T{}, (T)1, T{},
				T{}, T{}, T{}, (T)1 };
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::Transpose(const Matrix4x4<T>& aMatrixToTranspose)
	{
		return { aMatrixToTranspose(1, 1), aMatrixToTranspose(2, 1), aMatrixToTranspose(3, 1), aMatrixToTranspose(4, 1),
				aMatrixToTranspose(1, 2), aMatrixToTranspose(2, 2),	aMatrixToTranspose(3, 2), aMatrixToTranspose(4, 2),
				aMatrixToTranspose(1, 3), aMatrixToTranspose(2, 3), aMatrixToTranspose(3, 3), aMatrixToTranspose(4, 3),
				aMatrixToTranspose(1, 4), aMatrixToTranspose(2, 4), aMatrixToTranspose(3, 4), aMatrixToTranspose(4, 4) };
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::GetFastInverse(const Matrix4x4<T>& aTransform)
	{
		Matrix4x4<T> m {aTransform(1, 1), aTransform(2, 1), aTransform(3, 1), aTransform(1, 4),
						aTransform(1, 2), aTransform(2, 2), aTransform(3, 2), aTransform(2, 4),
						aTransform(1, 3), aTransform(2, 3), aTransform(3, 3), aTransform(3, 4),
						aTransform(4, 1), aTransform(4, 2), aTransform(4, 3), aTransform(4, 4) };
		
		T tX = -aTransform(4, 1);
		T tY = -aTransform(4, 2);
		T tZ = -aTransform(4, 3);

		m(4, 1) = tX * m(1, 1) + tY * m(2, 1) + tZ * m(3, 1);
		m(4, 2) = tX * m(1, 2) + tY * m(2, 2) + tZ * m(3, 2);
		m(4, 3) = tX * m(1, 3) + tY * m(2, 3) + tZ * m(3, 3);

		return m;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::GetInverse(const Matrix4x4<T>& aTransform)
	{
		return { ((T)1 / GetDeterminant(aTransform)) * GetAdjoint(aTransform) };
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::GetAdjoint(const Matrix4x4<T>& aMatrix)
	{
		Matrix4x4<T> m{};
		for (int i{ 1 }; i < 5; i++)
		{
			for (int j{ 1 }; j < 5; j++)
			{
				m(i, j) = GetIJthMinor(aMatrix, i, j);
				if ((i + j) % 2 != T{})
				{
					m(i, j) = -m(i, j);
				}
			}
		}
		return Transpose(m);
	}

	template<class T>
	inline T Matrix4x4<T>::GetIJthMinor(const Matrix4x4<T>& aMatrix, const int& aI, const int& aJ)
	{
		int i1{ 1 };
		if (i1 == aI) ++i1;
		int i2{ i1 + 1 };
		if (i2 == aI) ++i2;
		int i3{ i2 + 1 };
		if (i3 == aI) ++i3;

		int j1{ 1 };
		if (j1 == aJ) ++j1;
		int j2{ j1 + 1 };
		if (j2 == aJ) ++j2;
		int j3{ j2 + 1 };
		if (j3 == aJ) ++j3;

		return	aMatrix(i1, j1) * aMatrix(i2, j2) * aMatrix(i3, j3) +
				aMatrix(i1, j2) * aMatrix(i2, j3) * aMatrix(i3, j1) +
				aMatrix(i1, j3) * aMatrix(i2, j1) * aMatrix(i3, j2) -

				aMatrix(i1, j3) * aMatrix(i2, j2) * aMatrix(i3, j1) -
				aMatrix(i1, j2) * aMatrix(i2, j1) * aMatrix(i3, j3) -
				aMatrix(i1, j1) * aMatrix(i2, j3) * aMatrix(i3, j2);
	}


	template<class T>
	inline T Matrix4x4<T>::GetDeterminant(const Matrix4x4<T>& aMatrix)
	{
		return	aMatrix(1, 4) * aMatrix(2, 3) * aMatrix(3, 2) * aMatrix(4, 1) - aMatrix(1, 3) * aMatrix(2, 4) * aMatrix(3, 2) * aMatrix(4, 1) -
				aMatrix(1, 4) * aMatrix(2, 2) * aMatrix(3, 3) * aMatrix(4, 1) + aMatrix(1, 2) * aMatrix(2, 4) * aMatrix(3, 3) * aMatrix(4, 1) +
				aMatrix(1, 3) * aMatrix(2, 2) * aMatrix(3, 4) * aMatrix(4, 1) - aMatrix(1, 2) * aMatrix(2, 3) * aMatrix(3, 4) * aMatrix(4, 1) -
				aMatrix(1, 4) * aMatrix(2, 3) * aMatrix(3, 1) * aMatrix(4, 2) + aMatrix(1, 3) * aMatrix(2, 4) * aMatrix(3, 1) * aMatrix(4, 2) +
				aMatrix(1, 4) * aMatrix(2, 1) * aMatrix(3, 3) * aMatrix(4, 2) - aMatrix(1, 1) * aMatrix(2, 4) * aMatrix(3, 3) * aMatrix(4, 2) -
				aMatrix(1, 3) * aMatrix(2, 1) * aMatrix(3, 4) * aMatrix(4, 2) + aMatrix(1, 1) * aMatrix(2, 3) * aMatrix(3, 4) * aMatrix(4, 2) +
				aMatrix(1, 4) * aMatrix(2, 2) * aMatrix(3, 1) * aMatrix(4, 3) - aMatrix(1, 2) * aMatrix(2, 4) * aMatrix(3, 1) * aMatrix(4, 3) -
				aMatrix(1, 4) * aMatrix(2, 1) * aMatrix(3, 2) * aMatrix(4, 3) + aMatrix(1, 1) * aMatrix(2, 4) * aMatrix(3, 2) * aMatrix(4, 3) +
				aMatrix(1, 2) * aMatrix(2, 1) * aMatrix(3, 4) * aMatrix(4, 3) - aMatrix(1, 1) * aMatrix(2, 2) * aMatrix(3, 4) * aMatrix(4, 3) -
				aMatrix(1, 3) * aMatrix(2, 2) * aMatrix(3, 1) * aMatrix(4, 4) + aMatrix(1, 2) * aMatrix(2, 3) * aMatrix(3, 1) * aMatrix(4, 4) +
				aMatrix(1, 3) * aMatrix(2, 1) * aMatrix(3, 2) * aMatrix(4, 4) - aMatrix(1, 1) * aMatrix(2, 3) * aMatrix(3, 2) * aMatrix(4, 4) -
				aMatrix(1, 2) * aMatrix(2, 1) * aMatrix(3, 3) * aMatrix(4, 4) + aMatrix(1, 1) * aMatrix(2, 2) * aMatrix(3, 3) * aMatrix(4, 4);
	}
}