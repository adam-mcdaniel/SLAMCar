// PX2ScopedCS.hpp

#ifndef PX2SCOPEDCS_HPP
#define PX2SCOPEDCS_HPP

#include "PX2CorePre.hpp"
#include "PX2Mutex.hpp"

namespace PX2
{

	/// ����Χ�ࣺScoped critial section
	/**
	* ������һ����Χ�������ľֲ���ʱ���󴴽�һ��mutex�����˳�����ʱ���Զ��ͷ�
	* ���mutex��
	*/
	class PX2_ENGINE_ITEM ScopedCS
	{
	public:
		ScopedCS (Mutex* mutex);
		~ScopedCS ();

	private:
		ScopedCS();
		ScopedCS(const ScopedCS&);
		ScopedCS& operator = (const ScopedCS&);

		Mutex* mMutex;
	};

}

#endif
