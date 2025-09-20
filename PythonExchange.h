#pragma once

#include "Packet.h"

/*
 *	��ȯ â ����
 */
class CPythonExchange : public CSingleton<CPythonExchange>
{
public:
	enum
	{
#ifdef ENABLE_EXCHANGE_RENEWAL
		EXCHANGE_ITEM_MAX_NUM = 24,
#else
		EXCHANGE_ITEM_MAX_NUM = 12,
#endif
	};

	typedef struct trade
	{
		char					name[CHARACTER_NAME_MAX_LEN + 1];

		DWORD					item_vnum[EXCHANGE_ITEM_MAX_NUM];
#ifdef ENABLE_ITEM_COUNT_LIMIT_SYSTEM
		WORD					item_count[EXCHANGE_ITEM_MAX_NUM];
#else
		BYTE					item_count[EXCHANGE_ITEM_MAX_NUM];
#endif
		DWORD					item_metin[EXCHANGE_ITEM_MAX_NUM][ITEM_SOCKET_SLOT_MAX_NUM];
		TPlayerItemAttribute	item_attr[EXCHANGE_ITEM_MAX_NUM][ITEM_ATTRIBUTE_SLOT_MAX_NUM];

		BYTE					accept;
#ifdef ENABLE_YANG_LIMIT_SYSTEM
			long long				elk;
#else
			DWORD					elk;
#endif
#ifdef ENABLE_EXCHANGE_RENEWAL
			DWORD					race;

			DWORD					level;
#endif
	} TExchangeData;

public:
	CPythonExchange();
	virtual ~CPythonExchange();

	void			Clear();

	void			Start();
	void			End();
	bool			isTrading();

	// Interface

	void			SetSelfName(const char* name);
	void			SetTargetName(const char* name);

	char* GetNameFromSelf();
	char* GetNameFromTarget();

#ifdef ENABLE_YANG_LIMIT_SYSTEM
		void			SetElkToTarget(long long elk);
		void			SetElkToSelf(long long elk);

		long long		GetElkFromTarget();
		long long		GetElkFromSelf();
#else
		void			SetElkToTarget(DWORD elk);
		void			SetElkToSelf(DWORD elk);

		DWORD			GetElkFromTarget();
		DWORD			GetElkFromSelf();
#endif

#ifdef ENABLE_EXCHANGE_RENEWAL	
		void			SetSelfRace(DWORD race);
		void			SetTargetRace(DWORD race);

		DWORD			GetRaceFromSelf();
		DWORD			GetRaceFromTarget();

		void			SetSelfLevel(DWORD level);
		void			SetTargetLevel(DWORD level);

		DWORD			GetLevelFromSelf();
		DWORD			GetLevelFromTarget();
#endif

#ifdef ENABLE_ITEM_COUNT_LIMIT_SYSTEM
	void			SetItemToTarget(DWORD pos, DWORD vnum, WORD count);
	void			SetItemToSelf(DWORD pos, DWORD vnum, WORD count);
#else
	void			SetItemToTarget(DWORD pos, DWORD vnum, BYTE count);
	void			SetItemToSelf(DWORD pos, DWORD vnum, BYTE count);
#endif
	void			SetItemMetinSocketToTarget(int pos, int imetinpos, DWORD vnum);
	void			SetItemMetinSocketToSelf(int pos, int imetinpos, DWORD vnum);

	void			SetItemAttributeToTarget(int pos, int iattrpos, BYTE byType, short sValue);
	void			SetItemAttributeToSelf(int pos, int iattrpos, BYTE byType, short sValue);

	void			DelItemOfTarget(BYTE pos);
	void			DelItemOfSelf(BYTE pos);

	DWORD			GetItemVnumFromTarget(BYTE pos);
	DWORD			GetItemVnumFromSelf(BYTE pos);

#ifdef ENABLE_ITEM_COUNT_LIMIT_SYSTEM
	WORD			GetItemCountFromTarget(BYTE pos);
	WORD			GetItemCountFromSelf(BYTE pos);
#else
	BYTE			GetItemCountFromTarget(BYTE pos);
	BYTE			GetItemCountFromSelf(BYTE pos);
#endif

	DWORD			GetItemMetinSocketFromTarget(BYTE pos, int iMetinSocketPos);
	DWORD			GetItemMetinSocketFromSelf(BYTE pos, int iMetinSocketPos);

	void			GetItemAttributeFromTarget(BYTE pos, int iAttrPos, BYTE* pbyType, short* psValue);
	void			GetItemAttributeFromSelf(BYTE pos, int iAttrPos, BYTE* pbyType, short* psValue);

	void			SetAcceptToTarget(BYTE Accept);
	void			SetAcceptToSelf(BYTE Accept);

	bool			GetAcceptFromTarget();
	bool			GetAcceptFromSelf();

	bool			GetElkMode();
	void			SetElkMode(bool value);

protected:
	bool				m_isTrading;

	bool				m_elk_mode;   // ��ũ�� Ŭ���ؼ� ��ȯ�������� ���� ������.
	TExchangeData		m_self;
	TExchangeData		m_victim;
};
