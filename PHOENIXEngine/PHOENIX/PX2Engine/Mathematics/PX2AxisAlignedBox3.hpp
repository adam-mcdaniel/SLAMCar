// PX2AxisAlignedBox3.hpp

#ifndef PX2AXISALIGNEDBOX3_HPP
#define PX2AXISALIGNEDBOX3_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Vector3.hpp"

namespace PX2
{

	/// ���������
	template <typename Real>
	class AxisAlignedBox3
	{
	public:
		AxisAlignedBox3 ();  //< δ��ʼ��
		~AxisAlignedBox3 ();

		/**
		* �����߱���ȷ��xmin <= xmax��ymin <= ymax������zmin <= zmax��
		*/
		AxisAlignedBox3 (Real xmin, Real xmax, Real ymin, Real ymax,
			Real zmin, Real zmax);
		AxisAlignedBox3 (const Vector3<Real> &min, const Vector3<Real> &max);

		/// ����������ĺ���չ�������ӳ��ȵ�һ�룩
		void GetCenterExtents (Vector3<Real>& center, Real extent[3]);

		// �ص����ཻ����⣨���߻��߹��涼�����ص����ཻ����
		bool HasXOverlap (const AxisAlignedBox3& box) const;
		bool HasYOverlap (const AxisAlignedBox3& box) const;
		bool HasZOverlap (const AxisAlignedBox3& box) const;
		bool TestIntersection (const AxisAlignedBox3& box) const;
		bool Contain (const AxisAlignedBox3& box) const;
		bool Contain(const Vector3f &pos);

		/// �ཻ���
		/**
		* ������������ཻ������'true'������������£��ཻ�Ĳ��ֱ��洢��
		* 'intersection'�С������ΧΪ'false'���������ཻ��'intersection'��һ��
		* δ����ֵ��
		*/
		bool FindIntersection (const AxisAlignedBox3& box,
			AxisAlignedBox3& intersection) const;

		Vector3<Real> GetMin();
		Vector3<Real> GetMax();

		Real Min[3], Max[3];
	};

#include "PX2AxisAlignedBox3.inl"

	typedef AxisAlignedBox3<float> AxisAlignedBox3f;
	typedef AxisAlignedBox3<double> AxisAlignedBox3d;

}

#endif