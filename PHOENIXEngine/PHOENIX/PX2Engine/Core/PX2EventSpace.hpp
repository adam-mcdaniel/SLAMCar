// PX2EventChannel.hpp

#ifndef PX2EVENTSPACE_HPP
#define PX2EVENTSPACE_HPP

#include "PX2Event.hpp"
#include "PX2Assert.hpp"

namespace PX2
{

	/// �¼��ռ�
	class PX2_ENGINE_ITEM EventSpace
	{
	public:
		// �ռ�
		const std::string& GetName () const;
		int GetEventQuantity ();
		bool IsIn (Event* event) const;

		/// ��������
		Event* CreateEventX (Event::EventType eventType);
		Event::EventType UnCover (Event::EventType eventType);
		Event::EventType Cover (Event* event) const;

		/// event�ڱ������ռ��eventType�������Ƿ���ͬ
		bool IsEqual (Event* event, Event::EventType eventType) const;

	protected:
		EventSpace (const std::string &name, int iEventQuantity);
		virtual ~EventSpace ();

	private:
		friend class EventFactoryImplement;
		void SetSpace (int shift);

		class Space
		{
		public:
			Space ();
			~Space ();

			void Reset (int begin, int end); 

			Event::EventType Cover (Event* event) const;
			Event::EventType UnCover (Event::EventType eventType) const;

			bool IsIn (Event* event) const;
			bool IsEqual (Event* event, Event::EventType eventType) const; //< �¼��ڵ�ǰ�ռ�Cover���Ƿ��eventType���

		private:
			int mBegin;
			int mEnd;
		};

		Space mSpace;
		std::string mName;
		int mEventQuantity;
	};

#include "PX2EventSpace.inl"

}

#endif