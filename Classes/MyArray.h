#ifndef __MY_ARRAY_H__
#define __MY_ARRAY_H__

#include <vector>

template <class T>
class MyArray
{
public:

	MyArray(size_t size);
	~MyArray();

	class array_at;

	[[nodiscard]] const array_at insert(T& elem);
	void remove(const array_at& _where);
	std::vector<T> getElements() const;
	size_t getElementsCount() const;

	class array_at
	{
	public:
		array_at(int _array_at, int _group_at)
			: _array_at(_array_at), _group_at(_group_at) {}
		array_at() {}
		const array_at& operator=(const array_at& at)
		{
			this->badStatus = at.badStatus;
			this->_array_at = at._array_at;
			this->_group_at = at._group_at;
			return *this;
		}
		friend class MyArray;

	private:
		//是否为无效位置
		bool badStatus = false;
		//元素在总数组的位置
		int _array_at = 0;
		//元素在哪一个组
		int _group_at = 0;
	};

private:
	struct array_group
	{

		int first = 0;
		int last = 0;

		int size = 0;
		int maxSize = 0;
	};
	struct elem
	{
		bool isFree = true;
		T elem;
	};
	size_t arraySize = 0;
	size_t arrayGroupSize = 0;
	int elementsNumber = 0;
	int arrayGrouopElementsNumber = 0;
	elem* _array = nullptr;
	array_group* arrayGroup = nullptr;
};

#include "MyArray.inl"

#endif
