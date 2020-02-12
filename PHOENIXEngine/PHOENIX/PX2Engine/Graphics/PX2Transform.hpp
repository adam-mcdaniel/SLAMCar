// PX2Transform.hpp

#ifndef PX2TRANSFORM_HPP
#define PX2TRANSFORM_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2HMatrix.hpp"
#include "PX2Math.hpp"
#include "PX2Function.hpp"
#include "PX2APoint.hpp"

namespace PX2
{

	class FunObject;

	/// �任��
	/**
	* �任�����ʽ��Y = M*X+T��M��3x3�ľ���T��һ���任�������ھ����������£�
	* M = R��һ����ת����������M=R*S��R��һ����ת��������S��һ�����ĶԽ����ž�
	* ��MҲ�������κ�3x3��������X����ǰ�任��Y������任ʱ��Y�任��X��X =
	* M^{-1}*(Y-T)�����ض��������M = R*S������任��X = S^{-1}*R^t*(Y-T)��
	* S^{-1}����ĶԽǺ�S�ĶԽǻ�Ϊ������R^t��R��ת�þ��������任���ձ������
	* ����mHMatrix������任Ϊ{Y,1} = H*{X,1}������任Ϊ{X,1} = H^{-1}*{Y,1}��
	*/
	class PX2_ENGINE_ITEM Transform
	{
	public:
		/**
		* ��ʼΪ��λ�任��
		*/
		Transform ();
		~Transform ();

		/// ���任����Ϊ��λ�任��
		void MakeIdentity ();

		/// �����Ŵ���Ϊ��λ����
		void MakeUnitScale ();

		// �任��Ϣ
		inline bool IsIdentity () const;      // I
		inline bool IsRSMatrix () const;      // R*S
		inline bool IsUniformScale () const;  // R*S, S = c*I

		// ��Ա����
		/*
		*	(1) Set*�����ὫmIsIdentity��־��Ϊfalse��
		*	(2) SetRotate�����ὫmIsRSMatrix��־��Ϊtrue�����mIsRSMatrixΪfalse��
		*		��Debugģʽ�£�����GetRotate�����ᴥ��һ������ʧ�ܡ�
		*	(3) SetMatrix�����ὫmIsRSMatrix��mIsUniformScale��Ϊfalse��
		*	(4) SetScale�����ὫmIsUniformScale��־��Ϊfalse��
		*		SetUniformScale������mIsUniformScale��־��Ϊtrue�����mIsUniformScale
		*		Ϊfalse����Debugģʽ�£�����GetUniformScale�����ĵ��ûᴥ��һ������ʧ�ܡ�
		*	(5) ����Set*��������mInverseNeedsUpdateΪtrue����GetInverse����������ʱ��
		*		��������¼��㣬Ȼ��mInverseNeedsUpdate������Ϊfalse��
		*/
		void SetRotate (const HMatrix& rotate);
		void SetRotate(float x, float y, float z);
		void SetRotate(const APoint &rot);
		void SetRotateDegree(float xDegree, float yDegree, float zDegree);
		void SetRotateDegreeX(float degree);
		void SetRotateDegreeY(float degree);
		void SetRotateDegreeZ(float degree);
		void SetRotate(const AVector &r, const AVector &d, const AVector &u);
		void SetMatrix (const HMatrix& matrix);
		void SetTranslate (const APoint& translate);
		void SetTranslate (float x, float y, float z);
		void SetTranslateX (float x);
		void SetTranslateY (float y);
		void SetTranslateZ (float z);
		void SetTranslateXY(float x, float z);
		void SetTranslateXZ(float x, float z);
		void SetScale (const APoint& scale);
		void SetUniformScale (float scale);
		inline const HMatrix& GetRotate () const;
		void GetRotate(float &x, float &y, float &z) const;
		APoint GetRotateXYZ() const;
		float GetRotateX() const;
		float GetRotateY() const;
		float GetRotateZ() const;
		APoint GetRotateDegreeXYZ() const;
		float GetRotateDegreeX() const;
		float GetRotateDegreeY() const;
		float GetRotateDegreeZ() const;
		inline const HMatrix& GetMatrix () const;
		inline const APoint& GetTranslate () const;
		inline const APoint& GetScale () const;
		inline float GetUniformScale () const;

		void SetDU(const AVector &dir, const AVector &uping = AVector::UNIT_Z);
		AVector GetDirection() const;
		void GetRDUVector(AVector &r, AVector &d, AVector &u) const;

		// For M = R*S, the largest value of S in absolute value is returned.
		// For general M, the max-row-sum norm is returned, which is a reasonable
		// measure of maximum scale of the transformation.
		float GetNorm () const;

		// �Ե���б任mHMatrix*pnt
		inline APoint operator* (const APoint& pnt) const;

		// ���������б任mHMatrix*vec
		inline AVector operator* (const AVector& vec) const;

		// Matrix-matrix multiplication.
		Transform operator* (const Transform& transform) const;

		// ������մ��������ξ���
		inline const HMatrix& Matrix () const;

		/// ������մ��������ξ���������
		/**
		* �����Ҫ���¼���Ļ��������¼��㡣���H = {{M,T},{0,1}}����ôH^{-1} =
		* {{M^{-1},-M^{-1}*T},{0,1}}��
		*/
		const HMatrix& Inverse () const;

		/// ��÷���任
		Transform InverseTransform () const;

		// �ص�ֵ
		static const Transform IDENTITY;

		// �־û�֧��
		inline static int GetStreamingSize ();

	private:
		// ��mMatrix��mTranslate����mScale�иı�ʱ���������վ���mHMatrix��
		void UpdateHMatrix ();

		// ������������Ͻǵ�3x3�Ĳ��ֽ�������
		static void Invert3x3 (const HMatrix& mat, HMatrix& invMat);

		// ���մ������4x4��ξ���H��H = {{M,T},{0,1}}�����������H^{-1} = 
		// {M^{-1},-M^{-1}*T,{0,1}}�������mInvHMatrix��Inverse������ʱ�Żᱻ
		// ���á�
		HMatrix mHMatrix;
		mutable HMatrix mInvHMatrix;

		HMatrix mMatrix;     // M (general) or R (rotation)
		APoint mTranslate;   // T
		APoint mScale;       // S
		bool mIsIdentity, mIsRSMatrix, mIsUniformScale;
		mutable bool mInverseNeedsUpdate;

		// �־û�֧��
		friend class InStream;
		friend class OutStream;
	};

#include "PX2Transform.inl"

}

#endif