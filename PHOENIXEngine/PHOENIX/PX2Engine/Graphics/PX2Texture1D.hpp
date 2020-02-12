// PX2Texture1D.hpp

#ifndef PX2TEXTURE1D_HPP
#define PX2TEXTURE1D_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Texture.hpp"

namespace PX2
{
	
	/// 1D������
	class PX2_ENGINE_ITEM Texture1D : public Texture
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(Texture1D);

	public:
		Texture1D (Format tformat, int dimension0, int numLevels,
			Buffer::Usage usage = Buffer::BU_TEXTURE);

		virtual ~Texture1D ();

		// ��õ�0����mipmap�Ŀ��
		inline int GetLength () const;

		// mipmap����֧��
		void GenerateMipmaps ();
		bool HasMipmaps () const;

		// ������������
		char* GetData (int level) const;

	protected:
		// ����֧������mipmap
		void ComputeNumLevelBytes ();
		void GenerateNextMipmap (int length, const char* texels, int lengthNext,
			char* texelsNext, Float4* rgba);
	};

	PX2_REGISTER_STREAM(Texture1D);
	typedef Pointer0<Texture1D> Texture1DPtr;
#include "PX2Texture1D.inl"

}

#endif
