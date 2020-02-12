// PX2HQuaternion.hpp

#ifndef PX2HQUATERNION_HPP
#define PX2HQUATERNION_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Float4.hpp"
#include "PX2HMatrix.hpp"

namespace PX2
{

	/// ������Ԫ����
	/**
	* ��Ԫ���ı�����ʽΪq = w + x*i + y*j + z*k ��w,x,y,z��û�б�Ҫ�ǵ�λ������
	* ����
	*/
	class PX2_ENGINE_ITEM HQuaternion
	{
	public:
		HQuaternion ();  //< δ��ʼ��
		HQuaternion (float w, float x, float y, float z);
		HQuaternion (const HQuaternion& q);

		/// �����������ת������
		HQuaternion (const HMatrix& rot);

		/// ������ת�����ת�ǹ���
		HQuaternion (const AVector& axis, float angle);

		// �������ʣ�0 = w, 1 = x, 2 = y, 3 = z.
		inline operator const float* () const;
		inline operator float* ();
		inline float operator[] (int i) const;
		inline float& operator[] (int i);
		inline float W () const;
		inline float& W ();
		inline float X () const;
		inline float& X ();
		inline float Y () const;
		inline float& Y ();
		inline float Z () const;
		inline float& Z ();

		// ��ֵ
		HQuaternion& operator= (const HQuaternion& q);

		// �Ƚϣ�����֧��STL��������Ȳ�����
		bool operator== (const HQuaternion& q) const;
		bool operator!= (const HQuaternion& q) const;
		bool operator<  (const HQuaternion& q) const;
		bool operator<= (const HQuaternion& q) const;
		bool operator>  (const HQuaternion& q) const;
		bool operator>= (const HQuaternion& q) const;

		// ����
		HQuaternion operator+ (const HQuaternion& q) const;
		HQuaternion operator- (const HQuaternion& q) const;
		HQuaternion operator* (const HQuaternion& q) const;
		HQuaternion operator* (float scalar) const;
		HQuaternion operator/ (float scalar) const;
		HQuaternion operator- () const;

		PX2_ENGINE_ITEM
		friend HQuaternion operator* (float scalar, const HQuaternion& q)
		{
			return q*scalar;
		}

		// �������
		HQuaternion& operator+= (const HQuaternion& q);
		HQuaternion& operator-= (const HQuaternion& q);
		HQuaternion& operator*= (float scalar);
		HQuaternion& operator/= (float scalar);

		//  ��Ԫ��������axis-angle֮���ת��
		void FromRotationMatrix (const HMatrix& rot);
		void ToRotationMatrix (HMatrix& rot) const;
		void FromAxisAngle (const AVector& axis, float angle);
		void ToAxisAngle (AVector& axis, float& angle) const;

		// ��������
		float Length () const;  //< 4��Ԫ�صĳ���
		float SquaredLength () const;  //< 4��Ԫ�صĳ��ȵ�ƽ��
		float Dot (const HQuaternion& q) const;  //< 4��Ԫ�ص��֮��
		float Normalize (const float epsilon = Mathf::ZERO_TOLERANCE);
		HQuaternion Inverse () const;  // apply to non-zero quaternion
		HQuaternion Conjugate () const;  // negate x, y, and z terms
		HQuaternion Exp () const;  // apply to quaternion with w = 0
		HQuaternion Log () const;  // apply to unit-length quaternion

		/// ʹ����Ԫ����ת����
		AVector Rotate (const AVector& vec) const;

		/// ��Ԫ����ת��ֵ
		HQuaternion& Slerp (float t, const HQuaternion& p, const HQuaternion& q);

		// Intermediate terms for spherical quadratic interpolation.
		HQuaternion& Intermediate (const HQuaternion& q0, const HQuaternion& q1,
			const HQuaternion& q2);

		// Spherical quadratic interpolation.
		HQuaternion& Squad (float t, const HQuaternion& q0, const HQuaternion& a0,
			const HQuaternion& a1, const HQuaternion& q1);

		// �ض�ֵ
		static const HQuaternion ZERO;
		static const HQuaternion IDENTITY;

	private:
		// �洢ѭ��Ϊ(w,x,y,z).
		float mTuple[4];
	};

#include "PX2HQuaternion.inl"

}

#endif