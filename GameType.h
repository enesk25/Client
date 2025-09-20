#pragma once
#include "../GameLib/ItemData.h"

struct SAffects
{
	enum
	{
		AFFECT_MAX_NUM = 32,
	};

	SAffects() : dwAffects(0) {}
	SAffects(const DWORD& c_rAffects)
	{
		__SetAffects(c_rAffects);
	}
	int operator = (const DWORD& c_rAffects)
	{
		__SetAffects(c_rAffects);
	}

	BOOL IsAffect(BYTE byIndex)
	{
		return dwAffects & (1 << byIndex);
	}

	void __SetAffects(const DWORD& c_rAffects)
	{
		dwAffects = c_rAffects;
	}

	DWORD dwAffects;
};

extern std::string g_strGuildSymbolPathName;

const DWORD c_Name_Max_Length = 64;
const DWORD c_FileName_Max_Length = 128;
const DWORD c_Short_Name_Max_Length = 32;

const DWORD c_Inventory_Page_Column = 5;
const DWORD c_Inventory_Page_Row = 9;
const DWORD c_Inventory_Page_Size = c_Inventory_Page_Column * c_Inventory_Page_Row; // x*y
#ifdef ENABLE_EXTEND_INVEN_SYSTEM
const DWORD c_Inventory_Page_Count = 4;
#else
const DWORD c_Inventory_Page_Count = 2;
#endif
const DWORD c_ItemSlot_Count = c_Inventory_Page_Size * c_Inventory_Page_Count;
const DWORD c_Equipment_Count = 12;

const DWORD c_Equipment_Start = c_ItemSlot_Count;

const DWORD c_Equipment_Body = c_Equipment_Start + 0;
const DWORD c_Equipment_Head = c_Equipment_Start + 1;
const DWORD c_Equipment_Shoes = c_Equipment_Start + 2;
const DWORD c_Equipment_Wrist = c_Equipment_Start + 3;
const DWORD c_Equipment_Weapon = c_Equipment_Start + 4;
const DWORD c_Equipment_Neck = c_Equipment_Start + 5;
const DWORD c_Equipment_Ear = c_Equipment_Start + 6;
const DWORD c_Equipment_Unique1 = c_Equipment_Start + 7;
const DWORD c_Equipment_Unique2 = c_Equipment_Start + 8;
const DWORD c_Equipment_Arrow = c_Equipment_Start + 9;
const DWORD c_Equipment_Shield = c_Equipment_Start + 10;

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
const DWORD c_New_Equipment_Start = c_Equipment_Start + 23;
#else
const DWORD c_New_Equipment_Start = c_Equipment_Start + 22;
#endif
#else
const DWORD c_New_Equipment_Start = c_Equipment_Start + 21;
#endif
#ifdef ENABLE_EXTENDED_PET_SYSTEM
const DWORD c_New_Equipment_Count = 4;
#else
const DWORD c_New_Equipment_Count = 3;
#endif
const DWORD c_Equipment_Ring1 = c_New_Equipment_Start + 0;
const DWORD c_Equipment_Ring2 = c_New_Equipment_Start + 1;
const DWORD c_Equipment_Belt = c_New_Equipment_Start + 2;
#ifdef ENABLE_EXTENDED_PET_SYSTEM
const DWORD c_Equipment_Pet  = c_New_Equipment_Start + 3;
#endif
#endif

enum EDragonSoulDeckType
{
	DS_DECK_1,
	DS_DECK_2,
	DS_DECK_MAX_NUM = 2,
};

enum EDragonSoulGradeTypes
{
	DRAGON_SOUL_GRADE_NORMAL,
	DRAGON_SOUL_GRADE_BRILLIANT,
	DRAGON_SOUL_GRADE_RARE,
	DRAGON_SOUL_GRADE_ANCIENT,
	DRAGON_SOUL_GRADE_LEGENDARY,
	DRAGON_SOUL_GRADE_MAX,
};

enum EDragonSoulStepTypes
{
	DRAGON_SOUL_STEP_LOWEST,
	DRAGON_SOUL_STEP_LOW,
	DRAGON_SOUL_STEP_MID,
	DRAGON_SOUL_STEP_HIGH,
	DRAGON_SOUL_STEP_HIGHEST,
	DRAGON_SOUL_STEP_MAX,
};

#ifdef ENABLE_COSTUME_SYSTEM
const DWORD c_Costume_Slot_Start = c_Equipment_Start + 19;
const DWORD	c_Costume_Slot_Body = c_Costume_Slot_Start + 0;
const DWORD	c_Costume_Slot_Hair = c_Costume_Slot_Start + 1;
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
const DWORD	c_Costume_Slot_Weapon = c_Costume_Slot_Start + 2;
#endif
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
const DWORD	c_Costume_Slot_Mount = c_Costume_Slot_Start + 3;
#endif

#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
const DWORD c_Costume_Slot_Count = 4;
#else
const DWORD c_Costume_Slot_Count = 3;
#endif
#else
const DWORD c_Costume_Slot_Count = 2;
#endif

const DWORD c_Costume_Slot_End = c_Costume_Slot_Start + c_Costume_Slot_Count;

#endif

const DWORD c_Wear_Max = 32;
const DWORD c_DragonSoul_Equip_Start = c_ItemSlot_Count + c_Wear_Max;
const DWORD c_DragonSoul_Equip_Slot_Max = 6;
const DWORD c_DragonSoul_Equip_End = c_DragonSoul_Equip_Start + c_DragonSoul_Equip_Slot_Max * DS_DECK_MAX_NUM;

const DWORD c_DragonSoul_Equip_Reserved_Count = c_DragonSoul_Equip_Slot_Max * 3;

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
const DWORD c_Belt_Inventory_Slot_Start = c_DragonSoul_Equip_End + c_DragonSoul_Equip_Reserved_Count;
const DWORD c_Belt_Inventory_Width = 4;
const DWORD c_Belt_Inventory_Height = 4;
const DWORD c_Belt_Inventory_Slot_Count = c_Belt_Inventory_Width * c_Belt_Inventory_Height;
const DWORD c_Belt_Inventory_Slot_End = c_Belt_Inventory_Slot_Start + c_Belt_Inventory_Slot_Count;

const DWORD c_Inventory_Count = c_Belt_Inventory_Slot_End;
#else
const DWORD c_Inventory_Count = c_DragonSoul_Equip_End;
#endif

const DWORD c_DragonSoul_Inventory_Start = 0;
const DWORD c_DragonSoul_Inventory_Box_Size = 32;
const DWORD c_DragonSoul_Inventory_Count = CItemData::DS_SLOT_NUM_TYPES * DRAGON_SOUL_GRADE_MAX * c_DragonSoul_Inventory_Box_Size;
const DWORD c_DragonSoul_Inventory_End = c_DragonSoul_Inventory_Start + c_DragonSoul_Inventory_Count;

#ifdef ENABLE_SPECIAL_STORAGE
const DWORD c_Special_Inventory_Page_Size = 5 * 9;
const DWORD c_Special_Inventory_Page_Count = 4;
const DWORD c_Special_ItemSlot_Count = c_Special_Inventory_Page_Size * c_Special_Inventory_Page_Count;
#endif

enum ESlotType
{
	SLOT_TYPE_NONE,
	SLOT_TYPE_INVENTORY,
	SLOT_TYPE_SKILL,
	SLOT_TYPE_EMOTION,
	SLOT_TYPE_SHOP,
	SLOT_TYPE_EXCHANGE_OWNER,
	SLOT_TYPE_EXCHANGE_TARGET,
	SLOT_TYPE_QUICK_SLOT,
	SLOT_TYPE_SAFEBOX,
	SLOT_TYPE_PRIVATE_SHOP,
	SLOT_TYPE_MALL,
	SLOT_TYPE_DRAGON_SOUL_INVENTORY,
#ifdef ENABLE_SPECIAL_STORAGE
	SLOT_TYPE_UPGRADE_INVENTORY,
	SLOT_TYPE_BOOK_INVENTORY,
	SLOT_TYPE_STONE_INVENTORY,
#endif
	SLOT_TYPE_MAX,
};

enum EWindows
{
	RESERVED_WINDOW,
	INVENTORY,
	EQUIPMENT,
	SAFEBOX,
	MALL,
	DRAGON_SOUL_INVENTORY,
#ifdef ENABLE_SPECIAL_STORAGE
	UPGRADE_INVENTORY,
	BOOK_INVENTORY,
	STONE_INVENTORY,
#endif
	GROUND,
	BELT_INVENTORY,

	WINDOW_TYPE_MAX,
};

enum EDSInventoryMaxNum
{
	DS_INVENTORY_MAX_NUM = c_DragonSoul_Inventory_Count,
	DS_REFINE_WINDOW_MAX_NUM = 15,
};

#pragma pack (push, 1)
#define WORD_MAX 0xffff

typedef struct SItemPos
{
	BYTE window_type;
	WORD cell;
	SItemPos()
	{
		window_type = INVENTORY;
		cell = WORD_MAX;
	}
	SItemPos(BYTE _window_type, WORD _cell)
	{
		window_type = _window_type;
		cell = _cell;
	}

	bool IsValidCell()
	{
		switch (window_type)
		{
		case INVENTORY:
			return cell < c_Inventory_Count;
			break;
		case EQUIPMENT:
			return cell < c_DragonSoul_Equip_End;
			break;
		case DRAGON_SOUL_INVENTORY:
			return cell < (DS_INVENTORY_MAX_NUM);
			break;
#ifdef ENABLE_SPECIAL_STORAGE
		case UPGRADE_INVENTORY:
			return cell < c_Special_ItemSlot_Count;
			break;
		case BOOK_INVENTORY:
			return cell < c_Special_ItemSlot_Count;
			break;
		case STONE_INVENTORY:
			return cell < c_Special_ItemSlot_Count;
			break;
#endif
		default:
			return false;
		}
	}
	bool IsEquipCell()
	{
		switch (window_type)
		{
		case INVENTORY:
		case EQUIPMENT:
			return (c_Equipment_Start + c_Wear_Max > cell) && (c_Equipment_Start <= cell);
			break;

		case BELT_INVENTORY:
		case DRAGON_SOUL_INVENTORY:
#ifdef ENABLE_SPECIAL_STORAGE
		case UPGRADE_INVENTORY:
		case BOOK_INVENTORY:
		case STONE_INVENTORY:
#endif
			return false;
			break;

		default:
			return false;
		}
	}

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	bool IsBeltInventoryCell()
	{
		bool bResult = c_Belt_Inventory_Slot_Start <= cell && c_Belt_Inventory_Slot_End > cell;
		return bResult;
	}
#endif

	bool operator==(const struct SItemPos& rhs) const
	{
		return (window_type == rhs.window_type) && (cell == rhs.cell);
	}

	bool operator<(const struct SItemPos& rhs) const
	{
		return (window_type < rhs.window_type) || ((window_type == rhs.window_type) && (cell < rhs.cell));
	}
} TItemPos;
#pragma pack(pop)

const DWORD c_QuickBar_Line_Count = 3;
const DWORD c_QuickBar_Slot_Count = 12;

const float c_Idle_WaitTime = 5.0f;

const int c_Monster_Race_Start_Number = 6;
const int c_Monster_Model_Start_Number = 20001;

const float c_fAttack_Delay_Time = 0.2f;
const float c_fHit_Delay_Time = 0.1f;
const float c_fCrash_Wave_Time = 0.2f;
const float c_fCrash_Wave_Distance = 3.0f;

const float c_fHeight_Step_Distance = 50.0f;

enum
{
	DISTANCE_TYPE_FOUR_WAY,
	DISTANCE_TYPE_EIGHT_WAY,
	DISTANCE_TYPE_ONE_WAY,
	DISTANCE_TYPE_MAX_NUM,
};

const float c_fMagic_Script_Version = 1.0f;
const float c_fSkill_Script_Version = 1.0f;
const float c_fMagicSoundInformation_Version = 1.0f;
const float c_fBattleCommand_Script_Version = 1.0f;
const float c_fEmotionCommand_Script_Version = 1.0f;
const float c_fActive_Script_Version = 1.0f;
const float c_fPassive_Script_Version = 1.0f;

const float c_fWalkDistance = 175.0f;
const float c_fRunDistance = 310.0f;

#define FILE_MAX_LEN 128

enum
{
	ITEM_SOCKET_SLOT_MAX_NUM = 3,
	ITEM_ATTRIBUTE_SLOT_MAX_NUM = 7,
};

#pragma pack(push)
#pragma pack(1)

typedef struct SQuickSlot
{
	BYTE Type;
	BYTE Position;
} TQuickSlot;

typedef struct TPlayerItemAttribute
{
	BYTE        bType;
	short       sValue;
} TPlayerItemAttribute;

typedef struct packet_item
{
	DWORD       vnum;
#ifdef ENABLE_ITEM_COUNT_LIMIT_SYSTEM
	WORD		count;
#else
    BYTE        count;
#endif
	DWORD		flags;
	DWORD		anti_flags;
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_BASIC_ITEM_SYSTEM
	bool		is_basic;
#endif
} TItemData;

typedef struct packet_shop_item
{
	DWORD       vnum;
#ifdef ENABLE_YANG_LIMIT_SYSTEM
	long long	price;
#else
    DWORD       price;
#endif
#ifdef ENABLE_ITEM_COUNT_LIMIT_SYSTEM
	WORD		count;
#else
	BYTE        count;
#endif
	BYTE		display_pos;
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_BUY_WITH_ITEM
	DWORD		witemVnum;
#endif
} TShopItemData;

#pragma pack(pop)

inline float GetSqrtDistance(int ix1, int iy1, int ix2, int iy2)
{
	float dx, dy;

	dx = float(ix1 - ix2);
	dy = float(iy1 - iy2);

	return sqrtf(dx * dx + dy * dy);
}

void DefaultFont_Startup();
void DefaultFont_Cleanup();
void DefaultFont_SetName(const char* c_szFontName);
CResource* DefaultFont_GetResource();
CResource* DefaultItalicFont_GetResource();

void SetGuildSymbolPath(const char* c_szPathName);
const char* GetGuildSymbolFileName(DWORD dwGuildID);
BYTE SlotTypeToInvenType(BYTE bSlotType);
