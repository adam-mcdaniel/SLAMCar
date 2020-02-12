// PX2VertexFormat.hpp

#ifndef PX2VERTEXFORMAT_HPP
#define PX2VERTEXFORMAT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"

namespace PX2
{
	
	/// �����ʽ��
	class PX2_ENGINE_ITEM VertexFormat : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(VertexFormat);

	public:
		enum
		{
			// һ�������ʽ���ͨ����
			AM_MAX_ATTRIBUTES = 16,

			// һ�������ʽ�����������ͨ����
			AM_MAX_TCOORD_UNITS = 8,

			// һ�������ʽ��ɫͨ����
			AM_MAX_COLOR_UNITS = 2
		};

		/// ÿ��ͨ�������ݸ�ʽ
		enum AttributeType
		{
			AT_NONE,
			AT_FLOAT1,
			AT_FLOAT2,
			AT_FLOAT3,
			AT_FLOAT4,
			AT_HPPALF1,
			AT_HPPALF2,
			AT_HPPALF3,
			AT_HPPALF4,
			AT_UBYTE4,
			AT_SHORT1,
			AT_SHORT2,
			AT_SHORT4,
			AT_QUANTITY
		};

		/// �����ʽÿ��ͨ������
		enum AttributeUsage
		{
			AU_NONE,
			AU_POSITION,      // attr 0
			AU_NORMAL,        // attr 2
			AU_TANGENT,       // attr 14
			AU_BINORMAL,      // attr 15
			AU_TEXCOORD,      // attr 8-15
			AU_COLOR,         // attr 3-4
			AU_BLENDINDICES,  // attr 7
			AU_BLENDWEIGHT,   // attr 1
			AU_FOGCOORD,      // attr 5
			AU_PSIZE,         // attr 6
			AU_QUANTITY
		};

		VertexFormat ();
		VertexFormat (int numAttributes);
		virtual ~VertexFormat ();

		/// ���������ʽ
		/**
		* numAttributes��ʾ����ͨ���ĸ�����ʣ�²����������˳��
		*	(AttributeUsage, AttributeType, usage index)
		*	 ...
		*/
		static VertexFormat* Create (int numAttributes, ...);

		/// ���������ʽ
		void Add (AttributeUsage usage, AttributeType type, unsigned int usageIndex);
		void Create ();

		/// ���ø���ͨ���Ķ�������
		void SetAttribute (int attribute, unsigned int streamIndex,
			unsigned int offset, AttributeType type, AttributeUsage usage,
			unsigned int usageIndex);

		void SetStride (int stride);

		/// ���ͨ������
		inline int GetNumAttributes () const;

		inline unsigned int GetStreamIndex (int attribute) const;
		inline unsigned int GetOffset (int attribute) const;
		inline AttributeType GetAttributeType (int attribute) const;
		inline AttributeUsage GetAttributeUsage (int attribute) const;
		inline unsigned int GetUsageIndex (int attribute) const;

		void GetAttribute (int attribute, unsigned int& streamIndex,
			unsigned int& offset, AttributeType& type, AttributeUsage& usage,
			unsigned int& usageIndex) const;

		inline int GetStride () const;

		/// ���usage���Ե�ͨ������
		/**
		* ���usage�ڶ����ʽ�д��ڣ����ص�����i���㣬0 <= i < GetNumAttributes()��
		* ���usage�����ڣ�����-1��
		*/
		int GetIndex (AttributeUsage usage, unsigned int usageIndex = 0) const;

		/// ���type��ÿ��Ԫ����ֽڴ�С
		inline static int GetComponentSize (AttributeType type);

		/// ���type��Ԫ�����
		inline static int GetNumComponents (AttributeType type);

		/// ÿ��ͨ�����ֽڴ�С
		inline static int GetTypeSize (AttributeType type);

		// pdr
	public:
		void *PdrPointer;

		/// �����ʽͨ����Ϣ
		class Element
		{
		public:
			unsigned int StreamIndex;
			unsigned int Offset;
			AttributeType Type;
			AttributeUsage Usage;
			unsigned int UsageIndex;
		};

		int mOffsetTemp; // ���������ʽ�ã����ô洢

		int mNumAttributes;
		Element mElements[AM_MAX_ATTRIBUTES];
		int mStride;

		static int msComponentSize[AT_QUANTITY]; //< ÿ��AttributeType��ÿ��Ԫ����ֽڴ�С
		static int msNumComponents[AT_QUANTITY]; //< ÿ��AttributeType��Ԫ�����
		static int msTypeSize[AT_QUANTITY]; //< ÿ��AttributeType���ֽڴ�С��msComponentSize[i]*msNumComponents[i]��
	};

	PX2_REGISTER_STREAM(VertexFormat);
	typedef Pointer0<VertexFormat> VertexFormatPtr;
#include "PX2VertexFormat.inl"

}

#endif
