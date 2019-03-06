#include <lib/string.h>
#include <lib/buffer.hpp>

template<class it_src, class it_dest>
it_dest copy(it_src src_b, it_src src_e,it_dest dest_b)
{
	auto it=src_b;
	auto wit=dest_b;
	for(;it!=src_e;++it,++wit)
		*wit=*it;
	return wit;
}

extern "C"{

	int memcmp(const void* lhs, const void* rhs, size_t len)
	{
		auto lit = (char*)lhs;
		auto rit = (char*)rhs;
		for(size_t idx=0;idx<len;++idx)
		{
			if(*lit>*rit)
				return 1;
			else if(*lit<*rit)
				return -1;
		}
		return 0;
	}

	void* memcpy(void* dest, const void* src, size_t len)
	{
		auto begin=(char*)src;
		auto end=begin+len;
		copy(begin,end,(char*)dest);
		return dest;
	}

	
	void* memmove(void* dest, const void* src, size_t len)
	{
		ksdk::buffer<char> source((char*)src,len);
		if (dest < src) {
			copy(
				source.begin(),
				source.end(),
				(char*)dest
			);
		} else {
			copy(
				source.rbegin(),
				source.rend(),
				KSDK_INT::r_op_iterator<char>((char*)dest)
			);
		}
		return dest;
	}
	
	void* memset(void* dest, int v, size_t len)
	{
		auto begin = (char*)dest;
		auto end=begin+len;
		for(auto it=begin;it!=end;++it)
			*it=v;
		return dest;
	}
	
	size_t strlen(const char* s)
	{
		size_t len=0;
		while(s[len])len++;
		return len;
	}

	bool strcompare (const char* a, const char* b) {
		size_t size = strlen(a);
		if (size != strlen(b)) {
			return false;
		}
		for (size_t offset = 0; offset < size; offset++) {
			if (a[offset] != b[offset]) {
				return false;
			}
		}
		return true;
	}
}