// PX2Culler.hpp

#ifndef PX2CULLER_HPP
#define PX2CULLER_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Camera.hpp"
#include "PX2Movable.hpp"
#include "PX2VisibleSet.hpp"
#include "PX2Culler.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class Canvas;
	
	/// �����ü�����
	class PX2_ENGINE_ITEM Culler
	{
	public:
		/**
		* �ü�����Ҫ������������cameraû�б����ݸ����캯��������Ҫ��
		* ComputeVisibleSet����֮ǰ����SetCamera��
		*/
		Culler ();
		Culler (const Camera* camera);
		virtual ~Culler ();

		// 0 ingore, 1 do, 2 not do
		void SetFlag_CastShadow(int cast);
		int GetFlag_CastShadow() const;

		// ����ͽ�ͷ������
		inline void SetCamera (const Camera* camera);
		inline const Camera* GetCamera () const;
		void SetFrustum (const float* frustum);
		inline const float* GetFrustum () const;
		inline VisibleSet& GetVisibleSet ();
		void Clear();

		/// ����ɼ����󵽿ɼ��б�
		/**
		* �����������ش˺��������������ʵ��һ��ѡ�����飬ʵ����С����״̬�ı䣬
		* ����ʵ��һ������֧�����ϵͳ��
		*/
		virtual void Insert (Renderable* visible);

		// ����Բ�������ӽ�ͷ��֮��Ĳü��棬����ü���Ҫ��������������ϵ��
		enum { MAX_PLANE_QUANTITY = 32 };
		inline int GetPlaneQuantity () const;
		inline const HPlane* GetPlanes () const;
		inline void SetPlaneState (unsigned int planeState);
		inline unsigned int GetPlaneState () const;
		inline void PushPlane (const HPlane& plane);
		inline void PopPlane ();

		/// ������������Χ��Ͳü�����бȽϣ����ɼ��жϣ�ֻ��Movable���ô˺�����
		bool IsVisible (const Bound& bound);

		/// �ü������ĺ���
		/**
		* ������������ü�������������������������У�������������������Ǳ�ڿ�
		* ������
		*/
		void ComputeVisibleSet (Movable* scene);
		Movable *GetCurCullingScene();

	protected:
		// �ü�����Ҫ�����
		const Camera* mCamera;

		int mFlag_CastShadow;

		/** 
		* ��������Ľ�ͷ��Ŀ��������������ʽϵͳ�ڲü���ʱ��ı��ͷ�壨����]
		* ��˵�������ϵͳ������Ӱ�쵽�����
		*/
		float mFrustum[Camera::VF_QUANTITY];

		/** 
		* ����ü��������ͷ����صģ�����Ҳ���������û��Զ���Ĳü��档��Ա
		* mPlaneState����bit�洢��Ϊ��־�ģ���ʾ�����ڲü�ϵͳ���Ƿ񼤻bitΪ
		* 1��ʾƽ��ʽ����ģ�����ƽ���ǲ�����ġ�һ�������ƽ������Χ����Ƚϣ�
		* ���м����ƽ�治�Ƚϡ���������֧�ָ�Ч�ĵȼ��ü���������˵�����һ��
		* Node�İ�Χ�����������ü��桱���ڲ�����ô�������ü��桱������Ϊ��
		* �����(inactive)����Ϊ������ڵ�����еĺ��Ӷ��ڡ���ü��桱�ڣ�����
		* Ҫ���롰��ü��桱�Ƚ��ˡ�
		*/
		int mPlaneQuantity;
		HPlane mPlane[MAX_PLANE_QUANTITY];
		unsigned int mPlaneState;

		/// �ɼ���
		VisibleSet mVisibleSet;

		Movable *mCurCullingScene;
	};
	typedef Pointer0<Culler > CullerPtr;
#include "PX2Culler.inl"

}

#endif