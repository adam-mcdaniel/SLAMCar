// PX2ConvexPolygon2.hpp

#ifndef PX2CONVEXPOLYGON2_HPP
#define PX2CONVEXPOLYGON2_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Polygon2.hpp"
#include <set>

namespace PX2
{

	/// ��ά͹�������
	/**
	* �����߱���ȷ�����붥������ʱ��˳��
	*/
	template <typename Real>
	class ConvexPolygon2 : public Polygon2<Real>
	{
	public:
		// �ߵı�����ʽΪDot(N,X) = c����һ��pair�ǵ�λ���ȵķ��������ڶ�������
		// �ĳ���c��X�����ϵ�����һ�㡣
		typedef typename std::pair<Vector2<Real>, Real> NCLine;

		ConvexPolygon2 (int numVertices, Vector2<Real>* vertices, NCLine* lines);
		ConvexPolygon2 (const ConvexPolygon2& polygon);
		virtual ~ConvexPolygon2 ();

		// ��ֵ
		ConvexPolygon2& operator= (const ConvexPolygon2& polygon);

		// ��Ա����
		const NCLine* GetLines () const;
		const NCLine& GetLine (int i) const;

		// �޸Ķ��㣬�����߱���ȷ�������ɵĶ������͹��
		virtual void SetVertex (int i, const Vector2<Real>& vertex);
		void UpdateLines ();

		// Test for convexity.  This function will iterate over the edges of the
		// polygon and verify for each edge that the polygon vertices are all on
		// the nonnegative side of the edge line.  A signed distance test is used,
		// so a vertex is on the wrong side of a line (for convexity) when its
		// signed distance satisfies d < 0.  Numerical round-off errors can
		// generate incorrect convexity tests, so a small negative threshold
		// t may be passed to this function, in which case the distance test
		// becomes d < t < 0.
		// ��⼸��ͼ���Ƿ���͹��
		bool IsConvex (Real threshold = (Real)0) const;

		// Point-in-polygon test, performed by which-side queries between the
		// point and the lines of the edges, an O(n) algorithm for n vertices.
		// This is not the optimal algorithm.  TODO:  Move the binary-search-based
		// algorithm to this class.  It is an O(log n) algorithm.
		bool Contains (const Vector2<Real>& p, Real threshold = (Real)0)
			const;

	protected:
		using Polygon2<Real>::mNumVertices;
		using Polygon2<Real>::mVertices;
		using Polygon2<Real>::ComputeVertexAverage;

		// Support for efficient updating of edge lines.  The set stores the
		// indices for those edges that share modified vertices.
		void UpdateLine (int i, const Vector2<Real>& average);

		// �ж��ٸ����㣬���ж�������
		Pointer1<NCLine> mLines;

		// The sharing edges processed by UpdateLine.
		std::set<int> mSharingEdges;
	};

#include "PX2ConvexPolygon2.inl"

	typedef ConvexPolygon2<float> ConvexPolygon2f;
	typedef ConvexPolygon2<double> ConvexPolygon2d;

}

#endif