// PX2Dx9RenderTarget.hpp

#ifdef PX2_USE_DX9

#ifndef PX2DX9RENDERTARGET_HPP
#define PX2DX9RENDERTARGET_HPP

#include "PX2Dx9RendererPre.hpp"
#include "PX2RenderTarget.hpp"

namespace PX2
{

	class Renderer;

	class PdrRenderTarget
	{
	public:
		PdrRenderTarget (Renderer* renderer, const RenderTarget* renderTarget);
		~PdrRenderTarget ();

		// Render target ����
		void Enable (Renderer* renderer);
		void Disable (Renderer* renderer);
		void ReadColor (int i, Renderer* renderer, Texture2D*& texture);

	private:
		// ֧�ֶ��render target
		int mNumTargets;
		unsigned int mWidth, mHeight;
		Texture::Format mFormat;
		bool mHasDepthStencil;

		// ��ɫ��ȾĿ������ж����depth-stencil��ȾĿ��ֻ��Ҫһ�������ģ��
		// ֵֻ��Ҫ����һ�Ρ�
		IDirect3DTexture9** mColorTextures;
		IDirect3DSurface9** mColorSurfaces;
		IDirect3DTexture9* mDepthStencilTexture;
		IDirect3DSurface9* mDepthStencilSurface;

		// ����֮ǰ����ȾĿ�꣬������render target֮����Ҫ�ûء�
		IDirect3DSurface9* mSaveColorSurface;
		IDirect3DSurface9* mSaveDepthStencilSurface;
	};

}

#endif

#endif