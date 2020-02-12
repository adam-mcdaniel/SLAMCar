// PX2PushTransformController.hpp

#ifndef PX2PUSHTRANSFORMCONTROLLER_HPP
#define PX2PUSHTRANSFORMCONTROLLER_HPP

#include "PX2Controller.hpp"
#include "PX2AVector.hpp"

namespace PX2
{

	// ��һ������Ȼ��������Լ�����
	class PX2_ENGINE_ITEM PushTransformController : public Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(PushTransformController);

	public:
		PushTransformController ();
		virtual ~PushTransformController ();

		enum TransType
		{
			TT_TRANSLATE,
			TT_ROTATE,
			TT_SCALE,
			TT_MAX_TYPE
		};
		void SetTransType (TransType transType);
		TransType GetTransType ();

		void SetXY (bool isXY);
		bool IsXY () const;

		void SetMaxVelocity (float forceMax);
		float GetMaxVelocity ();

		// ���൱�븽�ӵ��ٶ�
		void AddVelocity (const AVector &addedVec);
		virtual void SetVelocity (const AVector &vec);
		const AVector &GetVelocity () const;

		// ÿ�뽵�͵�����
		void SetFriction (float fricton);
		float GetFriction ();

		virtual bool Update(double applicationTime, double elapsedTime);

		// ���������ش˺�����ʵ�־�����ƶ�
		virtual void OnMoving (const AVector &dis);

	protected:
		TransType mTransType;
		float mMaxVelocity;
		float mFriction;
		AVector mVelocity;

		bool mIsXY;
	};

	PX2_REGISTER_STREAM(PushTransformController);
	typedef Pointer0<PushTransformController> PushTransformControllerPtr;
#include "PX2PushTransformController.inl"

}

#endif