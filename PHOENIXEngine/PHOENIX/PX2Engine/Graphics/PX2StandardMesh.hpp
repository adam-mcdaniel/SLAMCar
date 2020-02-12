// PX2StandardMesh.hpp

#ifndef PX2STANDARDMESH_HPP
#define PX2STANDARDMESH_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2VertexBufferAccessor.hpp"
#include "PX2TriMesh.hpp"
#include "PX2TriFan.hpp"
#include "PX2Polysegment.hpp"
#include "PX2Node.hpp"

namespace PX2
{

	/// ��׼mesh������
	/**	
	* ÿ��mesh��(0,0,0)�������������ϵ�������Ϊ(0,0,1)������ϵͳ����������Ϊ(1,
	* 0,0)��(0,1,0)��Ӧ�ó�������ٱ�Ҫ��������滻meshes��iXSamples��ʾX����
	* �������iYSamples��ʾY���򶥵������Ӧ�ó������ͨ������'transform'������
	* �����н��б�Ҫ�ı任��inside���������Ƿ���ڲ��۲죬�⽫����������������
	* ��˳������'isStatic'Ϊtrue������VertexBufferʹ��Buffer::BU_STATIC������
	* ʹ��Buffer::BU_DYNAMIC��
	*/
	class PX2_ENGINE_ITEM StandardMesh
	{
	public:
		StandardMesh (VertexFormat* vformat, bool isStatic = true,
			bool inside = false, const Transform* transform = 0);

		~StandardMesh ();

		// �ռ�任
		void SetVertexColor (Float4 color);
		Float4 GetVertexColor ();
		void SetTransform (const Transform& transform);
		const Transform& GetTransform () const;

		// The comments for the various objects are for the case when the input
		// 'transform' is NULL (the identity transform).

		// The rectangle is in the plane z = 0 and is visible to an observer who
		// is on the side of the plane to which the normal (0,0,1) points.  It has
		// corners (-xExtent, -yExtent, 0), (+xExtent, -yExtent, 0),
		// (-xExtent, +yExtent, 0), and (+xExtent, +yExtent, 0).  The mesh has
		// xSamples vertices in the x-direction and ySamples vertices in the
		// y-direction for a total of xSamples*ySamples vertices.
		TriMesh* Rectangle (int xSamples, int ySamples, float xExtent,
			float yExtent, const Float2 &anchorPoint=Float2(0.5f, 0.5f));

		// The circular disk is in the plane z = 0 and is visible to an observer
		// who is on the side of the plane to which the normal (0,0,1) points.
		// It has center (0,0,0) and the specified 'radius'.  The mesh has its
		// first vertex at the center.  Samples are placed along rays whose
		// common origin is the center.  There are 'radialSamples' rays.  Along
		// each ray the mesh has 'shellSamples' vertices.
		TriMesh* Disk (int shellSamples, int radialSamples, float radius);

		// The box has center (0,0,0); unit-length axes (1,0,0), (0,1,0), and
		// (0,0,1); and extents (half-lengths) 'xExtent', 'yExtent', and
		// 'zExtent'.  The mesh has 8 vertices and 12 triangles.  For example,
		// the box corner in the first octant is (xExtent, yExtent, zExtent).
		TriMesh* Box (float xExtent, float yExtent, float zExtent);
		TriMesh* Box1 (float xExtent, float yExtent, float zExtent);
		Node *BoxPieces(int samples, float extent);

		// The cylinder has center (0,0,0), the specified 'radius', and the
		// specified 'height'.  The cylinder axis is a line segment of the
		// form (0,0,0) + t*(0,0,1) for |t| <= height/2.  The cylinder wall
		// is implicitly defined by x^2+y^2 = radius^2.  If 'open' is 'true',
		// the cylinder end-disks are omitted; you have an open tube.  If
		// 'open' is 'false', the end-disks are included.  Each end-disk is
		// a regular polygon that is tessellated by including a vertex at
		// the center of the polygon and decomposing the polygon into triangles
		// that all share the center vertex and each triangle containing an
		// edge of the polygon.
		TriMesh* Cylinder (int axisSamples, int radialSamples, float radius,
			float height, bool open);

		// The sphere has center (0,0,0) and the specified 'radius'.  The north
		// pole is at (0,0,radius) and the south pole is at (0,0,-radius).  The
		// mesh has the topology of an open cylinder (which is also the topology
		// of a rectangle with wrap-around for one pair of parallel edges) and
		// is then stitched to the north and south poles.  The triangles are
		// unevenly distributed.  If you want a more even distribution, create
		// an icosahedron and subdivide it.
		TriMesh* Sphere (int zSamples, int radialSamples, float radius);

		// The torus has center (0,0,0).  If you observe the torus along the
		// line with direction (0,0,1), you will see an annulus.  The circle
		// that is the center of the annulus has radius 'outerRadius'.  The
		// distance from this circle to the boundaries of the annulus is the
		// 'inner radius'.
		TriMesh* Torus (int circleSamples, int radialSamples, float outerRadius,
			float innerRadius);

		TriFan *Cone (int samples, float radius, float height);

		// Platonic solids, all inscribed in a unit sphere centered at (0,0,0).
		TriMesh* Tetrahedron ();
		TriMesh* Hexahedron ();
		TriMesh* Octahedron ();
		TriMesh* Dodecahedron ();
		TriMesh* Icosahedron ();

		/// ��������
		/**
		* �༭��ʹ�õ���ʽ�������᲻��Ҫ��������
		* xSamplesΪx���򶥵�����
		* xExtentΪx�����ȵ�һ��
		*/
		Polysegment *LineRectangleForEditor (int xSamples, int ySamples, float xExtent,
			float yExtent, Float4 color);

		/**
		* �༭��ʹ�õ���ʽ�������ᴴ����
		*/
		Polysegment *LineRectangleForEditor1 (int xSamples, int ySamples, float xExtent,
			float yExtent, Float4 color);

		// ��������
		Polysegment *Rectangle(float xLength, float yLength);

		/// ����Բ
		/**
		* samplesΪ����(��β���)
		*/
		Polysegment *Circle (float radius, int samples);

		///  ������Χ����Ȧ
		Node *CircleSphere (float radius, int samples, Float4 color0=Float4::RED,
			Float4 color1 = Float4::GREEN, Float4 color2 = Float4::BLUE, bool alpha=false);

	private:
		void TransformData (VertexBufferAccessor& vba);
		void ReverseTriangleOrder (int numTriangles, unsigned short* indices);
		void CreatePlatonicNormals (VertexBufferAccessor& vba);
		void CreatePlatonicColors (VertexBufferAccessor& vba);
		void CreatePlatonicUVs (VertexBufferAccessor& vba);

		enum { MAX_UNITS = VertexFormat::AM_MAX_TCOORD_UNITS };
		VertexFormatPtr mVFormat;
		Float3 mVertexColor3;
		Float4 mVertexColor4;
		Transform mTransform;
		bool mIsStatic, mInside, mHasNormals, mHasVertexColor;
		int mColorNum; // 3, 4
		bool mHasTCoords[MAX_UNITS];
		Buffer::Usage mUsage;
	};

}

#endif
