// PX2Font.hpp

#ifndef PX2FONT_HPP
#define PX2FONT_HPP

#include "PX2UnityPre.hpp"
#include "PX2CharCoding.hpp"
#include "PX2FontDefine.hpp"
#include "PX2FontGlyphMap.hpp"
#include "PX2TriMesh.hpp"
#include "PX2Texture2D.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Font
	{
	public:
		enum FontType
		{
			FT_BITMAP,
			FT_COMMON,
			FT_MAX_TYPE
		};
		Font (FontType type);
		virtual ~Font ();

		void DumpTex ();

		void UpdateFontTexs ();
		void SetNeedUpdate();

		FontType GetFontType ();

		virtual bool Initlize (int fontWidth, int fontHeight, 
			const char *fontFilename, CharCodingType codingType,
			unsigned int fontExtStyle);

		/// ���ݴ���Ĳ����ж���Ҫ�����������Ƿ��뱾�����ǵȼ۵�
		virtual bool IsSame (int fontWidth, int fontHeight,
			const char *fontFilename, CharCodingType codingType,
			unsigned int fontExtStyle) = 0;
		const std::string &GetFontFilename () const;
		unsigned int GetFontExtStyle () const;
		virtual void GetFontSize (int &width, int &height);

		virtual void SetCharSize (int width, int height);
		virtual void GetCharSize (float &width, float &height);

		virtual void SetHorInterval (float interval);
		virtual void GetHorInterval (float &interval);

		virtual void SetLineInterval (float interval);
		virtual void GetLineInterval (float &interval);	

		/// ���һ���ַ���ʾ�ߴ�
		virtual void GetCharSize (const unsigned char *character, float &width,
			float &height);
		virtual void GetCharExtent (const unsigned char *character,
			float &width, float &height, float fontScale);

		/// ȡ���ַ�������ʾ�ߴ磬�˺����ɴ����п��Ʒ�
		virtual void GetTextExtent (const char *text, float &width,
			float &height, bool doTransfer, float fontScale);

		/// ��ȡ���ַ���ͷ�����޶���ȷ�Χ�ڣ��������������ַ�����Ϣ
		/**
		* width: ���ַ���ͷ�����޶���ȷ�Χ�ڣ����������Ĳ����ַ�����ʾ���
		* numBytes: ���ַ���ͷ�����޶���ȷ�Χ�ڣ����������Ĳ����ַ����ֽڳ���
		*/
		virtual void GetTextExtentFitInWidth (const char *text, float fixedWidth,
			float &width, int &numBytes, bool doTransfer, float fontScale);

		/// ��ʾ�ַ���
		virtual void TextOutM (TriMesh *mesh, const std::string &text, float xPos, float yPos,
			const Float2 &space, const Float4 &color, const Float4 &shadowBorderColor4, 
			float shadowBorderSize, unsigned int drawStyle=FES_NORMAL, bool doTransfer=false,
			float scale = 1.0f, float depth=0.0f);

		/// �ھ�����������ʾ�ı�
		/**
		* autoWrap��ʾ�Ƿ��Զ�����
		*/
		virtual void TextOutRect(TriMesh *mesh, const std::string &text, Rectf &rect,
			const Float2 &space,
			float offsetX, float offsetY, bool autoWrap, const Float4 &color,
			const Float4 &shadowBorderColor4, float shadowBorderSize,
			int drawStyle = FES_NORMAL, bool doTransfer = false, float scale = 1.0f,
			bool isPointAsPunctuation = true);

		Texture2D *GetTexture ();

		bool IsNeedUpdate ();

	protected:
		Font ();
		void RenderText (TriMesh *mesh, float depth = 0.0f);
		// ���ַ�����ʼ������Ŀɻ���λ�õľ���
		float GetMinDisOfAWord(const char *text, bool doTransfer, bool isPointAsPunctuation, float fontScale);
		virtual Texture2D *TexUVMaping (const unsigned char *character, Rectf &rectUV) = 0;

		int mFontPicSize;
		FontType mFontType;
		std::string mFontFilename;
		CharCodingType mCodingType;
		unsigned int mFontExtStyle;
		int mFontWidth;
		int mFontHeight;
		float mCharWidth;
		float mCharHeight;
		float mHorInterval; //< �м��
		float mLineInterval; //< �м��
		int mCharNumMaxCache; //< һ��ͼ֧�ֵ�����ַ���
		int mTexWidth;
		int mTexHeight;
		CharCoding *mCharCodingObj; //< ���뷽ʽ�������
		FontGlyphMap *mGlyphMap; //< ����ӳ�����
		int mShowNum; //< ��ǰ׼������Ķ������
		Texture2DPtr mTexture; //< ��ǰ��Ҫ���������ʹ�õ���ͼ
		std::vector<FontDrawRect> mDrawRects;
		bool mIsNeedUpdate;

		static unsigned int msCounts;
	};

	typedef Pointer0<Font> FontPtr;
#include "PX2Font.inl"

}

#endif