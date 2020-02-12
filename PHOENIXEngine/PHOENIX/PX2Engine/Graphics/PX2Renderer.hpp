// PX2Renderer.hpp

#ifndef PX2RENDERER_HPP
#define PX2RENDERER_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2AlphaProperty.hpp"
#include "PX2Camera.hpp"
#include "PX2CullProperty.hpp"
#include "PX2DepthProperty.hpp"
#include "PX2Float4.hpp"
#include "PX2Vector2.hpp"
#include "PX2Rect.hpp"
#include "PX2HMatrix.hpp"
#include "PX2IndexBuffer.hpp"
#include "PX2Mutex.hpp"
#include "PX2OffsetProperty.hpp"
#include "PX2PixelShader.hpp"
#include "PX2RenderTarget.hpp"
#include "PX2StencilProperty.hpp"
#include "PX2Texture1D.hpp"
#include "PX2Texture2D.hpp"
#include "PX2Texture3D.hpp"
#include "PX2TextureCube.hpp"
#include "PX2VertexBuffer.hpp"
#include "PX2VertexFormat.hpp"
#include "PX2VertexShader.hpp"
#include "PX2VisibleSet.hpp"
#include "PX2Renderable.hpp"
#include "PX2WireProperty.hpp"
#include "PX2Node.hpp"

namespace PX2
{

	class RendererInput;
	class RendererData;
	class GlobalMaterial;

	// ������ƽ̨�����Դ����platform-dependent resource (pdr)
	class PdrIndexBuffer;
	class PdrPixelShader;
	class PdrRenderTarget;
	class PdrTexture1D;
	class PdrTexture2D;
	class PdrTexture3D;
	class PdrTextureCube;
	class PdrVertexBuffer;
	class PdrVertexFormat;
	class PdrVertexShader;
	class PdrMaterialPass;

	/// ��Ⱦ����
	/**
	* Renderer����һ������ӿڡ�ͨ����ͬ�ĺ꣬�����Ϊ����ƽ̨��ص���Ⱦ������
	* Щƽ̨������Direct3D��OpenGL���ȵȣ���Renderer�Ľӿڱ���Ϊ�����֣���һ��
	* ����ƽ̨�޹صĺ��������ݣ��ڶ�������ƽ̨��صĺ��������ݡ�
	*/
	class PX2_ENGINE_ITEM Renderer
	{
		//============================================================================
		// ��ƽ̨�޹صĽӿ�
	public:
		/// ��Ⱦ�����캯���ڲ����øú��������г�ʼ����
		void Initialize (int width, int height, Texture::Format colorFormat,
			Texture::Format depthStencilFormat, int numMultisamples);

		/// ��Ⱦ�����������ڲ����øú����������ͷš�
		void Terminate ();

		// ͨ����˵����Ϸ��ֻ��һ����Ⱦ�������ô˽ӿ������£����㱩¶��Ⱦ���ӿڳ���
		static void SetDefaultRenderer (Renderer *renderer);
		static Renderer *GetDefaultRenderer ();

		// ��ù��캯�����������
		void SetName(const std::string &name);
		const std::string &GetName() const;
		inline int GetWidth () const;
		inline int GetHeight () const;
		inline Texture::Format GetColorFormat () const;
		inline Texture::Format GetDepthStencilFormat () const;
		inline int GetNumMultisamples () const;

		// === ��Դ����
		/*
		* ��Щ��Դ�ǣ�vertex formats��vertex buffers��index buffers��textures��
		* 1D��2D��3D��cube����render targets��vertex shader����pixel shader����
		* Щ��Դӵ������Ĳ���

		* Bind����������Ķ��󣬴�����֮��ص��Դ���Դ����Ⱦ������mapping�����
		*	�Դ���Դ�����Enable����Lock������Bind֮ǰ�����ã���Ⱦ���ڲ�������
		*	�������Դ���Դ����������쳣��
		*
		* BindAll��Ϊ������Ⱦ�������������������������Դ���Դ��
		*
		* Unbind���ͷ������������������Դ���Դ����Ⱦ���ڲ����Ƴ�������Դ���
		*	Դ��mipping��
		*
		* UnbindAll���ͷ�������Ⱦ���أ������������������Դ���Դ��
		*
		* Enable��������Դ��
		*
		* Disable��ȡ��������Դ��
		*
		* Lock����������������ص��Դ���Դ��ָ�롣����Lockס�ĵ�ַ��AGP�ڴ���
		*	��������ͨ�ڴ��У�ȡ����ͼ��API�Լ���Դ�Ĵ������ԣ�ʹ�������������
		*	�Դ���Դ���������ֻ���ķ�ʽLockס��������ȡ��������ҪLock��Դ����
		*	�����ٶȣ�Խ��Խ�á�
		*
		* UnLock���ͷ�Lockס��ָ�롣
		*
		* Update���ڲ�Lockס�Դ���Դ�����ڴ����ݿ�������ʾ�ڴ��У�Ȼ��Unlock�Դ�
		*	�����Ǹ����Դ���Դ����÷�ʽ�������������ڴ�Ȼ�����Update����
		*
		* UpdateAll��Ϊ������Ⱦ���������������������Դ���Դ��
		*
		* ReadColor���������ֻΪRenderTarget��ơ��ڵ���ReadColor֮ǰ��
		* 	RenderTarget�����ȱ�disabled�������������RenderTarget��ɫ�Դ��һ
		*	��Texture2D�����������߸������ɾ��������ص�Texture2D����
		*/

		// �����ʽ����
		/*
		* vformat�����Ѿ���Ӧ�ó�����䡣
		*/
		void Bind (const VertexFormat* vformat);
		static void BindAll (const VertexFormat* vformat);
		void Unbind (const VertexFormat* vformat);
		static void UnbindAll (const VertexFormat* vformat);
		void Enable (const VertexFormat* vformat);
		void Disable (const VertexFormat* vformat);

		// ���㻺��������
		/*
		* vbuffer�����Ѿ���Ӧ�ó�����䡣
		*/
		void Bind (const VertexBuffer* vbuffer);
		bool IsBind (const VertexBuffer* vbuffer);
		static bool IsOneBind (const VertexBuffer* vbuffer);
		static void BindAll (const VertexBuffer* vbuffer);
		void Unbind (const VertexBuffer* vbuffer);
		static void UnbindAll (const VertexBuffer* vbuffer);
		void Enable (const VertexBuffer* vbuffer,
			unsigned int streamIndex = 0, unsigned int offset = 0);
		void Disable (const VertexBuffer* vbuffer,
			unsigned int streamIndex = 0);
		void* Lock (const VertexBuffer* vbuffer,
			Buffer::Locking mode);
		void Unlock (const VertexBuffer* vbuffer);
		void Update (const VertexBuffer* vbuffer);
		static void UpdateAll (const VertexBuffer* vbuffer);

		// ������������������
		/*
		* ibuffer�����Ѿ���Ӧ�ó�����䡣
		*/
		void Bind (const IndexBuffer* ibuffer);
		bool IsBind (const IndexBuffer* ibuffer);
		static bool IsOneBind (const IndexBuffer* ibuffer);
		static void BindAll (const IndexBuffer* ibuffer);
		void Unbind (const IndexBuffer* ibuffer);
		static void UnbindAll (const IndexBuffer* ibuffer);
		void Enable (const IndexBuffer* ibuffer);
		void Disable (const IndexBuffer* ibuffer);
		void* Lock (const IndexBuffer* ibuffer, Buffer::Locking mode);
		void Unlock (const IndexBuffer* ibuffer);
		void Update (const IndexBuffer* ibuffer);
		static void UpdateAll (const IndexBuffer* ibuffer);

		// 1D�������
		/*
		* texture�����Ѿ���Ӧ�ó�����䡣
		*/
		void Bind (const Texture1D* texture);
		static void BindAll (const Texture1D* texture);
		void Unbind (const Texture1D* texture);
		static void UnbindAll (const Texture1D* texture);
		void Enable (const Texture1D* texture, int textureUnit);
		void Disable (const Texture1D* texture, int textureUnit);
		void* Lock (const Texture1D* texture, int level,
			Buffer::Locking mode);
		void Unlock (const Texture1D* texture, int level);
		void Update (const Texture1D* texture, int level);
		static void UpdateAll (const Texture1D* texture, int level);

		// 2D�������
		/*
		* texture�����Ѿ���Ӧ�ó�����䡣
		*/
		void Bind (const Texture2D* texture);
		static void BindAll (const Texture2D* texture);
		void Unbind (const Texture2D* texture);
		static void UnbindAll (const Texture2D* texture);
		void Enable (const Texture2D* texture, int textureUnit);
		void Enable (const Texture2D* texture, int textureUnit, int loc); // opengles2
		void Disable (const Texture2D* texture, int textureUnit);
		void* Lock (const Texture2D* texture, int level,
			Buffer::Locking mode);
		void Unlock (const Texture2D* texture, int level);
		void Update (const Texture2D* texture, int level);
		static void UpdateAll (const Texture2D* texture, int level);

		// 3D�������
		/*
		* texture�����Ѿ���Ӧ�ó�����䡣
		*/
		void Bind (const Texture3D* texture);
		static void BindAll (const Texture3D* texture);
		void Unbind (const Texture3D* texture);
		static void UnbindAll (const Texture3D* texture);
		void Enable (const Texture3D* texture, int textureUnit);
		void Disable (const Texture3D* texture, int textureUnit);
		void* Lock (const Texture3D* texture, int level,
			Buffer::Locking mode);
		void Unlock (const Texture3D* texture, int level);
		void Update (const Texture3D* texture, int level);
		static void UpdateAll (const Texture3D* texture, int level);

		// Cube�������
		/*
		* texture�����Ѿ���Ӧ�ó�����䡣
		*/
		void Bind (const TextureCube* texture);
		static void BindAll (const TextureCube* texture);
		void Unbind (const TextureCube* texture);
		static void UnbindAll (const TextureCube* texture);
		void Enable (const TextureCube* texture, int textureUnit);
		void Disable (const TextureCube* texture, int textureUnit);
		void* Lock (const TextureCube* texture, int face, int level,
			Buffer::Locking mode);
		void Unlock (const TextureCube* texture, int face, int level);
		void Update (const TextureCube* texture, int face, int level);
		static void UpdateAll (const TextureCube* texture, int face, int level);

		// Render target����
		/*
		* renderTarget�����Ѿ���Ӧ�ó�����䡣
		* ReadColor�����У�������ݵ�textureָ��ΪNULL��ReadColor��Ϊ�㴴����Ӧ
		* ��������������Ҫ�Լ�������ȷ�����ʽ����ȷ��С������
		*/
		void Bind (const RenderTarget* renderTarget);
		static void BindAll (const RenderTarget* renderTarget);
		void Unbind (const RenderTarget* renderTarget);
		static void UnbindAll (const RenderTarget* renderTarget);
		void Enable (const RenderTarget* renderTarget);
		void Disable (const RenderTarget* renderTarget);
		void ReadColor (int i, const RenderTarget* renderTarget,
			Texture2D*& texture);

		// Vertex shader����
		/*
		* vshader�����Ѿ���Ӧ�ó��򴴽���
		*/
		void Bind (const VertexShader* vshader);
		static void BindAll (const VertexShader* vshader);
		void Unbind (const VertexShader* vshader);
		static void UnbindAll (const VertexShader* vshader);
		void Enable (const VertexShader* vshader,
			const ShaderParameters* parameters);
		void Disable (const VertexShader* vshader,
			const ShaderParameters* parameters);

		// Pixel shader����
		/*
		* pshader�����Ѿ���Ӧ�ó��򴴽���
		*/
		void Bind (const PixelShader* pshader);
		static void BindAll (const PixelShader* pshader);
		void Unbind (const PixelShader* pshader);
		static void UnbindAll (const PixelShader* pshader);
		void Enable (const PixelShader* pshader,
			const ShaderParameters* parameters);
		void Disable (const PixelShader* pshader,
			const ShaderParameters* parameters);

		void ForceBind (Movable *mov);
		void ForceBind (Renderable *renderable);

		void OnLostDevice();
		void OnRestoreDevice();

		int GetNumPdrVertexFormat () const;
		int GetNumPdrVertexBuffer () const;
		int GetNumPdrIndexBuffer () const;
		int GetNumPdrTexture1D () const;
		int GetNumPdrTexture2D () const;
		int GetNumPdrTexture3D () const;
		int GetNumPdrTextureCube () const;
		int GetNumPdrRenderTarget () const;
		int GetNumPdrVertexShader () const;
		int GetNumPdrPixelShader () const;
		int GetNumPdrMaterialPass () const;

		int GetNumGPUVertexShader () const;
		int GetNumGPUPixelShader () const;
		int GetNumGPUMaterialPass () const;

		int GetNumDrawPrimitivePerFrame () const;

		// === End resource management.

		// ȫ����Ⱦ״̬����
		inline const AlphaProperty* GetAlphaProperty () const;
		inline const CullProperty* GetCullProperty () const;
		inline const DepthProperty* GetDepthProperty () const;
		inline const OffsetProperty* GetOffsetProperty () const;
		inline const StencilProperty* GetStencilProperty () const;
		inline const WireProperty* GetWireProperty () const;

		// ��ǰ�������
		inline void SetCamera (Camera* camera);
		inline Camera* GetCamera ();
		inline const Camera* GetCamera () const;
		inline const HMatrix& GetViewMatrix () const;
		inline const HMatrix& GetProjectionMatrix () const;
		inline const HMatrix& GetPostProjectionMatrix () const;

		// ViewPort
		void SetViewport (const Rectf &rect);
		const Rectf &GetViewPort() const;

		/// ���ѡ������
		/**
		* ����������������ϵ��(ԭ�������½ǣ�y�����ϣ�x������)������(x,y)�͵�ǰ
		* ����������ѡ�����ߡ��������origin�������λ�ã�direction�ǵ�λ����
		* �ķ������������Ƕ�������������ϵ�еġ����(x,y)���ӿڷ�Χ�ڣ�����
		* ��true����
		*/
		bool GetPickRay (int x, int y, APoint& origin, AVector& direction) const;

		/// ���3d������Ļ������
		Vector2f PointWorldToViewPort (const APoint &point, bool *isInBack=0);

		// ���������ɫ�������ȣ����ģ��ֵ��
		inline void SetClearColor (const Float4& clearColor);
		inline const Float4& GetClearColor () const;
		inline void SetClearDepth (float clearDepth);
		inline float GetClearDepth () const;
		inline void SetClearStencil (unsigned int clearStencil);
		inline unsigned int GetClearStencil () const;

		// ��ɫͨ��mask���ʡ�
		inline void GetColorMask (bool& allowRed, bool& allowGreen,
			bool& allowBlue, bool& allowAlpha) const;

		// ����������Ⱦ״̬
		/*
		* ���������������Ⱦ״̬����������MaterialPass����Ⱦ״̬֮�ϡ�ȡ������
		* ��Ⱦ״̬��ֻҪ����0ָ�롣
		*/
		inline void SetOverrideAlphaProperty (const AlphaProperty* alphaState);
		inline void SetOverrideCullProperty (const CullProperty* cullState);
		inline void SetOverrideDepthProperty (const DepthProperty* depthState);
		inline void SetOverrideOffsetProperty (const OffsetProperty* offsetState);
		inline void SetOverrideStencilProperty (const StencilProperty* stencilState);
		inline void SetOverrideWireProperty (const WireProperty* wireState);
		inline const AlphaProperty* GetOverrideAlphaProperty () const;
		inline const CullProperty* GetOverrideCullProperty () const;
		inline const DepthProperty* GetOverrideDepthProperty () const;
		inline const OffsetProperty* GetOverrideOffsetProperty () const;
		inline const StencilProperty* GetOverrideStencilProperty () const;
		inline const WireProperty* GetOverrideWireProperty () const;
		inline void SetReverseCullOrder (bool reverseCullOrder);
		inline bool GetReverseCullOrder () const;

		// �������������ɼ��������
		void Draw (const VisibleSet& visibleSet, GlobalMaterial* globalMaterial = 0);

		// ���Ƶ�����������
		void Draw (Renderable* renderable);
		void Draw (Renderable* renderable, const MaterialInstance* instance);

	private:
		static Mutex &GetMutex ();
		// �ͷ����������Ⱦ����ص���Դ��(��Ⱦ�����������ڻ���ã���������ʽ��
		// ����Դ�ͷź���)
		void DestroyAllVertexFormats ();
		void DestroyAllVertexBuffers ();
		void DestroyAllIndexBuffers ();
		void DestroyAllTexture1Ds ();
		void DestroyAllTexture2Ds ();
		void DestroyAllTexture3Ds ();
		void DestroyAllTextureCubes ();
		void DestroyAllRenderTargets ();
		void DestroyAllVertexShaders ();
		void DestroyAllPixelShaders ();
		void DestroyAllMaterialPasses ();

		// ��Ⱦ���������
		std::string mName;
		int mWidth;
		int mHeight;
		Texture::Format mColorFormat;
		Texture::Format mDepthStencilFormat;
		int mNumMultisamples;
		Rectf mViewPort;

		// ȫ����Ⱦ״̬
		AlphaProperty* mDefaultAlphaProperty; // ȱʡ״̬
		CullProperty* mDefaultCullProperty;
		DepthProperty* mDefaultDepthProperty;
		OffsetProperty* mDefaultOffsetProperty;
		StencilProperty* mDefaultStencilProperty;
		WireProperty* mDefaultWireProperty;
		const AlphaProperty* mAlphaProperty; // ��ǰ���õ�״̬
		const CullProperty* mCullProperty;
		const DepthProperty* mDepthProperty;
		const OffsetProperty* mOffsetProperty;
		const StencilProperty* mStencilProperty;
		const WireProperty* mWireProperty;
		bool mReverseCullOrder;

		// ������Ⱦ״̬
		const AlphaProperty* mOverrideAlphaProperty; // override״̬
		const CullProperty* mOverrideCullProperty;
		const DepthProperty* mOverrideDepthProperty;
		const OffsetProperty* mOverrideOffsetProperty;
		const StencilProperty* mOverrideStencilProperty;
		const WireProperty* mOverrideWireProperty;

		/// ��ǰ����ͼ�α任״̬�е�����������洢view��projection��
		/// postprojection����
		Camera* mCamera;

		// Framebuffer ��ղ���
		Float4 mClearColor;
		float mClearDepth;
		unsigned int mClearStencil;

		// �󱳻�������ɫģ��ֵ
		bool mAllowRed, mAllowGreen, mAllowBlue, mAllowAlpha;

		// �������Դ
		typedef std::map<const VertexFormat*, PdrVertexFormat*> VertexFormatMap;
		typedef std::map<const VertexBuffer*, PdrVertexBuffer*> VertexBufferMap;
		typedef std::map<const IndexBuffer*, PdrIndexBuffer*> IndexBufferMap;
		typedef std::map<const Texture1D*, PdrTexture1D*> Texture1DMap;
		typedef std::map<const Texture2D*, PdrTexture2D*> Texture2DMap;
		typedef std::map<const Texture3D*, PdrTexture3D*> Texture3DMap;
		typedef std::map<const TextureCube*, PdrTextureCube*> TextureCubeMap;
		typedef std::map<const RenderTarget*, PdrRenderTarget*> RenderTargetMap;
		typedef std::map<const VertexShader*, PdrVertexShader*> VertexShaderMap;
		typedef std::map<const PixelShader*, PdrPixelShader*> PixelShaderMap;
		typedef std::map<const MaterialPass*, PdrMaterialPass*> MaterialPassMap;
		VertexFormatMap mVertexFormats;
		VertexBufferMap mVertexBuffers;
		IndexBufferMap mIndexBuffers;
		Texture1DMap mTexture1Ds;
		Texture2DMap mTexture2Ds;
		Texture3DMap mTexture3Ds;
		TextureCubeMap mTextureCubes;
		RenderTargetMap mRenderTargets;
		VertexShaderMap mVertexShaders;
		PixelShaderMap mPixelShaders;
		MaterialPassMap mMaterialPasses;

		VertexFormatPtr mLastVertexFormat;
		VertexBufferPtr mLastEnableVertexBuffer;
		IndexBufferPtr mLastEnableIndexBuffer;
		VertexShaderPtr mLastVertexShader;
		PixelShaderPtr mLastPixelShader;
		MaterialInstancePtr mLastMaterialInstance;

		// ��Ⱦ������
		typedef std::set<Renderer*> RendererSet;
		static RendererSet msRenderers;
		static Renderer *msDefaultRenderer;

		int mNumDrawPrimitivePerFrame;

		//============================================================================
		// ƽ̨��ؽӿ�
	public:
		Renderer (RendererInput *input, int width, int height,
			Texture::Format colorFormat, Texture::Format depthStencilFormat,
			int numMultisamples);
		~Renderer ();

		static Renderer *CreateRenderer(void *ptData, void *ptDataLinuxWindow,
			void *ptDataLinuxVisual, int width, int height,
			int numMultisamples, RendererInput* &renderInput);
		static std::string GetRenderTag();

		void InitRenderStates ();

		// Pass ����
		void Bind (const MaterialPass *pass);
		static void BindAll (const MaterialPass *pass);
		void Unbind (const MaterialPass *pass);
		static void UnbindAll (const MaterialPass *pass);
		void Enable (const Renderable* renderable,
			const MaterialInstance* instance, int index);
		void Disable (const Renderable* renderable,
			const MaterialInstance* instance, int index);

		// Renderable����Ⱦ״̬����
		void SetAlphaProperty (const AlphaProperty* alphaState);
		void SetCullProperty (const CullProperty* cullState);
		void SetDepthProperty (const DepthProperty* depthState);
		void SetOffsetProperty (const OffsetProperty* offsetState);
		void SetStencilProperty (const StencilProperty* stencilState);
		void SetWireProperty (const WireProperty* wireState);

		// Viewport����
		/*
		* viewport����������ϵ��ԭ������Ļ���½ǣ�y�����ϣ�x�����ҡ�
		*/
		void SetViewport (int xPosition, int yPosition, int width,
			int height);
		void GetViewport (int& xPosition, int& yPosition, int& width,
			int& height) const;
		void SetDepthRange (float zMin, float zMax);
		void GetDepthRange (float& zMin, float& zMax) const;
		void Resize (int width, int height);

		// ����
		void ResizeWindow (int width, int height);

		// �����ɫ����ȣ���ģ��֧��
		void ClearColorBuffer ();
		void ClearDepthBuffer ();
		void ClearStencilBuffer ();
		void ClearBuffers ();
		void ClearColorBuffer (int x, int y, int w, int h);
		void ClearDepthBuffer (int x, int y, int w, int h);
		void ClearStencilBuffer (int x, int y, int w, int h);
		void ClearBuffers (int x, int y, int w, int h);
		void DisplayColorBuffer ();

		// ������ɫmask
		void SetColorMask (bool allowRed, bool allowGreen,
			bool allowBlue, bool allowAlpha);

		// ������Ⱦ�ӿڵ��ö������ں���PreDraw ()��PostDraw ()֮�䡣ͨ���ĸ�ʽ���£�
		//   if (renderer->PreDraw())
		//   {
		//       <��Ⱦ��������>;
		//       renderer->PostDraw();
		//   }
		bool PreDraw ();
		void PostDraw ();

		// ����2Dbuffers�����
		void Draw (const unsigned char* screenBuffer, bool reflectY = false);

		// ����2D���ֵ����
		void Draw (int x, int y, const Float4& color,
			const std::string& message);

	private:
		// ����3d�������ڵ㺯��
		void DrawPrimitive (const Renderable* renderable);

public_internal:
		/// ��ƽ̨�����Ϣ
		/**
		* ���ڹ��нӿ��������Դ����з��ʡ�
		*/
		RendererData* mData;
		const Renderable *mCurRenderable;

		PdrVertexFormat* GetResource (const VertexFormat* vformat);
		PdrVertexBuffer* GetResource (const VertexBuffer* vbuffer);
		PdrIndexBuffer* GetResource (const IndexBuffer* ibuffer);
		PdrTexture1D* GetResource (const Texture1D* texture);
		PdrTexture2D* GetResource (const Texture2D* texture);
		PdrTexture3D* GetResource (const Texture3D* texture);
		PdrTextureCube* GetResource (const TextureCube* texture);
		PdrRenderTarget* GetResource (const RenderTarget* renderTarget);
		PdrVertexShader* GetResource (const VertexShader* vshader);
		PdrPixelShader* GetResource (const PixelShader* pshader);
		PdrMaterialPass* GetResource (const MaterialPass* pass);

		// ������render target����
		inline bool InTexture2DMap (const Texture2D* texture);
		inline void InsertInTexture2DMap (const Texture2D* texture,
			PdrTexture2D* platformTexture);

		class SharePdrData
		{
		public:
			SharePdrData ();
			~SharePdrData ();

			void *DataPtr;

			int DataInt;
			char ES_V_CLoc[16];
			char ES_V_TLoc[16];
			char ES_P_CLoc[16];
			char ES_P_TLoc[16];
		};
		std::map<int, SharePdrData> mSharePdrVertexShaders;
		std::map<int, SharePdrData> mSharePdrPixelShaders;
		std::map<int, SharePdrData> mSharePdrMaterialPasses;
		void ClearSharePdrData ();
		//============================================================================
	};

#include "PX2Renderer.inl"

}

#endif
