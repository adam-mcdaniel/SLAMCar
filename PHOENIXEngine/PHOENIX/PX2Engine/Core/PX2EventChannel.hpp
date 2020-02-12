// PX2EventChannel.hpp

#ifndef PX2EVENTCHANNEL_HPP
#define PX2EVENTCHANNEL_HPP

#include "PX2CorePre.hpp"
#include "PX2Assert.hpp"

namespace PX2
{

	/// ͨ�����Ͷ���
	typedef unsigned int EventChannelType;

	// 32λ��64λ
	template <class T,int I = (sizeof(T)>>2)>
	class ChannelLength
	{
	public:
		enum
		{
			Left = (I<<3),
			Right = (sizeof(T) - I)<<3,
			Length = Left + Right
		};
	};

	// 16λ
	template <class T>
	class ChannelLength <T, 0>
	{
	public:
		enum
		{
			Left = 8,
			Right = (sizeof(T) - 1)<<3,
			Length = Left + Right
		};
	};

	/// ͨ������
	/**
	* ��EventChannel˵��
	*/
	enum
	{
		SystemChannelLength = ChannelLength<EventChannelType>::Left,
		UserChannelLength = ChannelLength<EventChannelType>::Right,
		AllChannelLength = ChannelLength<EventChannelType>::Length,
	};

	// �û�Ƶ��
	template <int N> 
	class UserChannel;

	template <int N>
	class UserChannel
	{
	public:
		enum {Value = UserChannel<N-1>::Value<<1};
	};

	template <>
	class UserChannel<0>
	{
	public:
		enum {Value = 1<<SystemChannelLength};

	};

	// ϵͳƵ��
	// ����
	template <int N>
	class SystemChannel
	{
	public:
		enum {Value = SystemChannel<N-1>::Value<<1};
	};

	template <>
	class SystemChannel<0>
	{
	public:
		enum {Value = 1};
	};

	/// �¼�ͨ��
	/**
	* ������EventChannelType���з�װ����Ϊû���麯��������
	* sizeof(MessageChannel) ���� sizeof(MessageChannel_Type)��
	* ����32bit��EventChannelType��8bitΪϵͳƵ����24bitΪ�û�Ƶ����
	* ����16bit��EventChannelType��8bitΪϵͳƵ����8bitΪ�û�Ƶ����
	* ����64bit��EventChannelType��16bitΪϵͳƵ����48bitΪ�û�Ƶ����
	*/
	class PX2_ENGINE_ITEM EventChannel
	{
	public:
		EventChannel ();
		EventChannel (const EventChannel &channel);
		~EventChannel ();

		// ��ֵ
		EventChannel &operator= (const EventChannel &channel); 

		// Ƶ������

		/**
		* �����������ã����԰������淽ʽ���ã�
		* EventChannel::Create()
		*		.AddUserChannel<2>()
		*		.AddUserChannel<3>()
		*/
		template <int N>
		EventChannel &AddUserChannel ();		

		template <int N>
		EventChannel &RemoveUserChannel ();

		template <int N>
		EventChannel &AddSystemChannel ();

		template <int N>
		EventChannel &RemoveSystemChannel ();

		EventChannel &Fill ();
		EventChannel &Clear ();
		EventChannel &FillUserChannel ();
		EventChannel &ClearUserChannel ();
		EventChannel &FillSystemChannel ();
		EventChannel &ClearSystemChannel ();

		// �ϲ����Ƴ�
		void Merge (const EventChannel &channel);
		void DisMerge (const EventChannel &channel);

		// ����
		bool IsEmpty () const;
		bool IsListening (const EventChannel &channel) const; //< ����ͨ���Ƿ�����ͬ����Ϣ�ڱ���

		// ����
		static EventChannel Create ();

	protected:
		void Initlize (const EventChannel &channel);
		void AddChannel (const EventChannelType type);
		void RemoveChannel (const EventChannelType type);
		
		EventChannelType mEventChannelType;
	};

#include "PX2EventChannel.inl"

}

#endif