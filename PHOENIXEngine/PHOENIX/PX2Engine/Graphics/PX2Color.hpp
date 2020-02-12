// PX2Color.hpp

#ifndef PX2COLOR_HPP
#define PX2COLOR_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Float4.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Color
	{
	public:
		/// ��8-bit��ͨ�������ɫת����32-bit����ɫ��alphaͨ�������õ�255��
		inline static unsigned int MakeR8G8B8 (unsigned char red,
			unsigned char green, unsigned char blue);

		/// ��8-bit��ͨ�������ɫת����32-bit����ɫ��
		inline static unsigned int MakeR8G8B8A8 (unsigned char red,
			unsigned char green, unsigned char blue, unsigned char alpha);

		/// ��32-bit��ɫ����8-bit����ɫ��alphaͨ�������ԡ�
		inline static void ExtractR8G8B8 (unsigned int color,
			unsigned char& red, unsigned char& green, unsigned char& blue);

		/// ��32-bit��ɫ����8-bit����ɫ��
		inline static void ExtractR8G8B8A8 (unsigned int color,
			unsigned char& red, unsigned char& green, unsigned char& blue,
			unsigned char& alpha);

		enum
		{
			CF_QUANTITY = 23  // = Texture::TF_QUANTITY
		};

		// ���ض���ʽ��ת����32-bit��RGBA
		static void ConvertFromR5G6B5 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromA1R5G5B5 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromA4R4G4B4 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromA8 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromL8 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromA8L8 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromR8G8B8 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromA8R8G8B8 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromA8B8G8R8 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromL16 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromG16R16 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromA16B16G16R16 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromR16F (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromG16R16F (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromA16B16G16R16F (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromR32F (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromG32R32F (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromA32B32G32R32F (int numTexels, const char* inTexels,
			Float4* outTexels);

		// ��32-bit��RGBAת�����ض���ʽ
		static void ConvertToR5G6B5 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToA1R5G5B5 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToA4R4G4B4 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToA8 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToL8 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToA8L8 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToR8G8B8 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToA8R8G8B8 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToA8B8G8R8 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToL16 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToG16R16 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToA16B16G16R16 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToR16F (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToG16R16F (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToA16B16G16R16F (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToR32F (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToG32R32F (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToA32B32G32R32F (int numTexels, const Float4* inTexels,
			char* outTexels);

		// һ�����ұ�����Ӧ�ó������ʱʹ�á�
		typedef void (*ConvertFromFunction)(int, const char*, Float4*);
		static ConvertFromFunction FromFunction[CF_QUANTITY];

		typedef void (*ConvertToFunction)(int, const Float4*, char*);
		static ConvertToFunction ToFunction[CF_QUANTITY];
	};

#include "PX2Color.inl"
}

#endif