// PX2MaterialTechnique.hpp

#ifndef PX2MATERIALTECHNIQUE_HPP
#define PX2MATERIALTECHNIQUE_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2MaterialPass.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM MaterialTechnique : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(MaterialTechnique);

	public:
		MaterialTechnique ();
		virtual ~MaterialTechnique ();

		// ����һ���µ�pass��pass����ĩβ��
		void InsertPass (MaterialPass* pass);
		void PopLastPass ();

		// ��Ա����
		inline int GetNumPasses () const;
		MaterialPass* GetPass (int passIndex) const;

		// ����ÿ��pass��Ԫ��
		VertexShader* GetVertexShader (int passIndex) const;
		PixelShader* GetPixelShader (int passIndex) const;
		AlphaProperty* GetAlphaProperty (int passIndex) const;
		CullProperty* GetCullProperty (int passIndex) const;
		DepthProperty* GetDepthProperty (int passIndex) const;
		OffsetProperty* GetOffsetProperty (int passIndex) const;
		StencilProperty* GetStencilProperty (int passIndex) const;
		WireProperty* GetWireProperty (int passIndex) const;

	protected:
		std::vector<MaterialPassPtr> mPasses;
	};

	PX2_REGISTER_STREAM(MaterialTechnique);
	typedef Pointer0<MaterialTechnique> MaterialTechniquePtr;
#include "PX2MaterialTechnique.inl"

}

#endif
