// PX2RobotExtData.hpp

#ifndef PX2ROBOTEXTDATA_HPP
#define PX2ROBOTEXTDATA_HPP

#include "PX2Vector2.hpp"
#include "PX2RobotEx_Pre.hpp"

namespace PX2
{

	struct SLAM_DLL_ITEM RobotState
	{
		RobotState();
		~RobotState();

		// x���꣬y���꣬���������ٶȣ����ٶ�
		float PosX;
		float PosY;
		float Orientation;
		float LeftSpeed;
		float RightSpeed;
	};

	struct SLAM_DLL_ITEM EvaluationPara
	{
		EvaluationPara();
		~EvaluationPara();

		int Index;

		float DiffHeading;
		float Clearance;
		float Leftspped;
		float RightSpeed;
		float Speed;

		float LastValue;
	};

}

#endif