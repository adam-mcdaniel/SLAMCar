// PX2Camera.hpp

#ifndef PX2CAMERA_HPP
#define PX2CAMERA_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"
#include "PX2HMatrix.hpp"
#include "PX2Float4.hpp"
#include "PX2Size.hpp"
#include "PX2APoint.hpp"
#include "PX2Vector2.hpp"
#include "PX2Rect.hpp"

namespace PX2
{
	class CameraNode;
	class Renderer;

	typedef void(*CameraDrawCallback) (Renderer *renderer);

	/// �����
	class PX2_ENGINE_ITEM Camera : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(Camera);

	public:
		/**
		* ȱʡ�����field-of-viewΪ90��aspect ratioΪ1��near valueΪ1��far		
		* valueΪ10000��
		*/
		Camera (bool isPerspective = true);
		virtual ~Camera ();

		void SetCameraNode(CameraNode *camNode);
		CameraNode *GetCameraNode();

		// �����������������ϵ
		//   default position  P = (0, 0,  0; 1)
		//   default direction D = (0, 0, -1; 0)
		//   default up        U = (0, 1,  0; 0)
		//   default right     R = (1, 0,  0; 0)
		void SetFrame (const APoint& position, const AVector& dVector,
			const AVector& uVector, const AVector& rVector);

		void SetPosition (const APoint& position);

		void SetAxes (const AVector& dVector, const AVector& uVector,
			const AVector& rVector);

		inline APoint GetPosition () const;
		inline AVector GetDVector () const;
		inline AVector GetUVector () const;
		inline AVector GetRVector () const;

		/// ����������
		/**
		* ���D = (d0,d1,d2)��U = (u0,u1,u2)��R = (r0,r1,r2)���������
		*	+-                     -+
		*	| r0  r1  r2  -Dot(R,P) |
		*	| u0  u1  u2  -Dot(U,P) |
		*	| d0  d1  d2  -Dot(D,P) |
		*	|  0   0   0          1 |
		*	+-                     -+
		* �������������viewMat*vector4
		*/
		inline const HMatrix& GetViewMatrix () const;

		/// ��ͷ��ƥ����Ϣ
		/**
		* ��ͷ�������[rmin,rmax], [umin,umax]����[dmin,dmax]������[rmin,rmax]
		* ����R�����Ͻ��еĲ�������¼��left��right��Ϣ��[umin,umax]����U������
		* ���еĲ���,��¼��bottom��top��Ϣ��[dmin,dmax]����D�����Ͻ��еĲ�����
		* ��¼��near��far��Ϣ����ͷ����Ϣ�������һ������������������Ϣ����
		* ƥ�䡣
		*/
		enum ViewFrustum
		{
			VF_DMIN     = 0,  //< near
			VF_DMAX     = 1,  //< far
			VF_UMIN     = 2,  //< bottom
			VF_UMAX     = 3,  //< top
			VF_RMIN     = 4,  //< left
			VF_RMAX     = 5,  //< right
			VF_QUANTITY = 6
		};

		void SetPerspective (bool per);
		inline bool IsPerspective () const;

		/// ���ý�ͷ��
		void SetFrustum (float dMin, float dMax, float uMin, float uMax,
			float rMin, float rMax);

		/// ���ý�ͷ��
		/**
		* ��ͷ������Ĵ��˳�����£�dmin, dmax, umin, umax, rmin, rmax��
		*/
		void SetFrustum (const float* frustum);

		/// ���öԳƵĽ�ͷ��
		/**
		* ����һ���ԳƵĽ�ͷ��(umin = -umax��rmin = -rmax)ʹ��һ�����·���Ľ�
		* ��fUpFovDegrees��fAspectRatio(width/height)��fUpFovDegrees�ķ�Χ����
		* ������(0,180)�С�
		*/
		void SetFrustum (float upFovDegrees, float aspectRatio, float dMin,
			float dMax);

		/// ��������ͷ��
		void GetFrustum (float& dMin, float& dMax, float& uMin, float& uMax,
			float& rMin, float& rMax) const;

		/// ��������ͷ������
		inline const float* GetFrustum () const;

		/// ��ý�ͷ����Ϣ
		/**
		* �����������ǣ���õĲ�������Ч��
		*/
		bool GetFrustum (float& upFovDegrees, float& aspectRatio, float& dMin,
			float& dMax) const;

		// ��õ����������ͷ����
		inline float GetDMin () const;
		inline float GetDMax () const;
		inline float GetUMin () const;
		inline float GetUMax () const;
		inline float GetRMin () const;
		inline float GetRMax () const;

		// ���͸�Ӿ�������
		/**
		* ��Direct3D�У���Ⱦ������ȱ�ƥ�䵽[0,1]��OpenGL�У���Ⱦ������ȱ�
		* ƥ�䵽[-1,1]��
		*/
		enum DepthType
		{
			PM_DEPTH_ZERO_TO_ONE,       // [0,1]
			PM_DEPTH_MINUS_ONE_TO_ONE,  // [-1,1]
			PM_DEPTH_QUANTITY
		};

		inline DepthType GetDepthType () const;

		/// ��������͸�Ӿ���
		/**
		* ͸�Ӿ���������msDepthType��mIsPerspective�������ҳ�����projMat*vector4��
		* ��ͷ���ֵΪ��N (near), F (far), B (bottom), T (top), L (left), ��
		* R (right)������͸�Ӿ������ʽ���£�
		*	perspective, depth [0,1]
		*	+-                                               -+
		*	| 2*N/(R-L)  0           -(R+L)/(R-L)  0          |
		*	| 0          2*N/(T-B)   -(T+B)/(T-B)  0          |
		*	| 0          0           F/(F-N)       -N*F/(F-N) |
		*	| 0          0           1             0          |
		*	+-                                               -+
		*
		*	perspective, depth [-1,1]
		*	+-                                                 -+
		*	| 2*N/(R-L)  0           -(R+L)/(R-L)  0            |
		*	| 0          2*N/(T-B)   -(T+B)/(T-B)  0            |
		*	| 0          0           (F+N)/(F-N)   -2*F*N/(F-N) |
		*	| 0          0           1             0			|
		*	+-                                                 -+
		*
		*	orthographic, depth [0,1]
		*	+-                                       -+
		*	| 2/(R-L)  0  0              -(R+L)/(R-L) |
		*	| 0        2/(T-B)  0        -(T+B)/(T-B) |
		*	| 0        0        1/(F-N)  -N/(F-N)  0  |
		*	| 0        0        0        1            |
		*	+-                                       -+
		*
		*	orthographic, depth [-1,1]
		*	+-                                       -+
		*	| 2/(R-L)  0        0        -(R+L)/(R-L) |
		*	| 0        2/(T-B)  0        -(T+B)/(T-B) |
		*	| 0        0        2/(F-N)  -(F+N)/(F-N) |
		*	| 0        0        0        1            |
		*	+-                                       -+
		*/
		inline const HMatrix& GetProjectionMatrix () const;

		// ���projection-view����
		/**
		* Ϊ�˱����ظ����㣬���ǽ�projection-view�洢��������
		*/
		inline const HMatrix& GetProjectionViewMatrix () const;

		/// ����preview matrix
		/**
		* ���������model-to-world֮��view matrix֮ǰ�������á������������
		* �������䣬ȱʡֵ��һ����λ����
		*/
		void SetPreViewMatrix (const HMatrix& preViewMatrix);
		inline const HMatrix& GetPreViewMatrix () const;
		inline bool PreViewIsIdentity () const;

		/// ����postprojection matrix
		/**
		* �������������screen-space�任������rendered image�ı任��ȱʡֵ��һ
		* ����λ����
		*/
		void SetPostProjectionMatrix (const HMatrix& postProjMatrix);
		inline const HMatrix& GetPostProjectionMatrix () const;
		inline bool PostProjectionIsIdentity () const;

		bool GetPickRay(float sizePosX, float sizePosZ, const Sizef &size,
			APoint& origin, AVector& direction);

		bool GetPickRay(float posSizePercentWdith, float posSizePercentHeight,
			APoint& origin, AVector& direction);

		Vector2f WorldPos3DTo2D(const Rectf& viewPort, const APoint &screenPos, 
			bool *isInBack=0);
		Vector2f WorldDir3DTo2D(const AVector &worldDir);

		void SetCameraDrawCallback(CameraDrawCallback callback);
		CameraDrawCallback GetCameraDrawCallback();

	protected:
		// ����ı�����¼���ȡ������
		void OnFrameChange ();

		// �ڽ�ͷ��ı�֮�󣬼���͸�Ӿ���
		void OnFrustumChange ();

		// Compute the product postproj-proj-view-preview.
		void UpdatePVMatrix ();

		CameraNode *mCameraNode;

		// The world coordinate frame.
		APoint mPosition;
		AVector mDVector, mUVector, mRVector;

		// �����view matrix��
		HMatrix mViewMatrix;

		// �����ͷ�����洢�ڴ������� dmin (near), dmax (far), umin (bottom), 
		// umax (top), rmin (left), and rmax (right).
		float mFrustum[VF_QUANTITY];

		// ���͸�Ӿ���
		HMatrix mProjectionMatrix[PM_DEPTH_QUANTITY];

		// ͸�Ӿ������ȡ������
		HMatrix mProjectionViewMatrix[PM_DEPTH_QUANTITY];

		// preview matrix
		HMatrix mPreViewMatrix;
		bool mPreViewIsIdentity;

		// �����postprojection matrix
		HMatrix mPostProjectionMatrix;
		bool mPostProjectionIsIdentity;

		/// Ϊ'true'��ʾһ��͸��ͶӰ�����Ϊ'false'��ʾһ������ͶӰ�����
		bool mIsPerspective;

		// ���ͶӰ�������͡����ֵ�ڹ��캯������ʱ������ʼ��Ϊ
		// msDefaultDepthType�����ֵ����Ҫ�־û����Ӵ���������Projector�Ḳ��
		// ���ֵ�����ҽ���־û���
		DepthType mDepthType;

		CameraDrawCallback mCameraDrawCallback;

		// ȱʡͶӰ�������ͣ���DirectX��OpenGL��ͬ�������ֵ�ڹ��캯����������ʼ
		// ��mDepthType��
		static DepthType msDefaultDepthType;

public_internal:
		inline static void SetDefaultDepthType (DepthType type);
		inline static DepthType GetDefaultDepthType ();

		// ���ԭ������PX2GraphicsPre.hpp��
private_internal:
#ifdef PX2_VALIDATE_CAMERA_FRAME_ONCE
		bool mValidateCameraFrame;
#endif

		// Render
	public:
		void SetClearColor(const Float4 &color);
		const Float4 &GetClearColor() const;

		void SetClearDepth(float depth);
		float GetClearDepth() const;

		void SetClearStencil(unsigned int stencil);
		unsigned int GetClearStencil() const;

		void SetClearFlag(bool color, bool depth, bool stencil);
		void GetClearFlag(bool &color, bool &depth, bool &stencil);

	protected:
		Float4 mClearColor;
		float mClearDepth;
		unsigned int mClearStencil;
		bool mClearFlagColor;
		bool mClearFlagDepth;
		bool mClearFlagStencil;
	};

	PX2_REGISTER_STREAM(Camera);
	typedef Pointer0<Camera> CameraPtr;
#include "PX2Camera.inl"

}

#endif
