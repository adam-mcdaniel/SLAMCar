// PX2CorePre.hpp

#ifndef PX2COREPRE_HPP
#define PX2COREPRE_HPP

//----------------------------------------------------------------------------
// ƽ̨��Ϣ.����ĺ��ƽ̨���п���.
// _WIN32 or WIN32  :  Microsoft Windows
// __APPLE__        :  Macintosh OS X
// __LINUX__        :  Linux
// __ANDROID__		:  Android
// __IOS__          :  IOS
//----------------------------------------------------------------------------
// Microsoft Windows
//----------------------------------------------------------------------------
#if defined(_WIN32) || defined(WIN32)

#ifndef _WIN32
#define _WIN32
#endif

#ifndef WIN32
#define WIN32
#endif

#define PX2_LITTLE_ENDIAN

#if defined(_MSC_VER)
// MSVC6 is version 12.00, MSVC7.0 is version 13.00, MSVC7.1 is version 13.10,
// MSVC8.0 is version 14.00, and MSVC9.0 is version 15.00.  Currently, only
// version 9.0 or later are supported.
#if _MSC_VER < 1400
#error MSVC 8.0 or later is required
#elif _MSC_VER < 1500
#define PX2_USING_VC80
#else
#define PX2_USING_VC90
#endif

// Disable the Microsoft warnings about not using the secure functions.
#pragma warning(disable : 4996)

// The use of PX2_<libname>_ITEM to export an entire class generates warnings
// when member data and functions involving templates or inlines occur.  To
// avoid the warning, PX2_<libname>_ITEM can be applied only to those items
// that really need to be exported.
#pragma warning(disable : 4251)

// Support for standard integer types.  This is only a small part of what
// stdint.h provides on Unix platforms.
#include <climits>

#if _MSC_VER >= 1600
#include <stdint.h>
#else
typedef __int8              int8_t;
typedef __int16             int16_t;
typedef __int32             int32_t;
typedef __int64             int64_t;
typedef unsigned __int8     uint8_t;
typedef unsigned __int16    uint16_t;
typedef unsigned __int32    uint32_t;
typedef unsigned __int64    uint64_t;

#define INT8_MIN            _I8_MIN
#define INT8_MAX            _I8_MAX
#define INT16_MIN           _I16_MIN
#define INT16_MAX           _I16_MAX
#define INT32_MIN           _I32_MIN
#define INT32_MAX           _I32_MAX
#define INT64_MIN           _I64_MIN
#define INT64_MAX           _I64_MAX
#define UINT8_MAX           _UI8_MAX
#define UINT16_MAX          _UI16_MAX
#define UINT32_MAX          _UI32_MAX
#define UINT64_MAX          _UI64_MAX
#endif

#endif

#endif
//----------------------------------------------------------------------------
// Macintosh OS X
//----------------------------------------------------------------------------
#if defined(__APPLE__)

#if defined(__BIG_ENDIAN__)
#define PX2_BIG_ENDIAN
#else
#define PX2_LITTLE_ENDIAN
#endif

#define PX2_USE_PTHREAD
#endif
//----------------------------------------------------------------------------
// IOS
//----------------------------------------------------------------------------
#if defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) || defined(__IPHONE_OS_VERSION_MIN_REQUIRED)

#ifndef __IOS__
#define __IOS__
#endif

#endif
//----------------------------------------------------------------------------
// Linux
//----------------------------------------------------------------------------
#if defined(__LINUX__)

#include <inttypes.h>

#define PX2_LITTLE_ENDIAN
#define PX2_USE_PTHREAD

#if defined(__LP64__)
#define PX2_PTR_IS_64_BIT 1
#define PX2_LONG_IS_64_BIT 1
typedef signed long        int64_t;
typedef unsigned long      uint64_t;
#else
typedef signed long long   int64_t;
typedef unsigned long long uint64_t;
#endif

#define PX2_HAVE_INT64 1

#endif
//----------------------------------------------------------------------------
// Android
//----------------------------------------------------------------------------
#if defined(__ANDROID__)

#include <inttypes.h>

#define PX2_LITTLE_ENDIAN
#define PX2_USE_PTHREAD
#endif
//----------------------------------------------------------------------------

#if defined(PX2_ENGINE_DLL_EXPORT)
// For the DLL library.
#define PX2_ENGINE_ITEM __declspec(dllexport)
#elif defined(PX2_ENGINE_DLL_IMPORT)
// For a client of the DLL library.
#define PX2_ENGINE_ITEM __declspec(dllimport)
#else
// For the static library and for Apple/Linux.
#define PX2_ENGINE_ITEM
#endif

// ���ñ�׼ͷ�ļ�
#include <cassert>
#include <cctype>
#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <fstream>
#ifdef PX2_USE_PTHREAD
#include <pthread.h>
#endif

// STL
#include <algorithm>
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <bitset>

// int64
#define PX2_HAVE_INT64 1

// �û��Զ���ؼ���
#define public_internal public
#define protected_internal protected
#define private_internal private

// ��ֹδʹ�ñ��������ľ���.��Debug�»�������棬��Ҫ��������������棻
// ��Release�²����������.
#define PX2_UNUSED(variable) (void)variable

// ��Щ������������ֶ���ϵͳ.
// ע�⣺��ǰֻ��Microsoft Visual Studio������֧��.
#ifdef _DEBUG
	#if defined(WIN32) && defined(_MSC_VER)
		#define PX2_USE_ASSERT
		#ifdef PX2_USE_ASSERT
			#define PX2_USE_ASSERT_WRITE_TO_OUTPUT_WINDOW	// ������д����Դ���
			#define PX2_USE_ASSERT_WRITE_TO_MESSAGE_BOX		// ������д��MessageBox
		#endif
	#endif
	#define PX2_USE_ASSERT_LOG_TO_FILE
#endif

// ���������ڴ����ĺ�
#ifdef _DEBUG

	#define PX2_USE_MEMORY // ʹ��Phoenix���ڴ�������

	#ifdef PX2_USE_MEMORY
		// ����main����֮ǰ�����ڴ������main�˳�֮���ͷ��ڴ棬������
		#define PX2_USE_MEMORY_ASSERT_ON_PREMAIN_POSTMAIN_OPERATIONS

		// ���ָ��ָ����ڴ治����Phoenix�ڴ������Ʒ���ģ��ڴ������
		// delete0��delete1��delete2����delete3�ĵ��û��������ԡ����ʹ�ô˺꣬
		// ��ʹ��c++��׼'delete'����'delete[]'�ͷ���'new','new[]'������ڴ档
		#define PX2_USE_MEMORY_ALLOW_DELETE_ON_FAILED_MAP_LOOKUP
	#endif
#endif

// FileIO��BufferIO
#ifdef _DEBUG
#define PX2_FILEIO_VALIDATE_OPERATION		// ����ļ���������ȷ��
#define PX2_BUFFERIO_VALIDATE_OPERATION		// ����ڴ��������ȷ��
#endif

#define SMART_POINTER_USEMUTEX

#endif
