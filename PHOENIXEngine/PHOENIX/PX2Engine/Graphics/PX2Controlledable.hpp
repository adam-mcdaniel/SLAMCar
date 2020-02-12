// PX2Controlledable.hpp

#ifndef PX2CONTROLLEDOBJECT_HPP
#define PX2CONTROLLEDOBJECT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Controller.hpp"

namespace PX2
{
	
	/// ����������
	/**
	* �Ӵ�������������а󶨿������Ĺ��ܡ�
	*/
	class PX2_ENGINE_ITEM Controlledable : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_IDS;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(Controlledable);

	protected:
		Controlledable ();
	public:
		virtual ~Controlledable ();

		void ResetPlay ();
		virtual void Play ();
		virtual bool IsPlaying () const;
		virtual void Pause();
		virtual void Reset ();
		void SetSelfCtrled (bool selfCtrled);
		bool IsSelfCtrled () const;
		void SetAutoPlay(bool autoPlay);
		bool IsAutoPlay() const;

		// ���ʿ��ƶ���Ŀ�����
		int GetNumControllers () const;
		Controller* GetController (int i) const;
		Controller *GetControllerByName (const std::string &name) const;
		template <typename CLASSTYPE>
		CLASSTYPE *GetController() const;
		bool HasController (Controller* controller);
		void AttachController (Controller* controller);
		void DetachController (Controller* controller);
		void DetachAllControllers ();
		void SortControls (); // �������ȼ��Ӵ�С����
		bool UpdateControllers (double applicationTime, double elapsedTime);

	private:
		bool mIsSelfCtrled;
		bool mIsAutoPlay;
		std::vector<ControllerPtr> mControllers;

		double mControlUpdateTimeLast;
		double mControlUpdateTimeMin;
	};

	PX2_REGISTER_STREAM(Controlledable);
	typedef Pointer0<Controlledable> ControlledablePtr;
#include "PX2Controlledable.inl"

}

#endif