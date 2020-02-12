// PX2Texture.hpp

#ifndef PX2TEXTURE_HPP
#define PX2TEXTURE_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"
#include "PX2Buffer.hpp"
#include "PX2FileIO.hpp"
#include "PX2Float4.hpp"

namespace PX2
{

	/// �������
	class PX2_ENGINE_ITEM Texture : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(Texture);

	public:
		/// �������ظ�ʽ
		enum Format
		{
			TF_NONE,

			// Small-bit color formats.
			TF_R5G6B5,
			TF_A1R5G5B5,
			TF_A4R4G4B4,

			// 8-bit integer formats.
			TF_A8,
			TF_L8,
			TF_A8L8,
			TF_R8G8B8,
			TF_A8R8G8B8,
			TF_A8B8G8R8,

			// 16-bit integer formats.
			TF_L16,
			TF_G16R16,
			TF_A16B16G16R16,

			// 16-bit floating-point formats ('half float' channels).
			TF_R16F,
			TF_G16R16F,
			TF_A16B16G16R16F,

			// 32-bit floating-point formats ('float' channels).
			TF_R32F,
			TF_G32R32F,
			TF_A32B32G32R32F,

			// DXT compressed formats.
			TF_DXT1,
			TF_DXT3,
			TF_DXT5,

			// Depth-stencil format.
			TF_D16,
			TF_D24S8,

			//ios compressed formats
			TF_RGB_PVRTC_4B,
			TF_RGB_PVRTC_2B,
			TF_RGBA_PVRTC_4B,
			TF_RGBA_PVRTC_2B,

			//android compressed format
			TF_RGB_ETC1,

			TF_QUANTITY
		};

		enum Type
		{
			TT_1D,
			TT_2D,
			TT_3D,
			TT_CUBE,
			TT_QUANTITY
		};

		// ��������mipmap�ȼ�������ȵ����Կ�֧�ֵĵȼ��ߡ�һ��4096x4096��λͼ
		// ��13���ȼ���
		enum
		{
			MM_MAX_MIPMAP_LEVELS = 16
		};

	protected:
		// �������
		Texture (Format tformat, Type type, Buffer::Usage usage, int numLevels);
	public:
		virtual ~Texture ();

		// ��Ա����
		inline Format GetFormat () const;
		inline Type GetTextureType () const;
		inline Buffer::Usage GetUsage () const;
		inline int GetNumLevels () const;
		inline int GetNumDimensions () const;
		inline int GetDimension (int i, int level) const;
		inline int GetNumLevelBytes (int level) const;
		inline int GetNumTotalBytes () const;
		inline int GetLevelOffset (int level) const;
		inline int GetPixelSize () const;
		inline static int GetPixelSize (Format tformat);
		inline bool IsCompressed () const;
		inline bool IsMipmapable () const;

		/// �������buffer
		/**
		* ͨ���˺��������Ի��buffer���ڴ棬buffer���ֽ���ͨ��GetNumTotalBytes()
		* ��á��������bufferΪֻ���ġ���Ҫ����ת��Ϊ"char*"���������ڸı�buffer
		* ����Ⱦ���޷�֪����
		*/
		inline const char* GetData () const;
		bool IsDataNull () const;

		void SetBindDoClearData (bool doClearData);
		bool IsBindDoClearData () const;
		void ClearData ();

		// �û��Զ�������
		/**
		* ��Щ��Ӧ�ó������ã�Texture���Ҳ�֪�������á�
		*/
		enum { MAX_USER_FIELDS = 8 };
		inline void SetUserField (int i, int userField);
		inline int GetUserField (int i) const;

		// pdr
	public:
		void *PdrPointer;

	public_internal:
		static int msNumDimensions[TT_QUANTITY];
		static int msPixelSize[TF_QUANTITY];
		static bool msMipmapable[TF_QUANTITY];

	protected:
		// �������캯��������
		Format mFormat;
		Type mType;
		Buffer::Usage mUsage;
		int mNumLevels;

		// �ɹ��캯���������๹�캯�������������
		int mNumDimensions; // ά�ȵ����ֱ�ʾ1��2��3
		int mDimension[3][MM_MAX_MIPMAP_LEVELS]; // ����ÿ��ά��ÿ���������ؿ��
		int mNumLevelBytes[MM_MAX_MIPMAP_LEVELS]; // ����ÿ�������ֽ���
		int mNumTotalBytes; // �ֽ�����
		int mLevelOffsets[MM_MAX_MIPMAP_LEVELS];

		/// �û��Զ���ģ��洢Ӧ�ó����ر������
		int mUserField[MAX_USER_FIELDS];

		/// ���ڴ��д洢��ͼƬ����
		char* mData;
		bool mIsBindDoClearData;

		// mipmap����֧��
		/*
		* ÿһ�ָ�ʽ���ȱ�ת��Ϊÿ������32bit��mipmap���ɺ󣬽�����ת����ԭʼ��
		* �ظ�ʽ��
		*/

		typedef void (*ConvertFrom)(int, const char*, Float4*);
		static ConvertFrom msConvertFrom[TF_QUANTITY];

		typedef void (*ConvertTo)(int, const Float4*, char*);
		static ConvertTo msConvertTo[TF_QUANTITY];
	};

	PX2_REGISTER_STREAM(Texture);
	typedef Pointer0<Texture> TexturePtr;
#include "PX2Texture.inl"

}

#endif