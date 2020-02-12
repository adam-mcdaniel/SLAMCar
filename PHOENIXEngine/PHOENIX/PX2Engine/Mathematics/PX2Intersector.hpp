// PX2Intersector.hpp

#ifndef PX2INTERSECTOR_HPP
#define PX2INTERSECTOR_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Vector2.hpp"
#include "PX2Vector3.hpp"

namespace PX2
{

	template <typename Real, typename TVector>
	class PX2_ENGINE_ITEM Intersector
	{
	public:
		virtual ~Intersector ();

		/// ��̬����Ƿ��ཻ
		/** 
		* ȱʡʵ�ַ���'false'��
		*/
		virtual bool Test ();

		/// ��̬�����ཻ��
		/** 
		* ����һ���ཻ����������Ի���ཻ��������Ϊ�ཻ������������ཻ����
		*/
		virtual bool Find ();

		/// ��̬����Ƿ��ཻ
		/** ȱʡʵ�ַ���'false'��*/
		virtual bool Test (Real tmax, const TVector& velocity0,
			const TVector& velocity1);

		/// ��̬�����ཻ��
		/** ������ʵ�ִ˺������ṩ�����ཻ����ʵ�ַ�����*/
		virtual bool Find (Real tmax, const TVector& velocity0,
			const TVector& velocity1);

		/// ���ض�̬�ཻ���������һ���ཻ��ʱ�䣨�ڶ�̬�ཻ����У�
		Real GetContactTime () const;

		/// �ཻ��������Ϣ
		enum
		{
			IT_EMPTY,
			IT_POINT,
			IT_SEGMENT,
			IT_RAY,
			IT_LINE,
			IT_POLYGON,
			IT_PLANE,
			IT_POLYHEDRON,
			IT_OTHER
		};
		int GetIntersectionType () const;

	protected:
		Intersector ();

		int mIntersectionType;
		Real mContactTime;
	};

	typedef Intersector<float, Vector2<float> > Intersector2f;
	typedef Intersector<float, Vector3<float> > Intersector3f;
	typedef Intersector<double, Vector2<double> > Intersector2d;
	typedef Intersector<double, Vector3<double> > Intersector3d;

}

#endif