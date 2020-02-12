// PX2Circle3.hpp

#ifndef PX2CIRCLE3_HPP
#define PX2CIRCLE3_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Vector3.hpp"

namespace PX2
{

	/// 3dԲ��
	/**
	* ����Բ��ƽ��ΪDot(N,X-C) = 0��XΪƽ���ϵ�һ�㡣U��V��N����������ϵ�µ���
	* ������ϵ�������ƽ���ϵ�Բ�ɺ���X = C + R*(cos(t)*U + sin(t)*V)��ʾ��t��
	* ��ΧΪ[-pi,pi)��
	*/
	template <typename Real>
	class Circle3
	{
	public:
		Circle3 ();  //< δ��ʼ��
		~Circle3 ();

		/**
		* U = direction0��V = direction1��N = normal
		*/
		Circle3 (const Vector3<Real>& center, const Vector3<Real>& direction0,
			const Vector3<Real>& direction1, const Vector3<Real>& normal,
			const Real radius);

		Vector3<Real> Center, Direction0, Direction1, Normal;
		Real Radius;
	};

#include "PX2Circle3.inl"

	typedef Circle3<float> Circle3f;
	typedef Circle3<double> Circle3d;

}

#endif