#ifndef __MY_ARRAY_INL__
#define __MY_ARRAY_INL__

#include "MyArray.h"

template <class T>
MyArray<T>::MyArray(size_t size)
{
	this->_array = new elem[size]{ elem() };
	this->arraySize = size;

	//每一组里面元素的个数
	this->arrayGrouopElementsNumber = static_cast<int>(sqrt(size));
	//组的数量
	this->arrayGroupSize = size / arrayGrouopElementsNumber;
	//不够分配完整一组,剩下元素的数量
	int lastNumber = size - arrayGroupSize * arrayGrouopElementsNumber;
	//有剩余
	if (lastNumber != 0)
	{
		//要多分配一组
		++arrayGroupSize;
	}
	this->arrayGroup = new array_group[arrayGroupSize];
	for (size_t x = 0; x < arrayGroupSize; ++x)
	{
		if (lastNumber != 0 && x == arrayGroupSize - 1)
		{
			//要进行最后一组不满元素的分配
			arrayGroup[x].maxSize = lastNumber;
			arrayGroup[x].first = x * arrayGrouopElementsNumber;
			arrayGroup[x].last = (x + 1) * arrayGrouopElementsNumber - 1;
		}
		else {
			arrayGroup[x].maxSize = arrayGrouopElementsNumber;
			arrayGroup[x].first = x * arrayGrouopElementsNumber;
			arrayGroup[x].last = (x + 1) * arrayGrouopElementsNumber - 1;
		}
	}
}

template <class T>
MyArray<T>::~MyArray() {
	delete[] this->arrayGroup;
	delete[] this->_array;
}

template <class T>
const typename MyArray<T>::array_at MyArray<T>::insert(T& elem)
{
	for (size_t x = 0; x < arrayGroupSize; ++x)
	{
		auto nowGroup = arrayGroup[x];
		//如果当前组没有满
		if (nowGroup.size != nowGroup.maxSize)
		{
			//寻找组内的空余位置
			for (int y = nowGroup.first; y <= nowGroup.last; ++y)
			{
				//发现空余
				auto nowElem = _array[y];
				if (nowElem.isFree)
				{
					nowElem.isFree = false;
					nowElem.elem = elem;

					//组的成员数量+1
					++elementsNumber;
					++nowGroup.size;

					return array_at(y, x);
				}
			}
		}
	}
	array_at at(-1, -1);
	at.badStatus = true;
	return at;
}

template <class T>
void MyArray<T>::remove(const array_at& _where)
{
	if (!_where.badStatus) 
	{
		this->_array[_where._array_at].isFree = true;
		--elementsNumber;
		this->arrayGroup[_where._group_at].size -= 1;
	}
}

template <class T>
std::vector<T> MyArray<T>::getElements() const
{
	if (elementsNumber != 0)
	{
		std::vector<T> temp;
		temp.reserve(elementsNumber);
		for (size_t x = 0; x < arraySize; ++x)
		{
			if (!_array[x].isFree)
			{
				temp.push_back(_array[x].elem);
			}
		}
		return temp;
	}
	return std::vector<T>();
}

template <class T>
inline size_t MyArray<T>::getElementsCount() const
{
	return this->elementsNumber;
}


#endif
