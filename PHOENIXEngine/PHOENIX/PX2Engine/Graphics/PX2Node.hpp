// PX2Node.hpp

#ifndef PX2NODE_HPP
#define PX2NODE_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Movable.hpp"
#include "PX2Culler.hpp"
#include "PX2Function.hpp"

namespace PX2
{

	typedef void(*TravelExecuteFun) (Movable *mov, Any *data, bool &goOn);

	/// �����ڵ���
	/**
	* ����ʵ�֡�����-���ӡ����ƣ�������֯�����Ĺ������Ӹ������������ͬ�����磺
	* BspNode��SwitchNode�ȣ�ʵ�ֲ�ͬ�ĳ�����֯��ʽ��
	*/
	class PX2_ENGINE_ITEM Node : public Movable
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_NEW(Node);
		PX2_DECLARE_STREAM(Node);

	public:
		Node ();
		virtual ~Node ();

		virtual void Play ();
		virtual bool IsPlaying () const;
		virtual void Pause();
		virtual void Reset ();

		/// ��ú�������
		int GetNumChildren () const;
		int GetNumValidChildren () const;

		virtual int AttachChild (Movable* child);
		void InsertChild (Movable *before, Movable *child);
		virtual int DetachChild (Movable* child);
		virtual bool DetachChildByName (const std::string &name);
		virtual MovablePtr DetachChildAt (int i);
		virtual void DetachAllChildren ();
		virtual MovablePtr SetChild (int i, Movable* child);
		virtual Movable *GetChild (int i);
		std::vector<MovablePtr> &GetChildren();
		virtual Movable *GetChildByName (const std::string &name);
		bool IsHasChild(const Movable *child) const;

		void ShowOnlyChild(const std::string &childName);

		virtual void Enable(bool enable);
		virtual void SetActivate(bool act);

		virtual void MarkAlphaColorBrightnessChanged();

		virtual void SetCastShadow(bool castShadow);
		virtual void SetReceiveShadow(bool reciveShadow);
		
		void SetDoPickPriority (bool doPickPriority); // һ������ΪNode����
		bool IsDoPickPriority () const;
		virtual void OnNotPicked (int pickInfo);

		void SetNeedCalUpdateChild (bool needCal);
		bool IsNeedCalUpdateChild () const;

		void SetAnchorID (int anchorID);
		int GetAnchorID () const;

		static void TravelExecute(Movable *mov, TravelExecuteFun fun, Any *data=0);

	public_internal:
		virtual void OnCulled(Culler& culler);

	protected:
		virtual void OnChildAttached(Movable *child);
		virtual void OnChildDetach(Movable *child);

		// ����ͼ�θ���
		virtual void UpdateWorldData (double applicationTime, double elapsedTime);
		virtual void UpdateWorldBound ();
		void _UpdateWorldDataChild(double applicationTime, double elapsedTime);

		// �����̳вü�
		virtual void OnGetVisibleSet (Culler& culler, bool noCull);

		// �����б�
		std::vector<MovablePtr> mChild;

		int mAnchorID;
		
		std::vector<Movable*> mUpdateChild;
		bool mIsNeedCalUpdateChild;

		bool mIsDoPickPriority;
	};

	PX2_REGISTER_STREAM(Node);
	typedef Pointer0<Node> NodePtr;
#include "PX2Node.inl"

}

#endif
