/**
 * @file CMapManager.cpp
 * @brief マップ
 * @author 織戸　喜隆
 * @date 日付（2017.02.14）
 */

#include "stdafx.h"
#include "CMapManager.h"

const eChipType kMap[kMapVertical][kMapHorizontal]
{
	{	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock },
	{	eBlock,	ePlayer,eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eBlock,	eBlock,	eEmpty,	eEmpty,	eBlock,	eEmpty,	eEmpty,	eBlock },
	{	eBlock,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eBlock,	eBlock,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eBlock },
	{	eBlock,	eEmpty,	eEmpty,	eEmpty,	eBlock,	eBlock,	eBlock,	eEmpty,	eEmpty,	eEmpty,	eBlock,	eBlock,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eBlock },
	{	eBlock,	eEmpty,	eEmpty,	eEmpty,	eBlock,	eEmpty,	eEmpty,	eBlock,	eEmpty,	eEmpty,	eBlock,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eBlock },
	{	eBlock,	eEmpty,	eEmpty,	eEmpty,	eBlock,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eBlock,	eEmpty,	eEmpty,	eBlock,	eEmpty,	eEmpty,	eBlock },
	{	eBlock,	eEmpty,	eEmpty,	eEmpty,	eBlock,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eBlock,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eBlock },
	{	eBlock,	eEmpty,	eEmpty,	eEmpty,	eBlock,	eEmpty,	eEmpty,	eBlock,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eBlock },
	{	eBlock,	eEmpty,	eEmpty,	eEmpty,	eBlock,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eBlock,	eEmpty,	eEmpty,	eEmpty,	eBlock },
	{	eBlock,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eBlock,	eEmpty,	eEmpty,	eBlock,	eBlock },
	{	eBlock,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eBlock,	eBlock,	eBlock,	eEmpty,	eEmpty,	eBlock,	eEmpty,	eEmpty,	eEmpty,	eBlock },
	{	eBlock,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eEmpty,	eBlock,	eBlock,	eBlock,	eEmpty,	eEmpty,	eEmpty,	eBlock,	eEmpty,	eGoal,	eBlock },
	{	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock,	eBlock },
};

static MapChip gMap[kMapVertical][kMapHorizontal];

// 1/5
static std::vector <QuadrangleTexel>  gTexel1_5 = {
	QuadrangleTexel{ { k1_5 * 0, 1.0f },{ k1_5 * 0, 0.0f },{ k1_5 * 1, 1.0f },{ k1_5 * 1, 0.0f } },
	QuadrangleTexel{ { k1_5 * 1, 1.0f },{ k1_5 * 1, 0.0f },{ k1_5 * 2, 1.0f },{ k1_5 * 2, 0.0f } },
	QuadrangleTexel{ { k1_5 * 2, 1.0f },{ k1_5 * 2, 0.0f },{ k1_5 * 3, 1.0f },{ k1_5 * 3, 0.0f } },
	QuadrangleTexel{ { k1_5 * 3, 1.0f },{ k1_5 * 3, 0.0f },{ k1_5 * 4, 1.0f },{ k1_5 * 4, 0.0f } },
	QuadrangleTexel{ { k1_5 * 4, 1.0f },{ k1_5 * 4, 0.0f },{ k1_5 * 5, 1.0f },{ k1_5 * 5, 0.0f }}
};
 //!< 1/4
static std::vector <QuadrangleTexel>  gTexel1_4 = {
	QuadrangleTexel{ { k1_4 * 0, 1.0f },{ k1_4 * 0, 0.0f }, { k1_4 * 1, 1.0f }, { k1_4 * 1, 0.0f } },
	QuadrangleTexel{ { k1_4 * 1, 1.0f },{ k1_4 * 1, 0.0f }, { k1_4 * 2, 1.0f }, { k1_4 * 2, 0.0f } },
	QuadrangleTexel{ { k1_4 * 2, 1.0f },{ k1_4 * 2, 0.0f }, { k1_4 * 3, 1.0f }, { k1_4 * 3, 0.0f } },
	QuadrangleTexel{ { k1_4 * 3, 1.0f },{ k1_4 * 3, 0.0f }, { k1_4 * 4, 1.0f }, { k1_4 * 4, 0.0f } }
};
 //!< 1/3
static std::vector <QuadrangleTexel>  gTexel1_3 = {
	QuadrangleTexel{ { k1_3 * 0, 1.0f },{ k1_3 * 0, 0.0f }, { k1_3 * 1, 1.0f }, { k1_3 * 1, 0.0f } },
	QuadrangleTexel{ { k1_3 * 1, 1.0f },{ k1_3 * 1, 0.0f }, { k1_3 * 2, 1.0f }, { k1_3 * 2, 0.0f } },
	QuadrangleTexel{ { k1_3 * 2, 1.0f },{ k1_3 * 2, 0.0f }, { k1_3 * 3, 1.0f }, { k1_3 * 3, 0.0f } }
};
 //!< 1/2
static std::vector <QuadrangleTexel>  gTexel1_2 = {
	QuadrangleTexel{ { k1_2 * 0, 1.0f }, { k1_2 * 0, 0.0f }, { k1_2 * 1, 1.0f }, { k1_2 * 1, 0.0f } },
	QuadrangleTexel{ { k1_2 * 1, 1.0f }, { k1_2 * 1, 0.0f }, { k1_2 * 2, 1.0f }, { k1_2 * 2, 0.0f } }
};

vector <QuadrangleTexel>  gTexel1_1 = {
	QuadrangleTexel{ { 0, 1.0f },{ 0, 0.0f },{ 1, 1.0f },{ 1, 0.0f } }
};


/**
* @fn std::unique_ptr<CMapManager> CMapManager::create(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_,)
* @brief インスタンスを生成し初期化 【クラスメソッド】
* @param pDevice_			デバイス
* @param pDeviceContext_	デバイスコンテキスト
* @param viewPort_[]		ビューポート
* @param shaderName_		シェーダープログラム
* @return					CMapChipのユニークポインタ
*/
std::unique_ptr<CMapManager> CMapManager::create(
	ID3D11Device*			pDevice_,
	ID3D11DeviceContext*	pDeviceContext_,
	D3D11_VIEWPORT          viewPort_[],
	LPCWSTR					shaderName_)
{
	std::unique_ptr<CMapManager> object = unique_ptr<CMapManager>(new CMapManager());
	object->init(pDevice_, pDeviceContext_, viewPort_, shaderName_);
	return object;
}

/**
* @fn void CMapManager::init(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_)
* @brief インスタンスを初期化
* @param pDevice_			デバイス
* @param pDeviceContext_	デバイスコンテキスト
* @param viewPort_[]		ビューポート
* @param shaderName_		シェーダープログラム
* @return					無し
*/
void CMapManager::init(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_)
{
	for (int y = 0; y < kMapVertical; y++)
	{
		for (int x = 0; x < kMapHorizontal; x++)
		{
			gMap[y][x].type = kMap[y][x];
			switch (kMap[y][x])
			{
			case eEmpty:
				break;
			case eBlock:
				gMap[y][x].chip	= CMapChip::create(pDevice_, pDeviceContext_, viewPort_, HLSL_PlaneHLSL, Tex_Block, kBlockSize, { x, y }, gTexel1_1);
				gMap[y][x].chip->setName("Block");
				break;
			case eDot:
				gMap[y][x].chip	= CMapChip::create(pDevice_, pDeviceContext_, viewPort_, HLSL_PlaneHLSL, Tex_Dot, kDotSize, { x, y }, gTexel1_3);
				m_DotMax++;
				m_DotCounter++;
				gMap[y][x].chip->setName("Empty");
				break;
			case ePlayer:
				m_pPlayer		= CPlayer::create(pDevice_, pDeviceContext_, viewPort_, HLSL_PlaneHLSL, Tex_Player, kPlayerSize, { x, y }, gTexel1_4);
				break;
			case eEnemy:
				m_pEnemy		= CMapChip::create(pDevice_, pDeviceContext_, viewPort_, HLSL_PlaneHLSL, Tex_Enemy, kEnemySize, { x, y }, gTexel1_3);
				break;
			case eGoal:
				m_pGoal = CMapChip::create(pDevice_, pDeviceContext_, viewPort_, HLSL_PlaneHLSL, Tex_Goal, kPlayerSize, { x, y }, gTexel1_1);
			default:
				break;
			}
		}
	}
}

CMapManager::CMapManager()
{
	for (int y = 0; y < kMapVertical; y++)
	{
		for (int x = 0; x < kMapHorizontal; x++)
		{
			gMap[y][x].type = eEmpty;
		}
	}
}

CMapManager::~CMapManager()
{
}

/**
* @fn void CMapManager::update(eDirection dir_, int frameCnt_)
* @brief シーンを更新
* @param eDirection	プレーヤーの移動方向
* @param frameCnt_	フレーム数
* @return	無し
*/
void CMapManager::update(int frameCnt_, int inputX_, int inputY_, eGravityDirection dir_, bool jump_)
{
	 //!< ゲームクリア
	//if (m_DotCounter <= 0)			throw eGameClear;
	//if (encounterEachOther())		throw eGameOver;

	if (frameCnt_ % 10 == 0)
	{
		//m_animIndexPlayer++;	m_animIndexPlayer	= m_animIndexPlayer	>= gTexel1_5.size() ? 0 : m_animIndexPlayer;
		m_animIndexDot++;		m_animIndexDot		= m_animIndexDot	>= gTexel1_3.size() ? 0 : m_animIndexDot;
		m_animIndexEnemy++;		m_animIndexEnemy	= m_animIndexEnemy	>= gTexel1_3.size() ? 0 : m_animIndexEnemy;
	}
	//
	//movePlayer(dir_);

	if (m_pPlayer->getGround() && m_pPlayer->getRattlingFrame() != 0)
	{
		m_pPlayer->shake();
	}
	else if (!m_pPlayer->getGround())
	{
		m_pPlayer->CountUpRattlingFrame();
	}
	if (!m_pPlayer->getGround() || m_pPlayer->getRattlingFrame() == 0)
	{
		if (dir_ != eGNone)	m_pPlayer->setGravityDirection(dir_);
		m_animIndexPlayer = m_pPlayer->getAnimIndex();
		m_pPlayer->gravityProc();
		m_pPlayer->moveHorizone(inputX_);
		m_pPlayer->moveVertical(inputY_);
		if (jump_)	m_pPlayer->jump();
		m_pPlayer->offsetPos(m_pPlayer->getSpeed());
		m_pPlayer->recomposeColPos();
		XMFLOAT4 checkPos = {};
		checkPos.y -= isHitBlock(m_pPlayer->getPartCol().head.pos, m_pPlayer->getPartCol().head.size, eY);
		checkPos.y += isHitBlock(m_pPlayer->getPartCol().foot.pos, m_pPlayer->getPartCol().foot.size, eY);
		checkPos.x -= isHitBlock(m_pPlayer->getPartCol().right.pos, m_pPlayer->getPartCol().right.size, eX);
		checkPos.x += isHitBlock(m_pPlayer->getPartCol().left.pos, m_pPlayer->getPartCol().left.size, eX);
		m_pPlayer->checkAndOffset(checkPos);
		if (m_pPlayer->collisionRect(m_pGoal->getPos(), {16.0f, 16.0f} ))
		{
			throw eGameClear;
		}
	}
	if (frameCnt_ % 10 == 0)
	{
		eDirection dir[] = { eLeft, eRight, eUp, eDown };
		moveEnemy(dir[rand() % 4]);	 //!< 敵の移動
	}
}

// ブロックに当たったかどうか
// 戻り値:どれくらい当たっているか
FLOAT CMapManager::isHitBlock(XMFLOAT4 pos_, XMFLOAT2 size_, eCheckAxis check_)
{
	BOOL isHit = FALSE;
	for (int y = 0; y < kMapVertical; ++y)
	{
		for (int x = 0; x < kMapHorizontal; ++x)
		{
			if (gMap[y][x].type != eBlock) { continue; }
			//else if (gMap[y][x].chip->outOfWindow()) { continue; }
			if (gMap[y][x].chip->collisionRect(pos_, size_))
			{
				return gMap[y][x].chip->colDifference(pos_, size_, check_);
			}
		}
	}
	return 0.0f;
}

/**
* @fn BOOL CMapManager::encounterEachOther()
* @brief プレーヤー杼木との遭遇
* @param	無し
* @return	遭遇した
*/
BOOL CMapManager::encounterEachOther()
{
	if (m_pEnemy == NULL) { return FALSE; }
	return m_pPlayer->getIndex() == m_pEnemy->getIndex();
}

/**
* @fn MapIndex CMapManager::getNextIndex(std::shared_ptr<CMapChip> obj_, eDirection direction_)
* @brief マップ上の位置を求める
* @param obj_		プレーヤー、敵
* @param direction_	移動方向
* @return			マップ上の位置
*/
MapIndex CMapManager::getNextIndex(std::shared_ptr<CMapChip> obj_, eDirection direction_)
{
	if (obj_ == NULL) { return { 0,0 }; }
	MapIndex next = obj_->getIndex();
	switch (direction_)
	{
	case eLeft:		next.x--;	next.x = max(0, next.x);					break;
	case eRight:	next.x++;	next.x = min(kMapHorizontal - 1, next.x);	break;
	case eUp:		next.y--;	next.y = max(0, next.y);					break;
	case eDown:		next.y++;	next.y = min(kMapVertical - 1, next.y);		break;
	case eNone:																break;
	default:																break;
	}
	return next;
}

/**
* @fn BOOL CMapManager::isItMovable(std::shared_ptr<CMapChip> obj_, eDirection direction_)
* @brief 指定された位置に移動可能か
* @param obj_		プレーヤー、敵
* @param direction_	移動方向
* @return	移動可能か
*/
BOOL CMapManager::isItMovable(std::shared_ptr<CMapChip> obj_, eDirection direction_)
{
	MapIndex temp = getNextIndex(obj_, direction_);
	return gMap[temp.y][temp.x].type != eBlock;
}

/**
* @fn void CMapManager::tryEatDot(MapIndex index_)
* @brief ドットを食べてみる
* @param index_	マップ上の位置
* @return	無し
*/
void CMapManager::tryEatDot(MapIndex index_)
{
	if (gMap[index_.y][index_.x].type == eDot)
	{
		gMap[index_.y][index_.x].type = eEmpty;	 //!< 移動先がeDotなら食べる、そして空にする
		m_DotCounter--;
	}
}

/**
* @fn void CMapManager::movePlayer(eDirection direction_)
* @brief プレーヤーの移動
* @param direction_	移動方向
* @return	無し
*/
void CMapManager::movePlayer(eDirection direction_)
{
	MapIndex current = m_pPlayer->getIndex();
	MapIndex next = getNextIndex(m_pPlayer, direction_);

	if (isItMovable(m_pPlayer, direction_))
	{
		tryEatDot(next);			 //!< 移動先でトライする
		m_pPlayer->setIndex(next);	 //!< インデックス(y, x)を更新
	}
}

/**
* @fn void CMapManager::moveEnemy(eDirection direction_)
* @brief 敵の移動
* @param direction_	移動方向
* @return	無し
*/
void CMapManager::moveEnemy(eDirection direction_)
{
	if (m_pEnemy == NULL) { return; }
	MapIndex current = m_pEnemy->getIndex();
	MapIndex next = getNextIndex(m_pEnemy, direction_);
	if (isItMovable(m_pEnemy, direction_))
	{
		m_pEnemy->setIndex(next);	 //!< インデックス(y, x)を更新
	}
}

/**
 * @fn void CMapManager::render()
 * @brief シーンを画面にレンダリング
 * @param matView_	ビュー行列
 * @param matProj_	プロジェクション行列
 * @return	無し
 */
void CMapManager::render(XMFLOAT4X4 matView_, XMFLOAT4X4 matProj_, vector<shared_ptr<CTexture>> vTexture_)
{
	for (int y = 0; y < kMapVertical; y++)
	{
		for (int x = 0; x < kMapHorizontal; x++)
		{
			switch (gMap[y][x].type)
			{					
			case eBlock:
				gMap[y][x].chip->render(matView_, matProj_, vTexture_[eBlockTexture], 0);
				break;
			case eDot:
				if (m_pEnemy != NULL && gMap[y][x].chip->getIndex() == m_pEnemy->getIndex())
				{
					gMap[y][x].chip->render(matView_, matProj_, vTexture_[eEnemyTexture], m_animIndexDot);
				}
				else
				{
					gMap[y][x].chip->render(matView_, matProj_, vTexture_[eDotTexture], m_animIndexDot);
				}
				break;
			default:
				break;
			}
		}
	}
	//m_pPlayer->debugRectRender(matView_, matProj_, vTexture_[eDarkTexture]);
	m_pPlayer->render(matView_, matProj_, vTexture_[ePlayerTexture], m_animIndexPlayer);
	m_pGoal->render(matView_, matProj_, vTexture_[eGoalTexture]);

	if (m_pEnemy != NULL)
	{
		m_pEnemy->render(matView_, matProj_, vTexture_[eEnemyTexture], m_animIndexEnemy);
	}
}

/**
 * @fn void CMapManager::release()
 * @brief DirectXのリソースを解放
 * @param	無し
 * @return	無し
 */
void CMapManager::release()
{
	m_pPlayer->release();
	if (m_pEnemy != NULL)
	{
		m_pEnemy->release();
	}
	for (int y = 0; y < kMapVertical; y++)
	{
		for (int x = 0; x < kMapHorizontal; x++)
		{
			if (gMap[y][x].chip)
			{
				gMap[y][x].chip->release();
			}
		}
	}
}