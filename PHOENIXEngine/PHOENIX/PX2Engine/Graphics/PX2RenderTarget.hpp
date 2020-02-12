// PX2RenderTarget.hpp

#ifndef PX2RENDERTARGET_HPP
#define PX2RENDERTARGET_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Texture2D.hpp"

namespace PX2
{

	/// ��Ⱦ�����������
	class PX2_ENGINE_ITEM RenderTarget : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(RenderTarget);

	public:
		/// ���캯��
		/**
		* ֧�ֵ�render target������Ӳ����numTargets����Ϊ1��
		*/
		RenderTarget (int numTargets, Texture::Format tformat, int width,
			int height, bool hasMipmaps, bool hasDepthStencil, bool isOnlyDepthNoStencil);

		virtual ~RenderTarget ();

		// ��Ա����
		inline int GetNumTargets () const;
		inline Texture::Format GetFormat () const;
		inline int GetWidth () const;
		inline int GetHeight () const;
		inline Texture2D* GetColorTexture (int i) const;
		inline Texture2D* GetDepthStencilTexture () const;
		inline bool HasMipmaps () const;
		inline bool HasDepthStencil () const;
		inline bool IsOnlyDepth16NoStencil() const;

		void *PdrPointer;

	protected:
		int mNumTargets;
		Texture::Format mFormat;
		int mWidth;
		int mHeight;
		bool mHasDepthStencil;
		bool mIsOnlyDepth16NoStencil;
		Texture2DPtr* mColorTextures;
		Texture2DPtr mDepthStencilTexture;
		bool mHasMipmaps;
	};

	PX2_REGISTER_STREAM(RenderTarget);
	typedef Pointer0<RenderTarget> RenderTargetPtr;
#include "PX2RenderTarget.inl"

}

#endif
