// PX2Dx9RendererData.hpp

#ifdef PX2_USE_DX9

#ifndef PX2DX9RENDERERDATA_HPP
#define PX2DX9RENDERERDATA_HPP

#include "PX2Dx9RendererPre.hpp"
#include "PX2Dx9RendererInput.hpp"
#include "PX2AlphaProperty.hpp"
#include "PX2CullProperty.hpp"
#include "PX2DepthProperty.hpp"
#include "PX2OffsetProperty.hpp"
#include "PX2StencilProperty.hpp"
#include "PX2WireProperty.hpp"
#include "PX2Texture.hpp"

namespace PX2
{

	class RendererData
	{
	public:
		RendererData (RendererInput *input, int width, int height,
			Texture::Format colorFormat, Texture::Format depthStencilFormat,
			int numMultisamples);

		~RendererData ();

		void CreateUniqueFont ();
		void DestroyUniqueFont ();
		void SetDefaultProperties (const AlphaProperty* aproperty, 
			const CullProperty* cproperty, const DepthProperty* dproperty,
			const OffsetProperty* oproperty, const StencilProperty* sproperty,
			const WireProperty* wproperty);

		// ���浱ǰ����Ⱦ״̬��������Ⱦ״̬��ת��
		class RenderState
		{
		public:
			RenderState ();

			void Initialize (IDirect3DDevice9* device, const AlphaProperty* aproperty,
				const CullProperty* cproperty, const DepthProperty* dproperty,
				const OffsetProperty* oproperty, const StencilProperty* sproperty,
				const WireProperty* wproperty);

			// AlphaProperty
			DWORD mAlphaBlendEnable;
			DWORD mAlphaSrcBlend;
			DWORD mAlphaDstBlend;
			DWORD mAlphaTestEnable;
			DWORD mAlphaFunc;
			DWORD mAlphaRef;
			D3DCOLOR mBlendFactor;

			// CullProperty
			DWORD mCullMode;

			// DepthProperty
			DWORD mZEnable;
			DWORD mZFunc;
			DWORD mZWriteEnable;

			// OffsetProperty
			DWORD mSlopeScaleDepthBias;
			DWORD mDepthBias;

			// StencilProperty
			DWORD mStencilEnable;
			DWORD mStencilFunc;
			DWORD mStencilRef;
			DWORD mStencilMask;
			DWORD mStencilWriteMask;
			DWORD mStencilFail;
			DWORD mStencilZFail;
			DWORD mStencilPass;

			// WireProperty
			DWORD mFillMode;
		};

		enum
		{
			MAX_NUM_VSAMPLERS = 4,  //< VSModel 3 ֧�� 4 ������, VSModel 2 ��֧��.
			MAX_NUM_PSAMPLERS = 16  //< PSModel 2 �� PSModel 3 ��֧�� 16 ������.
		};
		
		// ���浱ǰ�Ĳ���״̬������״̬��ת��
		class SamplerState
		{
		public:
			SamplerState ();

			float mAnisotropy;
			float mLodBias;
			DWORD mMagFilter;
			DWORD mMinFilter;
			DWORD mMipFilter;
			D3DCOLOR mBorderColor;
			DWORD mWrap[3];
		};

		// ƽ̨��ص�����
		IDirect3DDevice9* mDevice;
		HWND mWindowHandle;
		D3DPRESENT_PARAMETERS mPresent;
		LPD3DXFONT mFont;
		bool mDeviceLost;

		// ��ǰ��Ⱦ����
		RenderState mCurrentRS;
		SamplerState mCurrentVSState[MAX_NUM_VSAMPLERS];
		SamplerState mCurrentPSState[MAX_NUM_PSAMPLERS];

		// �豸��������Ҫ����ʱȷ����
		int mMaxVShaderImages;
		int mMaxPShaderImages;
		int mMaxTCoords;
	};

}

#endif

#endif