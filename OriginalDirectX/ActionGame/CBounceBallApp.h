#pragma once

#include "stdafx.h"
#include "CApplication.h"
#include "CMapManager.h"
#include "CText.h"

class CBounceBallApp : public CApplication {
	typedef CApplication super;

	std::unique_ptr<CPicture> m_pOpening = NULL;
	std::unique_ptr<CPicture> m_pClear = NULL;
	std::unique_ptr<CPicture> m_pGameOver = NULL;
	std::unique_ptr<CPicture> m_pBackground = NULL;

	std::unique_ptr<CMapManager>	m_pMapManager = NULL;
	std::vector<shared_ptr<CText>> m_pvText;
	std::vector<shared_ptr<CTexture>> m_pvTexture;

	BOOL m_isGameStart = FALSE;		// 入力を受けてゲームをスタートさせるか
	int m_startCnt = 3;				// スタートさせるまでのカウントダウン

	bool createSprites(void);
	void initCameraInfo();
	void createTexture();
	void createMap();
	void doOpenning();
	void doPlaying();
	void doGameClear();
	void doGameOver();
	void doEnding();
	void render(XMFLOAT4X4	matView_, XMFLOAT4X4 matProj_);
public:
	CBounceBallApp();
	~CBounceBallApp();
};
