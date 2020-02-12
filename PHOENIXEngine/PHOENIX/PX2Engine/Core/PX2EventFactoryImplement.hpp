// PX2EventFactoryImpl.hpp

#ifndef PX2EVENTFACTORYIMPLEMENT_HPP
#define PX2EVENTFACTORYIMPLEMENT_HPP

#include "PX2CorePre.hpp"
#include "PX2Event.hpp"
#include "PX2Memory.hpp"

namespace PX2
{
	class Event;
	class EventSpace;

	/// �¼�����ʵ����
	class PX2_ENGINE_ITEM EventFactoryImplement
	{
	public:
		EventFactoryImplement ();
		~EventFactoryImplement ();

		// �¼�
		Event* CreateEventX ();
		void DestoryEvent (Event* event);

		// �¼��ռ�
		void RegisterEventSpace (EventSpace* eventSpace);
		void UnRegisterEventSpace (const std::string &name);
		EventSpace* GetEventSpace (const std::string &name) const;
		EventSpace* FindEventSpace (Event* event) const;

	private:
		typedef std::map<std::string, EventSpace*> EventSpaceMap;

		EventSpaceMap mSpaces;
		int mEventQuantity;
	};

}

#endif