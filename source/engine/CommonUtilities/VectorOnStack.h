#pragma once
#include <cstring>
#include <cassert>

namespace CommonUtilities
{
	template <typename Type, int size, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	class VectorOnStack
	{
	public:
		VectorOnStack();
		VectorOnStack(const VectorOnStack& aVectorOnStack);
		VectorOnStack(const std::initializer_list<Type>& anInitList);
		~VectorOnStack();

		VectorOnStack& operator=(const VectorOnStack& aVectorOnStack);

		inline const Type& operator[](const CountType anIndex) const;
		inline Type& operator[](const CountType anIndex);
		inline void Add(const Type& anObject);
		inline void Insert(const CountType anIndex, const Type& anObject);
		inline void RemoveCyclic(const Type& anObject);
		inline void RemoveCyclicAtIndex(const CountType anIndex);
		inline void Clear();
		inline CountType Size() const;

	private:
		Type myArray[size];
		CountType mySize;
	};

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, size, CountType, UseSafeModeFlag>::VectorOnStack() :
		myArray{},
		mySize{}
	{
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, size, CountType, UseSafeModeFlag>::VectorOnStack(const VectorOnStack& aVectorOnStack)
		: VectorOnStack<Type, size, CountType, UseSafeModeFlag>()
	{
		mySize = aVectorOnStack.mySize;
		switch (UseSafeModeFlag)
		{
		case true:
		{
			for (CountType index{}; index < mySize; index++)
			{
				myArray[index] = aVectorOnStack.myArray[index];
			}
			break;
		}
		case false:
		{
			std::memcpy(myArray, &aVectorOnStack.myArray, sizeof(Type) * mySize);
			break;
		}
		}
	}


	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, size, CountType, UseSafeModeFlag>::VectorOnStack(const std::initializer_list<Type>& anInitList)
		: VectorOnStack<Type, size, CountType, UseSafeModeFlag>()
	{
		assert(size >= (int)anInitList.size());
		mySize = (CountType)anInitList.size();

		for (CountType index{}; index < size; index++)
		{
			if (anInitList.size() > index)
			{
				myArray[index] = *(anInitList.begin() + index);
			}
		}
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, size, CountType, UseSafeModeFlag>::~VectorOnStack()
	{
		mySize = CountType{};
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, size, CountType, UseSafeModeFlag>& VectorOnStack<Type, size, CountType, UseSafeModeFlag>::operator=(const VectorOnStack& aVectorOnStack)
	{
		mySize = aVectorOnStack.mySize;
		switch (UseSafeModeFlag)
		{
		case true:
		{
			for (CountType index{}; index < mySize; index++)
			{
				myArray[index] = aVectorOnStack.myArray[index];
			}
			break;
		}
		case false:
		{
			std::memcpy(myArray, &aVectorOnStack.myArray, sizeof(Type) * mySize);
			break;
		}
		}
		return *this;
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	inline const Type& VectorOnStack<Type, size, CountType, UseSafeModeFlag>::operator[](const CountType anIndex) const
	{
		assert(anIndex >= CountType{} && anIndex < mySize);
		return myArray[anIndex];
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	inline Type& VectorOnStack<Type, size, CountType, UseSafeModeFlag>::operator[](const CountType anIndex)
	{
		assert(anIndex >= CountType{} && anIndex < mySize);
		return myArray[anIndex];
	}


	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::Add(const Type& anObject)
	{
		assert(mySize < (CountType)size);

		switch (UseSafeModeFlag)
		{
		case true:
		{
			myArray[mySize++] = anObject;
			break;
		}
		case false:
		{
			std::memcpy(&myArray[mySize++], &anObject, sizeof(Type));
			break;
		}
		}

	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::Insert(const CountType anIndex, const Type& anObject)
	{

		assert(mySize < (CountType)size && (anIndex >= CountType{} && anIndex <= mySize || anIndex == CountType{}));

		CountType currentIndex{ mySize++ };

		switch (UseSafeModeFlag)
		{
		case true:
		{
			while (currentIndex >= anIndex && currentIndex > CountType{})
			{
				myArray[currentIndex] = myArray[currentIndex - 1];
				--currentIndex;
			}
			myArray[anIndex] = anObject;
			break;
		}
		case false:
		{
			while (currentIndex >= anIndex && currentIndex > CountType{})
			{
				std::memcpy(&myArray[currentIndex], &myArray[currentIndex - 1], sizeof(Type));
				--currentIndex;
			}
			std::memcpy(&myArray[anIndex], &anObject, sizeof(Type));
			break;
		}
		}
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::RemoveCyclic(const Type& anObject)
	{
		for (CountType index{}; index < mySize; index++)
		{
			if (myArray[index] == anObject)
			{
				myArray[index] = myArray[--mySize];
			}
		}
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::RemoveCyclicAtIndex(const CountType anIndex)
	{
		assert(anIndex >= CountType{} && anIndex < mySize);
		myArray[anIndex] = myArray[--mySize];
	}


	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::Clear()
	{
		mySize = CountType{};
	}

	template<typename Type, int size, typename CountType, bool UseSafeModeFlag>
	__forceinline CountType VectorOnStack<Type, size, CountType, UseSafeModeFlag>::Size() const
	{
		return mySize;
	}
}