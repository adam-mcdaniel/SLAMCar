// PX2NotificationQueue.hpp

#ifndef PX2NOTFICATIONQUEUE_HPP
#define PX2NOTFICATIONQUEUE_HPP

#include "PX2NetPre.hpp"
#include "PX2Notification.hpp"
#include "PX2Eventt.hpp"

namespace PX2
{

	/// NotificationQueue�����ṩ��һ��ʵ���첽�ķ���
	/// ֪ͨ�� ����ڷ���֪ͨ�ر�����
	/// ��һ���̵߳���һ���̣߳�����Ӻ�̨�̵߳������û����棩�̡߳�
	///
	/// NotificationQueueҲ�������ڷַ�����
	/// һ�������̵߳�һ�����������̡߳� ÿ�������߳�
	/// �ظ�����waitDequeueNotification�����������ص�Notification�� 
	/// �ر�ʱ����ע��رպ����ٶ��е��Ƽ�˳����
	/// 1.Ϊÿ�������߳�������ֹ��־
	/// 2.����wakeUpAll��������
	/// 3.����ÿ�������߳�
	/// 4.�ƻ�֪ͨ���С�
	class PX2_ENGINE_ITEM NotificationQueue
	{
	public:
		NotificationQueue();
		~NotificationQueue();

		void EnqueueNotification(Notification *notification);
		void EnqueueUrgentNotification(Notification *notification);

		NotificationPtr DequeueNotification();
		NotificationPtr WaitDequeueNotification();
		NotificationPtr WaitDequeueNotification(long milliseconds);

		void WakeUpAll();
		bool IsEmpty() const;
		int GetSize() const;
		void Clear();
		bool HasIdleThreads() const;

	protected:
		NotificationPtr DequeueOne();

	private:
		typedef std::deque<NotificationPtr> NfQueue;
		NfQueue mNFQueue;

		struct WaitInfo
		{
			NotificationPtr Notif;
			Eventt EventAvailable;
		};
		typedef std::deque<WaitInfo*> WaitQueue;
		WaitQueue mWaitQueue;

		mutable Mutex mMutex;
	};


}

#endif