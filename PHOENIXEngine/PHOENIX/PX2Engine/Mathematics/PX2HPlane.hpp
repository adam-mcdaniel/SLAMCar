// PX2HPlane.hpp

#ifndef PX2HPLANE_HPP
#define PX2HPLANE_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2APoint.hpp"

namespace PX2
{

	/// ���ƽ����
	/**
	* homogeneous plane��ƽ��ı�����ʽΪDot(N,X) - C = 0, N = (n0,n1,n2,0)��һ
	* ����λ���ȵķ���������C��ƽ�泣����X = (x0,x1,x2,1)��ƽ���ϵ�����һ�㡣��
	* ����ȷ�����������ǵ�λ���ȡ������洢����Ԫ��Ϊ(n0,n1,n2,-c)��
	*/
	class PX2_ENGINE_ITEM HPlane
	{
	public:
		HPlane ();  //< δ��ʼ��
		HPlane (const HPlane& plane);
		~HPlane ();

		// ֱ�����ⷨ�������ͳ���C
		HPlane (float normal0, float normal1, float normal2, float constant);
		HPlane (const AVector& normal, float constant);

		/// ���뷨�ߺ�ƽ����һ��
		/**
		* ������ΪDot(N,X)��
		*/
		HPlane (const AVector& normal, const APoint& p);

		/// ����ƽ����������P0,P1,P2����
		/** 
		* N = Cross(P1-P0,P2-P0)/Length(Cross(P1-P0,P2-P0)), C = Dot(N,P0)
		*/
		HPlane (const APoint& p0, const APoint& p1, const APoint& p2);

		/// ��������Ԫ��(n0,n1,n2,-c)����ƽ�档
		HPlane (const HPoint& tuple);

		// ��ʽת����HPoint
		inline operator HPoint ();
		inline operator HPoint () const;

		// ����
		inline operator const float* () const;
		inline operator float* ();
		inline float operator[] (int i) const;
		inline float& operator[] (int i);

		// ��ֵ
		HPlane& operator= (const HPlane& plane);

		// �Ƚ� (STL��������֧��)
		bool operator== (const HPlane& plane) const;
		bool operator!= (const HPlane& plane) const;
		bool operator<  (const HPlane& plane) const;
		bool operator<= (const HPlane& plane) const;
		bool operator>  (const HPlane& plane) const;
		bool operator>= (const HPlane& plane) const;

		// ��Ա
		inline void SetNormal (const AVector& normal);
		inline void SetConstant (float constant);
		inline AVector GetNormal () const;
		inline float GetConstant () const;

		// ����L = Length(n0,n1,n2)��������ƽ�浽(n0,n1,n2,-c)/L��
		float Normalize (const float epsilon = Mathf::ZERO_TOLERANCE);

		/// ����㵽ƽ��ľ���
		/**
		* D = Dot(N,Q)-C,N��ƽ��ķ��ߣ�C��ƽ�泣����D�Ǵ����ŵľ��룬DΪ������
		* ������ƽ������棻Ϊ������������ƽ��ĸ��棻Ϊ0����������ƽ���ϡ�
		*/
		float DistanceTo (const APoint& p) const;

		/// �жϵ���ƽ�����һ��
		/** 
		* ����ָ�����Ϊ����,��һ��Ϊ���档�����溯������+1�����淵��-1��0��ʾ
		* ����ƽ���ϡ�
		*/
		int WhichSide (const APoint& p) const;

	private:
		// �洢����Ԫ��(n0,n1,n2,-c).
		HPoint mTuple;
	};

#include "PX2HPlane.inl"

}

#endif