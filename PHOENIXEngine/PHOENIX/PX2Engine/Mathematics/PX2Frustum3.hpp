// PX2Frustum3.hpp

#ifndef PX2FRUSTUM3_HPP
#define PX2FRUSTUM3_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Vector3.hpp"

namespace PX2
{

	/// ������ͷ��ģ����
	/** 
	* E�������ԭ�㣬D�Ƿ���������U����������R������������u( > 0 )��r�� > 0����
	* ΪU��R�������չ����n��f��ΪD�������չ0 < n < f����ͷ����ü�����ĸ���
	* ΪE + n*D + s0*u*U + s1*r*R��|s0| = |s1| = 1������ͷ��Զ�ü�����ĸ���Ϊ
	* E + f*D + (f/n)*(s0*u*U + s1*r*R)��|s0| = |s1| = 1����
	*/
	template <typename Real>
	class Frustum3
	{
	public:
		Frustum3 ();  //< δ��ʼ��
		~Frustum3 ();

		Frustum3 (const Vector3<Real>& origin, const Vector3<Real>& dvector,
			const Vector3<Real>& uvector, const Vector3<Real>& rvector,
			Real dmin, Real dmax, Real ubound, Real rbound);

		/**
		* ��DMin, DMac, UBound, ���� RBound�����κα仯ʱ��Update()������á�
		* DRatio��MTwoUF��MTwoRF������Щ�ı䣬�����ڵ���Update()��ŵ���
		* Get*()��
		*/
		void Update ();
		inline Real GetDRatio () const;
		inline Real GetMTwoUF () const;
		inline Real GetMTwoRF () const;

		void ComputeVertices (Vector3<Real> vertex[8]) const;

		Vector3<Real> Origin, DVector, UVector, RVector;
		Real DMin, DMax, UBound, RBound;

	protected:
		Real mDRatio, mMTwoUF, mMTwoRF;
	};

#include "PX2Frustum3.inl"

	typedef Frustum3<float> Frustum3f;
	typedef Frustum3<double> Frustum3d;

}

#endif