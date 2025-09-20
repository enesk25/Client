#include "StdAfx.h"
#include "PythonSystem.h"
#include "PythonApplication.h"

#define DEFAULT_VALUE_ALWAYS_SHOW_NAME		true

void CPythonSystem::SetInterfaceHandler(PyObject* poHandler)
{
	// NOTE : ���۷��� ī��Ʈ�� �ٲ��� �ʴ´�. ���۷����� ���� �־� Python���� ������ �������� �ʱ� ����.
	//        ��ſ� __del__�� Destroy�� ȣ���� Handler�� NULL�� �����Ѵ�. - [levites]
	//	if (m_poInterfaceHandler)
	//		Py_DECREF(m_poInterfaceHandler);

	m_poInterfaceHandler = poHandler;

	//	if (m_poInterfaceHandler)
	//		Py_INCREF(m_poInterfaceHandler);
}

void CPythonSystem::DestroyInterfaceHandler()
{
	m_poInterfaceHandler = NULL;
}

void CPythonSystem::SaveWindowStatus(int iIndex, int iVisible, int iMinimized, int ix, int iy, int iHeight)
{
	m_WindowStatus[iIndex].isVisible = iVisible;
	m_WindowStatus[iIndex].isMinimized = iMinimized;
	m_WindowStatus[iIndex].ixPosition = ix;
	m_WindowStatus[iIndex].iyPosition = iy;
	m_WindowStatus[iIndex].iHeight = iHeight;
}

void CPythonSystem::GetDisplaySettings()
{
	memset(m_ResolutionList, 0, sizeof(TResolution) * RESOLUTION_MAX_NUM);
	m_ResolutionCount = 0;

	LPDIRECT3D8 lpD3D = CPythonGraphic::Instance().GetD3D();

	D3DADAPTER_IDENTIFIER8 d3dAdapterIdentifier;
	D3DDISPLAYMODE d3ddmDesktop;

	lpD3D->GetAdapterIdentifier(0, D3DENUM_NO_WHQL_LEVEL, &d3dAdapterIdentifier);
	lpD3D->GetAdapterDisplayMode(0, &d3ddmDesktop);

	// �� ��Ͱ� ������ �ִ� ���÷��� ��尹���� �����Ѵ�..
	DWORD dwNumAdapterModes = lpD3D->GetAdapterModeCount(0);

	for (UINT iMode = 0; iMode < dwNumAdapterModes; iMode++)
	{
		D3DDISPLAYMODE DisplayMode;
		lpD3D->EnumAdapterModes(0, iMode, &DisplayMode);
		DWORD bpp = 0;

		// 800 600 �̻� �ɷ�����.
		if (DisplayMode.Width < 800 || DisplayMode.Height < 600)
			continue;

		// �ϴ� 16bbp �� 32bbp�� �������.
		// 16bbp�� ó���ϰԲ� ���� - [levites]
		if (DisplayMode.Format == D3DFMT_R5G6B5)
			bpp = 16;
		else if (DisplayMode.Format == D3DFMT_X8R8G8B8)
			bpp = 32;
		else
			continue;

		int check_res = false;

		for (int i = 0; !check_res && i < m_ResolutionCount; ++i)
		{
			if (m_ResolutionList[i].bpp != bpp ||
				m_ResolutionList[i].width != DisplayMode.Width ||
				m_ResolutionList[i].height != DisplayMode.Height)
				continue;

			int check_fre = false;

			// �������ø� �ٸ��Ƿ� �������ø� �������ش�.
			for (int j = 0; j < m_ResolutionList[i].frequency_count; ++j)
			{
				if (m_ResolutionList[i].frequency[j] == DisplayMode.RefreshRate)
				{
					check_fre = true;
					break;
				}
			}

			if (!check_fre)
				if (m_ResolutionList[i].frequency_count < FREQUENCY_MAX_NUM)
					m_ResolutionList[i].frequency[m_ResolutionList[i].frequency_count++] = DisplayMode.RefreshRate;

			check_res = true;
		}

		if (!check_res)
		{
			// ���ο� �Ŵϱ� �߰�������.
			if (m_ResolutionCount < RESOLUTION_MAX_NUM)
			{
				m_ResolutionList[m_ResolutionCount].width = DisplayMode.Width;
				m_ResolutionList[m_ResolutionCount].height = DisplayMode.Height;
				m_ResolutionList[m_ResolutionCount].bpp = bpp;
				m_ResolutionList[m_ResolutionCount].frequency[0] = DisplayMode.RefreshRate;
				m_ResolutionList[m_ResolutionCount].frequency_count = 1;

				++m_ResolutionCount;
			}
		}
	}
}

int	CPythonSystem::GetResolutionCount()
{
	return m_ResolutionCount;
}

int CPythonSystem::GetFrequencyCount(int index)
{
	if (index >= m_ResolutionCount)
		return 0;

	return m_ResolutionList[index].frequency_count;
}

bool CPythonSystem::GetResolution(int index, OUT DWORD* width, OUT DWORD* height, OUT DWORD* bpp)
{
	if (index >= m_ResolutionCount)
		return false;

	*width = m_ResolutionList[index].width;
	*height = m_ResolutionList[index].height;
	*bpp = m_ResolutionList[index].bpp;
	return true;
}

bool CPythonSystem::GetFrequency(int index, int freq_index, OUT DWORD* frequncy)
{
	if (index >= m_ResolutionCount)
		return false;

	if (freq_index >= m_ResolutionList[index].frequency_count)
		return false;

	*frequncy = m_ResolutionList[index].frequency[freq_index];
	return true;
}

int	CPythonSystem::GetResolutionIndex(DWORD width, DWORD height, DWORD bit)
{
	DWORD re_width, re_height, re_bit;
	int i = 0;

	while (GetResolution(i, &re_width, &re_height, &re_bit))
	{
		if (re_width == width)
			if (re_height == height)
				if (re_bit == bit)
					return i;
		i++;
	}

	return 0;
}

int	CPythonSystem::GetFrequencyIndex(int res_index, DWORD frequency)
{
	DWORD re_frequency;
	int i = 0;

	while (GetFrequency(res_index, i, &re_frequency))
	{
		if (re_frequency == frequency)
			return i;

		i++;
	}

	return 0;
}

DWORD CPythonSystem::GetWidth()
{
	return m_Config.width;
}

DWORD CPythonSystem::GetHeight()
{
	return m_Config.height;
}
DWORD CPythonSystem::GetBPP()
{
	return m_Config.bpp;
}
DWORD CPythonSystem::GetFrequency()
{
	return m_Config.frequency;
}

bool CPythonSystem::IsNoSoundCard()
{
	return m_Config.bNoSoundCard;
}

bool CPythonSystem::IsSoftwareCursor()
{
	return m_Config.is_software_cursor;
}

float CPythonSystem::GetMusicVolume()
{
	return m_Config.music_volume;
}

int CPythonSystem::GetSoundVolume()
{
	return m_Config.voice_volume;
}

void CPythonSystem::SetMusicVolume(float fVolume)
{
	m_Config.music_volume = fVolume;
}

void CPythonSystem::SetSoundVolumef(float fVolume)
{
	m_Config.voice_volume = int(5 * fVolume);
}

#ifdef ENABLE_FOV_OPTION
void CPythonSystem::SetFOVLevel(float fFOV)
{
	m_Config.iFOVLevel = fMINMAX(30.0f, fFOV, 120.0f);
}

float CPythonSystem::GetFOVLevel()
{
	return m_Config.iFOVLevel;
}
#endif

int CPythonSystem::GetDistance()
{
	return m_Config.iDistance;
}

#if defined(ENABLE_SHADOW_RENDER_QUALITY_OPTION)
int CPythonSystem::GetShadowTargetLevel()
{
	return MIN(MAX(m_Config.iShadowTargetLevel,
		CPythonBackground::SHADOW_TARGET_NONE), /* min */
		CPythonBackground::SHADOW_TARGET_ALL /* max */
	);
}

void CPythonSystem::SetShadowTargetLevel(unsigned int level)
{
	m_Config.iShadowTargetLevel = MIN(MAX(level,
		CPythonBackground::SHADOW_TARGET_NONE), /* min */
		CPythonBackground::SHADOW_TARGET_ALL /* max */
	);
}
#endif

int CPythonSystem::GetShadowLevel()
{
	return m_Config.iShadowLevel;
}

void CPythonSystem::SetShadowLevel(unsigned int level)
{
	m_Config.iShadowLevel = MIN(level, 5);
	CPythonBackground::instance().RefreshShadowLevel();
}

int CPythonSystem::IsSaveID()
{
	return m_Config.isSaveID;
}

const char* CPythonSystem::GetSaveID()
{
	return m_Config.SaveID;
}

bool CPythonSystem::isViewCulling()
{
	return m_Config.is_object_culling;
}

void CPythonSystem::SetSaveID(int iValue, const char* c_szSaveID)
{
	if (iValue != 1)
		return;

	m_Config.isSaveID = iValue;
	strncpy(m_Config.SaveID, c_szSaveID, sizeof(m_Config.SaveID) - 1);
}

CPythonSystem::TConfig* CPythonSystem::GetConfig()
{
	return &m_Config;
}

void CPythonSystem::SetConfig(TConfig* pNewConfig)
{
	m_Config = *pNewConfig;
}

void CPythonSystem::SetDefaultConfig()
{
	memset(&m_Config, 0, sizeof(m_Config));

	m_Config.width = 1024;
	m_Config.height = 768;
	m_Config.bpp = 32;
	m_Config.bWindowed = false;

	m_Config.is_software_cursor = false;
	m_Config.is_object_culling = true;
	m_Config.iDistance = 3;

	m_Config.gamma = 3;
	m_Config.music_volume = 1.0f;
	m_Config.voice_volume = 5;

	m_Config.bDecompressDDS = 0;
	m_Config.bSoftwareTiling = 0;
	m_Config.iShadowLevel = 3;
#if defined(ENABLE_SHADOW_RENDER_QUALITY_OPTION)
	m_Config.iShadowTargetLevel = CPythonBackground::SHADOW_TARGET_ALL;
#endif
	m_Config.bViewChat = true;
	m_Config.bAlwaysShowName = DEFAULT_VALUE_ALWAYS_SHOW_NAME;
	m_Config.bShowDamage = true;
	m_Config.bShowSalesText = true;
#ifdef ENABLE_HIGHLIGHT_NEW_ITEM
	m_Config.bItemHighlight = true;
#endif
#ifdef ENABLE_LEVEL_SHOW_HIDE
	m_Config.bShowLevel = true;
#endif
#ifdef ENABLE_AUTO_PICKUP_SYSTEM
	m_Config.bAutoPickup		= false;
#endif
#if defined(ENABLE_FOG_FIX)
	m_Config.bShowFogMode = false;
#endif
#if defined(ENABLE_ENVIRONMENT_EFFECT_OPTION)
	m_Config.bShowNightMode = true;
	m_Config.bShowSnowMode = true;
	m_Config.bShowSnowTextureMode = true;
#endif
#ifdef ENABLE_GRAPHIC_ON_OFF
	m_Config.iEffectLevel = 0;
	m_Config.iPrivateShopLevel = 0;
	m_Config.iDropItemLevel = 0;

	m_Config.bPetStatus = false;
	m_Config.bNpcNameStatus = false;
#endif
#ifdef ENABLE_SPECIAL_STORAGE
	m_Config.bStorageOpenWithInventory = true;
#endif
#ifdef ENABLE_FOV_OPTION
	m_Config.iFOVLevel = 30.0f;
#endif
}

bool CPythonSystem::IsWindowed()
{
	return m_Config.bWindowed;
}

bool CPythonSystem::IsViewChat()
{
	return m_Config.bViewChat;
}

void CPythonSystem::SetViewChatFlag(int iFlag)
{
	m_Config.bViewChat = iFlag == 1 ? true : false;
}

bool CPythonSystem::IsAlwaysShowName()
{
	return m_Config.bAlwaysShowName;
}

void CPythonSystem::SetAlwaysShowNameFlag(int iFlag)
{
	m_Config.bAlwaysShowName = iFlag == 1 ? true : false;
}

bool CPythonSystem::IsShowDamage()
{
	return m_Config.bShowDamage;
}

void CPythonSystem::SetShowDamageFlag(int iFlag)
{
	m_Config.bShowDamage = iFlag == 1 ? true : false;
}

bool CPythonSystem::IsShowSalesText()
{
	return m_Config.bShowSalesText;
}

void CPythonSystem::SetShowSalesTextFlag(int iFlag)
{
	m_Config.bShowSalesText = iFlag == 1 ? true : false;
}

#ifdef ENABLE_HIGHLIGHT_NEW_ITEM
bool CPythonSystem::IsItemHighlight()
{
	return m_Config.bItemHighlight;
}

void CPythonSystem::SetItemHighlight(int iFlag)
{
	m_Config.bItemHighlight = iFlag == 1 ? true : false;
}
#endif

#ifdef ENABLE_LEVEL_SHOW_HIDE
bool CPythonSystem::IsShowLevel()
{
	return m_Config.bShowLevel;
}

void CPythonSystem::SetShowLevelFlag(int iFlag)
{
	m_Config.bShowLevel = iFlag == 1 ? true : false;
}
#endif

#ifdef ENABLE_AUTO_PICKUP_SYSTEM
void CPythonSystem::SetAutoPickup(int iFlag)
{
	m_Config.bAutoPickup = iFlag == 1 ? true : false;
}

bool CPythonSystem::IsAutoPickup()
{
	return m_Config.bAutoPickup;
}
#endif

#if defined(ENABLE_FOG_FIX)
void CPythonSystem::SetFogModeOption(int iOpt)
{
	m_Config.bShowFogMode = iOpt == 1 ? true : false;
}

bool CPythonSystem::GetFogModeOption()
{
	return m_Config.bShowFogMode;
}
#endif

#if defined(ENABLE_ENVIRONMENT_EFFECT_OPTION)
void CPythonSystem::SetNightModeOption(int iOpt)
{
	m_Config.bShowNightMode = iOpt == 1 ? true : false;
}

bool CPythonSystem::GetNightModeOption()
{
	return m_Config.bShowNightMode;
}

void CPythonSystem::SetSnowModeOption(int iOpt)
{
	m_Config.bShowSnowMode = iOpt == 1 ? true : false;
}

bool CPythonSystem::GetSnowModeOption()
{
	return m_Config.bShowSnowMode;
}

void CPythonSystem::SetSnowTextureModeOption(int iOpt)
{
	m_Config.bShowSnowTextureMode = iOpt == 1 ? true : false;
}

bool CPythonSystem::GetSnowTextureModeOption()
{
	return m_Config.bShowSnowTextureMode;
}
#endif

#ifdef ENABLE_GRAPHIC_ON_OFF
int CPythonSystem::GetEffectLevel()
{
	return m_Config.iEffectLevel;
}
void CPythonSystem::SetEffectLevel(unsigned int level)
{
	m_Config.iEffectLevel = MIN(level, 5);
}

int CPythonSystem::GetPrivateShopLevel()
{
	return m_Config.iPrivateShopLevel;
}
void CPythonSystem::SetPrivateShopLevel(unsigned int level)
{
	m_Config.iPrivateShopLevel = MIN(level, 5);
}

int CPythonSystem::GetDropItemLevel()
{
	return m_Config.iDropItemLevel;
}
void CPythonSystem::SetDropItemLevel(unsigned int level)
{
	m_Config.iDropItemLevel = MIN(level, 5);
}

bool CPythonSystem::IsPetStatus()
{
	return m_Config.bPetStatus;
}
void CPythonSystem::SetPetStatusFlag(int iFlag)
{
	m_Config.bPetStatus = iFlag == 1 ? true : false;
}

bool CPythonSystem::IsNpcNameStatus()
{
	return m_Config.bNpcNameStatus;
}
void CPythonSystem::SetNpcNameStatusFlag(int iFlag)
{
	m_Config.bNpcNameStatus = iFlag == 1 ? true : false;
}
#endif

#ifdef ENABLE_SPECIAL_STORAGE
bool CPythonSystem::IsStorageOpenWithInventory()
{
	return m_Config.bStorageOpenWithInventory;
}

void CPythonSystem::SetStorageOpenWithInventory(int iFlag)
{
	m_Config.bStorageOpenWithInventory = iFlag == 1 ? true : false;
}
#endif

bool CPythonSystem::IsAutoTiling()
{
	if (m_Config.bSoftwareTiling == 0)
		return true;

	return false;
}

void CPythonSystem::SetSoftwareTiling(bool isEnable)
{
	if (isEnable)
		m_Config.bSoftwareTiling = 1;
	else
		m_Config.bSoftwareTiling = 2;
}

bool CPythonSystem::IsSoftwareTiling()
{
	if (m_Config.bSoftwareTiling == 1)
		return true;

	return false;
}

bool CPythonSystem::IsUseDefaultIME()
{
	return m_Config.bUseDefaultIME;
}

bool CPythonSystem::LoadConfig()
{
	FILE* fp = NULL;

	if (NULL == (fp = fopen("userdata/metin2.cfg", "rt")))
		return false;

	char buf[256];
	char command[256];
	char value[256];

	while (fgets(buf, 256, fp))
	{
		if (sscanf(buf, " %s %s\n", command, value) == EOF)
			break;

		if (!stricmp(command, "WIDTH"))
			m_Config.width = atoi(value);
		else if (!stricmp(command, "HEIGHT"))
			m_Config.height = atoi(value);
		else if (!stricmp(command, "BPP"))
			m_Config.bpp = atoi(value);
		else if (!stricmp(command, "FREQUENCY"))
			m_Config.frequency = atoi(value);
		else if (!stricmp(command, "SOFTWARE_CURSOR"))
			m_Config.is_software_cursor = atoi(value) ? true : false;
		else if (!stricmp(command, "OBJECT_CULLING"))
			m_Config.is_object_culling = atoi(value) ? true : false;
		else if (!stricmp(command, "VISIBILITY"))
			m_Config.iDistance = atoi(value);
		else if (!stricmp(command, "MUSIC_VOLUME")) {
			if (strchr(value, '.') == 0) { // Old compatiability
				m_Config.music_volume = pow(10.0f, (-1.0f + (((float)atoi(value)) / 5.0f)));
				if (atoi(value) == 0)
					m_Config.music_volume = 0.0f;
			}
			else
				m_Config.music_volume = atof(value);
		}
		else if (!stricmp(command, "VOICE_VOLUME"))
			m_Config.voice_volume = (char)atoi(value);
		else if (!stricmp(command, "GAMMA"))
			m_Config.gamma = atoi(value);
		else if (!stricmp(command, "IS_SAVE_ID"))
			m_Config.isSaveID = atoi(value);
		else if (!stricmp(command, "SAVE_ID"))
			strncpy(m_Config.SaveID, value, 20);
		else if (!stricmp(command, "PRE_LOADING_DELAY_TIME"))
			g_iLoadingDelayTime = atoi(value);
		else if (!stricmp(command, "WINDOWED"))
		{
			m_Config.bWindowed = atoi(value) == 1 ? true : false;
		}
		else if (!stricmp(command, "USE_DEFAULT_IME"))
			m_Config.bUseDefaultIME = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "SOFTWARE_TILING"))
			m_Config.bSoftwareTiling = atoi(value);
#if defined(ENABLE_SHADOW_RENDER_QUALITY_OPTION)
		else if (!stricmp(command, "SHADOW_TARGET_LEVEL"))
			m_Config.iShadowTargetLevel = atoi(value);
#else
		else if (!stricmp(command, "SHADOW_LEVEL"))
			m_Config.iShadowLevel = atoi(value);
#endif
		else if (!stricmp(command, "DECOMPRESSED_TEXTURE"))
			m_Config.bDecompressDDS = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "NO_SOUND_CARD"))
			m_Config.bNoSoundCard = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "VIEW_CHAT"))
			m_Config.bViewChat = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "ALWAYS_VIEW_NAME"))
			m_Config.bAlwaysShowName = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "SHOW_DAMAGE"))
			m_Config.bShowDamage = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "SHOW_SALESTEXT"))
			m_Config.bShowSalesText = atoi(value) == 1 ? true : false;
#ifdef ENABLE_HIGHLIGHT_NEW_ITEM
		else if (!stricmp(command, "ITEM_HIGHLIGHT"))
			m_Config.bItemHighlight = atoi(value) == 1 ? true : false;
#endif
#ifdef ENABLE_LEVEL_SHOW_HIDE
		else if (!stricmp(command, "SHOW_LEVEL"))
			m_Config.bShowLevel = atoi(value) == 1 ? true : false;
#endif
#ifdef ENABLE_AUTO_PICKUP_SYSTEM
		else if (!stricmp(command, "AUTO_PICKUP"))
			m_Config.bAutoPickup = atoi(value) == 1 ? true : false;
#endif
#if defined(ENABLE_FOG_FIX)
		else if (!stricmp(command, "FOG_MODE_ON"))
			m_Config.bShowFogMode = atoi(value) == 1 ? true : false;
#endif
#if defined(ENABLE_ENVIRONMENT_EFFECT_OPTION)
		else if (!stricmp(command, "NIGHT_MODE_ON"))
			m_Config.bShowNightMode = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "SNOW_MODE_ON"))
			m_Config.bShowSnowMode = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "SNOW_TEXTURE_MODE"))
			m_Config.bShowSnowTextureMode = atoi(value) == 1 ? true : false;
#endif
#ifdef ENABLE_GRAPHIC_ON_OFF
		else if (!stricmp(command, "EFFECT_LEVEL"))
			m_Config.iEffectLevel = atoi(value);
		else if (!stricmp(command, "PRIVATE_SHOP_LEVEL"))
			m_Config.iPrivateShopLevel = atoi(value);
		else if (!stricmp(command, "DROP_ITEM_LEVEL"))
			m_Config.iDropItemLevel = atoi(value);

		else if (!stricmp(command, "PET_STATUS"))
			m_Config.bPetStatus = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "NPC_NAME_STATUS"))
			m_Config.bNpcNameStatus = atoi(value) == 1 ? true : false;
#endif
#ifdef ENABLE_SPECIAL_STORAGE
		else if (!stricmp(command, "STORAGE_OPEN_WITH_INVENTORY"))
			m_Config.bStorageOpenWithInventory = atoi(value) == 1 ? true : false;
#endif
#ifdef ENABLE_FOV_OPTION
		else if (!stricmp(command, "FIELD_OF_VIEW"))
			m_Config.iFOVLevel = atoi(value);
#endif
	}

	if (m_Config.bWindowed)
	{
		unsigned screen_width = GetSystemMetrics(SM_CXFULLSCREEN);
		unsigned screen_height = GetSystemMetrics(SM_CYFULLSCREEN);

		if (m_Config.width >= screen_width)
		{
			m_Config.width = screen_width;
		}
		if (m_Config.height >= screen_height) //@fixme313
		{
			int config_height = m_Config.height;
			int difference = (config_height - screen_height) + 7;
			m_Config.height = config_height - difference;
		}
	}

	m_OldConfig = m_Config;

	fclose(fp);

	//	Tracef("LoadConfig: Resolution: %dx%d %dBPP %dHZ Software Cursor: %d, Music/Voice Volume: %d/%d Gamma: %d\n",
	//		m_Config.width,
	//		m_Config.height,
	//		m_Config.bpp,
	//		m_Config.frequency,
	//		m_Config.is_software_cursor,
	//		m_Config.music_volume,
	//		m_Config.voice_volume,
	//		m_Config.gamma);

	return true;
}

bool CPythonSystem::SaveConfig()
{
	FILE* fp;

	if (NULL == (fp = fopen("userdata/metin2.cfg", "wt")))
		return false;

	fprintf(fp, "WIDTH						%d\n"
		"HEIGHT						%d\n"
		"BPP						%d\n"
		"FREQUENCY					%d\n"
		"SOFTWARE_CURSOR			%d\n"
		"OBJECT_CULLING				%d\n"
		"VISIBILITY					%d\n"
		"MUSIC_VOLUME				%.3f\n"
		"VOICE_VOLUME				%d\n"
		"GAMMA						%d\n"
		"IS_SAVE_ID					%d\n"
		"SAVE_ID					%s\n"
		"PRE_LOADING_DELAY_TIME		%d\n"
		"DECOMPRESSED_TEXTURE		%d\n",
		m_Config.width,
		m_Config.height,
		m_Config.bpp,
		m_Config.frequency,
		m_Config.is_software_cursor,
		m_Config.is_object_culling,
		m_Config.iDistance,
		m_Config.music_volume,
		m_Config.voice_volume,
		m_Config.gamma,
		m_Config.isSaveID,
		m_Config.SaveID,
		g_iLoadingDelayTime,
		m_Config.bDecompressDDS);

	if (m_Config.bWindowed == 1)
		fprintf(fp, "WINDOWED				%d\n", m_Config.bWindowed);
	if (m_Config.bViewChat == 0)
		fprintf(fp, "VIEW_CHAT				%d\n", m_Config.bViewChat);
	if (m_Config.bAlwaysShowName != DEFAULT_VALUE_ALWAYS_SHOW_NAME)
		fprintf(fp, "ALWAYS_VIEW_NAME		%d\n", m_Config.bAlwaysShowName);
	if (m_Config.bShowDamage == 0)
		fprintf(fp, "SHOW_DAMAGE		%d\n", m_Config.bShowDamage);
	if (m_Config.bShowSalesText == 0)
		fprintf(fp, "SHOW_SALESTEXT		%d\n", m_Config.bShowSalesText);
#ifdef ENABLE_HIGHLIGHT_NEW_ITEM
	fprintf(fp, "ITEM_HIGHLIGHT			%d\n", m_Config.bItemHighlight);
#endif
#ifdef ENABLE_LEVEL_SHOW_HIDE
	fprintf(fp, "SHOW_LEVEL			%d\n", m_Config.bShowLevel);
#endif
#ifdef ENABLE_AUTO_PICKUP_SYSTEM
	fprintf(fp, "AUTO_PICKUP			%d\n", m_Config.bAutoPickup);
#endif
#if defined(ENABLE_FOG_FIX)
	fprintf(fp, "FOG_MODE_ON			%d\n", m_Config.bShowFogMode);
#endif
#if defined(ENABLE_ENVIRONMENT_EFFECT_OPTION)
	fprintf(fp, "NIGHT_MODE_ON		 	%d\n", m_Config.bShowNightMode);
	fprintf(fp, "SNOW_MODE_ON			%d\n", m_Config.bShowSnowMode);
	fprintf(fp, "SNOW_TEXTURE_MODE		%d\n", m_Config.bShowSnowTextureMode);
#endif
#ifdef ENABLE_GRAPHIC_ON_OFF
	fprintf(fp, "EFFECT_LEVEL				%d\n", m_Config.iEffectLevel);
	fprintf(fp, "PRIVATE_SHOP_LEVEL			%d\n", m_Config.iPrivateShopLevel);
	fprintf(fp, "DROP_ITEM_LEVEL			%d\n", m_Config.iDropItemLevel);
	fprintf(fp, "PET_STATUS					%d\n", m_Config.bPetStatus);
	fprintf(fp, "NPC_NAME_STATUS			%d\n", m_Config.bNpcNameStatus);
#endif
#ifdef ENABLE_SPECIAL_STORAGE
	fprintf(fp, "STORAGE_OPEN_WITH_INVENTORY			%d\n", m_Config.bStorageOpenWithInventory);
#endif
#ifdef ENABLE_FOV_OPTION
	fprintf(fp, "FIELD_OF_VIEW			%.1f\n", m_Config.iFOVLevel);
#endif
	fprintf(fp, "USE_DEFAULT_IME		%d\n", m_Config.bUseDefaultIME);
	fprintf(fp, "SOFTWARE_TILING		%d\n", m_Config.bSoftwareTiling);
#if defined(ENABLE_SHADOW_RENDER_QUALITY_OPTION)
	fprintf(fp, "SHADOW_TARGET_LEVEL		%d\n", m_Config.iShadowTargetLevel);
#else
	fprintf(fp, "SHADOW_LEVEL				%d\n", m_Config.iShadowLevel);
#endif
	fprintf(fp, "\n");

	fclose(fp);
	return true;
}

bool CPythonSystem::LoadInterfaceStatus()
{
	FILE* File;
	File = fopen("interface.cfg", "rb");

	if (!File)
		return false;

	fread(m_WindowStatus, 1, sizeof(TWindowStatus) * WINDOW_MAX_NUM, File);
	fclose(File);
	return true;
}

void CPythonSystem::SaveInterfaceStatus()
{
	if (!m_poInterfaceHandler)
		return;

	PyCallClassMemberFunc(m_poInterfaceHandler, "OnSaveInterfaceStatus", Py_BuildValue("()"));

	FILE* File;

	File = fopen("interface.cfg", "wb");

	if (!File)
	{
		TraceError("Cannot open interface.cfg");
		return;
	}

	fwrite(m_WindowStatus, 1, sizeof(TWindowStatus) * WINDOW_MAX_NUM, File);
	fclose(File);
}

bool CPythonSystem::isInterfaceConfig()
{
	return m_isInterfaceConfig;
}

const CPythonSystem::TWindowStatus& CPythonSystem::GetWindowStatusReference(int iIndex)
{
	return m_WindowStatus[iIndex];
}

void CPythonSystem::ApplyConfig() // ���� ������ ���� ������ ���ؼ� �ٲ� ������ ���� �Ѵ�.
{
	if (m_OldConfig.gamma != m_Config.gamma)
	{
		float val = 1.0f;

		switch (m_Config.gamma)
		{
		case 0: val = 0.4f;	break;
		case 1: val = 0.7f; break;
		case 2: val = 1.0f; break;
		case 3: val = 1.2f; break;
		case 4: val = 1.4f; break;
		}

		CPythonGraphic::Instance().SetGamma(val);
	}

	if (m_OldConfig.is_software_cursor != m_Config.is_software_cursor)
	{
		if (m_Config.is_software_cursor)
			CPythonApplication::Instance().SetCursorMode(CPythonApplication::CURSOR_MODE_SOFTWARE);
		else
			CPythonApplication::Instance().SetCursorMode(CPythonApplication::CURSOR_MODE_HARDWARE);
	}

	m_OldConfig = m_Config;

	ChangeSystem();
}

void CPythonSystem::ChangeSystem()
{
	// Shadow
	/*
	if (m_Config.is_shadow)
		CScreen::SetShadowFlag(true);
	else
		CScreen::SetShadowFlag(false);
	*/
	CSoundManager& rkSndMgr = CSoundManager::Instance();
	/*
	float fMusicVolume;
	if (0 == m_Config.music_volume)
		fMusicVolume = 0.0f;
	else
		fMusicVolume= (float)pow(10.0f, (-1.0f + (float)m_Config.music_volume / 5.0f));
		*/
	rkSndMgr.SetMusicVolume(m_Config.music_volume);

	/*
	float fVoiceVolume;
	if (0 == m_Config.voice_volume)
		fVoiceVolume = 0.0f;
	else
		fVoiceVolume = (float)pow(10.0f, (-1.0f + (float)m_Config.voice_volume / 5.0f));
	*/
	rkSndMgr.SetSoundVolumeGrade(m_Config.voice_volume);
}

void CPythonSystem::Clear()
{
	SetInterfaceHandler(NULL);
}

CPythonSystem::CPythonSystem()
{
	memset(&m_Config, 0, sizeof(TConfig));

	m_poInterfaceHandler = NULL;

	SetDefaultConfig();

	LoadConfig();

	ChangeSystem();

	if (LoadInterfaceStatus())
		m_isInterfaceConfig = true;
	else
		m_isInterfaceConfig = false;
}

CPythonSystem::~CPythonSystem()
{
	assert(m_poInterfaceHandler == NULL && "CPythonSystem MUST CLEAR!");
}

#if defined(ENABLE_PICKUP_FILTER_SYSTEM)
#include <fstream>

CPythonSystem::CPickUpFilter::CPickUpFilter()
{
	///// READ ////
	std::ifstream inputFile;
	inputFile.open(cPickUpFilterFileName, std::ios::binary);
	if (inputFile.is_open())
	{
		inputFile.read(reinterpret_cast<char*>(bPickFilter),	sizeof(bool) * EPICKFILTER::EPICKFILTER_MAX);
		inputFile.read(reinterpret_cast<char*>(bPickSize),	sizeof(bool) * ESIZE::ESIZE_MAX);
		inputFile.read(reinterpret_cast<char*>(&bModeAll),	sizeof(bool));
		inputFile.read(reinterpret_cast<char*>(&m_bRefineMin),	sizeof(BYTE));
		inputFile.read(reinterpret_cast<char*>(&m_bRefineMax),	sizeof(BYTE));
		inputFile.read(reinterpret_cast<char*>(&m_lLevelMin),	sizeof(long));
		inputFile.read(reinterpret_cast<char*>(&m_lLevelMax),	sizeof(long));

		inputFile.close();
	}
	else
	{
		///// SET DEFAULT ////
		std::fill(std::begin(bPickFilter), std::end(bPickFilter), true);
		std::fill(std::begin(bPickSize), std::end(bPickSize), true);

		bModeAll = false;
		
		m_bRefineMin = 0;
		m_bRefineMax = 9;

		m_lLevelMin = 0;
		m_lLevelMax = 999;
	}
}

CPythonSystem::CPickUpFilter::~CPickUpFilter()
{
	///// SAVE ////
	std::ofstream outputFile;
	outputFile.open(cPickUpFilterFileName, std::ios::binary);
	if (outputFile.is_open())
	{
		outputFile.write(reinterpret_cast<char*>(bPickFilter),		sizeof(bool) * EPICKFILTER::EPICKFILTER_MAX);
		outputFile.write(reinterpret_cast<char*>(bPickSize),		sizeof(bool) * ESIZE::ESIZE_MAX);
		outputFile.write(reinterpret_cast<char*>(&bModeAll),		sizeof(bool));
		outputFile.write(reinterpret_cast<char*>(&m_bRefineMin),	sizeof(BYTE));
		outputFile.write(reinterpret_cast<char*>(&m_bRefineMax),	sizeof(BYTE));
		outputFile.write(reinterpret_cast<char*>(&m_lLevelMin),		sizeof(long));
		outputFile.write(reinterpret_cast<char*>(&m_lLevelMax),		sizeof(long));

		outputFile.close();
	}
	else
	{
		Tracenf("CPickUpFilter::~CPickUpFilter() Cannot create a file for save settings.");
	}
}

void CPythonSystem::CPickUpFilter::SetFilter(size_t sIndex, bool b)
{
	if (sIndex >= EPICKFILTER::EPICKFILTER_MAX)
	{
		Tracenf("CPickUpFilter::SetFilter(Index=%d) : Out of range", sIndex);
		return;
	}

	bPickFilter[sIndex] = b;
}

void CPythonSystem::CPickUpFilter::SetSize(size_t sIndex, bool b)
{
	if (sIndex >= ESIZE::ESIZE_MAX)
	{
		Tracenf("CPickUpFilter::SetSize(Index=%d) : Out of range", sIndex);
		return;
	}

	if (sIndex == ESIZE::BIG && b == false)
		SetFilter(EPICKFILTER::SUB_WEAPON_TWO_HANDED, false);

	bPickSize[sIndex] = b;
}

void CPythonSystem::CPickUpFilter::SetRefine(BYTE min, BYTE max)
{
	m_bRefineMin = static_cast<BYTE>(MINMAX(0, min, 9));
	m_bRefineMax = static_cast<BYTE>(MINMAX(0, max, 9));
}

void CPythonSystem::CPickUpFilter::SetLevel(long min, long max)
{
	m_lLevelMin = MINMAX(0, min, 999);
	m_lLevelMax = MINMAX(0, max, 999);
}

void CPythonSystem::CPickUpFilter::SetModeAll(bool b)
{
	bModeAll = b;
}

bool CPythonSystem::CPickUpFilter::CanPickItem(DWORD dwIID)
{
	CItemData* pItemData;
	if (!CItemManager::Instance().GetItemDataPointer(CPythonItem::Instance().GetVirtualNumberOfGroundItem(dwIID), &pItemData))
	{
		Tracenf("CPickUpFilter::CanPickItem(dwIID=%d) : Non-exist item.", dwIID);
		return true;
	}

	if (CheckRefine(pItemData) == false)
		return false;

	if (CheckLevel(pItemData) == false)
		return false;

	if (CheckSize(pItemData) == false)
		return false;

	if (CheckType(pItemData) == false)
		return false;
	
	return true;
}

std::pair<BYTE, BYTE> CPythonSystem::CPickUpFilter::GetRefine()
{
	return std::make_pair(m_bRefineMin, m_bRefineMax);
}

std::pair<long, long> CPythonSystem::CPickUpFilter::GetLevel()
{
	return std::make_pair(m_lLevelMin, m_lLevelMax);
}

bool CPythonSystem::CPickUpFilter::GetFilter(size_t sIndex) const
{
	if (sIndex >= EPICKFILTER::EPICKFILTER_MAX)
	{
		Tracenf("CPickUpFilter::GetFilter(Index=%d) : Out of range", sIndex);
		return false;
	}

	return bPickFilter[sIndex];
}

bool CPythonSystem::CPickUpFilter::GetSize(size_t sIndex) const
{
	if (sIndex >= ESIZE::ESIZE_MAX)
	{
		Tracenf("CPickUpFilter::GetSize(Index=%d) : Out of range", sIndex);
		return false;
	}

	return bPickSize[sIndex];
}

bool CPythonSystem::CPickUpFilter::IsModeAll() const
{
	return bModeAll;
}

bool CPythonSystem::CPickUpFilter::CheckRefine(const CItemData* pItem) const
{
	const BYTE bRefineLevel = static_cast<BYTE>(pItem->GetRefine());
	if (bRefineLevel >= m_bRefineMin && bRefineLevel <= m_bRefineMax)
		return true;

	return false;
}

bool CPythonSystem::CPickUpFilter::CheckLevel(const CItemData* pItem) const
{
	CItemData::TItemLimit ItemLimit;
	for (BYTE i = 0; i < CItemData::ITEM_LIMIT_MAX_NUM; i++)
	{
		if (!pItem->GetLimit(i, &ItemLimit))
			continue;

		if (ItemLimit.bType != CItemData::LIMIT_LEVEL)
			continue;

		const long lLimitLevel = ItemLimit.lValue;
		return (lLimitLevel >= m_lLevelMin && lLimitLevel <= m_lLevelMax);
	}

	return true;
}

bool CPythonSystem::CPickUpFilter::CheckSize(const CItemData* pItem) const
{
	return GetSize(pItem->GetSize() - 1);
}

bool CPythonSystem::CPickUpFilter::CheckType(const CItemData* pItem) const
{
	const BYTE bType = pItem->GetType();
	const BYTE bSubType = pItem->GetSubType();

	switch (bType)
	{
	case CItemData::EItemType::ITEM_TYPE_WEAPON:
		switch (bSubType)
		{
		case CItemData::EWeaponSubTypes::WEAPON_SWORD:
			return GetFilter(EPICKFILTER::SUB_WEAPON_SWORD);

		case CItemData::EWeaponSubTypes::WEAPON_DAGGER:
			return GetFilter(EPICKFILTER::SUB_WEAPON_DAGGER);

		case CItemData::EWeaponSubTypes::WEAPON_BOW:
			return GetFilter(EPICKFILTER::SUB_WEAPON_BOW);

		case CItemData::EWeaponSubTypes::WEAPON_TWO_HANDED:
			return GetFilter(EPICKFILTER::SUB_WEAPON_TWO_HANDED);

		case CItemData::EWeaponSubTypes::WEAPON_BELL:
			return GetFilter(EPICKFILTER::SUB_WEAPON_BELL);

		case CItemData::EWeaponSubTypes::WEAPON_FAN:
			return GetFilter(EPICKFILTER::SUB_WEAPON_FAN);

		case CItemData::EWeaponSubTypes::WEAPON_ARROW:
			return GetFilter(EPICKFILTER::SUB_WEAPON_ARROW);

		}
		break;

	case CItemData::EItemType::ITEM_TYPE_ARMOR:
		switch (bSubType)
		{
		case CItemData::EArmorSubTypes::ARMOR_BODY:
			return GetFilter(EPICKFILTER::SUB_ARMOR_BODY);

		case CItemData::EArmorSubTypes::ARMOR_HEAD:
			return GetFilter(EPICKFILTER::SUB_ARMOR_HEAD);

		case CItemData::EArmorSubTypes::ARMOR_SHIELD:
			return GetFilter(EPICKFILTER::SUB_ARMOR_SHIELD);

		case CItemData::EArmorSubTypes::ARMOR_WRIST:
			return GetFilter(EPICKFILTER::SUB_ARMOR_WRIST);

		case CItemData::EArmorSubTypes::ARMOR_FOOTS:
			return GetFilter(EPICKFILTER::SUB_ARMOR_FOOTS);

		case CItemData::EArmorSubTypes::ARMOR_NECK:
			return GetFilter(EPICKFILTER::SUB_ARMOR_NECK);

		case CItemData::EArmorSubTypes::ARMOR_EAR:
			return GetFilter(EPICKFILTER::SUB_ARMOR_EAR);
		}
		break;

	case CItemData::EItemType::ITEM_TYPE_METIN:
		return GetFilter(EPICKFILTER::TYPE_METIN);

	case CItemData::EItemType::ITEM_TYPE_ELK:
		return GetFilter(EPICKFILTER::TYPE_YANG);

	case CItemData::EItemType::ITEM_TYPE_SKILLBOOK:
		return GetFilter(EPICKFILTER::TYPE_SKILLBOOK);

	case CItemData::EItemType::ITEM_TYPE_GIFTBOX:
		return GetFilter(EPICKFILTER::TYPE_GIFTBOX);

	case CItemData::EItemType::ITEM_TYPE_POLYMORPH:
		return GetFilter(EPICKFILTER::TYPE_POLY);

	case CItemData::EItemType::ITEM_TYPE_RING:
		return GetFilter(EPICKFILTER::TYPE_RING);
	
	case CItemData::EItemType::ITEM_TYPE_USE:
		switch (bSubType)
		{
		case CItemData::EUseSubTypes::USE_POTION:
		case CItemData::EUseSubTypes::USE_ABILITY_UP:
		case CItemData::EUseSubTypes::USE_POTION_NODELAY:
		case CItemData::EUseSubTypes::USE_POTION_CONTINUE:
			return GetFilter(EPICKFILTER::SUB_POTION);
		}
		break;

	case CItemData::EItemType::ITEM_TYPE_MATERIAL:
		return GetFilter(EPICKFILTER::TYPE_MATERIAL);
	}

	return true;
}
#endif

