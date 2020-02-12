// PX2NetMsgQueue.hpp

#ifndef PX2MSGQUEUE_HPP
#define PX2MSGQUEUE_HPP

#include "PX2CorePre.hpp"
#include "PX2Mutex.hpp"
#include "PX2ScopedCS.hpp"

namespace PX2
{

	/// ��Ϣ����
	/* 
	* ������1��������(�Ӷ����ж���Ϣ)�����������(����������д��Ϣ)
	*/
	template <class MsgPacket>
	class MsgQueue
	{
	public:
		MsgQueue ();
		virtual ~MsgQueue ();

		void Clear();
		size_t GetAllSize();

		void AddMsg (MsgPacket *msg);
		MsgPacket *PopMsg();
		MsgPacket *Front();

		std::queue<MsgPacket*> &GetConsumeQueue();

	private:
		std::queue<MsgPacket*> mQueues[2]; // һ�������߶��У�һ�������߶���
		unsigned int mCurIndex; //��ǰ�������õĶ�������
		Mutex mMutex;
	};

#include "PX2MsgQueue.inl"

}

#endif