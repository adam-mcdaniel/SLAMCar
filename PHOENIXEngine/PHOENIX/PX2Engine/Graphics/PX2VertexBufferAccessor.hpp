// PX2VertexBufferAccessor.hpp

#ifndef PX2VERTEXBUFFERACCESSOR_HPP
#define PX2VERTEXBUFFERACCESSOR_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2VertexBuffer.hpp"
#include "PX2VertexFormat.hpp"
#include "PX2APoint.hpp"
#include "PX2FileIO.hpp"

namespace PX2
{

	class Renderable;

	/// ���㻺������������
	/**
	* ͨ�����࣬���Է���ط���VertexBuffer���޸�VertexBuffer��
	*/
	class PX2_ENGINE_ITEM VertexBufferAccessor
	{
	public:
		VertexBufferAccessor ();
		VertexBufferAccessor (VertexFormat* vformat, VertexBuffer* vbuffer);
		VertexBufferAccessor (Renderable* renderable);
		~VertexBufferAccessor ();

		/*
		* ������Ҫ�����buffer��
		*/
		void ApplyTo (VertexFormat* vformat, VertexBuffer* vbuffer);
		void ApplyTo (Renderable* renderable);

		inline char* GetData () const;
		inline int GetNumVertices () const;
		inline int GetStride () const;

		// ���ʶ��㻺�����ķ���
		template <typename T>
		inline T& Position (int i);
		inline bool HasPosition () const;
		inline int GetPositionChannels () const;
		inline float* PositionTuple (int i);

		template <typename T>
		inline T& Normal (int i);
		inline bool HasNormal () const;
		inline int GetNormalChannels () const;
		inline float* NormalTuple (int i);
		void SetNormal3 (int i, const AVector& normal);
		AVector GetNormal3 (int i) const;

		template <typename T>
		inline T& Tangent (int i);
		inline bool HasTangent () const;
		inline int GetTangentChannels () const;
		inline float* TangentTuple (int i);

		template <typename T>
		inline T& Binormal (int i);
		inline bool HasBinormal () const;
		inline int GetBinormalChannels () const;
		inline float* BinormalTuple (int i);

		template <typename T>
		inline T& TCoord (int unit, int i);
		inline bool HasTCoord (int unit) const;
		inline int GetTCoordChannels (int unit) const;
		inline float* TCoordTuple (int unit, int i);

		template <typename T>
		inline T& Color (int unit, int i);
		inline bool HasColor (int unit) const;
		inline int GetColorChannels (int unit) const;
		inline float* ColorTuple (int unit, int i);

		template <typename T>
		inline T& BlendIndices (int i);
		inline bool HasBlendIndices () const;

		template <typename T>
		inline T& BlendWeight (int i);
		inline bool HasBlendWeight () const;

		// �ļ���ȡ��д�루��endianness�жϣ�
		void Read (FileIO& inFile);
		void Write (FileIO& outFile);

	private:
		// ���캯������
		void Initialize ();

		VertexFormat* mVFormat;
		VertexBuffer* mVBuffer;
		int mStride;
		char* mData;

		char* mPosition;
		char* mNormal;
		char* mTangent;
		char* mBinormal;
		char* mTCoord[VertexFormat::AM_MAX_TCOORD_UNITS];
		char* mColor[VertexFormat::AM_MAX_COLOR_UNITS];
		char* mBlendIndices;
		char* mBlendWeight;

		int mPositionChannels;
		int mNormalChannels;
		int mTangentChannels;
		int mBinormalChannels;
		int mTCoordChannels[VertexFormat::AM_MAX_TCOORD_UNITS];
		int mColorChannels[VertexFormat::AM_MAX_COLOR_UNITS];
	};

#include "PX2VertexBufferAccessor.inl"

}

#endif
