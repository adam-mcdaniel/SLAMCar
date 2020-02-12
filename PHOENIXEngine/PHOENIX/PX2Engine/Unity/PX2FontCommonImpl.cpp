// PX2FontCommonImpl.cpp

#include "PX2FontCommonImpl.hpp"
#include "PX2FontGlyphMapTrueType.hpp"
#include "PX2FontGlyphMapFreeType.hpp"
#include "PX2CharCodingGBK.hpp"
#include "PX2CharCodingUTF8.hpp"
using namespace PX2;

#define FONT_TEXSTRIDE 4
//----------------------------------------------------------------------------
FontCommonImpl::FontCommonImpl (FontGlyphMap::GlyphType type)
	:
Font(Font::FT_COMMON),
mGlyphType(type),
mTexData(0),
mCharList(0),
mCharListHead(0),
mCharListEnd(0)
{
	mTexWidth = mFontPicSize;
	mTexHeight = mFontPicSize;
}
//----------------------------------------------------------------------------
FontCommonImpl::~FontCommonImpl ()
{
	if (mCharList)
	{
		delete1<CharLinkNode>(mCharList);
		mCharList = 0;
	}

	if (mGlyphMap)
	{
		delete0(mGlyphMap);
		mGlyphMap = 0;
	}

	if (mCharCodingObj)
	{
		delete0(mCharCodingObj);
		mCharCodingObj = 0;
	}
}
//----------------------------------------------------------------------------
bool FontCommonImpl::Initlize (int fontWidth, int fontHeight, 
	const char *fontFilename, CharCodingType codingType,
	unsigned int fontExtStyle)
{
	if (!fontFilename)
		return false;

	if (!Font::Initlize(fontWidth, fontHeight, fontFilename, codingType,
		fontExtStyle))
		return false;

	if (!CreateTexture())
		return false;

	// cache list
	mCharList = new1<CharLinkNode>(mCharNumMaxCache);
	mCharList[0].IndexToCache = 0;
	mCharList[0].CharUniqueID = 0;
	mCharList[0].Prev = 0;
	mCharList[0].Next = &mCharList[1];
	int end = mCharNumMaxCache-1;
	mCharList[end].IndexToCache = end;
	mCharList[end].CharUniqueID = 0;
	mCharList[end].Prev = &mCharList[end - 1];
	mCharList[end].Next = 0;
	for (int i=0; i<end; i++)
	{
		mCharList[i].IndexToCache = i;
		mCharList[i].CharUniqueID = 0;
		mCharList[i].Prev = &(mCharList[i-1]);
		mCharList[i].Next = &(mCharList[i+1]);
	}
	mCharListHead = &mCharList[0];
	mCharListEnd = &mCharList[end];

	// char coding
	if (CCT_UTF8 == codingType)
	{
		mCharCodingObj = new0 CharCodingUTF8;
	}
	else if (CCT_GBK == codingType)
	{
		mCharCodingObj = new0 CharCodingGBK;
	}

	// glyph map
	if (FontGlyphMap::GT_TRUETYPE == mGlyphType)
	{
		FontGlyphMapTrueType* pGlyphMapTrueType = new0 FontGlyphMapTrueType;
		mGlyphMap = pGlyphMapTrueType;
		if (!pGlyphMapTrueType->Initlize(mFontWidth, mFontHeight, fontFilename,
			fontExtStyle) )
			return false;
	}
	else
	{
		FontGlyphMapFreeType* glyphMapFreeType = new0 FontGlyphMapFreeType;
		mGlyphMap = glyphMapFreeType;
		if (!glyphMapFreeType->Initlize(mFontWidth, mFontHeight, fontFilename,
			fontExtStyle))
			return false;
	}

	return true;
}
//----------------------------------------------------------------------------
bool FontCommonImpl::IsSame (int fontWidth, int fontHeight,
	const char *fontFilename, CharCodingType codingType,
	unsigned int fontExtStyle)
{
	if (mFontWidth != fontWidth)
		return false;

	if( mFontHeight != fontHeight)
		return false;

	if( mCodingType != codingType)
		return false;

	if( mFontExtStyle != fontExtStyle )
		return false;

	return (mFontFilename == std::string(fontFilename));
}
//----------------------------------------------------------------------------
bool FontCommonImpl::CreateTexture ()
{
	Texture::Format fmt;
#if FONT_TEXSTRIDE == 1
	fmt = Texture::TF_A8;
#elif FONT_TEXSTRIDE == 2
	fmt = Texture::TF_A4R4G4B4;
#else
	fmt = Texture::TF_A8R8G8B8;
#endif

	mFontTex = new0 Texture2D(fmt, mTexWidth, mTexHeight, 1);

	mCharNumWidth = mTexWidth/mFontWidth;
	mCharNumHeight = mTexHeight/mFontHeight;
	mCharNumMaxCache = mCharNumWidth*mCharNumHeight;

	mFontTex->SetBindDoClearData(false);

	return true; 
}
//----------------------------------------------------------------------------
Texture2D *FontCommonImpl::TexUVMaping (const unsigned char *character,
	Rectf &rectUV)
{
	int cacheCell = -1;
	unsigned int uniqueID = mCharCodingObj->ToUniqueID(character);
	CacheMap::iterator it = mCacheMap.find(uniqueID);
	if (it != mCacheMap.end())
	{
		cacheCell = (*it).second;
	}

	// �ַ���ͼ���գ������δʹ�ñ��滻��ԭ�����
	// ���ʹ�õ��ַ��ŵ�����ĩβ

	// �����û�г��ֹ��������µ��ָ�����ͼ
	// ʹ�ù����ַ��ŵ�����ĩβ
	if (cacheCell == -1)
	{
		// �ַ���û�л���

		if (mCharListHead->CharUniqueID != 0)
		{
			// ���ͷ����ָ��Ԫ�Ѿ���ʹ�ã����ʹ�������Ԫ���ַ��Ļ���״̬
			mCacheMap[mCharListHead->CharUniqueID] = -1;
		}

		mCharListHead->CharUniqueID = uniqueID;
		mCacheMap[mCharListHead->CharUniqueID] = mCharListHead->IndexToCache;
		cacheCell = mCharListHead->IndexToCache;

		UseCacheCell(mCharListHead);

		// �������ַ���ͼ
		mIsNeedUpdate = true;
		UpdateTexture(character, cacheCell);
	}
	else
	{
		// �ַ��Ѿ����棬�����ַ��ŵ�����ĩβ

		UseCacheCell (mCharList + cacheCell);
	}

	// ���ݻ��浥Ԫ������������
	int texX = 0;
	int texY = 0;
	texX = cacheCell % mCharNumWidth;
	texY = cacheCell / mCharNumWidth;

	float charWidth = 0.0f;
	float charHeight = 0.0f;
	GetCharSize(character, charWidth, charHeight);

	float fX1 = 0.0f;
	float fY1 = 0.0f;
	float fX2 = 0.0f;
	float fY2 = 0.0f;
	fX1 = (float)(texX * mFontWidth) + 0.5f;
	fX2 = fX1 + charWidth;
	fX1 /= mTexWidth;
	fX2 /= mTexWidth;

	fY1 = (float)(mTexHeight - (texY+1)*(mFontHeight))+1.0f;
	fY2 = (float)(mTexHeight - texY*(mFontHeight));

	fY1 /= mTexHeight;
	fY2 /= mTexHeight;

	rectUV.Set(fX1, fY1, fX2, fY2);

	return mFontTex;
}
//----------------------------------------------------------------------------
void FontCommonImpl::UpdateTexture (const unsigned char *character, int cell)
{
	if (!mFontTex)
		return;

	int texX = 0;
	int texY = 0;
	texX = (cell % mCharNumWidth) * mFontWidth;
	texY = (cell / mCharNumWidth) * mFontHeight;

	// ����ַ��ڴ�
	wchar_t wChar = mCharCodingObj->ToUnicode(character);
	unsigned char *bitmap = mGlyphMap->GetUnicodeCharBitmap(wChar);

	// ���ַ��ڴ���µ���ͼ
	char *buffer = mFontTex->GetData(0);
	int texWidth = mFontTex->GetWidth();
	int texHeight = mFontTex->GetHeight();
	PX2_UNUSED(texHeight);

	unsigned char *src = 0;
	for (int y=0; y<mFontHeight; y++)
	{
		src = bitmap + mFontWidth * y;

		for (int x=0; x<mFontWidth; x++)
		{
			unsigned char val = (unsigned char)(*src);

			int charIndex = texWidth*(texY+y)+texX+x;
#if FONT_TEXSTRIDE == 4
			unsigned char *b = (unsigned char*)&(buffer[4*charIndex  ]); // B 
			unsigned char *g = (unsigned char*)&(buffer[4*charIndex+1]); // G
			unsigned char *r = (unsigned char*)&(buffer[4*charIndex+2]); // R
			unsigned char *a = (unsigned char*)&(buffer[4*charIndex+3]); // A
			*b = 255;
			*g = 255;
			*r = 255;
			*a = val;
#elif FONT_TEXSTRIDE == 2
			*(unsigned short *)&buffer[2*charIndex] = (val&0x0f) | 0xfff0;
#else
			buffer[charIndex] = val;
#endif
			src++;
		}
	}
}
//----------------------------------------------------------------------------
void FontCommonImpl::UseCacheCell (CharLinkNode *p)
{
	// ��p���End
	if (mCharListHead == p)
	{
		mCharListHead = mCharListHead->Next;
		mCharListHead->Prev = 0;
		mCharListEnd->Next = p;
		p->Prev = mCharListEnd;
		mCharListEnd = p;
		mCharListEnd->Next = 0;
	}
	else if (mCharListEnd != p)
	{
		p->Prev->Next = p->Next;
		p->Next->Prev = p->Prev;
		mCharListEnd->Next = p;
		p->Prev = mCharListEnd;
		mCharListEnd = p;
		mCharListEnd->Next = 0;
	}	
}
//----------------------------------------------------------------------------