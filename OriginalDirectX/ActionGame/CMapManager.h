/**
* @file CMapManager.h
* @brief マップ
* @author 織戸　喜隆
* @date 日付（2017.10.13）
*/
#pragma once

#include "stdafx.h"
#include "CMapChip.h"
#include "CPlayer.h"
#include "MediaResource.h"

/*! @struct MapChip
@brief  マップチップ
*/
typedef struct MapChip {
	eChipType type;	//!< チップタイプ
	std::shared_ptr<CMapChip> chip;	//!< オブジェクト
}MapChip;

class CMapManager
{
private:
	std::shared_ptr<CPlayer>	m_pPlayer		= NULL;	//!< プレーヤー
	std::shared_ptr<CMapChip>	m_pEnemy		= NULL;	//!< 敵
	std::shared_ptr<CMapChip>	m_pGoal			= NULL;	// ゴール

	int						m_DotMax			= 0;	//!< マップのドットの最大数
	int						m_DotCounter		= 0;	//!< 残りドット数	
	size_t					m_animIndexDot		= 0;	//!< アニメーションのインデックス
	size_t					m_animIndexPlayer	= 0;	//!< アニメーションのインデックス
	size_t					m_animIndexEnemy	= 0;	//!< アニメーションのインデックス
	
public:
	static std::unique_ptr<CMapManager> create(
		ID3D11Device*			pDevice_,
		ID3D11DeviceContext*	pDeviceContext_,
		D3D11_VIEWPORT          viewPort_[],
		LPCWSTR					shaderName_);
	void init(
		ID3D11Device*			pDevice_,
		ID3D11DeviceContext*	pDeviceContext_,
		D3D11_VIEWPORT          viewPort_[],
		LPCWSTR					shaderName_);

	CMapManager();
	~CMapManager();
	void update(int frameCnt_, int inputX_, int inputY_, eGravityDirection dir_, bool jump_);
	FLOAT isHitBlock(XMFLOAT4 pos_, XMFLOAT2 size_, eCheckAxis check_);
	BOOL encounterEachOther();
	BOOL isItMovable(std::shared_ptr<CMapChip> obj_, eDirection direction_);
	MapIndex getNextIndex(std::shared_ptr<CMapChip> obj_, eDirection direction_);
	void tryEatDot(MapIndex index_);
	void movePlayer(eDirection direction_);
	void moveEnemy(eDirection direction_);
	void render(XMFLOAT4X4 matView_, XMFLOAT4X4 matProj_, vector<shared_ptr<CTexture>> vTexture_);
	void release();
};