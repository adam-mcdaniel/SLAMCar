// PX2MutexType.hpp

#ifndef PX2MUTEXTYPE_HPP
#define PX2MUTEXTYPE_HPP

#include "PX2CorePre.hpp"

//----------------------------------------------------------------------------
#if defined (PX2_USE_PTHREAD)
//----------------------------------------------------------------------------
// ��Linux/Apple�У�pthread.h���������ԭʼ�������ݣ�����pthread.h���ᱩ¶
// ����Ľӿڡ�
//----------------------------------------------------------------------------
#include <pthread.h>
namespace PX2
{
	struct MutexType
	{
		pthread_mutexattr_t Attribute;
		pthread_mutex_t Mutex;
	};
}
#else

#if defined(_WIN32) || defined(WIN32)
//----------------------------------------------------------------------------
// ��Windows�У�Mutex��һ��HANDLE����Ҫ����<windows.h>�������ᱩ¶����
// Windows�ӿڡ�����������void*����HANDLE��
//----------------------------------------------------------------------------
namespace PX2
{
	typedef void* MutexType;
}

#endif

#endif

#endif
