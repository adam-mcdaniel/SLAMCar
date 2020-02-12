// PX2Eventt.hpp

#ifndef PX2EVENTT_HPP
#define PX2EVENTT_HPP

#include "PX2CorePre.hpp"

#if (defined(_WIN32) || defined(WIN32))
typedef void* EventtType;
#else
#include <pthread.h>
#endif

namespace PX2
{

	/// �ź�
	/**
	* �ź���һ���߳�ͬ���ṹ��ͨ��һ���̼߳�������źţ������̵߳ȴ�
	* ����źű����
	* 
	* �¼�����ͻ������һ���������ں˶���������һ��ʹ�ü�����һ�����ڱ�ʶ
	* ���¼���һ���Զ����û���һ���˹����õĲ���ֵ������һ������ָ�����¼�����
	* ��֪ͨ״̬����δ֪ͨ״̬�Ĳ���ֵ��
	* �¼�����ɷ�Ϊ���֣�һ�����˹����õģ���һ�����Զ����õġ����˹����õ���
	* ���õ�֪ͨʱ���ȴ����¼��������߳̾���Ϊ�ɵ����̡߳�����һ���Զ��¼��õ�
	* ֪ͨʱ���ȴ����¼��������߳���ֻ��һ���̱߳�Ϊ�ɵ����̡߳�
	* 
	* ������״̬�����źţ������źš�Set/Reset�ֱ�EVENT��Ϊ������״̬�ֱ��Ƿ�
	* �ź��벻���źš�Wait()�ȴ���ֱ��������ָ�����źų�Ϊ���ź�״̬ʱ�ŷ��ء�
	*
	* ������
	* �ȴ���һ��ȫ��Eventt����gEvent:
	* Eventt gEvent;
	* �ڳ����п���ͨ������Eventt::Set�����¼�Ϊ���ź�״̬��
	* ������һ���̺߳���ThreadProc()
	* static void ThreadProc()
	* {
	*	gEvent.Wait();
	*	For(;;)
	*	{
	*		...
	*	}
	*	return 0;
	* }
	* ������̺߳�����ֻ������gEventΪ���ź�״̬ʱ��ִ�������forѭ������Ϊ
	* gEvent��ȫ�ֱ������������ǿ����ڱ���߳���ͨ��gEvent.Set��������̡߳�
	*/
	class PX2_ENGINE_ITEM Eventt
	{
	public:
		/// ����һ���¼������autoResetΪtrue����Wait()�ɹ����¼��Զ�Reset��
		/// ��Ϊδ����״̬��
		Eventt (bool autoReset=true);
		~Eventt ();

		/// �����¼������autoResetΪtrue��ֻ��һ���ȴ����̱߳��������У����Ϊ
		/// false�����еȴ����̶߳����������С�
		void Set ();

		/// �����¼���δ����״̬
		void Reset ();

		/// �ȴ��¼�������
		void Wait ();

		/// ��һ��ʱ���ڵȴ��¼��������������˷���true�����򷵻�false
		// ���루0.001�룩
		bool Wait (long milliseconds);

	protected:
		bool mIsAutoReset;

#if (defined(_WIN32) || defined(WIN32))
		EventtType mEvent;
#else
		volatile bool mState;
		pthread_mutex_t mMutex;
		pthread_cond_t mCond;
#endif

	private:
		Eventt (const Eventt&);
		Eventt& operator = (const Eventt&);
	};

}

#endif