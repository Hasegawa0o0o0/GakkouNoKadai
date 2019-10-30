/**
* @file CShootingGameApp.h
* @brief スペースシップ（抽象クラス）
* @author 織戸　喜隆
* @date 日付（2017.02.14）
*/
#pragma once

#include "stdafx.h"
#include "CApplication.h"
#include "CScroll.h"
#include "CPlayer.h"
#include "CEnemy.h"

class CShootingGameApp :
	public CApplication
{
	typedef CApplication super;
private:
	//*****************************************************************
	//【Method】 <Private>
	//*****************************************************************
	std::unique_ptr<CPicture>	m_pTitle			= NULL;
	std::unique_ptr<CPicture>	m_pGameOver	= NULL;
	std::unique_ptr<CScroll>	m_pBack		= NULL;
	std::unique_ptr<CScroll>	m_pMiddle		= NULL;
	std::unique_ptr<CScroll>	m_pFront		= NULL;
	std::unique_ptr<CPlayer>	m_pPlayer		= NULL;	//!< 【ステップ6】CSpaceship
	std::shared_ptr<CEnemy>	m_pEnemys[5]	= { };	//!< 全ての要素をゼロにする書き方

	std::unique_ptr<CTexture>		m_pPlayerTex	= NULL;	//!< テクスチャ
	std::shared_ptr<CTexture>	m_pEnemyTex	= NULL;	//!< テクスチャ
	int m_activeEnemys										= 0;

	bool createSprites(void);
	bool createSpaceship(void);
	void initCameraInfo();
	void prepareFrame (shared_ptr<CEnemy> pEnemy_, int i);
	void doOpenning();
	void doPlaying();
	void doGameClear();
	void doGameOver();
	void doEnding();
	BOOL isActiveEnemys();
public:
	CShootingGameApp();
	~CShootingGameApp();
	void enumEnemys(std::function<BOOL(std::shared_ptr<CEnemy>, size_t)> callbackEnumeProc_);
	void render(XMFLOAT4X4 matView, XMFLOAT4X4 matProj);
	void release();
};