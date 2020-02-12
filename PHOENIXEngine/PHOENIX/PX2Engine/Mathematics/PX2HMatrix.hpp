// PX2HMatrix.hpp

#ifndef PX2HMATRIX_HPP
#define PX2HMATRIX_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2APoint.hpp"
#include "PX2Matrix3.hpp"

namespace PX2
{

	/// ��ξ�����
	/**
	* homogeneous matrix
	*/
	class PX2_ENGINE_ITEM HMatrix
	{
	public:
		HMatrix ();  //< δ��ʼ��
		HMatrix (const HMatrix& mat);
		HMatrix (const Matrix3f& mat);

		/// ���makeZeroΪ'true'������0���󣬷��򴴽���λ����
		HMatrix (bool makeZero);

		/// �����mrc��r�����У�c�����С�
		HMatrix (
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);

		/// ������������鴴��һ������
		/**
		* @param rowMajor
		*	true:  entry[0..15]={m00,m01,m02,m03,m10,m11,m12,m13,m20,m21,m22,
		*                       m23,m30,m31,m32,m33} [row major]
		*	false: entry[0..15]={m00,m10,m20,m30,m01,m11,m21,m31,m02,m12,m22,
		*                       m32,m03,m13,m23,m33} [col major]
		*						m32,m03,m13,m23,m33} [col major]
		*/
		HMatrix (const float* entry, bool rowMajor);

		/// ���ݵ�Ԫ�鴴������
		/**
		* @param columns
		*	true : �����Ԫ���Ǿ������
		*	false �������Ԫ���Ǿ������ 
		* r d u
		*/
		HMatrix (const float* tuple0, const float* tuple1, const float* tuple2,
			const float* tuple3, bool columns);

		/// �����ԽǾ���
		HMatrix (float m00, float m11, float m22);

		/// ������ת����
		/**
		* �������ĽǶ�Ϊ������ת����Ϊ��ʱ�롣�Ƕȱ����ǻ����ơ�
		*/
		HMatrix (const AVector& axis, float angle);

		~HMatrix ();

		/// ��ʽת����3x3�Ĳ��ֱ�������Matrix3f�����С�
		operator Matrix3f () const;

		// ����
		inline operator const float* () const;
		inline operator float* ();
		inline const float* operator[] (int row) const;
		inline float* operator[] (int row);
		inline float operator() (int row, int column) const;
		inline float& operator() (int row, int column);
		void SetRow (int row, const HPoint& hpnt);
		void GetRow (int row, HPoint& hpnt) const;
		// right, dir, up, AVector(0,0,0,1)
		void SetColumn (int column, const HPoint& hpnt);
		void GetColumn (int column, HPoint& hpnt) const;

		// ����ʽ���������������еģ�����ת��Ϊ�������С�����columnMajor����ӵ
		// ��16��Ԫ�صĿռ䡣
		void GetColumnMajor (float* columnMajor) const;

		// ��ֵ
		HMatrix& operator= (const HMatrix& mat);
		HMatrix& operator= (const Matrix3f& mat);

		// �Ƚ�
		bool operator== (const HMatrix& mat) const;
		bool operator!= (const HMatrix& mat) const;
		bool operator<  (const HMatrix& mat) const;
		bool operator<= (const HMatrix& mat) const;
		bool operator>  (const HMatrix& mat) const;
		bool operator>= (const HMatrix& mat) const;

		// ����
		HMatrix operator+ (const HMatrix& mat) const;
		HMatrix operator- (const HMatrix& mat) const;
		HMatrix operator* (float scalar) const;
		HMatrix operator/ (float scalar) const;
		HMatrix operator- () const;

		PX2_ENGINE_ITEM
		friend HMatrix operator* (float scalar, const HMatrix& mat);

		// �������
		HMatrix& operator+= (const HMatrix& mat);
		HMatrix& operator-= (const HMatrix& mat);
		HMatrix& operator*= (float scalar);
		HMatrix& operator/= (float scalar);

		// �����������
		HMatrix &MakeZero ();  //< Z
		HMatrix &MakeIdentity ();  //< I
		HMatrix &MakeDiagonal (float m00, float m11, float m22);  //< D
		HMatrix &MakeRotation (const AVector& axis, float angle);  //< R
		HMatrix Transpose () const;  //< M^T
		HMatrix Inverse (const float epsilon = 0.0f) const;  //< M^{-1}
		HMatrix Adjoint () const;  //< M^{adj}
		float Determinant () const;  //< det(M)
		HMatrix operator* (const HMatrix& mat) const;  //< M*mat
		HMatrix TransposeTimes (const HMatrix& mat) const;  //< M^T*mat
		HMatrix TimesTranspose (const HMatrix& mat) const;  //< M*mat^T
		HMatrix TransposeTimesTranspose (const HMatrix& mat) const;  //< M^T*mat^T
		HMatrix TimesDiagonal (const APoint& diag) const;  //< M*D
		HMatrix DiagonalTimes (const APoint& diag) const;  //< D*M
		void Orthonormalize (); //< �����3x3�����Ͻǲ���Ӧ�ô˺���

		// �������ε�֮��Ĳ���
		/*
		* M��P������εģ�M����Ҫʱ�������pҲ����Ҫw=1����
		*/
		HPoint operator* (const HPoint& p) const;  // M*p

		PX2_ENGINE_ITEM
		friend HPoint operator* (const HPoint& p, const HMatrix& mat);  // p*M

		void BatchMultiply (int numPoints, const HPoint* input,
			HPoint* output) const;  // M*p[0], ..., M*p[n-1]

		// �������ͷ����֮��Ĳ���
		APoint operator* (const APoint& p) const;  // M*p
		void BatchMultiply (int numPoints, const APoint* input,
			APoint* output) const;  // M*p[0], ..., M*p[n-1]

		// �������ͷ�������֮��Ĳ���
		AVector operator* (const AVector& p) const;  // M*v
		void BatchMultiply (int numPoints, const AVector* input,
			AVector* output) const;  // M*v[0], ..., M*v[n-1]

		// Compute a quadratic forms.
		float QForm (const HPoint& p0, const HPoint& p1) const;  // p0^T*M*p1

		// Set the transformation to an oblique projection matrix onto a
		// specified plane.  The plane has an 'origin' point and a unit-length
		// 'normal'.
		void MakeObliqueProjection (const APoint& origin, const AVector& normal,
			const AVector& direction);

		/// ����͸�Ӿ���
		/**
		* ����͸�ӵ��ض�ƽ��ľ���ƽ����ƽ��ԭ�㣬��λ���ȷ��ߣ�eyeΪ͸��ԭ�㡣
		*/
		void MakePerspectiveProjection (const APoint& origin,
			const AVector& normal, const APoint& eye);

		/// ���㷴�����
		/**
		* origin�Ƿ������ϵ�һ���㣬normal��һ����������
		*/
		void MakeReflection (const APoint& origin, const AVector& normal);

		// ����ֵ
		static const HMatrix ZERO;
		static const HMatrix IDENTITY;

	private:
		// ��������������˳��洢
		float mEntry[16];
	};

#include "PX2HMatrix.inl"

}

#endif