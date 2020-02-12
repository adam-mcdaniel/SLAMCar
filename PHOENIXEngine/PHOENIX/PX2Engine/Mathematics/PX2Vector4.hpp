// PX2Vector4.hpp

#ifndef PX2VECTOR4_HPP
#define PX2VECTOR4_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Tuple.hpp"
#include "PX2Math.hpp"

namespace PX2
{

	template <typename Real>
	class Vector4 : public Tuple<4,Real>
	{
	public:
		Vector4 ();  //< δ��ʼ��
		Vector4 (const Vector4& vec);
		Vector4 (const Tuple<4,Real>& tuple);
		Vector4 (Real x, Real y, Real z, Real w);

		// ��ֵ
		Vector4& operator= (const Vector4& vec);
		Vector4& operator= (const Tuple<4,Real>& tuple);

		// ����
		inline Real X () const;
		inline Real& X ();
		inline Real Y () const;
		inline Real& Y ();
		inline Real Z () const;
		inline Real& Z ();
		inline Real W () const;
		inline Real& W ();

		// ����
		inline Vector4 operator+ (const Vector4& vec) const;
		inline Vector4 operator- (const Vector4& vec) const;
		inline Vector4 operator* (Real scalar) const;
		inline Vector4 operator/ (Real scalar) const;
		inline Vector4 operator- () const;

		// �������
		inline Vector4& operator+= (const Vector4& vec);
		inline Vector4& operator-= (const Vector4& vec);
		inline Vector4& operator*= (Real scalar);
		inline Vector4& operator/= (Real scalar);

		// ��������
		inline Real Length () const;
		inline Real SquaredLength () const;
		inline Real Dot (const Vector4& vec) const;
		inline Real Normalize (const Real epsilon = Math<Real>::ZERO_TOLERANCE);

		// Compute the axis-aligned bounding box of the points.
		static void ComputeExtremes (int numVectors, const Vector4* vectors,
			Vector4& vmin, Vector4& vmax);

		// �ض�ֵ
		PX2_ENGINE_ITEM static const Vector4 ZERO;
		PX2_ENGINE_ITEM static const Vector4 UNIT_X;  //< (1,0,0,0)
		PX2_ENGINE_ITEM static const Vector4 UNIT_Y;  //< (0,1,0,0)
		PX2_ENGINE_ITEM static const Vector4 UNIT_Z;  //< (0,0,1,0)
		PX2_ENGINE_ITEM static const Vector4 UNIT_W;  //< (0,0,0,1)
		PX2_ENGINE_ITEM static const Vector4 UNIT;     //< (1,1,1,1)

	protected:
		using Tuple<4,Real>::mTuple;
	};

	// �������
	template <typename Real>
	inline Vector4<Real> operator* (Real scalar, const Vector4<Real>& vec);

	// Debug���
	template <typename Real>
	std::ostream& operator<< (std::ostream& outFile, const Vector4<Real>& vec);

#include "PX2Vector4.inl"

	typedef Vector4<float> Vector4f;
	typedef Vector4<double> Vector4d;

}

#endif