// PX2Renderable.hpp

#ifndef PX2RENDERABLE_HPP
#define PX2RENDERABLE_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Movable.hpp"
#include "PX2IndexBuffer.hpp"
#include "PX2VertexBuffer.hpp"
#include "PX2VertexFormat.hpp"
#include "PX2MaterialInstance.hpp"
#include "PX2Light.hpp"
#include "PX2DynamicBufferObject.hpp"
#include "PX2Shine.hpp"
#include "PX2Texture2D.hpp"

namespace PX2
{

	class Renderer;

	typedef void(*OnDrawCallback) (Renderer *renderer, Renderable *renderable);

	/// ��Ⱦ������
	/**
	* ������Ⱦ���󶼴Ӵ���������
	*/
	class PX2_ENGINE_ITEM Renderable : public Movable
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(Renderable);

	public:
		enum PrimitiveType
		{
			PT_NONE,  //< default constructor
			PT_POLYPOINT,
			PT_POLYSEGMENTS_DISJOINT,
			PT_POLYSEGMENTS_CONTIGUOUS,
			PT_TRIANGLES,  //< abstract
			PT_TRIMESH,
			PT_TRISTRIP,
			PT_TRIFAN,
			PT_MAX_QUANTITY
		};

	protected:
		/**
		* �������
		*/
		Renderable (PrimitiveType type = PT_NONE);
		Renderable (PrimitiveType type, VertexFormat* vformat, VertexBuffer* vbuffer,
			IndexBuffer* ibuffer);
	public:
		virtual ~Renderable ();

		inline PrimitiveType GetPrimitiveType () const;

		// vb ib
		inline void SetVertexFormat (VertexFormat* vformat);
		inline const VertexFormat* GetVertexFormat () const;
		inline VertexFormat* GetVertexFormat ();
		inline void SetVertexBuffer (VertexBuffer* vbuffer);
		inline const VertexBuffer* GetVertexBuffer () const;
		inline VertexBuffer* GetVertexBuffer ();
		inline void SetIndexBuffer (IndexBuffer* ibuffer);
		inline const IndexBuffer* GetIndexBuffer () const;
		inline IndexBuffer* GetIndexBuffer ();
		inline void SetIBOffset (int offset);
		inline int GetIBOffset () const;
		inline void SetIBNumElements (int num);
		inline int GetIBNumElements () const;

		// share vb ib
		inline void SetShareDBObject_V (DBObject_V *obj);
		inline const DBObject_V *GetShareDBObject_V () const;
		inline DBObject_V *GetShareDBObject_V ();
		inline void SetShareDBObject_I (DBObject_I *obj);
		inline const DBObject_I *GetShareDBObject_I () const;
		inline DBObject_I *GetShareDBObject_I ();
		void SetUseShareBuffers (bool use);
		bool IsUseShareBuffers () const;

		// bound
		inline const Bound& GetModelBound () const;
		inline Bound& GetModelBound ();
		void SetUseBoundPick (bool isUseBoundPick);
		bool IsUseBoundPick () const;

		/// ��Ⱦ�㼶
		enum RenderLayer
		{
			RL_BACKGROUND = 0,
			RL_SKY,
			RL_TERRAIN,
			RL_SCENE,
			RL_UIMASK,
			RL_UI,
			RL_MAX
		};
		void SetRenderLayer (RenderLayer layer, int sublayer=0);
		RenderLayer GetRenderLayer () const;
		int GetSubLayer () const;

		unsigned int GetSortIndex () const;

		// ���Ƽ���ͼ�ε�Ч��ʵ��
		inline void SetMaterialInstance (MaterialInstance* material);
		inline MaterialInstance* GetMaterialInstance () const;

		void SetFakeTransparent(bool fakeTrasparent);
		bool IsFakeTransparent() const;

		// �ƹ�
		/*
		* ��ǰ��Ⱦ���յ�Ӱ��ĵƹ�
		*/
		bool AddLight (Light *light);
		bool RemoveLight (Light *light);		
		void ClearLights ();
		int GetNumLights () const;
		Light *GetLight (int i);
		const Light *GetLight(int i) const;

		// Shine
		void SetShine (Shine *shine);
		Shine *GetShine ();
		const Shine *GetShine () const;

		Shine *GetBakeShine ();
		const Shine *GetBakeShine () const;

		void SetLightTexture (Texture2D *tex);
		const std::string &GetLightTexture () const;
		const std::string &GetNormalTexture () const;

		void SetBakeObject (bool bakeObject);
		bool IsBakeObject () const;
		void SetBakeTarget (bool bakeTarget);
		bool IsBakeTarget () const;
		
		// Bake
		enum BakeSizeType
		{
			BST_NORMAL,
			BST_64,
			BST_128,
			BST_256, 
			BST_512,
			BST_1024,
			BST_MAX_TYPE
		};
		void SetBakeSizeType (BakeSizeType type);
		BakeSizeType GetBakeSizeType () const;
		
		void SetUseLightTexture (bool light, Texture2D *lightTex);
		bool IsUseLightTexture () const;

		virtual void SetCastShadow(bool castShadow);
		virtual void SetReceiveShadow(bool reciveShadow);

		void SetFogInfulenceParam_Height (float param);
		float GetFogInfulenceParam_Height () const;

		void SetFogInfulenceParam_Distance (float param);
		float GetFogInfulenceParam_Distance () const;

		// ����ͼ�θ�������
		enum UpdateType
		{
			GU_MODEL_BOUND_ONLY = -3,
			GU_NORMALS = -2,
			GU_USE_GEOMETRY = -1,
			GU_USE_TCOORD_CHANNEL = 0
		};

		/// ���Ӷ���ģ�͸���
		/**
		* ������Ӷ����VertexBuffer���޸��ˣ�����Ҫ���£�normals,tangents,
		* bitangents���������type�������µĹ������£�
		* GU_MODEL_BOUND_ONLY
		*	����Ŀռ�λ�øı��ˣ�ֻ���°�Χ�С�
		* �����������£���Χ��Ҳ������¡�������Ӷ����Ǵ�Triangles�����Ļ���
		* ����з�����Ϣ�������ȸ��·�����Ϣ��
		* GU_NORMALS
		*	�����귨����Ϣ�������ٸ�����������ռ䡣
		* GU_USE_GEOMETRY
		* GU_USE_TCOORD_CHANNEL + nonnegative_integer
		*	ʹ�õ�nonnegative_integer���������������������ռ䡣
		* Ϊ�˽�ʡ�Դ棬����Կ���ֻ����normals��tangents��Ȼ��ʹ��������ɫ��
		* ����bitangent��
		*	bitangent = Cross(normal, tangent)			
		*/
		virtual void UpdateModelSpace (UpdateType type);

		virtual void OnForceBind ();

		void SetOnDrawCallback(OnDrawCallback dcb);
		OnDrawCallback GetOnDrawCallback();
		void OnDraw(Renderer *renderer);

		// ����
		enum PhysicsType
		{
			PHYS_NONE,
			PHYS_BOX,
			PHYS_SPHERE,
			PHYS_CONVEXHULL,
			PHYS_CONVEXTRIMESH,
			PHYS_HEIGHTFIELD,
			PHYS_MAX_TYPE
		};
		void SetPhysicsType (PhysicsType type);
		PhysicsType GetPhysicsType () const;

		void SetPhysicsParam (const Float3 &param);
		const Float3 &GetPhysicsParam () const;

public_internal:
		float mEyeDistance; // �������
		static bool LessThan (const Renderable *renderable0,
			const Renderable *renderable1);

	protected:
		void AdjustTransparent ();
		void _SetTransparent (bool transparent);

	public_internal:
		bool IsTransparent () const;
		Float4 UpdateFogParam (const Float4 &envFogParam) const;

		// shareDraw
		void EnableShareDraw(bool enableShareDraw);
		bool IsEnableShareDraw() const;
		enum PreDrawRetType
		{
			PBRT_NONE,
			PDRT_VB_FULL,
			PDRT_IB_FULL,
			PDRT_MTL_NOTEQUAL,
			PDRT_MAX_TYPE
		};
		virtual PreDrawRetType OnPreShareDraw(Renderable *shareMesh);
		virtual void OnShareDraw(Renderable *shareMesh);

	protected:
		// ����ͼ�θ���
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);
		virtual void UpdateWorldBound ();
		virtual void UpdateModelBound ();

		// �����ü�
		virtual void OnGetVisibleSet (Culler& culler, bool noCull);

	protected:
		// ��Ⱦ����
		unsigned int mSortIndex; // (layer<<24)|(sublayer<<16)|current

		// ����ͼ������
		PrimitiveType mType;
		VertexFormatPtr mVFormat;
		VertexBufferPtr mVBuffer;
		IndexBufferPtr mIBuffer;		
		Bound mModelBound;
		bool mIsUseBoundPick;

		DBObject_V *mDBObject_V;
		DBObject_I *mDBObject_I;
		bool mIsUseShareBuffers;

		int mIBOffset;
		int mIBNumElements;

		ShinePtr mDefaultShine;
		ShinePtr mBakeShine;

		bool mIsBackObject;
		bool mIsBackTarget;
		BakeSizeType mBakeSizeType;
		bool mIsUseLightTexture;
		std::string mLightTexPath;
		std::string mNormalTexPath;
		MaterialInstancePtr mNormalMaterialInstance;

		// ���Ƽ���ͼ�ε�Ч��
		MaterialInstancePtr mMaterialInstance;

		RenderLayer mLayer;
		int mSubLayer;

		// ��ǰ������Ӱ��ĵƹ�
		/*
		* �ƹⲻ�ó־û�����ÿ����Ⱦ֮ǰ������Ӱ��ĵƹ⡣
		*/
		std::vector<LightPtr> mInfulencedLights; 

		float mFogIP_Height;
		float mFogIP_Distance;

		PhysicsType mPhysicsType;
		Float3 mPhysicsParam;

		bool mIsFakeTransparent;

		bool mIsEnableShareDraw;
		OnDrawCallback mOnDrawCallback;
	};

	PX2_REGISTER_STREAM(Renderable);
	typedef Pointer0<Renderable> RenderablePtr;
#include "PX2Renderable.inl"

}

#endif
