/********************************************************** Ymir Defines Start ***********************************************************/
#define ENABLE_COSTUME_SYSTEM
#define ENABLE_ENERGY_SYSTEM
#define ENABLE_DRAGON_SOUL_SYSTEM
#define ENABLE_NEW_EQUIPMENT_SYSTEM
/********************************************************** Ymir Defines End ***********************************************************/

#define ENABLE_SERVERNAME_FROM_SOURCE
#define ENABLE_SERVERNAME "Test Server - H4ZEJ"

/********************************************************** General Defines Start ***********************************************************/
#define ENABLE_EXTEND_INVEN_SYSTEM // 4 Envanter
#define ENABLE_WEAPON_COSTUME_SYSTEM // Silah kostümü
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM // Silah kostümü
	#define USE_WEAPON_COSTUME_WITH_EFFECT
	#define USE_BODY_COSTUME_WITH_EFFECT
#endif
#define ENABLE_MOUNT_COSTUME_SYSTEM
#define ENABLE_EXTENDED_PET_SYSTEM
#define ENABLE_HIDE_COSTUME_SYSTEM /// < Hide costume parts
#define ENABLE_ITEM_ATTR_COSTUME // item_attr.sql has column for costume and other type items
#define ENABLE_EXTENDED_ITEMNAME_ON_GROUND // Yere düşen itemleri isimlerini görme
#define ENABLE_CHANNEL_SWITCH_SYSTEM // Hızlı kanal değiştirme
#define ENABLE_CHANNEL_INFO_UPDATE // Kanal bilgisi için düzenleme
#define ENABLE_TARGET_INFORMATION_SYSTEM // Moblardan düşenleri görme
#define ENABLE_VIEW_TARGET_DECIMAL_HP /// < View target decimal hp
#define ENABLE_VIEW_TARGET_PLAYER_HP /// < View target player's hp
#define ENABLE_DAMAGE_BAR // Target HP Board Damage extension
#define ENABLE_SPECIAL_STORAGE // Ek envanter
#define ENABLE_STYLE_ATTRIBUTE_SYSTEM // Kadim efsun
#define ENABLE_GREEN_ATTRIBUTE_SYSTEM // Yeşil efsun için manuel efsun değiştirici
#define ENABLE_BASIC_ITEM_SYSTEM // Basic(Start gift) item system
#define ENABLE_BUY_WITH_ITEM // Shop Item selling by configurable item vnum
#define ENABLE_NEW_ACTIONS // Yeni duygular
#define ENABLE_NEW_BONUS // Yeni bonuslar
#define ENABLE_NEW_PASSIVE_SKILL // Yeni pasif skiller
#define ENABLE_REFINE_RENEWAL // Refine düzenlemesi
#define ENABLE_HIGHLIGHT_NEW_ITEM // Like GF Inventory Highlight Item System
#define ENABLE_DISCORD_RPC // Discord rich presence
#define ENABLE_TEXT_LEVEL_REFRESH // Anlik seviye yenileme
#define ENABLE_DRAGON_BONE_EFFECTS
#define ENABLE_PVP_EFFECT_SYSTEM // PvP efekti
#define ENABLE_SMITH_EFFECT_SYSTEM // Demirci efekti
#define ENABLE_AFFECT_POLYMORPH_REMOVE // Dönüşümden çıkma
#define ENABLE_WHISPER_ALERT // Mesaj geldiğinde exe yanıp söner
#define ENABLE_RACE_HEIGHT_RENEWAL // isim koordinatları ayarlanacak //h4zej
#define ENABLE_FOV_OPTION //
#define ENABLE_FOG_FIX // Like GF Fog Mode System
#define ENABLE_DISABLE_SOFTWARE_TILING // < Disable software tiling
#define ENABLE_ENVIRONMENT_EFFECT_OPTION // < Environment options
#define ENABLE_GRAPHIC_ON_OFF // < System graphic options
#define ENABLE_EFFECT_CAMERA_VIEW_FIX
#define ENABLE_SHADOW_RENDER_QUALITY_OPTION
#define ENABLE_HIDE_OBJECTS_OPTION
#define ENABLE_MINIMAP_WHITEMARK_NEW // new minimap icon
#define ENABLE_INSTANT_PICKUP_SYSTEM // Instant Pickup system
#define ENABLE_AUTO_PICKUP_SYSTEM
#define ENABLE_PICKUP_FILTER_SYSTEM
#define ENABLE_METINSTONE_MINIMAP // Metinstone on minimap
#define ENABLE_BOSS_ON_MINIMAP // Boss on minimap
#define ENABLE_TEXTTAIL_BACKGROUND //YAZILARIN GÖLGESİNİ KALDIRIR FPS ARTAR
#define ENABLE_CONQUEROR_LEVEL_TEXTTAIL
#define ENABLE_LEVEL_SHOW_HIDE
#define ENABLE_FONT_CHANGE
#define ENABLE_SKILL_AFFECT_REMOVE // Skill affect remove system
#define ENABLE_SKILL_COOLTIME_UPDATE
#define ENABLE_GM_MINIMAP_TELEPORT
#define ENABLE_MINIMAP_PARTY_POSITION
#define ENABLE_DROP_DIALOG_EXTENDED_SYSTEM
#define ENABLE_EXP_BLOCK_SYSTEM
#define ENABLE_DUEL_BLOCK_SYSTEM
#define ENABLE_FONT_RENEWAL //Yazı Görünüm Fixi
#define ENABLE_FIX_MOBS_LAG
#ifdef ENABLE_FIX_MOBS_LAG
	// -> The define ENABLE_FIX_MOBS_LAG have problems in device reseting.
	// -> With this new define all this problems are fixed.
	#define FIX_MOBS_LAG_FIX
#endif
#define ENABLE_MOB_DAMAGE_FIX
#define ENABLE_DONT_LOAD_FROM_D_PATH
#define ENABLE_EMOJI_IN_TEXTLINE // Emoji module
#define ENABLE_FAST_TRANSACTIONS //Hızlı sat sil (emoji sistemine bağlı)
#define ENABLE_MULTIPLE_BUY //Shop'tan çoklu item alma
#define ENABLE_AGGREGATE_MONSTER_EFFECT // Like GF Bravery Cape Effect System
#define ENABLE_MESSENGER_TEAM_SYSTEM // Add team page for messenger
#define ENABLE_STATUS_UP_RENEWAL // Fast status up system
#define ENABLE_SORT_INVENTORY_ITEMS
#define ENABLE_EXTEND_EQUIPMENT_SYSTEM
#define ENABLE_FAST_INVENTORY
#define ENABLE_DOTTED_DAMAGE
#ifdef ENABLE_DOTTED_DAMAGE
//	#define USE_M_K	//exmp: 6.581.267.974 > 6T581M267K974
#endif
#define ENABLE_ITEM_COUNT_LIMIT_SYSTEM
#define ENABLE_YANG_LIMIT_SYSTEM
#define ENABLE_VIEW_EQUIPMENT_SYSTEM
#define ENABLE_MOB_SCALE_SYSTEM
#ifdef ENABLE_MOB_SCALE_SYSTEM
	#define ENABLE_OBJ_SCALLING // Mount vnumları ayarlanıcak
#endif
#define ENABLE_QUEST_RENEWAL
#define ENABLE_CUBE_RENEWAL
#define ENABLE_ITEM_DELETE_SYSTEM
#define ENABLE_CHATTING_WINDOW_RENEWAL
#define ENABLE_EXP_LOG
#define ENABLE_GLOBAL_CHAT
#define ENABLE_PM_IN_GLOBAL_CHAT
#define ENABLE_DROP_DESTROY_TIME
#define ENABLE_BRAVERY_CAPE_RENEWAL
#define ENABLE_BOSS_TRACKING
#define ENABLE_QUICK_CHEST_OPEN
#define ENABLE_RENDER_TARGET
#define ENABLE_COINS_RENEWAL_SYSTEM
#define ENABLE_SKILL_SELECT_SYSTEM
#define ENABLE_FAST_SKILLBOOK_READ_SYSTEM // Fast skill book read system
#define ENABLE_FAST_SOULSTONE_READ_SYSTEM // Fast soul stone read system
#define YENI_EFEKT
#ifdef YENI_EFEKT
	#define ZIRH_EFEKT
	#define SILAH_EFEKT
#endif
#define ENABLE_ITEM_SHOP_SYSTEM
#define ENABLE_ANTI_MULTIPLE_FARM
#define ENABLE_PLAYER_PIN_SYSTEM // Player Pin Code System
#define ENABLE_EXCHANGE_RENEWAL
#define ENABLE_CHARACTER_MOBS
#define ENABLE_CHEST_DROP_INFO
#ifdef ENABLE_CHEST_DROP_INFO
	#define ENABLE_CHEST_DROP_USELZO
#endif

/********************************************************** General Defines End ***********************************************************/

/********************************************************** Utility Defines Start ***********************************************************/

/********************************************************** [Guild Defines Start] ***********************************************************/
#define ENABLE_GUILD_LEADER_SYSTEM
#define ENABLE_WAR_PERMISSION // Guild war permission system
#define ENABLE_ONLINE_GUILD_LEADERS // Online guild leaders system
#define ENABLE_AUTO_GUILD_WAR_SYSTEM
#define ENABLE_GUILD_HISTORY // Guild war history system
#define ENABLE_GUILD_STATISTICS // Guild statistics system
#define ENABLE_GUILD_SPY_SYSTEM // Guild spy system
#define ENABLE_GUILD_MEETING // Guild meeting system
#define ENABLE_GUILD_NOTICE_SYSTEM // Guild notice system
#define ENABLE_WAR_KILL_NOTICE // Guild war kill notice system
#define ENABLE_IMPROVED_GUILD_WAR_SYSTEM // Extended guild war send/recv gui and guild war
/********************************************************** [Guild Defines End] ***********************************************************/

#define ENABLE_HORSE_RENEWAL // 1 seviye at atak yapar.
#define ENABLE_NAME_MAX_LEN_RENEWAL
#define ENABLE_LOAD_ALTER_ITEMICON // Iconu olmayan itemler için düzenleme
#define ENABLE_FALL_FIX // Yere düşme sorunu için düzenleme
#define ENABLE_CANSEEHIDDENTHING_FOR_GM // Invisible komutu için düzenleme
#define ENABLE_SAFEZONE_STOP_COLLISION // Etkinleştirilirse korumalı alanda objelere ve moblara takılmazsınız
#ifdef ENABLE_SAFEZONE_STOP_COLLISION
//	#define ENABLE_MOBS_WITHOUT_COLLISIONS //
	#define ENABLE_PETS_WITHOUT_COLLISIONS // Petlerin vnumları ayarlanıcak.
	#define ENABLE_SHOPS_WITHOUT_COLLISIONS // Shopların vnumları ayarlanıcak.
	#define ENABLE_MOUNTS_WITHOUT_COLLISIONS // Mountların vnumları ayarlanıcak.
	#define ENABLE_OX_PLAYER_WITHOUT_COLLISIONS // Remove player collision in ox event map
	#define ENABLE_GM_WITHOUT_COLLISIONS
#endif
#define ENABLE_ATLASINFO_FROM_ROOT // Atlas info düzenlemesi
#define ENABLE_FIX_CREATE_NO_APPROPRIATE_DEVICE
#define ENABLE_CAMERA_MINIMIZARE_CLIENT_FIX
#define ENABLE_SKILL_COOLTIME_FIX
#define ENABLE_HIDE_EMOTION_LOG
#define ENABLE_BOSS_FALL_FIX // Boss fall fixed
#define ENABLE_FIX_LAPTOPS_RESOLUTIONS // Previne o erro CREATE_NO_APPROPRIATE_DEVICE quando um computador não possui determinada resolução
#define ENABLE_SIMPLE_REFINED_EFFECT_CHECK // Simplifica o tipo de verificação para aplicar efeitos nas armas e armaduras
//#define ENABLE_NEW_FOLLOW_AND_ATTACK // tuşa entegre otomatik saldırı

/********************************************************** Utility Defines End ***********************************************************/
