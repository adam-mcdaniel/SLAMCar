// PX2FontGlyphMap.hpp

#ifndef PX2FONTGLYPHMAP_HPP
#define PX2FONTGLYPHMAP_HPP

#include "PX2UnityPre.hpp"

namespace PX2
{

	class FontGlyphMap
	{
	public:
		FontGlyphMap () {}
		virtual ~FontGlyphMap () {}

		enum GlyphType
		{
			GT_FREETYPE,
			GT_TRUETYPE,
			GT_MAX_TYPE
		};

		/// ȡ������UNICODE�ַ��ĳ��Ϳ�
		virtual void GetUnicodeCharSize (unsigned short wChar, float &width,
			float &height) = 0;
		
		/// ���ݴ����UNICODE�ַ����ض�Ӧ�ĵ���λͼ
		virtual unsigned char *GetUnicodeCharBitmap (unsigned short wChar) = 0;		
	};

}

#endif