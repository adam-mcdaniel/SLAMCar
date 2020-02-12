// PX2Line3.hpp

#ifndef PX2LINE3_HPP
#define PX2LINE3_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Vector3.hpp"

namespace PX2
{

	/// ��Ԫģ����
	/** 
	* �ߵı�ʾ��ʽΪP+t*D��P���ߵ�ԭ�㣬D�ǵ�λ���ȵķ����������û�����ȷ��
	* ����������Щ������
	*/
	template <typename Real>
	class Line3
	{
	public:
		Line3 ();  //< δ��ʼ��
		~Line3 ();

		Line3 (const Vector3<Real>& origin, const Vector3<Real>& direction);

		Vector3<Real> Origin, Direction;
	};

#include "PX2Line3.inl"

	typedef Line3<float> Line3f;
	typedef Line3<double> Line3d;

}

#endif
