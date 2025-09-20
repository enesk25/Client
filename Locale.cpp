#include "StdAfx.h"
#include "Locale.h"
#include "PythonApplication.h"
#include "resource.h"
#include "../eterBase/CRC32.h"
#include "../eterpack/EterPackManager.h"
#include "../eterLocale/Japanese.h"
#include <windowsx.h>

#ifndef LSS_SECURITY_KEY
#define LSS_SECURITY_KEY	"testtesttesttest"
#endif

std::string __SECURITY_KEY_STRING__ = LSS_SECURITY_KEY;

char	MULTI_LOCALE_SERVICE[256] = "EUROPE";
char	MULTI_LOCALE_PATH[256] = "locale/turkey";
char	MULTI_LOCALE_NAME[256] = "turkey";
int		MULTI_LOCALE_CODE = 949;
int		MULTI_LOCALE_REPORT_PORT = 10000;

void LocaleService_LoadConfig(const char* fileName)
{
	NANOBEGIN
		FILE* fp = fopen(fileName, "rt");

	if (fp)
	{
		char	line[256];
		char	name[256];
		int		code;
		int		id;
		if (fgets(line, sizeof(line) - 1, fp))
		{
			line[sizeof(line) - 1] = '\0';
			sscanf(line, "%d %d %s", &id, &code, name);

			MULTI_LOCALE_REPORT_PORT = id;
			MULTI_LOCALE_CODE = code;
			strcpy(MULTI_LOCALE_NAME, name);
			sprintf(MULTI_LOCALE_PATH, "locale/%s", MULTI_LOCALE_NAME);
		}
		fclose(fp);
	}
	NANOEND
}

unsigned LocaleService_GetLastExp(int level)
{
	static const int GUILD_LEVEL_MAX = 20;

	static DWORD INTERNATIONAL_GUILDEXP_LIST[GUILD_LEVEL_MAX + 1] =
	{
		0,			// 0
		6000UL,		// 1
		18000UL,	// 2
		36000UL,	// 3
		64000UL,	// 4
		94000UL,	// 5
		130000UL,	// 6
		172000UL,	// 7
		220000UL,	// 8
		274000UL,	// 9
		334000UL,	// 10
		400000UL,	// 11
		600000UL,	// 12
		840000UL,	// 13
		1120000UL,	// 14
		1440000UL,	// 15
		1800000UL,	// 16
		2600000UL,	// 17
		3200000UL,	// 18
		4000000UL,	// 19
		16800000UL	// 20
	};

	if (level < 0 && level >= GUILD_LEVEL_MAX)
		return 0;

	return INTERNATIONAL_GUILDEXP_LIST[level];
}

int LocaleService_GetSkillPower(unsigned level)
{
	static const unsigned SKILL_POWER_NUM = 50;

	if (level >= SKILL_POWER_NUM)
		return 0;

	// 0 5 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 50 52 54 56 58 60 63 66 69 72 82 85 88 91 94 98 102 106 110 115 125 125 125 125 125
	static unsigned INTERNATIONAL_SKILL_POWERS[SKILL_POWER_NUM] =
	{
		0,
			5,  6,  8, 10, 12,
			14, 16, 18, 20, 22,
			24, 26, 28, 30, 32,
			34, 36, 38, 40, 50, // master
			52, 54, 56, 58, 60,
			63, 66, 69, 72, 82, // grand_master
			85, 88, 91, 94, 98,
			102,106,110,115,125,// perfect_master
			125,
	};
	return INTERNATIONAL_SKILL_POWERS[level];
}

const char* LocaleService_GetSecurityKey()
{
	return "testtesttesttest";
}

void LocaleService_ForceSetLocale(const char* name, const char* localePath)
{
	strcpy(MULTI_LOCALE_NAME, name);
	strcpy(MULTI_LOCALE_PATH, localePath);

	// ���� õ�� ������ ���ӽÿ��� security key ���� (WE ���� Ŭ��� õ������ �����ϱ� ����)
	if (0 == stricmp(name, "turkey"))
		__SECURITY_KEY_STRING__ = "testtesttesttest";
	if (0 == stricmp(name, "we_korea"))
		__SECURITY_KEY_STRING__ = "1234abcd5678efgh";
}

const char* LocaleService_GetName()
{
	return MULTI_LOCALE_SERVICE;
}

unsigned int LocaleService_GetCodePage()
{
	return MULTI_LOCALE_CODE;
}

const char* LocaleService_GetLocaleName()
{
	return MULTI_LOCALE_NAME;
}

const char* LocaleService_GetLocalePath()
{
	return MULTI_LOCALE_PATH;
}

BOOL LocaleService_IsLeadByte(const char chByte)
{
	return (((unsigned char)chByte) & 0x80) != 0;
}

int LocaleService_StringCompareCI(LPCSTR szStringLeft, LPCSTR szStringRight, size_t sizeLength)
{
	return strnicmp(szStringLeft, szStringRight, sizeLength);
}