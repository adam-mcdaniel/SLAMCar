// PX2Vector3.hpp

#ifndef PX2VECTOR3_HPP
#define PX2VECTOR3_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Tuple.hpp"
#include "PX2Math.hpp"

namespace PX2
{

	template <typename Real>
	class Vector3 : public Tuple<3,Real>
	{
	public:
		Vector3 ();  //< δ��ʼ��
		Vector3 (const Vector3& vec);
		Vector3 (const Tuple<3,Real>& tuple);
		Vector3 (Real x, Real y, Real z);

		// ��ֵ
		Vector3& operator= (const Vector3& vec);
		Vector3& operator= (const Tuple<3,Real>& tuple);

		// ����
		inline Real X () const;
		inline Real& X ();
		inline Real Y () const;
		inline Real& Y ();
		inline Real Z () const;
		inline Real& Z ();

		// ����
		inline Vector3 operator+ (const Vector3& vec) const;
		inline Vector3 operator- (const Vector3& vec) const;
		inline Vector3 operator* (Real scalar) const;
		inline Vector3 operator/ (Real scalar) const;
		inline Vector3 operator- () const;

		// �������
		inline Vector3& operator+= (const Vector3& vec);
		inline Vector3& operator-= (const Vector3& vec);
		inline Vector3& operator*= (Real scalar);
		inline Vector3& operator/= (Real scalar);

		// ��������
		inline Real Length () const;
		inline Real SquaredLength () const;
		inline Real Dot (const Vector3& vec) const;
		inline Real Normalize (const Real epsilon = Math<Real>::ZERO_TOLERANCE);

		// ����㼯��axis-aligned bounding box
		static void ComputeExtremes (int numVectors, const Vector3* vectors,
			Vector3& vmin, Vector3& vmax);

		// ��˻��ڡ���������ϵ�����������Ҫ������������ϵ��������Ҫ�������
		// ����ÿһ��Ԫ��ȡ����ÿ��Ԫ��c��Ϊ-c����
		Vector3 Cross (const Vector3& vec) const;
		Vector3 UnitCross (const Vector3& vec) const; //< ��˺�λ��

		// Gram-Schmidt���������������������U��V��W�����ҽ����໥������(��λ
		// ���ȣ����ഹֱ)
		static void Orthonormalize (Vector3& u, Vector3& v, Vector3& w);
		static void Orthonormalize (Vector3* v);

		/// ���������W����ȱʡ��{U,V,W}
		/**
		* ����W�����Ƿ�0���������ʱ������������{U,V,W}����������W�ᱻ�������
		* ��λ�������W�Ѿ�����λ����ʹ��GenerateComplementBasis����U��V��
		*/
		static void GenerateOrthonormalBasis (Vector3& u, Vector3& v, Vector3& w);

		/// ���������W(W�����ǵ�λ����)����ȱʡ��{U,V,W}
		/**
		* ����W�����ǵ�λ���������ʱ������������{U,V,W}����������W�ᱻ�������
		* ��λ����
		*/
		static void GenerateComplementBasis (Vector3& u, Vector3& v,
			const Vector3& w);

		// Compute the barycentric coordinates of the point V with respect to the
		// tetrahedron <V0,V1,V2,V3>, V = b0*V0 + b1*V1 + b2*V2 + b3*V3, where
		// b0 + b1 + b2 + b3 = 1.
		void GetBarycentrics (const Vector3& v0, const Vector3& v1,
			const Vector3& v2, const Vector3& v3, Real bary[4]) const;

		struct Information
		{
			// The intrinsic dimension of the input set.  The parameter 'epsilon'
			// to the GetInformation function is used to provide a tolerance when
			// determining the dimension.
			int mDimension;

			// ���������Χ��ϵͳ
			// ���Χ��mMax[0]-mMin[0], mMax[1]-mMin[1],��
			// mMax[2]-mMin[2]�����ֵ��
			Real mMin[3], mMax[3];
			Real mMaxRange;

			// �����Χ��ϵͳ
			Vector3 mOrigin;
			Vector3 mDirection[3];

			// The indices that define the maximum dimensional extents.  The
			// values mExtreme[0] and mExtreme[1] are the indices for the points
			// that define the largest extent in one of the coordinate axis
			// directions.  If the dimension is 2, then mExtreme[2] is the index
			// for the point that generates the largest extent in the direction
			// perpendicular to the line through the points corresponding to
			// mExtreme[0] and mExtreme[1].  Furthermore, if the dimension is 3,
			// then mExtreme[3] is the index for the point that generates the
			// largest extent in the direction perpendicular to the triangle
			// defined by the other extreme points.  The tetrahedron formed by the
			// points V[extreme0], V[extreme1], V[extreme2], V[extreme3]> is
			// clockwise or counterclockwise, the condition stored in mExtremeCCW.
			int mExtreme[4];
			bool mExtremeCCW;
		};

		// ��õ㼯����Ϣ
		static void GetInformation (int numPoints, const Vector3* points,
			Real epsilon, Information& info);

		// ����ֵ
		PX2_ENGINE_ITEM static const Vector3 ZERO;    // (0,0,0)
		PX2_ENGINE_ITEM static const Vector3 UNIT_X;  // (1,0,0)
		PX2_ENGINE_ITEM static const Vector3 UNIT_Y;  // (0,1,0)
		PX2_ENGINE_ITEM static const Vector3 UNIT_Z;  // (0,0,1)
		PX2_ENGINE_ITEM static const Vector3 UNIT;     // (1,1,1)

	protected:
		using Tuple<3,Real>::mTuple;
	};

	// ����
	template <typename Real>
	inline Vector3<Real> operator* (Real scalar, const Vector3<Real>& vec);

	// Debug���
	template <typename Real>
	std::ostream& operator<< (std::ostream& outFile, const Vector3<Real>& vec);

#include "PX2Vector3.inl"

	typedef Vector3<float> Vector3f;
	typedef Vector3<double> Vector3d;

}

#endif