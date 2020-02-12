// PX2ExportSettings.cpp

#include "PX2ExportSettings.hpp"

//----------------------------------------------------------------------------
std::vector<std::string> ExportSettings::MtlTypes;
//----------------------------------------------------------------------------
ExportSettings::ExportSettings ()
{
	// object ����
	IncludeObjects = true;
	IncludeLights = true;
	IncludeMeshes = true;

	// mesh ����
	IncludeVertexColors = false;
	IncludeNormals = true;
	IncludeTargentBiNormal = false;
	IncludeTexCoords = true;
	NumTexCoords = 1;

	MtlTypes.clear();
	MtlTypes.push_back("materialcolor");
	MtlTypes.push_back("std_light");
	MtlTypes.push_back("std_lightnormal");
	MtlTypes.push_back("skinskeleton");
	MtlTypes.push_back("tex2d");
	MtlType = 0;

	// modifier ����    
	IncludeModifiers = true;
	IncludeSkins = true;

	// animation ����
	IncludeCurrentFrame = true;
	IncludeKeyFrames = false;
	IncludeAllFrames = false;
	UseLocalTime = true;
	StartFrame = 0;
	EndFrame = 0;

	memset(SrcRootDir, 0, 256);
	memset(DstRootDir, 0, 256);
}
//----------------------------------------------------------------------------
const std::string &ExportSettings::GetMtlTypeStr() const
{
	return MtlTypes[MtlType];
}
//----------------------------------------------------------------------------