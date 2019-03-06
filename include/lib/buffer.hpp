#pragma once
#include <stddef.h>

namespace KSDK_INT{

	template<typename T>
	T& ref(T& n)
	{
		return n;
	}

	template<class T>
	struct r_op_iterator
	{
		r_op_iterator(T* in):it(in){}
		T* it;
		auto operator++()
		{
			return it--;
		}
		auto operator++(int)
		{
			return --it;
		}
		auto operator--()
		{
			return it--;
		}
		auto operator--(int)
		{
			return --it;
		}
		T& operator*()
		{
			return *it;
		}
		T& operator[](const size_t idx)
		{
			return *(it-idx);
		}
		auto operator==(const r_op_iterator& rhs)
		{
			return it==rhs.it;
		}
		auto operator!=(const r_op_iterator& rhs)
		{
			return it!=rhs.it;
		}
		auto operator<=(const r_op_iterator& rhs)
		{
			return (rhs.it<=it);
		}
		auto operator>=(const r_op_iterator& rhs)
		{
			return (rhs.it>=it);
		}
		auto operator<(const r_op_iterator& rhs)
		{
			return (rhs.it<it);
		}
		auto operator>(const r_op_iterator& rhs)
		{
			return (rhs.it>it);
		}
	};
}

namespace ksdk{
	template<typename T>
	class buffer
	{
	protected:
		T* _buffer;
		size_t _size;

	public:
		constexpr buffer(T* b, T* e)
		:_buffer(b)
		,_size(e-b)
		{
		}

		constexpr buffer()
		:_buffer(nullptr)
		,_size(0)
		{
		}

		constexpr buffer(T* b, size_t sz)
		:_buffer(b)
		,_size(sz)
		{
		}

		size_t size()
		{
			return _size;
		}

		T* begin()
		{
			return _buffer;
		}

		T* end()
		{
			return _buffer+_size;
		}

		KSDK_INT::r_op_iterator<T> rbegin()
		{
			return KSDK_INT::r_op_iterator<T>(_buffer-1+_size);
		}

		KSDK_INT::r_op_iterator<T> rend()
		{
			return KSDK_INT::r_op_iterator<T>(_buffer-1);
		}

		T& operator[](size_t idx)
		{
			return _buffer[idx];
		}

	};
}