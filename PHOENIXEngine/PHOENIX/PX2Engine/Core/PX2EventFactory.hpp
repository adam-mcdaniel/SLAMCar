// PX2EventChannel.hpp

#ifndef PX2EVENTFACTORY_HPP
#define PX2EVENTFACTORY_HPP

#include "PX2EventFactoryImplement.hpp"

namespace PX2
{
	
	class PX2_ENGINE_ITEM EventFactory
	{
	public:
		// ����
		static EventFactory &GetInstance ();
		static EventFactory* GetInstancePtr ();

		// �¼�
		Event* CreateEventX ();
		void DestoryEvent (Event* event);

		// �¼��ռ�
		void RegisterEventSpace (EventSpace* eventSpace);
		void UnRegisterEventSpace (const std::string &name);
		EventSpace* GetEventSpace (const std::string &name) const;
		EventSpace* FindEventSpace (Event* event) const;

	private:
		EventFactory ();
		~EventFactory ();

		EventFactoryImplement* mEventImplement;
	};

}

#endif