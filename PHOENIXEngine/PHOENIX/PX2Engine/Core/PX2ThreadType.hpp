// PX2ThreadType.hpp

#ifndef PX2THREADTYPE_HPP
#define PX2THREADTYPE_HPP

#include "PX2CorePre.hpp"

//----------------------------------------------------------------------------
#if defined(PX2_USE_PTHREAD)
//----------------------------------------------------------------------------
// ��Linux/Apple�У�pthread.h���������ԭʼ�������ݣ�����pthread.h���ᱩ¶
// ����Ľӿڡ�
//----------------------------------------------------------------------------
#include <pthread.h>
namespace PX2
{
	typedef pthread_t ThreadType;
}
//----------------------------------------------------------------------------
#else

#if defined(_WIN32) || defined(WIN32)
//----------------------------------------------------------------------------
// ��Windows�У�Mutex��һ��HANDLE����Ҫ����<windows.h>�������ᱩ¶����
// Windows�ӿڡ�����������void*����HANDLE��
//----------------------------------------------------------------------------
namespace PX2
{
	typedef void* ThreadType;
}
#endif

#endif

#endif
