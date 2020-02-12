// PX2Semaphore.hpp

#ifndef PX2SEMAPHORE_HPP
#define PX2SEMAPHORE_HPP

#include "PX2CorePre.hpp"

#if defined(__LINUX__) || defined(__APPLE__) || defined(__ANDROID__)
#include<semaphore.h>
#endif

namespace PX2
{

	/// �ź���
	/**
	* �ź������ڶ��̻߳�����ʹ�õ�һ����ʩ��������Э�������̣߳��Ա�֤�����ܹ�
	* ��ȷ�������ʹ�ù�����Դ��
	*
	* �ź������������£��ź�����һ���Ǹ�����������ͨ�������̶߳��Ὣ��������
	* һ����������ֵΪ��ʱ��������ͼͨ�������̶߳������ڵȴ�״̬�����ź�������
	* �Ƕ������ֲ����� Wait���ȴ��� �� Set���ͷţ��� ��һ���̵߳���Wait���ȴ���
	* ����ʱ����Ҫôͨ��Ȼ���ź�����һ��Ҫôһֱ����ȥ��ֱ���ź�������һ��
	* ʱ�����ͷţ�ʵ���������ź�����ִ�мӲ������߳��ͷ������ź����ػ�����Դ��
	*/
	class PX2_ENGINE_ITEM Semaphore
	{
	public:
		Semaphore(int initNum);
		Semaphore(int initNum, int max);
		~Semaphore ();

		void Set();
		void Wait();

	private:
		Semaphore();
		Semaphore(const Semaphore&);
		Semaphore& operator = (const Semaphore&);

		int mInitNum;
		int mMax;

#if defined(_WIN32) || defined(WIN32)
		void *mHandle;
#elif defined(__LINUX__) || defined(__APPLE__) || defined(__ANDROID__)
		sem_t mSem;
#endif
	};

}

#endif