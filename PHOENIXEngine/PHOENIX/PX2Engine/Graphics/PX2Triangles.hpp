// PX2Triangles.hpp

#ifndef PX2TRIANGLES_HPP
#define PX2TRIANGLES_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Renderable.hpp"
#include "PX2Float2.hpp"
#include "PX2APoint.hpp"
#include "PX2VertexBufferAccessor.hpp"

namespace PX2
{
	
	/// �����μ���ͼ�λ���
	class PX2_ENGINE_ITEM Triangles : public Renderable
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(Triangles);

	protected:
		Triangles (PrimitiveType type = PT_TRIANGLES);
		Triangles (PrimitiveType type, VertexFormat* vformat,
			VertexBuffer* vbuffer, IndexBuffer* ibuffer);
	public:
		virtual ~Triangles ();

		virtual int GetNumTriangles () const = 0;
		virtual bool GetTriangle (int i, int& v0, int& v1, int& v2) const = 0;
		bool GetModelTriangle (int i, APoint* modelTriangle) const;
		bool GetWorldTriangle (int i, APoint* worldTriangle) const;

		inline int GetNumVertices () const;
		inline const Transform& GetWorldTransform () const;
		Float3 GetPosition (int v) const; //< ���������v�����ڶ�������buffer��

		/// ����ͼ�θ��£���PX2Renderable.hpp�п�����˵����
		virtual void UpdateModelSpace (UpdateType type);

	private:
		// ����֧�ּ���ͼ�θ���
		void UpdateModelNormals (VertexBufferAccessor& vba);

		// tangent space����
		void UpdateModelTangentsUseGeometry (VertexBufferAccessor& vba);
		void UpdateModelTangentsUseTCoords (VertexBufferAccessor& vba);

		///
		/**
		* ��ʹ��UpdateModelTangentsUseTCoords����ʱ���ú������㶥��position0��
		* ��tangent���������ڵ������ε�������Ϊposition0��position1����
		* position2��������ʱ��˳�����У����������Ӧ����������ֱ�Ϊtcoord0, 
		* tcoord1, �� tcoord2��
		*/
		static AVector ComputeTangent (
			const APoint& position0, const Float2& tcoord0,
			const APoint& position1, const Float2& tcoord1,
			const APoint& position2, const Float2& tcoord2);
	};

	PX2_REGISTER_STREAM(Triangles);
	typedef Pointer0<Triangles> TrianglesPtr;
#include "PX2Triangles.inl"

}

#endif
