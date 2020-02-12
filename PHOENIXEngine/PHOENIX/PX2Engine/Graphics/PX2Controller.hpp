// PX2Controller.hpp

#ifndef PX2CONTROLLER_HPP
#define PX2CONTROLLER_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"

namespace PX2
{

	class Controller;
	typedef void (*CtrlPlayedCallback) (Controller *ctrl);

	class Controlledable;

	class PX2_ENGINE_ITEM Controller : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(Controller);

	protected:
		Controller ();
	public:
		virtual ~Controller ();

		// ���ȼ�Խ�ߣ�Խ��ǰ����
		void SetPriority (int priority);
		int GetPriority () const;

		// ����ϵͳ
		void SetPlaySelfCtrl(bool playSelf);
		bool IsPlaySelfCtrl() const;
		void ResetPlay ();
		virtual void Play ();
		virtual bool IsPlaying () const;
		virtual void Pause ();
		virtual void Reset ();

		// ����ʱ��
		void SetMaxPlayTime (float time); // -1.0f forever
		float GetMaxPlayTime () const;
		void SetDelayPlayTime (float time);
		float GetDelayPlayTime () const;
		float GetPlayedTime () const;
		float GetPlayedTimeMinusDelay () const; // GetPlayedTime()-GetDelayPlayTime()

		// ���ſ���
		void SetPlayedDoDetach(bool doDetach);
		bool IsPlayedDoDetach () const;
		void SetPlayedCallback (CtrlPlayedCallback callback);
		CtrlPlayedCallback GetPlayedCallback ();

		// ��Ա����
		Controlledable* GetControlledable () const;

		// �������£�applicationTime���Ժ��������㵥λ��
		virtual bool Update (double applicationTime, double elapsedTime);

		/// ������������
		enum RepeatType
		{
			RT_NONE,
			RT_CLAMP,
			RT_WRAP,
			RT_CYCLE
		};
		
		// ��Ա����
		RepeatType Repeat;  //< default = RT_NONE
		double MinTime;     //< default = 0
		double MaxTime;     //< default = 0
		double Phase;       //< default = 0
		double Frequency;   //< default = 1
		bool Active;        //< default = true

public_internal:
		/// ���ñ����ƵĶ���
		virtual void SetControlledable (Controlledable* object);
		static bool LessThan (const Controller *ctrl0, const Controller *ctrl1);

	protected:
		/// ��Ӧ�ó����ʱ��ת�����������ĵ�ǰʱ�䡣�������ڸ����Լ���ʱ���
		/// �õ��˺�����
		double GetControlTimeByRangeTime (double rangeTime);
		virtual void _InitUpdate(double applicationTime, double elapsedTime);
		virtual void _Update (double applicationTime, double elapsedTime);

		/// ʹ��һ������ָ��ָ�򱻿��ƵĶ�����������controller��mObject֮��
		/// smartָ���ѭ������
		Controlledable* mObject;

		int mPriority;
		bool mIsPlaySelfCtrl;
		float mDelayPlayTime;
		float mMaxPlayTime;
		bool mIsPlayedDoDetach;
		bool mIsPlaying; // ����Ҫ����
		float mPlayedTime; // ����Ҫ����

		CtrlPlayedCallback mPlayedCallback;

		bool mIsEverPlayed;
	};

	PX2_REGISTER_STREAM(Controller);
	typedef Pointer0<Controller> ControllerPtr;
#include "PX2Controller.inl"

}

#endif