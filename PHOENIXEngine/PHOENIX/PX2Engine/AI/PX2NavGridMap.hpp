// PX2NavGridMap.hpp

#ifndef PX2NAVGRIDMAP_HPP
#define PX2NAVGRIDMAP_HPP

#include "PX2CorePre.hpp"
#include "PX2Vector2.hpp"
#include "PX2Assert.hpp"
#include "PX2NavHeapNode.hpp"
#include "PX2NavPoint2D.hpp"
#include "PX2NavBitArray.hpp"

namespace PX2
{
	const float FIXPOINT_SCALE = 100.0f;
	class NavLogicMap;

	//Ѱ·���
	class NaviPath
	{
		friend class NavLogicMap;

		struct stPathPoint
		{
			int x; //������Ļ�ϵ���������
			int y;
			int len; //������ڵ㵽�¸���ľ���
		};
		std::vector<stPathPoint> mPoints;
		NavLogicMap *mMap;
		unsigned int mGridUpdateAccum;

	public:
		NaviPath();
		~NaviPath();

		void Reset(NavLogicMap *pmap);
		void Revert();
		void AddPathPoint(int x, int y);
		int  FindNextVisiblePoint(int i);
		Point2D GetPathPoint(int i);
		Point2D GetPathGoal();
		int GetPathSegLen(int i);
		int  GetNumPoint() const;
		void LooseGoal(int radius); //�ı�path, ��path��goal�ں�ԭ��Ŀ�����radius�ĵط�
		void SmoothPath();
		void ComputePathSegLen();
		void LimitPathPoint(int number)
		{
			assert(number >= 2);
			if (number >= int(mPoints.size())) return;

			mPoints.resize(number);
		}

		Point2D MoveOnPath(int &curpt, int &offset, int movelen); //���ص�ǰ�����
		bool IsPointOnPath(const Point2D &pt);
		int  GetPathLength();
		Point2D GetPathPosition(int curpt, int offset);
		bool IsPathOK(int curpt, int offset);

		static bool IsPointInSegment(int x, int y, int x1, int y1, int x2, int y2);
	};

	class NavLogicMap
	{
		friend class NaviPath;

	public:
		enum MaskBits
		{
			maskDynamic = 0x7f,   // ��ɫ���赲������+1�� �뿪-1
			maskStatic = 0x80,   // ��ͼ�����赲
			maskStop = 0xff,   //  
		};
		enum MaskPathResults
		{
			maskPathArrive = 0,  //�ѽ��_��
			maskPathFound = 1,  //�ҵ����Ե��_��·��
			maskPathNear = 2,  //����Ŀ��
			maskPathFail = 3,  //����ʧ��
			maskPathError = -1, //�e�`
		};
		enum MaskStepResults
		{
			maskStepArrive = 0,  //���_
			maskStepOnTheWay = 1,   //��;��
			maskStepObstruct = 2,   //�����
			maskStepOutstretch = 3,   //��������
		};

	public:
		NavLogicMap();
		~NavLogicMap();

		void CreateGraph(int lenghA, int lenghB, int cellsA, int cellsB, unsigned char *pmskbits = NULL);

		bool FindNaviPath(NaviPath &path, const Point2D &start, const Point2D &end);

		/**
		* �ӿ�ʼ���ƶ��������㣬ֻ�������赲
		* \param start ��ʼ�㣬������Ϊ��λ
		* \param delta �ƶ�ʸ����������Ϊ��λ
		* \return �ƶ��Ľ�����
		*/
		Point2D DoMotion(const Point2D &start, const Point2D &delta);

		/**
		* �����Ƿ��ܹ�ֱ�ߴ�start�ƶ���end
		*/
		bool LineOfSightTest(const Point2D &start, const Point2D &end, int mask = maskStop);
		bool LineOfSightTest1(const Point2D &start, const Point2D &end, int mask = maskStop);
		bool LineOfSightTest2(const Point2D &start, const Point2D &end, int mask = maskStop);

		/**
		* ��������ĵ㣬�ڸ����ҳ����ߵĵ㣬�Ӵ˵�����Χ��ɢ����
		* \return �Ƿ��ܹ��ҵ�
		*/
		bool FindPointWalkable(Point2D &point, int range = 50, int mask = maskStatic);
		bool CanWalkPixelCoord(int x, int y, int mask = maskStop);
		bool IsInSameGrid(const Point2D &p1, const Point2D &p2);
		void UpdateMask(int x1, int y1, int x2, int y2, bool b);
		unsigned int GetGridUpdateTime(int x, int y);

		bool CanWalkGridCoord(int x, int y, int mask = maskStop);
		bool IsPointInMap(const Point2D &point);

		//�ڰ뾶Ϊradius��ԲȦ����һ������ʹ�����mask(����ˮ��)
		bool FindDirFaceMask(Point2D &point, const Point2D &center, int radius, int mask);

	public:
		int mlWidthMasks;
		int mlHeightMasks;
		int mlMaskPixelWidth;
		int mlMaskPixelHeight;
		int mlPlaneWidth;
		int mlPlaneHeight;
		unsigned char *mMaskData;
		unsigned int *mGridUpdateTime;
		unsigned int mGridUpdateAccum;

		NavPathHeap mPathHeap;
		BitArray2D *mVisitMap;
		std::vector<stPathHeapNode> mClosedNodes;

		void TryGotoNeighbour(stPathHeapNode &node, int dx, int dy);

		unsigned char GetGridAttrib(int x, int y);
		bool CanWalk(int x, int y, int mask = maskStop);
		void CoordPixel2Grid(int &x, int &y);
		void CoordGrid2Pixel(int &x, int &y);
		/**
		* �ӿ�ʼ�㵽������֮���ҳ�һ��·��
		* \param path ���ڴ洢���ص�·��
		* \param start ��ʼ�㣬������Ϊ��λ
		* \param end �����㣬������Ϊ��λ
		* \return 0: �ɹ���
		*         1�������߼����󣨱����ϴ��ߵ��赲������ȥ�ˣ�
		*         2��Ѱ·�ڵ���ֹ࣬ͣѰ·��
		*         3: ��ʼ���Ŀ�����ͬһ����
		*/
		int FindNaviPath_Internal(NaviPath &path, const Point2D &start, const Point2D &end);

		//��Ŀ�����Χ��Ŀ���
		bool FindSimplePath_Internal(NaviPath &path, const Point2D &start, const Point2D &end, int range);
	};

#include "PX2NavGridMap.inl"

}

#endif