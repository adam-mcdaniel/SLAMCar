// PX2ContSphere3.hpp

#ifndef PX2CONTSPHERE3_HPP
#define PX2CONTSPHERE3_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Sphere3.hpp"

namespace PX2
{

	// ���ȼ���������AABB��Χ�У�Ȼ�����AABB�����ΰ�Χ��
	template <typename Real> PX2_ENGINE_ITEM
		Sphere3<Real> ContSphereOfAABB (int numPoints, const Vector3<Real>* points);

	// �����������С��Χ��
	template <typename Real> PX2_ENGINE_ITEM
		Sphere3<Real> ContSphereAverage (int numPoints, const Vector3<Real>* points);

	// �����Ƿ�������
	template <typename Real> PX2_ENGINE_ITEM
		bool InSphere (const Vector3<Real>& point, const Sphere3<Real>& sphere);

	// �������ں�
	template <typename Real> PX2_ENGINE_ITEM
		Sphere3<Real> MergeSpheres (const Sphere3<Real>& sphere0,
		const Sphere3<Real>& sphere1);

}

#endif
