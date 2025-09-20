//
// Ä³¸¯ÅÍ¸¦ µû¶ó´Ù´Ï´Â ÅØ½ºÆ® °ü·Ã ¼Ò½º (ÀÌ¸§, ±æµåÀÌ¸§, ±æµå¸¶Å© µî)
//
#include "stdafx.h"
#include "InstanceBase.h"
#include "resource.h"
#include "PythonTextTail.h"
#include "PythonCharacterManager.h"
#include "PythonGuild.h"
#include "Locale.h"
#include "MarkManager.h"
#ifdef ENABLE_LEVEL_SHOW_HIDE
#include "PythonSystem.h"
#endif

const D3DXCOLOR c_TextTail_Player_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR c_TextTail_Monster_Color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR c_TextTail_Item_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
#ifdef ENABLE_EXTENDED_ITEMNAME_ON_GROUND
const D3DXCOLOR c_TextTail_SpecialItem_Color = D3DXCOLOR(1.0f, 0.67f, 0.0f, 1.0f); //Golden
#endif
const D3DXCOLOR c_TextTail_Chat_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR c_TextTail_Info_Color = D3DXCOLOR(1.0f, 0.785f, 0.785f, 1.0f);
const D3DXCOLOR c_TextTail_Guild_Name_Color = 0xFFEFD3FF;
const float c_TextTail_Name_Position = -10.0f;
const float c_fxMarkPosition = 1.5f;
const float c_fyGuildNamePosition = 15.0f;
const float c_fyMarkPosition = 15.0f + 11.0f;
BOOL bPKTitleEnable = TRUE;

// TEXTTAIL_LIVINGTIME_CONTROL
long gs_TextTail_LivingTime = 5000;

long TextTail_GetLivingTime()
{
	assert(gs_TextTail_LivingTime > 1000);
	return gs_TextTail_LivingTime;
}

void TextTail_SetLivingTime(long livingTime)
{
	gs_TextTail_LivingTime = livingTime;
}
// END_OF_TEXTTAIL_LIVINGTIME_CONTROL

CGraphicText* ms_pFont = NULL;

void CPythonTextTail::GetInfo(std::string* pstInfo)
{
	char szInfo[256];
	sprintf(szInfo, "TextTail: ChatTail %d, ChrTail (Map %d, List %d), ItemTail (Map %d, List %d), Pool %d",
		m_ChatTailMap.size(),
		m_CharacterTextTailMap.size(), m_CharacterTextTailList.size(),
		m_ItemTextTailMap.size(), m_ItemTextTailList.size(),
		m_TextTailPool.GetCapacity());

	pstInfo->append(szInfo);
}

void CPythonTextTail::UpdateAllTextTail()
{
	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetMainInstancePtr();
	if (pInstance)
	{
		TPixelPosition pixelPos;
		pInstance->NEW_GetPixelPosition(&pixelPos);

		TTextTailMap::iterator itorMap;

		for (itorMap = m_CharacterTextTailMap.begin(); itorMap != m_CharacterTextTailMap.end(); ++itorMap)
		{
			UpdateDistance(pixelPos, itorMap->second);
		}

		for (itorMap = m_ItemTextTailMap.begin(); itorMap != m_ItemTextTailMap.end(); ++itorMap)
		{
			UpdateDistance(pixelPos, itorMap->second);
		}

		for (TChatTailMap::iterator itorChat = m_ChatTailMap.begin(); itorChat != m_ChatTailMap.end(); ++itorChat)
		{
			UpdateDistance(pixelPos, itorChat->second);

			// NOTE : Chat TextTailÀÌ ÀÖÀ¸¸é Ä³¸¯ÅÍ ÀÌ¸§µµ Ãâ·ÂÇÑ´Ù.
			if (itorChat->second->bNameFlag)
			{
				DWORD dwVID = itorChat->first;
				ShowCharacterTextTail(dwVID);
			}
		}
	}
}

void CPythonTextTail::UpdateShowingTextTail()
{
	TTextTailList::iterator itor;

	for (itor = m_ItemTextTailList.begin(); itor != m_ItemTextTailList.end(); ++itor)
	{
		UpdateTextTail(*itor);
	}

	for (TChatTailMap::iterator itorChat = m_ChatTailMap.begin(); itorChat != m_ChatTailMap.end(); ++itorChat)
	{
		UpdateTextTail(itorChat->second);
	}

	for (itor = m_CharacterTextTailList.begin(); itor != m_CharacterTextTailList.end(); ++itor)
	{
		TTextTail* pTextTail = *itor;
		UpdateTextTail(pTextTail);

		// NOTE : Chat TextTailÀÌ ÀÖÀ» °æ¿ì À§Ä¡¸¦ ¹Ù²Û´Ù.
		TChatTailMap::iterator itor = m_ChatTailMap.find(pTextTail->dwVirtualID);
		if (m_ChatTailMap.end() != itor)
		{
			TTextTail* pChatTail = itor->second;
			if (pChatTail->bNameFlag)
			{
				pTextTail->y = pChatTail->y - 17.0f;
			}
		}
	}
}

void CPythonTextTail::UpdateTextTail(TTextTail* pTextTail)
{
	if (!pTextTail->pOwner)
		return;

	/////

	CPythonGraphic& rpyGraphic = CPythonGraphic::Instance();
	rpyGraphic.Identity();

	const D3DXVECTOR3& c_rv3Position = pTextTail->pOwner->GetPosition();
	rpyGraphic.ProjectPosition(c_rv3Position.x,
		c_rv3Position.y,
		c_rv3Position.z + pTextTail->fHeight,
		&pTextTail->x,
		&pTextTail->y,
		&pTextTail->z);

	pTextTail->x = floorf(pTextTail->x);
	pTextTail->y = floorf(pTextTail->y);

	// NOTE : 13m ¹Û¿¡ ÀÖÀ»¶§¸¸ ±íÀÌ¸¦ ³Ö½À´Ï´Ù - [levites]
	if (pTextTail->fDistanceFromPlayer < 1300.0f)
	{
		pTextTail->z = 0.0f;
	}
	else
	{
		pTextTail->z = pTextTail->z * CPythonGraphic::Instance().GetOrthoDepth() * -1.0f;
		pTextTail->z += 10.0f;
	}
}

void CPythonTextTail::ArrangeTextTail()
{
	TTextTailList::iterator itor;
	TTextTailList::iterator itorCompare;

	DWORD dwTime = CTimer::Instance().GetCurrentMillisecond();

	for (itor = m_ItemTextTailList.begin(); itor != m_ItemTextTailList.end(); ++itor)
	{
		TTextTail* pInsertTextTail = *itor;

		int yTemp = 5;
		int LimitCount = 0;

		for (itorCompare = m_ItemTextTailList.begin(); itorCompare != m_ItemTextTailList.end();)
		{
			TTextTail* pCompareTextTail = *itorCompare;

			if (*itorCompare == *itor)
			{
				++itorCompare;
				continue;
			}

			if (LimitCount >= 20)
				break;

			if (isIn(pInsertTextTail, pCompareTextTail))
			{
				pInsertTextTail->y = (pCompareTextTail->y + pCompareTextTail->yEnd + yTemp);

				itorCompare = m_ItemTextTailList.begin();
				++LimitCount;
				continue;
			}

			++itorCompare;
		}

#ifdef ENABLE_DROP_DESTROY_TIME
		BYTE bLine = 0;
		for (CGraphicTextInstance* obj : { pInsertTextTail->pDestroyTimeTextInstance, pInsertTextTail->pOwnerTextInstance, pInsertTextTail->pTextInstance })
		{
			if (obj == nullptr)
				continue;

			if (obj == pInsertTextTail->pTextInstance)
				obj->SetColor(pInsertTextTail->Color.r, pInsertTextTail->Color.g, pInsertTextTail->Color.b);

			obj->SetPosition(pInsertTextTail->x, pInsertTextTail->y + ((bLine++) * 15.0f), pInsertTextTail->z);
			obj->Update();
		}
#else
		if (pInsertTextTail->pOwnerTextInstance)
		{
			pInsertTextTail->pOwnerTextInstance->SetPosition(pInsertTextTail->x, pInsertTextTail->y, pInsertTextTail->z);
			pInsertTextTail->pOwnerTextInstance->Update();

			pInsertTextTail->pTextInstance->SetColor(pInsertTextTail->Color.r, pInsertTextTail->Color.g, pInsertTextTail->Color.b);
			pInsertTextTail->pTextInstance->SetPosition(pInsertTextTail->x, pInsertTextTail->y + 15.0f, pInsertTextTail->z);
			pInsertTextTail->pTextInstance->Update();

		}
		else
		{
			pInsertTextTail->pTextInstance->SetColor(pInsertTextTail->Color.r, pInsertTextTail->Color.g, pInsertTextTail->Color.b);
			pInsertTextTail->pTextInstance->SetPosition(pInsertTextTail->x, pInsertTextTail->y, pInsertTextTail->z);
			pInsertTextTail->pTextInstance->Update();

		}
#endif
	}

	for (itor = m_CharacterTextTailList.begin(); itor != m_CharacterTextTailList.end(); ++itor)
	{
		TTextTail* pTextTail = *itor;

		float fxAdd = 0.0f;

		// Mark À§Ä¡ ¾÷µ¥ÀÌÆ®
		CGraphicMarkInstance* pMarkInstance = pTextTail->pMarkInstance;
		CGraphicTextInstance* pGuildNameInstance = pTextTail->pGuildNameTextInstance;
		if (pMarkInstance && pGuildNameInstance)
		{
			int iWidth, iHeight;
			int iImageHalfSize = pMarkInstance->GetWidth() / 2 + c_fxMarkPosition;
			pGuildNameInstance->GetTextSize(&iWidth, &iHeight);

			pMarkInstance->SetPosition(pTextTail->x - iWidth / 2 - iImageHalfSize, pTextTail->y - c_fyMarkPosition);
			pGuildNameInstance->SetPosition(pTextTail->x + iImageHalfSize, pTextTail->y - c_fyGuildNamePosition, pTextTail->z);
			pGuildNameInstance->Update();
		}

		int iNameWidth, iNameHeight;
		pTextTail->pTextInstance->GetTextSize(&iNameWidth, &iNameHeight);

		// Title À§Ä¡ ¾÷µ¥ÀÌÆ®
		CGraphicTextInstance* pTitle = pTextTail->pTitleTextInstance;
		if (pTitle)
		{
			int iTitleWidth, iTitleHeight;
			pTitle->GetTextSize(&iTitleWidth, &iTitleHeight);

			fxAdd = 8.0f;

			pTitle->SetPosition(pTextTail->x - (iNameWidth / 2), pTextTail->y, pTextTail->z);
			pTitle->Update();

			// Level À§Ä¡ ¾÷µ¥ÀÌÆ®
			CGraphicTextInstance* pLevel = pTextTail->pLevelTextInstance;
			if (pLevel)
			{
				int iLevelWidth, iLevelHeight;
				pLevel->GetTextSize(&iLevelWidth, &iLevelHeight);
				pLevel->SetPosition(pTextTail->x - (iNameWidth / 2) - iTitleWidth, pTextTail->y, pTextTail->z);
				pLevel->Update();
			}
		}
		else
		{
			fxAdd = 4.0f;

			// Level À§Ä¡ ¾÷µ¥ÀÌÆ®
			CGraphicTextInstance* pLevel = pTextTail->pLevelTextInstance;
			if (pLevel)
			{
				int iLevelWidth, iLevelHeight;
				pLevel->GetTextSize(&iLevelWidth, &iLevelHeight);
				pLevel->SetPosition(pTextTail->x - (iNameWidth / 2), pTextTail->y, pTextTail->z);
				pLevel->Update();
			}
		}

		pTextTail->pTextInstance->SetColor(pTextTail->Color.r, pTextTail->Color.g, pTextTail->Color.b);
		pTextTail->pTextInstance->SetPosition(pTextTail->x + fxAdd, pTextTail->y, pTextTail->z);
		pTextTail->pTextInstance->Update();
	}

	for (TChatTailMap::iterator itorChat = m_ChatTailMap.begin(); itorChat != m_ChatTailMap.end();)
	{
		TTextTail* pTextTail = itorChat->second;

		if (pTextTail->LivingTime < dwTime)
		{
			DeleteTextTail(pTextTail);
			itorChat = m_ChatTailMap.erase(itorChat);
			continue;
		}
		else
			++itorChat;

		pTextTail->pTextInstance->SetColor(pTextTail->Color);
		pTextTail->pTextInstance->SetPosition(pTextTail->x, pTextTail->y, pTextTail->z);
		pTextTail->pTextInstance->Update();
	}
}

void CPythonTextTail::Render()
{
	TTextTailList::iterator itor;

	for (itor = m_CharacterTextTailList.begin(); itor != m_CharacterTextTailList.end(); ++itor)
	{
		TTextTail* pTextTail = *itor;
		pTextTail->pTextInstance->Render();
		if (pTextTail->pMarkInstance && pTextTail->pGuildNameTextInstance)
		{
			pTextTail->pMarkInstance->Render();
			pTextTail->pGuildNameTextInstance->Render();
		}
		if (pTextTail->pTitleTextInstance)
		{
			pTextTail->pTitleTextInstance->Render();
		}
#ifdef ENABLE_LEVEL_SHOW_HIDE
		if (pTextTail->pLevelTextInstance && (pTextTail->bIsPC == TRUE && CPythonSystem::Instance().IsShowLevel()))
#else
		if (pTextTail->pLevelTextInstance)
#endif
		{
			pTextTail->pLevelTextInstance->Render();
		}
	}

	for (itor = m_ItemTextTailList.begin(); itor != m_ItemTextTailList.end(); ++itor)
	{
		TTextTail* pTextTail = *itor;

		RenderTextTailBox(pTextTail);
		pTextTail->pTextInstance->Render();
		if (pTextTail->pOwnerTextInstance)
			pTextTail->pOwnerTextInstance->Render();
#ifdef ENABLE_DROP_DESTROY_TIME
		if (pTextTail->pDestroyTimeTextInstance)
			pTextTail->pDestroyTimeTextInstance->Render();
#endif
	}

	for (TChatTailMap::iterator itorChat = m_ChatTailMap.begin(); itorChat != m_ChatTailMap.end(); ++itorChat)
	{
		TTextTail* pTextTail = itorChat->second;
		if (pTextTail->pOwner->isShow())
			RenderTextTailName(pTextTail);
	}
}

void CPythonTextTail::RenderTextTailBox(TTextTail* pTextTail)
{
	// °ËÀº»ö Å×µÎ¸®
	CPythonGraphic::Instance().SetDiffuseColor(0.0f, 0.0f, 0.0f, 1.0f);
	CPythonGraphic::Instance().RenderBox2d(pTextTail->x + pTextTail->xStart,
		pTextTail->y + pTextTail->yStart,
		pTextTail->x + pTextTail->xEnd,
		pTextTail->y + pTextTail->yEnd,
		pTextTail->z);

	// °ËÀº»ö Åõ¸í¹Ú½º
	CPythonGraphic::Instance().SetDiffuseColor(0.0f, 0.0f, 0.0f, 0.3f);
	CPythonGraphic::Instance().RenderBar2d(pTextTail->x + pTextTail->xStart,
		pTextTail->y + pTextTail->yStart,
		pTextTail->x + pTextTail->xEnd,
		pTextTail->y + pTextTail->yEnd,
		pTextTail->z);
}

void CPythonTextTail::RenderTextTailName(TTextTail* pTextTail)
{
	pTextTail->pTextInstance->Render();
}

void CPythonTextTail::HideAllTextTail()
{
	// NOTE : Show AllÀ» ÇØÁØµÚ Hide AllÀ» ÇØÁÖÁö ¾ÊÀ¸¸é ¹®Á¦ ¹ß»ý °¡´É¼º ÀÖÀ½
	//        µðÀÚÀÎ ÀÚÃ¼°¡ ±×·¸°Ô ±ò²ûÇÏ°Ô µÇÁö ¾Ê¾ÒÀ½ - [levites]
	m_CharacterTextTailList.clear();
	m_ItemTextTailList.clear();
}

void CPythonTextTail::UpdateDistance(const TPixelPosition& c_rCenterPosition, TTextTail* pTextTail)
{
	const D3DXVECTOR3& c_rv3Position = pTextTail->pOwner->GetPosition();
	D3DXVECTOR2 v2Distance(c_rv3Position.x - c_rCenterPosition.x, -c_rv3Position.y - c_rCenterPosition.y);
	pTextTail->fDistanceFromPlayer = D3DXVec2Length(&v2Distance);
}

void CPythonTextTail::ShowAllTextTail()
{
	TTextTailMap::iterator itor;
	for (itor = m_CharacterTextTailMap.begin(); itor != m_CharacterTextTailMap.end(); ++itor)
	{
		TTextTail* pTextTail = itor->second;
		if (pTextTail->fDistanceFromPlayer < 3500.0f)
			ShowCharacterTextTail(itor->first);
	}
	for (itor = m_ItemTextTailMap.begin(); itor != m_ItemTextTailMap.end(); ++itor)
	{
		TTextTail* pTextTail = itor->second;
		if (pTextTail->fDistanceFromPlayer < 3500.0f)
			ShowItemTextTail(itor->first);
	}
}

void CPythonTextTail::ShowCharacterTextTail(DWORD VirtualID)
{
	TTextTailMap::iterator itor = m_CharacterTextTailMap.find(VirtualID);

	if (m_CharacterTextTailMap.end() == itor)
		return;

	TTextTail* pTextTail = itor->second;

	if (m_CharacterTextTailList.end() != std::find(m_CharacterTextTailList.begin(), m_CharacterTextTailList.end(), pTextTail))
	{
		//Tracef("ÀÌ¹Ì ¸®½ºÆ®¿¡ ÀÖÀ½ : %d\n", VirtualID);
		return;
	}

	// NOTE : ShowAll ½Ã¿¡´Â ¸ðµç Instance ÀÇ Pointer ¸¦ Ã£¾Æ¼­ Ã¼Å©ÇÏ¹Ç·Î ºÎÇÏ°¡ °É¸± °¡´É¼ºµµ ÀÖ´Ù.
	//        CInstanceBase °¡ TextTail À» Á÷Á¢ °¡Áö°í ÀÖ´Â °ÍÀÌ °¡Àå ÁÁÀº ÇüÅÂÀÏ µí..
	if (!pTextTail->pOwner->isShow())
		return;

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(pTextTail->dwVirtualID);
	if (!pInstance)
		return;

#ifdef ENABLE_GRAPHIC_ON_OFF
	if (CPythonSystem::instance().IsNpcNameStatus() == 1)
		if (pInstance->IsNPC())
			return;
#endif

	if (pInstance->IsGuildWall())
		return;

#ifdef ENABLE_AUTO_GUILD_WAR_SYSTEM
	if (pInstance->GetRace() == 20375)
		return;
#endif

	if (pInstance->CanPickInstance())
		m_CharacterTextTailList.push_back(pTextTail);
}

void CPythonTextTail::ShowItemTextTail(DWORD VirtualID)
{
	TTextTailMap::iterator itor = m_ItemTextTailMap.find(VirtualID);

	if (m_ItemTextTailMap.end() == itor)
		return;

#ifdef ENABLE_GRAPHIC_ON_OFF
	if (CPythonSystem::instance().GetDropItemLevel() >= 2)
		return;
#endif

	TTextTail* pTextTail = itor->second;

	if (m_ItemTextTailList.end() != std::find(m_ItemTextTailList.begin(), m_ItemTextTailList.end(), pTextTail))
	{
		//Tracef("ÀÌ¹Ì ¸®½ºÆ®¿¡ ÀÖÀ½ : %d\n", VirtualID);
		return;
	}

	m_ItemTextTailList.push_back(pTextTail);
}

bool CPythonTextTail::isIn(CPythonTextTail::TTextTail* pSource, CPythonTextTail::TTextTail* pTarget)
{
	float x1Source = pSource->x + pSource->xStart;
	float y1Source = pSource->y + pSource->yStart;
	float x2Source = pSource->x + pSource->xEnd;
	float y2Source = pSource->y + pSource->yEnd;
	float x1Target = pTarget->x + pTarget->xStart;
	float y1Target = pTarget->y + pTarget->yStart;
	float x2Target = pTarget->x + pTarget->xEnd;
	float y2Target = pTarget->y + pTarget->yEnd;

	if (x1Source <= x2Target && x2Source >= x1Target &&
		y1Source <= y2Target && y2Source >= y1Target)
	{
		return true;
	}

	return false;
}

#ifdef ENABLE_GUILD_LEADER_SYSTEM
void CPythonTextTail::RegisterCharacterTextTail(DWORD dwGuildID, BYTE dwNewIsGuildName, DWORD dwVirtualID, const D3DXCOLOR& c_rColor, float fAddHeight)
#else
void CPythonTextTail::RegisterCharacterTextTail(DWORD dwGuildID, DWORD dwVirtualID, const D3DXCOLOR& c_rColor, float fAddHeight)
#endif
{
	CInstanceBase* pCharacterInstance = CPythonCharacterManager::Instance().GetInstancePtr(dwVirtualID);

	if (!pCharacterInstance)
		return;

	TTextTail* pTextTail = RegisterTextTail(dwVirtualID,
		pCharacterInstance->GetNameString(),
		pCharacterInstance->GetGraphicThingInstancePtr(),
		pCharacterInstance->GetGraphicThingInstanceRef().GetHeight() + fAddHeight,
		c_rColor);

	CGraphicTextInstance* pTextInstance = pTextTail->pTextInstance;
#ifdef ENABLE_TEXTTAIL_BACKGROUND
	pTextInstance->SetOutline(false);
#else
	pTextInstance->SetOutline(true);
#endif
	pTextInstance->SetVerticalAlign(CGraphicTextInstance::VERTICAL_ALIGN_BOTTOM);

	pTextTail->pMarkInstance = NULL;
	pTextTail->pGuildNameTextInstance = NULL;
	pTextTail->pTitleTextInstance = NULL;
	pTextTail->pLevelTextInstance = NULL;

	if (0 != dwGuildID)
	{
		pTextTail->pMarkInstance = CGraphicMarkInstance::New();

		DWORD dwMarkID = CGuildMarkManager::Instance().GetMarkID(dwGuildID);

		if (dwMarkID != CGuildMarkManager::INVALID_MARK_ID)
		{
			std::string markImagePath;

			if (CGuildMarkManager::Instance().GetMarkImageFilename(dwMarkID / CGuildMarkImage::MARK_TOTAL_COUNT, markImagePath))
			{
				pTextTail->pMarkInstance->SetImageFileName(markImagePath.c_str());
				pTextTail->pMarkInstance->Load();
				pTextTail->pMarkInstance->SetIndex(dwMarkID % CGuildMarkImage::MARK_TOTAL_COUNT);
			}
		}

		std::string strGuildName;
		if (!CPythonGuild::Instance().GetGuildName(dwGuildID, &strGuildName))
			strGuildName = "Noname";
#ifdef ENABLE_GUILD_LEADER_SYSTEM
		if (dwNewIsGuildName == 3)
			strGuildName.insert(0, "[Lonca Lideri] - ");
		else if (dwNewIsGuildName == 2)
			strGuildName.insert(0, "[Lonca Generali] - ");
		else
			strGuildName.insert(0, "[Lonca Üyesi] - ");
#endif

		CGraphicTextInstance*& prGuildNameInstance = pTextTail->pGuildNameTextInstance;
		prGuildNameInstance = CGraphicTextInstance::New();
		prGuildNameInstance->SetTextPointer(ms_pFont);
#ifdef ENABLE_TEXTTAIL_BACKGROUND
		prGuildNameInstance->SetOutline(false);
#else
		prGuildNameInstance->SetOutline(true);
#endif
		prGuildNameInstance->SetHorizonalAlign(CGraphicTextInstance::HORIZONTAL_ALIGN_CENTER);
		prGuildNameInstance->SetVerticalAlign(CGraphicTextInstance::VERTICAL_ALIGN_BOTTOM);
		prGuildNameInstance->SetValue(strGuildName.c_str());
		prGuildNameInstance->SetColor(c_TextTail_Guild_Name_Color.r, c_TextTail_Guild_Name_Color.g, c_TextTail_Guild_Name_Color.b);
		prGuildNameInstance->Update();
	}

	m_CharacterTextTailMap.insert(TTextTailMap::value_type(dwVirtualID, pTextTail));
#ifdef ENABLE_LEVEL_SHOW_HIDE
	pTextTail->bIsPC = pCharacterInstance->IsPC() != FALSE;
#endif
}

#ifdef ENABLE_EXTENDED_ITEMNAME_ON_GROUND
void CPythonTextTail::RegisterItemTextTail(DWORD VirtualID, const char* c_szText, CGraphicObjectInstance* pOwner, bool bHasAttr)
#else
void CPythonTextTail::RegisterItemTextTail(DWORD VirtualID, const char* c_szText, CGraphicObjectInstance* pOwner)
#endif
{
#ifdef __DEBUG
	char szName[256];
	spritnf(szName, "%s[%d]", c_szText, VirtualID);
#endif

	D3DXCOLOR c_d3dColor = c_TextTail_Item_Color;
#ifdef ENABLE_EXTENDED_ITEMNAME_ON_GROUND
	if (bHasAttr)
		c_d3dColor = c_TextTail_SpecialItem_Color;
#endif

	TTextTail* pTextTail = RegisterTextTail(VirtualID, c_szText, pOwner, c_TextTail_Name_Position, c_d3dColor);
	m_ItemTextTailMap.insert(TTextTailMap::value_type(VirtualID, pTextTail));
}

void CPythonTextTail::RegisterChatTail(DWORD VirtualID, const char* c_szChat)
{
	CInstanceBase* pCharacterInstance = CPythonCharacterManager::Instance().GetInstancePtr(VirtualID);

	if (!pCharacterInstance)
		return;

	TChatTailMap::iterator itor = m_ChatTailMap.find(VirtualID);

	if (m_ChatTailMap.end() != itor)
	{
		TTextTail* pTextTail = itor->second;

		pTextTail->pTextInstance->SetValue(c_szChat);
		pTextTail->pTextInstance->Update();
		pTextTail->Color = c_TextTail_Chat_Color;
		pTextTail->pTextInstance->SetColor(c_TextTail_Chat_Color);

		// TEXTTAIL_LIVINGTIME_CONTROL
		pTextTail->LivingTime = CTimer::Instance().GetCurrentMillisecond() + TextTail_GetLivingTime();
		// END_OF_TEXTTAIL_LIVINGTIME_CONTROL

		pTextTail->bNameFlag = TRUE;

		return;
	}

#ifdef ENABLE_RACE_HEIGHT_RENEWAL
	TTextTail* pTextTail = RegisterTextTail(VirtualID, c_szChat, pCharacterInstance->GetGraphicThingInstancePtr(), pCharacterInstance->IsMountingHorse() ? pCharacterInstance->GetGraphicThingInstanceRef().GetHeight() + 110.0f : pCharacterInstance->GetBaseHeight() + pCharacterInstance->GetGraphicThingInstanceRef().GetHeight() + 10.0f, c_TextTail_Chat_Color);
#else
	TTextTail* pTextTail = RegisterTextTail(VirtualID, c_szChat, pCharacterInstance->GetGraphicThingInstancePtr(), pCharacterInstance->GetGraphicThingInstanceRef().GetHeight() + 10.0f, c_TextTail_Chat_Color);
#endif

	// TEXTTAIL_LIVINGTIME_CONTROL
	pTextTail->LivingTime = CTimer::Instance().GetCurrentMillisecond() + TextTail_GetLivingTime();
	// END_OF_TEXTTAIL_LIVINGTIME_CONTROL

	pTextTail->bNameFlag = TRUE;
#ifdef ENABLE_TEXTTAIL_BACKGROUND
	pTextTail->pTextInstance->SetOutline(false);
#else
	pTextTail->pTextInstance->SetOutline(true);
#endif
	pTextTail->pTextInstance->SetVerticalAlign(CGraphicTextInstance::VERTICAL_ALIGN_BOTTOM);
	m_ChatTailMap.insert(TTextTailMap::value_type(VirtualID, pTextTail));
}

void CPythonTextTail::RegisterInfoTail(DWORD VirtualID, const char* c_szChat)
{
	CInstanceBase* pCharacterInstance = CPythonCharacterManager::Instance().GetInstancePtr(VirtualID);

	if (!pCharacterInstance)
		return;

	TChatTailMap::iterator itor = m_ChatTailMap.find(VirtualID);

	if (m_ChatTailMap.end() != itor)
	{
		TTextTail* pTextTail = itor->second;

		pTextTail->pTextInstance->SetValue(c_szChat);
		pTextTail->pTextInstance->Update();
		pTextTail->Color = c_TextTail_Info_Color;
		pTextTail->pTextInstance->SetColor(c_TextTail_Info_Color);

		// TEXTTAIL_LIVINGTIME_CONTROL
		pTextTail->LivingTime = CTimer::Instance().GetCurrentMillisecond() + TextTail_GetLivingTime();
		// END_OF_TEXTTAIL_LIVINGTIME_CONTROL

		pTextTail->bNameFlag = FALSE;

		return;
	}

	TTextTail* pTextTail = RegisterTextTail(VirtualID,
		c_szChat,
		pCharacterInstance->GetGraphicThingInstancePtr(),
		pCharacterInstance->GetGraphicThingInstanceRef().GetHeight() + 10.0f,
		c_TextTail_Info_Color);

	// TEXTTAIL_LIVINGTIME_CONTROL
	pTextTail->LivingTime = CTimer::Instance().GetCurrentMillisecond() + TextTail_GetLivingTime();
	// END_OF_TEXTTAIL_LIVINGTIME_CONTROL

	pTextTail->bNameFlag = FALSE;
#ifdef ENABLE_TEXTTAIL_BACKGROUND
	pTextTail->pTextInstance->SetOutline(false);
#else
	pTextTail->pTextInstance->SetOutline(true);
#endif
	pTextTail->pTextInstance->SetVerticalAlign(CGraphicTextInstance::VERTICAL_ALIGN_BOTTOM);
	m_ChatTailMap.insert(TTextTailMap::value_type(VirtualID, pTextTail));
}

bool CPythonTextTail::GetTextTailPosition(DWORD dwVID, float* px, float* py, float* pz)
{
	TTextTailMap::iterator itorCharacter = m_CharacterTextTailMap.find(dwVID);

	if (m_CharacterTextTailMap.end() == itorCharacter)
	{
		return false;
	}

	TTextTail* pTextTail = itorCharacter->second;
	*px = pTextTail->x;
	*py = pTextTail->y;
	*pz = pTextTail->z;

	return true;
}

bool CPythonTextTail::IsChatTextTail(DWORD dwVID)
{
	TChatTailMap::iterator itorChat = m_ChatTailMap.find(dwVID);

	if (m_ChatTailMap.end() == itorChat)
		return false;

	return true;
}

void CPythonTextTail::SetCharacterTextTailColor(DWORD VirtualID, const D3DXCOLOR& c_rColor)
{
	TTextTailMap::iterator itorCharacter = m_CharacterTextTailMap.find(VirtualID);

	if (m_CharacterTextTailMap.end() == itorCharacter)
		return;

	TTextTail* pTextTail = itorCharacter->second;
	pTextTail->pTextInstance->SetColor(c_rColor);
	pTextTail->Color = c_rColor;
}

void CPythonTextTail::SetItemTextTailOwner(DWORD dwVID, const char* c_szName)
{
	TTextTailMap::iterator itor = m_ItemTextTailMap.find(dwVID);
	if (m_ItemTextTailMap.end() == itor)
		return;

	TTextTail* pTextTail = itor->second;

	if (strlen(c_szName) > 0)
	{
		if (!pTextTail->pOwnerTextInstance)
		{
			pTextTail->pOwnerTextInstance = CGraphicTextInstance::New();
		}

		std::string strName = c_szName;
		static const std::string& strOwnership = ApplicationStringTable_GetString(IDS_POSSESSIVE_MORPHENE) == "" ? "'s" : ApplicationStringTable_GetString(IDS_POSSESSIVE_MORPHENE);
		strName += strOwnership;

		pTextTail->pOwnerTextInstance->SetTextPointer(ms_pFont);
		pTextTail->pOwnerTextInstance->SetHorizonalAlign(CGraphicTextInstance::HORIZONTAL_ALIGN_CENTER);
		pTextTail->pOwnerTextInstance->SetValue(strName.c_str());
#ifdef ENABLE_EXTENDED_ITEMNAME_ON_GROUND
		CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetMainInstancePtr();
		if (pInstance)
		{
			if (!strcmp(pInstance->GetNameString(), c_szName))
				pTextTail->pOwnerTextInstance->SetColor(1.0f, 1.0f, 0.0f);
			else
				pTextTail->pOwnerTextInstance->SetColor(1.0f, 0.0f, 0.0f);
		}
#else
		pTextTail->pOwnerTextInstance->SetColor(1.0f, 1.0f, 0.0f);
#endif
		pTextTail->pOwnerTextInstance->Update();

		int xOwnerSize, yOwnerSize;
		pTextTail->pOwnerTextInstance->GetTextSize(&xOwnerSize, &yOwnerSize);
		pTextTail->yStart = -2.0f;
		pTextTail->yEnd += float(yOwnerSize + 4);
		pTextTail->xStart = fMIN(pTextTail->xStart, float(-xOwnerSize / 2 - 1));
		pTextTail->xEnd = fMAX(pTextTail->xEnd, float(xOwnerSize / 2 + 1));
#ifdef ENABLE_DROP_DESTROY_TIME
		if (pTextTail->pDestroyTimeTextInstance)
		{
			int xDestroyTimeSize, yDestroyTimeSize;
			pTextTail->pDestroyTimeTextInstance->GetTextSize(&xDestroyTimeSize, &yDestroyTimeSize);
			
			pTextTail->xStart = fMIN(pTextTail->xStart, static_cast<float>(-xDestroyTimeSize / 2 - 1));
			pTextTail->xEnd = fMAX(pTextTail->xEnd, static_cast<float>(xDestroyTimeSize / 2 + 1));
		}
#endif
	}
	else
	{
		if (pTextTail->pOwnerTextInstance)
		{
			CGraphicTextInstance::Delete(pTextTail->pOwnerTextInstance);
			pTextTail->pOwnerTextInstance = NULL;
		}

		int xSize, ySize;
		pTextTail->pTextInstance->GetTextSize(&xSize, &ySize);
		pTextTail->xStart = (float)(-xSize / 2 - 2);
		pTextTail->yStart = -2.0f;
		pTextTail->xEnd = (float)(xSize / 2 + 2);
		pTextTail->yEnd = (float)ySize;
#ifdef ENABLE_DROP_DESTROY_TIME
		if (pTextTail->pDestroyTimeTextInstance)
		{
			int xDestroyTimeSize, yDestroyTimeSize;
			pTextTail->pDestroyTimeTextInstance->GetTextSize(&xDestroyTimeSize, &yDestroyTimeSize);

			pTextTail->xStart = fMIN(pTextTail->xStart, static_cast<float>(-xDestroyTimeSize / 2 - 1));
			pTextTail->xEnd = fMAX(pTextTail->xEnd, static_cast<float>(xDestroyTimeSize / 2 + 1));
			pTextTail->yEnd += static_cast<float>(yDestroyTimeSize + 4);
		}
#endif
	}
}

#ifdef ENABLE_DROP_DESTROY_TIME
void CPythonTextTail::SetItemTextTailDestroyTime(DWORD dwVID, time_t tDestroyTime)
{
	if (tDestroyTime <= 0)
		return;
	
	TTextTailMap::const_iterator itor = m_ItemTextTailMap.find(dwVID);
	if (m_ItemTextTailMap.end() == itor)
		return;

	TTextTail* pTextTail = itor->second;

	bool bCreate = false;
	if (!pTextTail->pDestroyTimeTextInstance)
	{
		pTextTail->pDestroyTimeTextInstance = CGraphicTextInstance::New();
		bCreate = true;
	}
	
	const int iSeconds = static_cast<int>(difftime(tDestroyTime, time(nullptr)));
	std::string strName = "(" + std::to_string(iSeconds) + "s)";
	pTextTail->pDestroyTimeTextInstance->SetTextPointer(ms_pFont);
	pTextTail->pDestroyTimeTextInstance->SetHorizonalAlign(CGraphicTextInstance::HORIZONTAL_ALIGN_CENTER);
	pTextTail->pDestroyTimeTextInstance->SetValue(strName.c_str());
	pTextTail->pDestroyTimeTextInstance->SetColor(0.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
	pTextTail->pDestroyTimeTextInstance->Update();

	int xDestroyTimeSize, yDestroyTimeSize;
	pTextTail->pDestroyTimeTextInstance->GetTextSize(&xDestroyTimeSize, &yDestroyTimeSize);

	pTextTail->yStart = -2.0f;

	if (bCreate)
		pTextTail->yEnd += float(yDestroyTimeSize + 4);

	pTextTail->xStart = fMIN(pTextTail->xStart, static_cast<float>(-xDestroyTimeSize / 2 - 1));
	pTextTail->xEnd = fMAX(pTextTail->xEnd, static_cast<float>(xDestroyTimeSize / 2 + 1));

	if (pTextTail->pOwnerTextInstance)
	{
		int xOwnerSize, yOwnerSize;
		pTextTail->pOwnerTextInstance->GetTextSize(&xOwnerSize, &yOwnerSize);

		pTextTail->xStart = fMIN(pTextTail->xStart, static_cast<float>(-xOwnerSize / 2 - 1));
		pTextTail->xEnd = fMAX(pTextTail->xEnd, static_cast<float>(xOwnerSize / 2 + 1));
	}
}
#endif

void CPythonTextTail::DeleteCharacterTextTail(DWORD VirtualID)
{
	TTextTailMap::iterator itorCharacter = m_CharacterTextTailMap.find(VirtualID);
	TTextTailMap::iterator itorChat = m_ChatTailMap.find(VirtualID);

	if (m_CharacterTextTailMap.end() != itorCharacter)
	{
		DeleteTextTail(itorCharacter->second);
		m_CharacterTextTailMap.erase(itorCharacter);
	}
	else
	{
		Tracenf("CPythonTextTail::DeleteCharacterTextTail - Find VID[%d] Error", VirtualID);
	}

	if (m_ChatTailMap.end() != itorChat)
	{
		DeleteTextTail(itorChat->second);
		m_ChatTailMap.erase(itorChat);
	}
}

void CPythonTextTail::DeleteItemTextTail(DWORD VirtualID)
{
	TTextTailMap::iterator itor = m_ItemTextTailMap.find(VirtualID);

	if (m_ItemTextTailMap.end() == itor)
	{
		Tracef(" CPythonTextTail::DeleteItemTextTail - None Item Text Tail\n");
		return;
	}

	DeleteTextTail(itor->second);
	m_ItemTextTailMap.erase(itor);
}

CPythonTextTail::TTextTail* CPythonTextTail::RegisterTextTail(DWORD dwVirtualID, const char* c_szText, CGraphicObjectInstance* pOwner, float fHeight, const D3DXCOLOR& c_rColor)
{
	TTextTail* pTextTail = m_TextTailPool.Alloc();

	pTextTail->dwVirtualID = dwVirtualID;
	pTextTail->pOwner = pOwner;
	pTextTail->pTextInstance = CGraphicTextInstance::New();
	pTextTail->pOwnerTextInstance = NULL;
#ifdef ENABLE_DROP_DESTROY_TIME
	pTextTail->pDestroyTimeTextInstance = nullptr;
#endif
	pTextTail->fHeight = fHeight;

	pTextTail->pTextInstance->SetTextPointer(ms_pFont);
	pTextTail->pTextInstance->SetHorizonalAlign(CGraphicTextInstance::HORIZONTAL_ALIGN_CENTER);
	pTextTail->pTextInstance->SetValue(c_szText);
	pTextTail->pTextInstance->SetColor(c_rColor.r, c_rColor.g, c_rColor.b);
	pTextTail->pTextInstance->Update();

	int xSize, ySize;
	pTextTail->pTextInstance->GetTextSize(&xSize, &ySize);
	pTextTail->xStart = (float)(-xSize / 2 - 2);
	pTextTail->yStart = -2.0f;
	pTextTail->xEnd = (float)(xSize / 2 + 2);
	pTextTail->yEnd = (float)ySize;
	pTextTail->Color = c_rColor;
	pTextTail->fDistanceFromPlayer = 0.0f;
	pTextTail->x = -100.0f;
	pTextTail->y = -100.0f;
	pTextTail->z = 0.0f;
	pTextTail->pMarkInstance = NULL;
	pTextTail->pGuildNameTextInstance = NULL;
	pTextTail->pTitleTextInstance = NULL;
	pTextTail->pLevelTextInstance = NULL;
	return pTextTail;
}

void CPythonTextTail::DeleteTextTail(TTextTail* pTextTail)
{
	if (pTextTail->pTextInstance)
	{
		CGraphicTextInstance::Delete(pTextTail->pTextInstance);
		pTextTail->pTextInstance = NULL;
	}
	if (pTextTail->pOwnerTextInstance)
	{
		CGraphicTextInstance::Delete(pTextTail->pOwnerTextInstance);
		pTextTail->pOwnerTextInstance = NULL;
	}
#ifdef ENABLE_DROP_DESTROY_TIME
	if (pTextTail->pDestroyTimeTextInstance)
	{
		CGraphicTextInstance::Delete(pTextTail->pDestroyTimeTextInstance);
		pTextTail->pDestroyTimeTextInstance = nullptr;
	}
#endif
	if (pTextTail->pMarkInstance)
	{
		CGraphicMarkInstance::Delete(pTextTail->pMarkInstance);
		pTextTail->pMarkInstance = NULL;
	}
	if (pTextTail->pGuildNameTextInstance)
	{
		CGraphicTextInstance::Delete(pTextTail->pGuildNameTextInstance);
		pTextTail->pGuildNameTextInstance = NULL;
	}
	if (pTextTail->pTitleTextInstance)
	{
		CGraphicTextInstance::Delete(pTextTail->pTitleTextInstance);
		pTextTail->pTitleTextInstance = NULL;
	}
	if (pTextTail->pLevelTextInstance)
	{
		CGraphicTextInstance::Delete(pTextTail->pLevelTextInstance);
		pTextTail->pLevelTextInstance = NULL;
	}

	m_TextTailPool.Free(pTextTail);
}

int CPythonTextTail::Pick(int ixMouse, int iyMouse)
{
	for (TTextTailMap::iterator itor = m_ItemTextTailMap.begin(); itor != m_ItemTextTailMap.end(); ++itor)
	{
		TTextTail* pTextTail = itor->second;

		if (ixMouse >= pTextTail->x + pTextTail->xStart && ixMouse <= pTextTail->x + pTextTail->xEnd &&
			iyMouse >= pTextTail->y + pTextTail->yStart && iyMouse <= pTextTail->y + pTextTail->yEnd)
		{
			SelectItemName(itor->first);
			return (itor->first);
		}
	}

	return -1;
}

void CPythonTextTail::SelectItemName(DWORD dwVirtualID)
{
	TTextTailMap::iterator itor = m_ItemTextTailMap.find(dwVirtualID);

	if (m_ItemTextTailMap.end() == itor)
		return;

	TTextTail* pTextTail = itor->second;
	pTextTail->pTextInstance->SetColor(0.1f, 0.9f, 0.1f);
}

void CPythonTextTail::AttachTitle(DWORD dwVID, const char* c_szName, const D3DXCOLOR& c_rColor)
{
	if (!bPKTitleEnable)
		return;

	TTextTailMap::iterator itor = m_CharacterTextTailMap.find(dwVID);
	if (m_CharacterTextTailMap.end() == itor)
		return;

	TTextTail* pTextTail = itor->second;

	CGraphicTextInstance*& prTitle = pTextTail->pTitleTextInstance;
	if (!prTitle)
	{
		prTitle = CGraphicTextInstance::New();
		prTitle->SetTextPointer(ms_pFont);
#ifdef ENABLE_TEXTTAIL_BACKGROUND
		prTitle->SetOutline(false);
#else
		prTitle->SetOutline(true);
#endif
		prTitle->SetHorizonalAlign(CGraphicTextInstance::HORIZONTAL_ALIGN_RIGHT);
		prTitle->SetVerticalAlign(CGraphicTextInstance::VERTICAL_ALIGN_BOTTOM);
	}

	prTitle->SetValue(c_szName);
	prTitle->SetColor(c_rColor.r, c_rColor.g, c_rColor.b);
	prTitle->Update();
}

void CPythonTextTail::DetachTitle(DWORD dwVID)
{
	if (!bPKTitleEnable)
		return;

	TTextTailMap::iterator itor = m_CharacterTextTailMap.find(dwVID);
	if (m_CharacterTextTailMap.end() == itor)
		return;

	TTextTail* pTextTail = itor->second;

	if (pTextTail->pTitleTextInstance)
	{
		CGraphicTextInstance::Delete(pTextTail->pTitleTextInstance);
		pTextTail->pTitleTextInstance = NULL;
	}
}

void CPythonTextTail::EnablePKTitle(BOOL bFlag)
{
	bPKTitleEnable = bFlag;
}

void CPythonTextTail::AttachLevel(DWORD dwVID, const char* c_szText, const D3DXCOLOR& c_rColor)
{
	if (!bPKTitleEnable)
		return;

	TTextTailMap::iterator itor = m_CharacterTextTailMap.find(dwVID);
	if (m_CharacterTextTailMap.end() == itor)
		return;

	TTextTail* pTextTail = itor->second;

	CGraphicTextInstance*& prLevel = pTextTail->pLevelTextInstance;
	if (!prLevel)
	{
		prLevel = CGraphicTextInstance::New();
		prLevel->SetTextPointer(ms_pFont);
#ifdef ENABLE_TEXTTAIL_BACKGROUND
		prLevel->SetOutline(false);
#else
		prLevel->SetOutline(true);
#endif
#ifdef ENABLE_CONQUEROR_LEVEL_TEXTTAIL
		prLevel->SetOutLineColor(0.0f / 255.0f, 102.0f / 255.0f, 255.0f / 255.0f, 1.0f);
#endif
		prLevel->SetHorizonalAlign(CGraphicTextInstance::HORIZONTAL_ALIGN_RIGHT);
		prLevel->SetVerticalAlign(CGraphicTextInstance::VERTICAL_ALIGN_BOTTOM);
	}

	prLevel->SetValue(c_szText);
	prLevel->SetColor(c_rColor.r, c_rColor.g, c_rColor.b);
	prLevel->Update();
}

void CPythonTextTail::DetachLevel(DWORD dwVID)
{
	if (!bPKTitleEnable)
		return;

	TTextTailMap::iterator itor = m_CharacterTextTailMap.find(dwVID);
	if (m_CharacterTextTailMap.end() == itor)
		return;

	TTextTail* pTextTail = itor->second;

	if (pTextTail->pLevelTextInstance)
	{
		CGraphicTextInstance::Delete(pTextTail->pLevelTextInstance);
		pTextTail->pLevelTextInstance = NULL;
	}
}

void CPythonTextTail::Initialize()
{
	// DEFAULT_FONT
	//ms_pFont = (CGraphicText *)CResourceManager::Instance().GetTypeResourcePointer(g_strDefaultFontName.c_str());

	CGraphicText* pkDefaultFont = static_cast<CGraphicText*>(DefaultFont_GetResource());
	if (!pkDefaultFont)
	{
		TraceError("CPythonTextTail::Initialize - CANNOT_FIND_DEFAULT_FONT");
		return;
	}

	ms_pFont = pkDefaultFont;
	// END_OF_DEFAULT_FONT
}

void CPythonTextTail::Destroy()
{
	m_TextTailPool.Clear();
}

void CPythonTextTail::Clear()
{
	m_CharacterTextTailMap.clear();
	m_CharacterTextTailList.clear();
	m_ItemTextTailMap.clear();
	m_ItemTextTailList.clear();
	m_ChatTailMap.clear();

	m_TextTailPool.Clear();
}

CPythonTextTail::CPythonTextTail()
{
	Clear();
}

CPythonTextTail::~CPythonTextTail()
{
	Destroy();
}