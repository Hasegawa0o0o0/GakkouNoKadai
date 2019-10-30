/**
 * @file CShootingGameApp.cpp
 * @brief アプリケーションのエントリ ポイントを定義します。
 * @author 織戸　喜隆
 * @date 日付（2017.02.14）
 */

#include "stdafx.h"
#include "CShootingGameApp.h"


CShootingGameApp::CShootingGameApp()
{
}


CShootingGameApp::~CShootingGameApp()
{
}

/**
 * @fn void CShootingGameApp::initCameraInfo()
 * @brief カメラの設定
 * @param	無し
 * @return	無し
 */
void CShootingGameApp::initCameraInfo()
{
	m_vEyePt	= XMFLOAT3{ 0.0f, 0.0f,  -582.0f };	//!< カメラ（視点）位置
	m_vLookatPt	= XMFLOAT3{ 0.0f, 0.0f,     0.0f };	//!< 注視位置
	m_vUpVec	= XMFLOAT3{ 0.0f, 1.0f,     0.0f };	//!< 上方位置
	m_angle		= (FLOAT)XM_PI / 4;					//!< 視野角
	m_aspect	= ASPECT;							//!< アスペクト比
	m_near		= 0.1f;								//!< 前方クリップ
	m_far		= 1000.0f;							//!< 後方クリップ
}

/**
* @fn bool CShootingGameApp::createSprites(void)
* @brief スプライトの生成
* @param	無し
* @return	成功・失敗
*/
bool CShootingGameApp::createSprites(void)
{
	//eScrollMode scrollMode = eHorizontal;	//!< スクロールする方向
	eScrollMode scrollMode = eVirtical;		//!< スクロールする方向
	super::createSprites(); //!< スーパークラスのメソッドを実行
	try {

		/**
		* オープニングのタイトルを生成
		*/
		vector <QuadrangleTexel>  singleTexels = {
			QuadrangleTexel{ { 0, 1.0f },{ 0, 0.0f },{ 1, 1.0f },{ 1, 0.0f } }	//!< テクセルの配列が一つの配列
		};
		m_pTitle = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, Tex_Title,
			{ 0.0f, 0.0f, 0.0f, 0.0f },	//!< スプライトの位置
			kSpriteScreenkSize,			//!< スプライトのサイズ
			singleTexels);				//!< テクセルの配列
		m_pGameOver = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, Tex_GameOver,
			{ 0.0f, 0.0f, 0.0f, 0.0f },	//!< スプライトの位置
			kSpriteScreenkSize,			//!< スプライトのサイズ
			singleTexels);				//!< テクセルの配列


		/**
		* 背景のタイトルを生成
		*/
		m_pBack = CScroll::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_Background, Tex_Back,
			{ 0.0f, 0.0f, 0.0f, 0.0f },		//!< スプライトの位置
			kSpriteScreenkSize,				//!< スプライトのサイズ
			singleTexels,					//!< テクセルの配列
			scrollMode);
		m_pMiddle = CScroll::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_Background, Tex_Middle,
			{ 0.0f, 0.0f, 0.0f, 0.0f },		//!< スプライトの位置
			kSpriteScreenkSize,				//!< スプライトのサイズ
			singleTexels,					//!< テクセルの配列
			scrollMode);
		m_pFront = CScroll::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_Background, Tex_Front,
			{ 0.0f, 0.0f, 0.0f, 0.0f },		//!< スプライトの位置
			kSpriteScreenkSize,				//!< スプライトのサイズ
			singleTexels,					//!< テクセルの配列
			scrollMode);

		createSpaceship();	// プレーヤと敵の生成
		return true;
	}
	catch (LPCWSTR str)
	{
		MessageBox(0, str, NULL, MB_OK);
		return false;
	}
}

/**
 * @fn bool CShootingGameApp::createSpaceship(void)
 * @brief スプライトの生成
 * @param	無し
 * @return	成功・失敗
 */
bool CShootingGameApp::createSpaceship(void)
{
	enumEnemys(
		[&](std::shared_ptr<CEnemy> enemy_, size_t index_) -> BOOL {
		SAFE_release(enemy_);
		return TRUE; //!< 列挙を続行											
	});
	SAFE_release(m_pPlayer);

	m_pPlayer = CPlayer::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, /*Tex_Spaceship,*/
		{ 0.0f, -180.0f, 0.0f, 0.0f },	//!< スプライトの位置
		kCharSpriteSize,				//!< スプライトのサイズ
		kTexelPlayerBulletIndex);			//!< テクセルの配列のインデックス（アニメーションのため）
	m_pPlayer->setState(eMoving);
	m_pPlayer->setScale(1.0f);

	m_activeEnemys = 0;
	XMFLOAT4 enemyPos = { -130.0f , 301.0f , 0.0f, 0.0f };		//!<  【ステップ4】敵
	for (size_t i = 0; i < _countof(m_pEnemys); i++)
	{
		m_pEnemys[i] = CEnemy::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, /*Tex_Spaceship,*/
			enemyPos,				//!< 位置
			kCharSpriteSize,		//!< サイズ
			kTexelEnemyBulletIndex);	//!< バレットテクセルの配列インデックス
		m_pEnemys[i]->setState(eMoving);
		enemyPos.x += m_pEnemys[i]->getSize().x + 45.0f;
	}
	m_pPlayerTex = CTexture::create(m_pDevice, Tex_Spaceship);	//!< テクセル
	m_pEnemyTex = CTexture::create(m_pDevice, Tex_Spaceship);	//!< テクセル

	return true;
}

/**
 * @fn void CShootingGameApp::doOpenning()
 * @brief オープニング
 * @param	無し
 * @return	無し
 */
void CShootingGameApp::doOpenning ()
{
	m_gameStatus = ePlaying;

	m_pTitle->setAngle({ 0.0000f, 0.0000f, 0.0000f });
	if (m_pDirectInput->isKeyPressed(DIK_X)) {
		m_gameStatus = ePlaying;
	}
}

/**
 * @fn void CShootingGameApp::doPlaying()
 * @brief プレー中
 * @param	無し
 * @return	無し
 */
void CShootingGameApp::doPlaying ()
{
	// 【ステップ4】
	/**
	* キャラクターのアニメーション
	*/
	m_frameCounter++;
	m_frameCounter = m_frameCounter > 65536 ? 0 : m_frameCounter;

	m_pPlayer->stepAnimation(m_frameCounter % kTexelIntervalFrames == 0);
	enumEnemys(
		[&](std::shared_ptr<CEnemy> enemy_, size_t index_) -> BOOL {
		enemy_->stepAnimation(m_frameCounter % kTexelIntervalFrames == 0);
		return TRUE; //!< 列挙を続行
	});
	m_pBack->scroll(-0.2f);
	m_pMiddle->scroll(-0.4f);
	m_pFront->scroll(-0.8f);
	/**
	* プレーヤーの制御
	*/
	if (m_pDirectInput->isKeyPressed(DIK_LEFT))		m_pPlayer->offsetPosInNormalRange({ -kPlayerQuantity,  0.00f, 0.0f, 0.0f });
	if (m_pDirectInput->isKeyPressed(DIK_RIGHT))	m_pPlayer->offsetPosInNormalRange({  kPlayerQuantity,  0.00f, 0.0f, 0.0f });
	if (m_pDirectInput->isKeyPressed(DIK_UP))		m_pPlayer->offsetPosInNormalRange({ 0.0f,  kPlayerQuantity, 0.0f, 0.0f });
	if (m_pDirectInput->isKeyPressed(DIK_DOWN))		m_pPlayer->offsetPosInNormalRange({ 0.0f, -kPlayerQuantity, 0.0f, 0.0f });

	if (m_pDirectInput->isKeyPressed(DIK_V))		m_pPlayer->addAngleZ( 5.0);
	if (m_pDirectInput->isKeyPressed(DIK_N))		m_pPlayer->addAngleZ(-5.0);
	if (m_pDirectInput->isPressedOnce(DIK_B))		m_pPlayer->shot();

	if (m_pDirectInput->isLeftButtonClicked())		m_pPlayer->shot();

	/**
	* 【ステップ5】敵の操作
	*/
	m_activeEnemys = 0;
	enumEnemys(
		[&](std::shared_ptr<CEnemy> enemy_, size_t index_) -> BOOL {
		prepareFrame(enemy_, index_);
		return TRUE; //!< 列挙を続行
	});
	if (m_activeEnemys <= 0)
	{
		m_gameStatus = eGameClear;
	}
}

/**
* @fn void CShootingGameApp::prepareFrame (shared_ptr<CEnemy> pEnemy_, int i)
* @brief 敵キャラクタのの操作
* @param	pEnemy_		敵
* @param	i			順序
* @return				無し
*/
void CShootingGameApp::prepareFrame(shared_ptr<CEnemy> pEnemy_, int i)
{
	/**
	* 無条件に敵の弾とプレーヤとの衝突をチェックする.
	*/
	if (pEnemy_->collisionBullet(m_pPlayer->getPos(), (kCharSpriteSize.x / 2 + kBulletSize.x / 2)))
	{
		if (m_pPlayer->receiveDamage())
		{
			m_pPlayer->explode();
			m_pPlayer->setActive(FALSE);
			m_pPlayer->setDamaged(TRUE);
		}
	}

	/**
	* 無条件に弾と弾の衝突
	*/
	pEnemy_->enumBullets(
		[=](std::shared_ptr<CQuadPolygon> enemyBullet_, size_t index_) -> BOOL {
			if (enemyBullet_->getState() == eMoving)
			{
				if (m_pPlayer->collisionBullet(enemyBullet_->getPos(), (kCharSpriteSize.x / 2 + kBulletSize.x / 2)))
				{
					enemyBullet_->reset(pEnemy_->computeBulletPos(index_));
				}
			}
		return TRUE; //!< 列挙を続行												
		}
	);

	/**
	* 敵が生存している
	*/
	if (pEnemy_->getActive())
	{
		pEnemy_->offsetPos(ComputePosByDirectionZ({ 0.0f, -kEnemyQuantity, 0.0f, 0.0f }, 0.0f));
		pEnemy_->shotOpt(m_frameCounter % kIntervalShot == 0);
		if (pEnemy_->getPos().y < WINDOW_BOTTOM)
		{
			pEnemy_->setPos({pEnemy_->getPos().x, 301.0f, 0.0f, 0.0});
		}

		/**
		* プレーヤーに敵との衝突をチェックさせる.
		*/
		if (m_pPlayer->getActive())
		{
			if (m_pPlayer->collisionBullet(pEnemy_->getPos(), (kCharSpriteSize.x / 2 + kBulletSize.x / 2)))
			{
				if (pEnemy_->receiveDamage())
				{
					pEnemy_->explode();
					pEnemy_->setActive(FALSE);
					pEnemy_->setDamaged(TRUE);
				}
			}
		}

		/**
		* 敵にプレーヤーとの衝突をチェックさせる.
		*/
		if (pEnemy_->collision(m_pPlayer->getPos(), (kCharSpriteSize.x / 2 + kBulletSize.x / 2)))
		{
			if (m_pPlayer->receiveDamage())
			{
				m_pPlayer->explode();
				m_pPlayer->setActive(FALSE);
				m_pPlayer->setDamaged(TRUE);
			}
		}
	}
	
	/**
	* プレーヤーの爆発が完了したらゲームオーバー.
	*/
	if (m_pPlayer->isExplosionCompleted())
	{
		m_gameStatus = eGameOver;
	}
	if (pEnemy_->getActive())
	{
		m_activeEnemys++;
	}
}

/**
* @fn void CShootingGameApp::isActiveEnemys()
* @brief すべての敵が生存しているか
* @param	無し
* @return	無し
*/
BOOL CShootingGameApp::isActiveEnemys()
{
	return FALSE;
}

/**
 * @fn void CShootingGameApp::doGameClear()
 * @brief ゲームクリア
 * @param	無し
 * @return	無し
 */
void CShootingGameApp::doGameClear()
{
	MessageBox(0, L"GameClear", NULL, MB_OK);
	createSpaceship();
	m_gameStatus = eOpenning;
}

/**
 * @fn void CShootingGameApp::doGameOver()
 * @brief ゲームオーバー
 * @param	無し
 * @return	無し
 */
void CShootingGameApp::doGameOver()
{
	//デバッグ
	createSpaceship();
	m_gameStatus = eOpenning;
}

/**
 * @fn void CShootingGameApp::doEnding()
 * @brief エンディング
 * @param	無し
 * @return	無し
 */
void CShootingGameApp::doEnding()
{
}

/**
 * @fn void CShootingGameApp::renderSprites(XMFLOAT4X4 matView_, XMFLOAT4X4 matProj_)
 * @brief レンダー
 * @param	matView_	ビュー行列
 * @param	matProj_	プロジェクション行列
 * @return				無し
 */
void CShootingGameApp::render(XMFLOAT4X4 matView_, XMFLOAT4X4 matProj_)
{
	switch (m_gameStatus)
	{
	case eOpenning:
		m_pTitle->render(matView_, matProj_);
		break;
	case ePlaying:
		// 引数描画する順番と結果を確認すること
		enumEnemys(
			[&](std::shared_ptr<CEnemy> enemy_, size_t index_) -> BOOL {
			enemy_->render(matView_, matProj_,
				[&](ID3D11DeviceContext* pDeviceContext_) -> void {
				m_pEnemyTex->setup(pDeviceContext_);
			});
			return TRUE; //!< 列挙を続行	
		});
		m_pPlayer->render(matView_, matProj_,
			[&](ID3D11DeviceContext* pDeviceContext_) -> void {
			m_pPlayerTex->setup(pDeviceContext_);
		});

		m_pFront->render(matView_, matProj_);
		m_pMiddle->render(matView_, matProj_);
		m_pBack->render(matView_, matProj_);
		break;
	case eGameOver:
		m_pGameOver->render(matView_, matProj_);
		break;
	default:
		break;
	}
}

/**
* @fn void CShootingGameApp::enumEnemys(std::function<BOOL(std::shared_ptr<CEnemy>, size_t)> callbackEnumeProc_)
* @brief 敵を列挙する
* @param	callbackEnumeProc_	コールバック<ラムダ式>
* @return	無し
*/
void CShootingGameApp::enumEnemys(std::function<BOOL(std::shared_ptr<CEnemy>, size_t)> callbackEnumeProc_)
{
	for (size_t i = 0; i < _countof(m_pEnemys); i++)
	{
		if (m_pEnemys[i] != NULL)
		{
			if (callbackEnumeProc_(m_pEnemys[i], i) == FALSE)
			{
				break; //!< 列挙を中断
			}
		}
	}
}

/**
* @fn void CShootingGameApp::release()
* @brief 解放
* @param	無し
* @return	無し
*/
void CShootingGameApp::release()
{
	super::release();
	m_pTitle->release();
	m_pGameOver->release();
	m_pBack->release();
	m_pMiddle->release();
	m_pFront->release();
	m_pPlayer->release();
	enumEnemys(
		[&](std::shared_ptr<CEnemy> enemy_, size_t index_) -> BOOL {
		enemy_->release();
		return TRUE; //!< 列挙を続行	
	});
	m_pPlayerTex->release();
	m_pEnemyTex->release();	
}