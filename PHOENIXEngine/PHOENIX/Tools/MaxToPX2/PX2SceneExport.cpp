// PX2SceneExport.cpp

#include "PX2SceneExport.hpp"
#include "PX2SceneBuilder.hpp"
#include "PX2MaxClassDesc.hpp"
#include "PX2StringHelp.hpp"
#include <Shlobj.h>
#include "resource.h"

//----------------------------------------------------------------------------
PX2SceneExport::PX2SceneExport ()
{
}
//----------------------------------------------------------------------------
PX2SceneExport::~PX2SceneExport ()
{
}
//----------------------------------------------------------------------------
int PX2SceneExport::ExtCount ()
{
	// ��������������ļ���չ��������ֻ��Ҫһ����*.px2obj����
	return 1;
}
//----------------------------------------------------------------------------
const TCHAR* PX2SceneExport::Ext (int)
{       
	// �����ļ���չ����
	return _T("px2obj");
}
//----------------------------------------------------------------------------
const TCHAR* PX2SceneExport::LongDesc ()
{
	// �ļ�������
	return _T("Phoenix2 model file.");
}
//----------------------------------------------------------------------------
const TCHAR* PX2SceneExport::ShortDesc () 
{           
	// �ļ�������
	return _T("Phoenix2");
}
//----------------------------------------------------------------------------
const TCHAR* PX2SceneExport::AuthorName ()
{           
	// ���������������
	return _T("realmany");
}
//----------------------------------------------------------------------------
const TCHAR* PX2SceneExport::CopyrightMessage () 
{   
	// ��Ȩ��Ϣ
	return
		_T("Phoenix3D, Inc. Copyright (c) 2009-2012. All Rights Reserved.");
}
//----------------------------------------------------------------------------
const TCHAR* PX2SceneExport::OtherMessage1 () 
{       
	// ���ظ�����Ϣ������Ϊ�ա�
	return _T("");
}
//----------------------------------------------------------------------------
const TCHAR* PX2SceneExport::OtherMessage2 () 
{       
	// ���ظ���ĸ�����Ϣ������Ϊ�ա�
	return _T("");
}
//----------------------------------------------------------------------------
unsigned int PX2SceneExport::Version ()
{               
	// ���ص�������汾�š�
	return MAXTOPX2_VERSION;
}
//----------------------------------------------------------------------------
void PX2SceneExport::ShowAbout (HWND)
{           
	// �õ������������ʾһ�����ڶԻ���
}
//----------------------------------------------------------------------------
BOOL PX2SceneExport::SupportsOptions (int, DWORD)
{
	return TRUE;
}
//----------------------------------------------------------------------------
static DWORD WINAPI ProgressBarCallback(LPVOID)
{
	return 0;
}
//----------------------------------------------------------------------------
int PX2SceneExport::DoExport (const TCHAR *filename, ExpInterface *ept,
							 Interface* max, BOOL suppressPrompts, 
							 DWORD options)
{
	// ��Ӳ�̼��ص�����������ļ��������û�����ѡ�����������Phoenix2�ĳ���ͼ��
	// �����ٴν��µ�����д�������ļ���
	//	filename:
	//		�û�Ҫ�����Phoenix2����ͼ�ļ�����
	//	export:
	//		Max�ĵ�������ָ��ӿڡ�
	//	max:
	//		Max��һ��ְ�ܽӿڡ�
	//	suppressPrompts:
	//		�Ƿ������û����롣
	//	options:
	//		ȷ���ǵ����������������ǵ���ѡ��ѡ��ġ�

	mMax = max;
	INode* exportNode = NULL;
	bool exportSelected;

	if (options == SCENE_EXPORT_SELECTED)
	{
		// �û�ѡ�񡰵���ѡ������塱��ȷ�ţ�ѡ��Ľڵ�ֻ��һ�������൱���û�ѡ��
		// ��Ҫ�����ĳ����������ڵ㣩��
		if (mMax->GetSelNodeCount() != 1)
		{
			assertion (false, "ֻ�ܵ���һ��ѡ��ڵ㡣");
			return 0;
		}

		exportNode = mMax->GetSelNode(0);
		exportSelected = true;
	}
	else
	{
		// ����������������Max�ĸ��ڵ㿪ʼ����
		exportNode = mMax->GetRootNode();
		exportSelected = false;
	}

	// ��õ�����������ļ�����λ��
	mStrConfigFile = mMax->GetDir(APP_PLUGCFG_DIR);
	mStrConfigFile += "\\MaxToPX2.cfg";

	// ��ȡ���������������Ϣ
	ReadConfiguration();

	// ��ʾ��������Ի���
	if (!suppressPrompts)
	{
		assertion(PX2MaxClassDesc::msInstance!=0, "msInstance must not be 0");

		INT_PTR piResult = DialogBoxParam(PX2MaxClassDesc::msInstance,
			MAKEINTRESOURCE(IDD_MAXTOPX2_DIALOG), GetActiveWindow(), 
			(DLGPROC)DialogProcedure, (LPARAM)this);

		if (!piResult)
			return 1;
	}

	max->ProgressStart(_T("���ڵ���:"), TRUE, ProgressBarCallback, NULL);

	std::string allFileName = filename;
	std::string outPath;
	std::string outBaseName;
	std::string outExtention;
	PX2::StringHelp::SplitFullFilename(allFileName, outPath, outBaseName, outExtention);
	outExtention = PX2::ToLower<std::string>(outExtention);
	std::string lastFilename = outPath + outBaseName + "." + outExtention;

	// ��Max����ת����Phoenix2����
	SceneBuilder(lastFilename.c_str(), exportSelected, &mEffectiveSettings, ept,
		max, exportNode);

	max->ProgressEnd();

	// ���浼�������������Ϣ
	WriteConfiguration();

	return 1;
}
//----------------------------------------------------------------------------
void PX2SceneExport::ReadConfiguration ()
{
	// ��Ӳ�̼��������ļ������õ��������������Ϣ����������ļ������ڣ�
	// ʹ��ȱʡ���á�

	// �Ӵ��̶�ȡ��������������ļ���
	FILE *configFile = fopen(mStrConfigFile, "rbS");
	if (configFile == NULL)
	{
		return;
	}

	int version;
	if (!fread(&version, sizeof(version), 1, configFile))
	{
		assertion(false, "MaxToPX2 config file is damaged.");
		fclose(configFile);
		return;
	}

	if (version != MAXTOPX2_VERSION)
	{
		fclose(configFile);
		return;
	}

	// ��ȡ��������
	ExportSettings settings;
	if ( fread(&settings, sizeof(settings), 1, configFile) != 1 )
	{
		assertion(false, "MaxToPX2 config file is damaged.");
		fclose(configFile);
		return;
	}

	// ����ȱʡ����
	memcpy(&mSettings, &settings, sizeof(settings));
}
//----------------------------------------------------------------------------
void PX2SceneExport::WriteConfiguration ()
{
	FILE *configFile = fopen(mStrConfigFile, "wbS");
	if (configFile == NULL)
	{
		// ���ܴ��������ļ���������Ŀ¼д�����ˣ���
		assertion(false, "Can't create configFile.");
		return;
	}

	// д��汾��
	int version = MAXTOPX2_VERSION;
	if (fwrite(&version, sizeof(version), 1, configFile) != 1)
	{
		assertion(false, "Can't write configFile.");
		fclose(configFile);
		return;
	}

	// д�뵼�������������Ϣ
	if (fwrite(&mSettings, sizeof(mSettings), 1, configFile) != 1)
	{
		assertion(false, "Can't write configFile.");
		fclose(configFile);
		return;
	}

	fclose(configFile);
}
//----------------------------------------------------------------------------
BOOL CALLBACK PX2SceneExport::DialogProcedure (HWND hWnd, UINT uiMsg,
											   WPARAM wParam, LPARAM lParam) 
{
	static PX2SceneExport *exporter = 0;

	if (uiMsg == WM_INITDIALOG)
	{
		exporter = (PX2SceneExport*)lParam;

		CenterWindow(hWnd, GetParent(hWnd)); 
		SetFocus(hWnd);

		// ��ʼ��ÿ��ʵ���ĶԻ��򴰿�
		return ((PX2SceneExport*)lParam)->OnInitDialog(hWnd);
	}

	switch (uiMsg)
	{
	case WM_COMMAND:
		// �û��ı��˵���ѡ�
		switch (LOWORD(wParam)) 
		{
		case IDC_CHECK_MESHES:
			return exporter->OnMeshChecked(hWnd,
				IsDlgButtonChecked(hWnd, IDC_CHECK_MESHES));
		case IDC_CHECK_TEXTURECOORDS:
			return exporter->OnTexCoordChecked(hWnd,
				IsDlgButtonChecked(hWnd, IDC_CHECK_TEXTURECOORDS));
		case IDC_CHECK_OBJECTS:
			return exporter->OnObjectsChecked(hWnd,
				IsDlgButtonChecked(hWnd, IDC_CHECK_OBJECTS));
		case IDC_CHECK_MODIFIERS:
			return exporter->OnModifiersChecked(hWnd,
				IsDlgButtonChecked(hWnd, IDC_CHECK_MODIFIERS));
		case IDC_RADIO_ALLFRAMES:
		case IDC_RADIO_KEYFRAMES:
			return exporter->OnEnableFrameSpinners(hWnd, TRUE);
		case IDC_RADIO_CURRENTFRAME:
			return exporter->OnEnableFrameSpinners(hWnd, FALSE);
		case IDC_BUTTON_SRC:
			return exporter->OnButtonSrcRootDir(hWnd);
		case IDC_BUTTON_DST:
			return exporter->OnButtonDstRootDir(hWnd);
		case IDOK:
			exporter->OnOK(hWnd);
			EndDialog(hWnd,1);
			break;
		case IDCANCEL:
			EndDialog(hWnd,0);
			break;
		default:
			return FALSE;
		}
		break;

	case CC_SPINNER_CHANGE:
		// �û��ı���spinner�ؼ�
		switch( LOWORD(wParam) )
		{
		case IDC_SPINNER_ENDFRAME:
			return exporter->OnEndFrameSpinnerChanged(hWnd);
		case IDC_SPINNER_STARTFRAME:
			return exporter->OnStartFrameSpinnerChanged(hWnd);
		}
		break;

	case WM_CLOSE:
		EndDialog(hWnd,0);
		return TRUE;
	}

	return FALSE;
}
//----------------------------------------------------------------------------
BOOL PX2SceneExport::OnEnableFrameSpinners (HWND hWnd, BOOL bEnabled)
{
	ISpinnerControl* spinner = GetISpinner(GetDlgItem(hWnd,
		IDC_SPINNER_STARTFRAME)); 
	spinner->Enable(bEnabled);
	ReleaseISpinner(spinner); 
	spinner = GetISpinner(GetDlgItem(hWnd, IDC_SPINNER_ENDFRAME)); 
	spinner->Enable(bEnabled);
	return TRUE;
}
//----------------------------------------------------------------------------
BOOL PX2SceneExport::OnStartFrameSpinnerChanged (HWND hWnd)
{
	// start frame
	ISpinnerControl* spinner = GetISpinner(GetDlgItem(hWnd,
		IDC_SPINNER_STARTFRAME)); 
	int startFrame = spinner->GetIVal();
	ReleaseISpinner(spinner);

	// end frame
	spinner = GetISpinner(GetDlgItem(hWnd,IDC_SPINNER_ENDFRAME)); 
	int endFrame = spinner->GetIVal();
	if ( endFrame < startFrame )
		spinner->SetValue(startFrame,FALSE);
	ReleaseISpinner(spinner); 

	return TRUE;
}
//----------------------------------------------------------------------------
BOOL PX2SceneExport::OnEndFrameSpinnerChanged (HWND hWnd)
{
	// start frame
	ISpinnerControl* spinner = GetISpinner(GetDlgItem(hWnd,
		IDC_SPINNER_STARTFRAME)); 
	int startFrame = spinner->GetIVal();
	ReleaseISpinner(spinner);

	// start frame < end fream
	spinner = GetISpinner(GetDlgItem(hWnd,IDC_SPINNER_ENDFRAME)); 
	int endFrame = spinner->GetIVal();

	if ( startFrame > endFrame )
		spinner->SetValue(startFrame,FALSE);

	ReleaseISpinner(spinner); 
	return TRUE;
}
//----------------------------------------------------------------------------
BOOL PX2SceneExport::OnInitDialog (HWND hWnd)
{
	// ���Ի�����WM_INITDIALOG��Ϣʱ�����г�ʼ����

	// �����û�����ĵ���ѡ���������ļ����ڣ���Щ����ѡ����û����һ��
	// ʹ�õ��������ѡ��һ��������ʹ��ȱʡ�����á�
	CheckDlgButton(hWnd,IDC_CHECK_OBJECTS,mSettings.IncludeObjects);
	CheckDlgButton(hWnd,IDC_CHECK_LIGHTS,mSettings.IncludeLights);
	CheckDlgButton(hWnd,IDC_CHECK_MESHES,mSettings.IncludeMeshes);
	CheckDlgButton(hWnd,IDC_CHECK_VERTEXCOLORS,mSettings.IncludeVertexColors);
	CheckDlgButton(hWnd,IDC_CHECK_NORMALS,mSettings.IncludeNormals);    
	CheckDlgButton(hWnd, IDC_CHECK_TANGENTBINORMAL, 
		mSettings.IncludeTargentBiNormal);
	CheckDlgButton(hWnd,IDC_CHECK_TEXTURECOORDS,mSettings.IncludeTexCoords);

	SendMessage(GetDlgItem(hWnd,IDC_COMMBO_NUMTCS),CB_ADDSTRING,0,(LPARAM)"1");
	SendMessage(GetDlgItem(hWnd,IDC_COMMBO_NUMTCS),CB_ADDSTRING,0,(LPARAM)"2");
	SendMessage(GetDlgItem(hWnd,IDC_COMMBO_NUMTCS), CB_SETCURSEL, 
		mSettings.NumTexCoords-1,0);

	for (int i = 0; i < (int)mSettings.MtlTypes.size(); i++)
	{
		std::string text = mSettings.MtlTypes[i];
		SendMessage(GetDlgItem(hWnd, IDC_COMMBO_MTLTYPE), CB_ADDSTRING, 0, (LPARAM)text.c_str());
	}
	SendMessage(GetDlgItem(hWnd, IDC_COMMBO_MTLTYPE), CB_SETCURSEL, mSettings.MtlType, 0);

	CheckDlgButton(hWnd,IDC_CHECK_MODIFIERS,mSettings.IncludeModifiers);
	CheckDlgButton(hWnd,IDC_CHECK_SKINS,mSettings.IncludeSkins);
	CheckDlgButton(hWnd,IDC_RADIO_CURRENTFRAME,mSettings.IncludeCurrentFrame);
	CheckDlgButton(hWnd,IDC_RADIO_KEYFRAMES,mSettings.IncludeKeyFrames);
	CheckDlgButton(hWnd,IDC_RADIO_ALLFRAMES,mSettings.IncludeAllFrames);
	CheckDlgButton(hWnd,IDC_CHECK_USELOCALTIME,mSettings.UseLocalTime);

	if(!mSettings.IncludeObjects)
	{
		// ȡ�����Objects��ѡ���
		OnObjectsChecked(hWnd, FALSE);
	}
	else if (!mSettings.IncludeMeshes)
	{
		// ȡ�����Mesh��ѡ���
		OnMeshChecked(hWnd, FALSE);
	}

	// ȡ������"Modifiers"ѡ���
	if (!mSettings.IncludeModifiers)
		OnModifiersChecked(hWnd, FALSE);

	// ���max�����е�֡��
	int startFrame = mMax->GetAnimRange().Start()/GetTicksPerFrame();
	int endFrame = mMax->GetAnimRange().End()/GetTicksPerFrame();
	int numFrames = mMax->GetAnimRange().Duration()/GetTicksPerFrame();
	PX2_UNUSED(numFrames);

	// spinner controls
	ISpinnerControl* spinner = GetISpinner(GetDlgItem(hWnd,
		IDC_SPINNER_STARTFRAME));
	spinner->LinkToEdit(GetDlgItem(hWnd,IDC_EDIT_STARTFRAME), EDITTYPE_INT);
	spinner->SetLimits(0, endFrame, TRUE);
	spinner->SetScale(1.0f);
	spinner->SetValue(startFrame, FALSE);
	if ( mSettings.IncludeCurrentFrame )
	{
		spinner->Enable(FALSE);
	}
	ReleaseISpinner(spinner);

	spinner = GetISpinner(GetDlgItem(hWnd,IDC_SPINNER_ENDFRAME));
	spinner->LinkToEdit(GetDlgItem(hWnd,IDC_EDIT_ENDFRAME),EDITTYPE_INT);
	spinner->SetLimits(0, endFrame,TRUE);
	spinner->SetScale(1.0f);
	spinner->SetValue(endFrame,FALSE);
	if (mSettings.IncludeCurrentFrame)
	{
		spinner->Enable(FALSE);
	}
	ReleaseISpinner(spinner);

	SetDlgItemText(hWnd, IDC_EDIT_SRCROOTDIR, mSettings.SrcRootDir);
	SetDlgItemText(hWnd, IDC_EDIT_DSTROOTDIR, mSettings.DstRootDir);

	return TRUE;
}
//----------------------------------------------------------------------------
BOOL PX2SceneExport::OnMeshChecked (HWND hWnd, BOOL bEnabled)
{
	// ��Mesh��ѡ��򱻸��£�������ѡ���
	EnableWindow(GetDlgItem(hWnd,IDC_CHECK_VERTEXCOLORS), bEnabled);
	EnableWindow(GetDlgItem(hWnd,IDC_CHECK_NORMALS), bEnabled);
	EnableWindow(GetDlgItem(hWnd,IDC_CHECK_TANGENTBINORMAL), bEnabled);
	EnableWindow(GetDlgItem(hWnd,IDC_CHECK_TEXTURECOORDS), bEnabled);
	OnTexCoordChecked(hWnd, bEnabled);

	WriteConfiguration();

	return TRUE;
}
//----------------------------------------------------------------------------
BOOL PX2SceneExport::OnTexCoordChecked (HWND wnd, BOOL enabled)
{
	EnableWindow(GetDlgItem(wnd,IDC_COMMBO_NUMTCS), enabled);
	EnableWindow(GetDlgItem(wnd,IDC_STATIC_NUMTC), enabled);

	WriteConfiguration();

	return TRUE;
}
//----------------------------------------------------------------------------
BOOL PX2SceneExport::OnModifiersChecked (HWND hWnd, BOOL bEnabled)
{
	// ��Modifiers��ѡ��򱻸��£�������ѡ���
	EnableWindow(GetDlgItem(hWnd,IDC_CHECK_SKINS), bEnabled);

	WriteConfiguration();

	return TRUE;
}
//----------------------------------------------------------------------------
BOOL PX2SceneExport::OnObjectsChecked (HWND hWnd, BOOL bEnabled)
{
	// ����Objects�������£�������ѡ���
	EnableWindow(GetDlgItem(hWnd,IDC_CHECK_MESHES), bEnabled);
	EnableWindow(GetDlgItem(hWnd,IDC_CHECK_LIGHTS), bEnabled);   

	// ��Mesh��ѡ��򱻸��£�������ѡ���
	OnMeshChecked(hWnd,bEnabled && IsDlgButtonChecked(hWnd, IDC_CHECK_MESHES));

	WriteConfiguration();

	return TRUE;
}
//----------------------------------------------------------------------------
static int SelectFolder (HWND hParent, char* folder)
{
	LPMALLOC lpMalloc;

	if (::SHGetMalloc(&lpMalloc) != NOERROR)
		return 0;

	char szDisplayName[_MAX_PATH];
	char szBuffer[_MAX_PATH];

	ITEMIDLIST *ppidl;
	SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP ,&ppidl);

	BROWSEINFO browseInfo;
	browseInfo.hwndOwner = hParent;
	browseInfo.pidlRoot = ppidl; // set root at Desktop
	browseInfo.pszDisplayName = szDisplayName;
	browseInfo.lpszTitle = "ѡ��Ŀ¼";
	browseInfo.ulFlags = BIF_RETURNFSANCESTORS|BIF_RETURNONLYFSDIRS;
	browseInfo.lpfn = NULL; //BrowseCallbackProc;
	browseInfo.lParam = 0;

	LPITEMIDLIST lpItemIDList;
	if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo)) != NULL)
	{
		// Get the path of the selected folder from the    item ID list.
		if (::SHGetPathFromIDList(lpItemIDList, szBuffer))
		{
			// At this point, szBuffer contains the path the user chose.
			if (szBuffer[0] == '\0')
				return 0;

			// We have a path in szBuffer! Return it.
			strcpy(folder, szBuffer);
			size_t len = strlen(folder);
			if(folder[len]=='\\')
				folder[len] = 0;

			lpMalloc->Free(lpItemIDList);
			lpMalloc->Release();

			return 1;
		}
	}

	return 1;
}
//----------------------------------------------------------------------------
BOOL PX2SceneExport::OnButtonSrcRootDir (HWND wnd)
{
	SelectFolder(wnd, mSettings.SrcRootDir);
	SetDlgItemText(wnd, IDC_EDIT_SRCROOTDIR, mSettings.SrcRootDir);

	WriteConfiguration();

	return TRUE;
}
//----------------------------------------------------------------------------
BOOL PX2SceneExport::OnButtonDstRootDir (HWND wnd)
{
	SelectFolder(wnd, mSettings.DstRootDir);
	SetDlgItemText(wnd, IDC_EDIT_DSTROOTDIR, mSettings.DstRootDir);

	WriteConfiguration();

	return TRUE;
}
//----------------------------------------------------------------------------
void PX2SceneExport::OnOK (HWND hWnd)
{
	// ���û����¶Ի����OK������õ�������ĵ���ѡ�

	mSettings.IncludeObjects =
		IsDlgButtonChecked(hWnd, IDC_CHECK_OBJECTS) == 1;
	mSettings.IncludeLights =
		IsDlgButtonChecked(hWnd, IDC_CHECK_LIGHTS) == 1;
	mSettings.IncludeMeshes =
		IsDlgButtonChecked(hWnd, IDC_CHECK_MESHES) == 1;
	mSettings.IncludeVertexColors =
		IsDlgButtonChecked(hWnd, IDC_CHECK_VERTEXCOLORS) == 1;
	mSettings.IncludeNormals =
		IsDlgButtonChecked(hWnd, IDC_CHECK_NORMALS) == 1;
	mSettings.IncludeTargentBiNormal =
		IsDlgButtonChecked(hWnd, IDC_CHECK_TANGENTBINORMAL) == 1;
	mSettings.IncludeTexCoords =
		IsDlgButtonChecked(hWnd, IDC_CHECK_TEXTURECOORDS) == 1;
	mSettings.NumTexCoords = 
		1 + (int)SendMessage(GetDlgItem(hWnd, IDC_COMMBO_NUMTCS), CB_GETCURSEL, 0, 0);

	mSettings.MtlType = 
		(int)SendMessage(GetDlgItem(hWnd, IDC_COMMBO_MTLTYPE), CB_GETCURSEL, 0, 0);

	mSettings.IncludeModifiers =
		IsDlgButtonChecked(hWnd, IDC_CHECK_MODIFIERS) == 1;
	mSettings.IncludeSkins =
		IsDlgButtonChecked(hWnd, IDC_CHECK_SKINS) == 1;
	mSettings.IncludeAllFrames =
		IsDlgButtonChecked(hWnd, IDC_RADIO_ALLFRAMES) == 1;
	mSettings.IncludeKeyFrames =
		IsDlgButtonChecked(hWnd, IDC_RADIO_KEYFRAMES) == 1;
	mSettings.IncludeCurrentFrame =
		IsDlgButtonChecked(hWnd, IDC_RADIO_CURRENTFRAME) == 1;
	mSettings.UseLocalTime =
		IsDlgButtonChecked(hWnd, IDC_CHECK_USELOCALTIME) == 1;

	// Start and End frames
	if ( mSettings.IncludeAllFrames || mSettings.IncludeKeyFrames )
	{
		ISpinnerControl* spinner = GetISpinner(GetDlgItem(hWnd,
			IDC_SPINNER_STARTFRAME)); 
		mSettings.StartFrame = spinner->GetIVal();
		ReleaseISpinner(spinner);

		spinner = GetISpinner(GetDlgItem(hWnd,IDC_SPINNER_ENDFRAME)); 
		mSettings.EndFrame = spinner->GetIVal();
		ReleaseISpinner(spinner);
	}

	mEffectiveSettings = mSettings;
	if (!mSettings.IncludeObjects)
	{
		mEffectiveSettings.IncludeLights = false;
		mEffectiveSettings.IncludeMeshes = false;
	}
	if (!mEffectiveSettings.IncludeMeshes)
	{
		mEffectiveSettings.IncludeVertexColors = false;
		mEffectiveSettings.IncludeNormals = false;
		mEffectiveSettings.IncludeTargentBiNormal = false;
		mEffectiveSettings.IncludeTexCoords = false;
	}

	if (!mSettings.IncludeModifiers)
	{
		mEffectiveSettings.IncludeSkins = false;
	}

	GetDlgItemText(hWnd, IDC_EDIT_SRCROOTDIR, mSettings.SrcRootDir, 256);
	GetDlgItemText(hWnd, IDC_EDIT_DSTROOTDIR, mSettings.DstRootDir, 256);

	WriteConfiguration();
}
//----------------------------------------------------------------------------