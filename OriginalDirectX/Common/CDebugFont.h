/**
* @file CDebugFont.h
* @brief フォント
* @author 織戸　喜隆
* @date 日付（2017.12.18）
*/
#pragma once
#include "CQuadPolygon.h"	//"CSprite.h"
#include "DotEat.h"

class CDebugFont// :
	//public CSprite
{
	//typedef CSprite super;
private:
	std::vector<std::unique_ptr<CQuadPolygon>> m_text;
public:
	CDebugFont();
	~CDebugFont();
	QuadrangleTexel createSprites(char s);
	void createSprites(char s[]);
};

