/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
*/
/*
	�ֽ��򽻻�����ģ�飺
		 ��������ͨ��һ�����BIG�ֽ���\Ҳ���������ֽ���.
 		 ����ʹ�õ�PC������Ƕ��ʽϵͳ����ʹ��BIG�ֽ���Ҳ����ʹ��LITTEN(С�ֽ���)
 		 �������Ǳ����ڴ�֮����һ���ֽ����ת����
*/
#ifndef __MEMORYSTREAMCONVERTER_H__
#define __MEMORYSTREAMCONVERTER_H__
// common include
#include "cstdkbe/cstdkbe.hpp"
#include <algorithm>
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#include <errno.h>
#endif
	
namespace KBEngine{

namespace MemoryStreamConverter
{
    template<size_t T>
        inline void convert(char *val)
    {
        std::swap(*val, *(val + T - 1));
        convert<T - 2>(val + 1);
    }

    template<> inline void convert<0>(char *) {}
    template<> inline void convert<1>(char *) {}            // ignore central byte

    template<typename T> inline void apply(T *val)
    {
        convert<sizeof(T)>((char *)(val));
    }
}

#if KBENGINE_ENDIAN == KBENGINE_BIG_ENDIAN			// ����ʹ��sys.isPlatformLittleEndian() ���в���
template<typename T> inline void EndianConvert(T& val) { MemoryStreamConverter::apply<T>(&val); }
template<typename T> inline void EndianConvertReverse(T&) { }
#else
template<typename T> inline void EndianConvert(T&) { }
template<typename T> inline void EndianConvertReverse(T& val) { MemoryStreamConverter::apply<T>(&val); }
#endif

template<typename T> void EndianConvert(T*);         // will generate link error
template<typename T> void EndianConvertReverse(T*);  // will generate link error

inline void EndianConvert(uint8&) { }
inline void EndianConvert(int8&) { }
inline void EndianConvertReverse(uint8&) { }
inline void EndianConvertReverse(int8&) { }

}
#endif