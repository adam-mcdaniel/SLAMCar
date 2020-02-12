// PX2ShaderParameters.hpp

#ifndef PX2SHADERPARAMETERS_HPP
#define PX2SHADERPARAMETERS_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Shader.hpp"
#include "PX2ShaderFloat.hpp"
#include "PX2Texture.hpp"
#include "PX2ShaderStruct.hpp"

namespace PX2
{

	/// ��ɫ��������
	/**
	* һ�����ʣ�Ϊÿ����Ⱦpassά��һ������ɫ����������һ������ɫ���������ڱ���
	* ��ɫ����������ġ���ɫ����������
	*/
	class PX2_ENGINE_ITEM ShaderParameters : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(ShaderParameters);

	public:
		ShaderParameters (Shader* shader);
		virtual ~ShaderParameters ();

		// ��Ա����
		inline int GetNumConstants () const;
		inline int GetNumTextures () const;
		inline ShaderFloatPtr* GetConstants () const;
		inline TexturePtr* GetTextures () const;

		// ��Щ��������constants/textures����������ɹ������������ڳ��������е�
		// ������
		int SetConstant (const std::string& name, ShaderFloat* sfloat);
		int SetTexture (const std::string& name, Texture* texture);
		int SetTexture(const std::string& name, const std::string &filename);

		// ��Ա����
		ShaderFloat* GetConstant (const std::string& name) const;
		Texture* GetTexture (const std::string& name) const;

		// handle�������Set*�������ػ��
		ShaderFloat* GetConstant (int handle) const;
		const std::string &GetConstantName(int handle) const;
		Texture* GetTexture (int handle) const;
		const std::string &GetTextureResPath(int handle) const;
		const std::string &GetSampleName(int handle) const;

		// �ڻ��Ƶ�ʱ����³���
		void UpdateConstants (const ShaderStruct *shaderStruc);

	public_internal:
		int _SetTexture(const std::string& name, Texture* texture);

	protected:
		ShaderPtr mShader;
		int mNumConstants;
		ShaderFloatPtr* mConstants;
		int mNumTextures;
		TexturePtr* mTextures;
		std::vector<std::string> mTextureResPath;
	};

	PX2_REGISTER_STREAM(ShaderParameters);
	typedef Pointer0<ShaderParameters> ShaderParametersPtr;
#include "PX2ShaderParameters.inl"

}

#endif
