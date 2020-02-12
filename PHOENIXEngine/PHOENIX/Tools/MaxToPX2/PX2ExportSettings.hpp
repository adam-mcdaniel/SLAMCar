// PX2ExportSettings.hpp

#ifndef PX2EXPORTSETTINGS_HPP
#define PX2EXPORTSETTINGS_HPP

#include "PX2MaxToPX2PluginPre.hpp"

class ExportSettings
{
public:
	ExportSettings ();

	// object ����
	bool IncludeObjects;
	bool IncludeLights;
	bool IncludeMeshes;

	// mesh ����
	bool IncludeVertexColors; 
	bool IncludeNormals;
	bool IncludeTargentBiNormal;
	bool IncludeTexCoords;
	int NumTexCoords;

	static std::vector<std::string> MtlTypes;
	int MtlType;
	const std::string &GetMtlTypeStr() const;

	// modifier ����
	bool IncludeModifiers;
	bool IncludeSkins;

	// animation ����
	bool IncludeCurrentFrame;
	bool IncludeKeyFrames;
	bool IncludeAllFrames;
	bool UseLocalTime;
	int StartFrame;
	int EndFrame;

	// Ŀ¼
	char SrcRootDir[255];
	char DstRootDir[255];
};

#endif