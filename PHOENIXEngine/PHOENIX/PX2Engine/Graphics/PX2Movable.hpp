// PX2Movable.hpp

#ifndef PX2SPATIAL_HPP
#define PX2SPATIAL_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Controlledable.hpp"
#include "PX2Bound.hpp"
#include "PX2Transform.hpp"
#include "PX2Function.hpp"
#include "PX2InputEventData.hpp"

namespace PX2
{

	class Culler;
	
	/// �ռ伸������
	/**
	* ������Ҫ�ռ�����֧�ֵ����Ͷ��Ӵ���������ͬʱ֧�ֽ�ͷ��ü����ܡ��ռ�����
	* �������ռ�任����Χ�С�
	*/
	class PX2_ENGINE_ITEM Movable : public Controlledable
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(Movable);

	protected:
		Movable ();
	public:
		virtual ~Movable ();

		/// ���غ�����任
		/**
		* ��ĳЩ����£���Ҳ����Ҫֱ����������任�����World�������ˣ�
		* WorldIsCurrent��ʶҲ��ñ�����Ϊtrue��
		*/
		Transform LocalTransform;
		Transform WorldTransform;
		bool WorldTransformIsCurrent;

		Transform &GetLocalTransform();
		Transform &GetWorldTransform();

		bool IsSkinCtrlSetWroldTrans;
		Transform BoundWorldTransform;

		/// �����Χ�з���
		/**
		* ��ĳЩ����£���Ҳ����Ҫֱ�����������Χ�С����WorldBound��ֱ������
		* ��WorldBoundIsCurrentӦ�ñ�����Ϊtrue��
		*/
		Bound WorldBound;
		bool WorldBoundIsCurrent;

		/// �ü�����
		enum CullingMode
		{
			/// ͨ���Ƚϰ�Χ�кͲü��棬��������Ŀɼ��ԡ�
			CULL_DYNAMIC,

			/// ǿ�����屻�ü��������һ��Node���ü��ˣ���Node�µ������ӽڵ�Ҳ
			/// ���ü���	
			CULL_ALWAYS,

			/// ��Զ���ü����塣���һ��Node�����ü�����Node�µ������ӽڵ�Ҳ��
			/// ���ü���
			CULL_NEVER
		};

		CullingMode Culling;

		virtual void Show (bool show);
		bool IsShow () const;

		/// ���¼���״̬�Ϳ�����
		/**
		* �������¸��³����任�����ϸ��������Χ�С�
		*/
		void Update (double applicationTime = -Mathd::MAX_REAL, double elapsedTime=0.0f,
			bool initiator = true);

		/// ����
		virtual void OnBeAttached();
		virtual void OnBeDetach();
		Movable* GetParent ();
		void DetachFromParent();
		void SetParentTransformIngore (bool trans, bool rotate, bool scale);
		void GetParentTransformIngore (bool &trans, bool &rotate, bool &scale);

		Movable *GetTopestParent();

		template <typename CLASSTYPE>
		CLASSTYPE *GetFirstParentDerivedFromType(int *numLevels = 0);

		// Pick
	public:
		virtual void OnPicked (int pickInfo); // called during Picker::Execute
		virtual void OnNotPicked (int pickInfo);

		void SetDoPick (bool doPick);
		bool IsDoPick () const;

		void SetPickIngoreCullingMode (bool pickIngoreCullingMode);
		bool IsPickIngoreCullingMode () const;

		void SetNotPickedParentChildrenNotPicked (bool use);
		bool IsNotPickedParentChildrenNotPicked () const;

		// Color Alpha Brightness
	public:
		void SetColorSelfCtrled(bool selfCtrled);
		bool IsColorSelfCtrled() const;
		virtual void SetColor(const Float3 &color);
		const Float3 &GetColor() const;

		void SetAlphaSelfCtrled (bool selfCtrled);
		bool IsAlphaSelfCtrled () const;
		virtual void SetAlpha(float alpha);
		float GetAlpha () const;

		void SetBrightnessSelfCtrled (bool selfCtrled);
		bool IsBrightnessSelfCtrled () const;
		virtual void SetBrightness(float brightness);
		float GetBrightness () const;

		const Float3 &GetLastColor() const;
		float GetLastAlpha() const;
		float GetLastBrightness() const;

		virtual void MarkAlphaColorBrightnessChanged();

		virtual void SetCastShadow(bool castShadow);
		bool IsCastShadow() const;
		virtual void SetReceiveShadow(bool reciveShadow);
		bool IsReceiveShadow() const;

	protected:
		bool mIsAlphaColorBrightnessChanged;

		void _AdjustAlphaColorBrightnes();

		// enable active
	public:
		void SetEnableSelfCtrled(bool selfCtrled);
		bool IsEnableSelfCtrled() const;

		void SetActivateSelfCtrled(bool selfCtrled);
		bool IsActivateSelfCtrled() const;

		// Update
	public:
		void SetFixUpdateTime (float time);
		float GetFixUpdateTime () const;
		void ResetFixUpdateTiming();

		void SetOnlyShowUpdate(bool onlyShowUpdate);
		bool IsOnlyShowUpdate() const;

		// ��ֵԽ�󣬸������ȼ�Խ��
		void SetUpdatePriority (int updatePriority);
		int GetUpdatePriority () const;

	protected:
		// ����״̬����
		virtual void UpdateWorldData (double applicationTime, double elapsedTime);
		virtual void UpdateWorldBound () = 0;
		void PropagateBoundToRoot ();

public_internal:
		// �����ü�
		void GetVisibleSet (Culler& culler, bool noCull);
		virtual void OnGetVisibleSet (Culler& culler, bool noCull) = 0;
		virtual void OnCulled(Culler& culler);

		// ���ø��ڵ㣬�ڵ���attach/detach��ʱ����ô˺�����
		void SetParent (Movable* parent);

		void SetSaveWriteIngore(bool doSaveWrite);
		bool IsSaveWriteIngore() const;

	protected:
		Movable* mParent;

	protected:
		bool mIsIngoreParent_Translate;
		bool mIsIngoreParent_Rotate;
		bool mIsIngoreParent_Scale;

		bool mIsAlphaSelfCtrled;
		float mAlpha;
		bool mIsColorSelfCtrled;
		Float3 mColor;
		bool mIsBrightnessSelfCtrled;
		float mBrightness;

		float mLastAlpha;
		Float3 mLastColor;
		float mLastBrightness;

		bool mIsEnableSelfCtrled;
		bool mIsActivateSelfCtrled;

		float mUpdateTime;
		float mUpdateTiming;

		bool mIsOnlyShowUpdate;
		int mUpdatePriority;

		bool mIsDoPick;
		bool mIsPickIngoreCullingMode;
		bool mIsNotPickedParentChildrenNotPicked;

		bool mIsSaveWriteIngore;
		
		bool mIsCastShadow;
		bool mIsReceiveShadow;
	};

	PX2_REGISTER_STREAM(Movable);
	typedef Pointer0<Movable> MovablePtr;
#include "PX2Movable.inl"

}

#endif