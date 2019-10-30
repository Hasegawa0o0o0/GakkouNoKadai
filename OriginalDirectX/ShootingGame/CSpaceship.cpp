/**
* @file CSpaceship.cpp
* @brief スペースシップ（抽象クラス）
* @author 織戸　喜隆
* @date 日付（2017.02.14）
*/

#include "stdafx.h"
#include "CSpaceship.h"	

CSpaceship::CSpaceship()
{
}


CSpaceship::~CSpaceship()
{
}

/**
* @fn void CSpaceship::init(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, XMFLOAT4 position_, XMFLOAT2 size_, std::vector<QuadrangleTexel> texels)
* @brief インスタンスを初期化
* @param pDevice_			デバイス
* @param pDeviceContext_	デバイスコンテキスト
* @param viewPort_[]		ビューポート
* @param shaderName_		シェーダープログラム
* @param position_			ウィンドウの位置
* @param size_				スプライトの大きさ
* @param texels			テクセルの配列
* @param bulletTexelIndex_		弾丸のテクセルのインデックス（敵と共有）
* @return					無し
*/
void CSpaceship::init(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, XMFLOAT4 position_, XMFLOAT2 size_, std::vector<QuadrangleTexel> texels, int bulletTexelIndex_)
{
	m_hp = kMaxHP;
	super::init(pDevice_, pDeviceContext_, viewPort_, shaderName_, position_, size_, texels);	//!< スーパークラスのメソッドを実行
	m_bulletTexelIndex = bulletTexelIndex_;	//!<  Player or Enemy
	
	m_pExplosion = CExplosion::create(pDevice_, pDeviceContext_, viewPort_, HLSL_PlaneHLSL,
		{ 0.0f, 0.0f, 0.0f, 0.0f },	//!< 位置
		kExplosionSize,				//!< サイズ
		kTexelExplosions,
		kExplosionTexelPauseFrames);	//!< テクスチャの切り替えるタイミング（フレーム数）

	for (size_t i = 0; i < _countof(m_pBullets); i++)
	{
		m_pBullets[i] = shared_ptr<CQuadPolygon>(new CQuadPolygon());
	}
	enumBullets (
		[=](std::shared_ptr<CQuadPolygon> bullet_, size_t index_) -> BOOL {
		bullet_->init (
			pDevice_, pDeviceContext_, viewPort_, shaderName_,
			computeBulletPos(index_),
			kBulletSize,
			kTexelBullets
		);
		bullet_->setState (eWaiting);		
		bullet_->setScale (1.0f);
		bullet_->setMoveQuantity(2.5);
		return TRUE; //!< 列挙を続行
	});

	// テクセル
	m_pBulletTex = CTexture::create(pDevice_, Tex_Bullet);	//!< テクセル
	m_pExplosionTex = CTexture::create(pDevice_, Tex_Explosion);	//!< テクセル
}

/**
* @fn void CSpaceship::render(XMFLOAT4X4	matView_, XMFLOAT4X4 matProj_, size_t texelIndex_, XMFLOAT4 diffuse_)
* @brief レンダリング
* @param matView_	ビュー行列
* @param matProj_	プロジェクション行列
* @param texelIndex_	テクセル・インデックス
* @param v4Diffuse_	ディフューズ
* @return			無し
*/
void CSpaceship::render(XMFLOAT4X4 matView_, XMFLOAT4X4 matProj_, std::function<void(ID3D11DeviceContext* pDeviceContext_)> callbackTextureProc_, size_t texelIndex_, XMFLOAT4 v4Diffuse_)
{
	enumBullets (	
		[=](std::shared_ptr<CQuadPolygon> bullet_, size_t index_) -> BOOL {
		bullet_->outOfWindow() ? bullet_->reset(computeBulletPos(index_)) : NULL;
		if (bullet_->getState() == eMoving)
		{
			FLOAT dir = bullet_->getForwardDirection();
			bullet_->offsetPos (
				ComputePosByDirectionZ (
					{
						0.0f,						// x
						bullet_->getMoveQuantity(),	// y
						0.0f,						// z
						0.0f						// w
					},
					dir
				));
			bullet_->render (matView_, matProj_,
				[&](ID3D11DeviceContext* pDeviceContext_) -> void {
				m_pBulletTex->setup(pDeviceContext_);
			}, m_bulletTexelIndex);
		}
		else {
			bullet_->reset(computeBulletPos(index_));
		}
		return TRUE;	//!< 列挙を続行
	});

	if (m_hp < kMaxHP)
	{
		super::render(matView_, matProj_, callbackTextureProc_, texelIndex_, {1, 0, 0, 0});	//!< スーパークラスのメソッドを実行
	}
	else {
		super::render(matView_, matProj_, callbackTextureProc_, texelIndex_);				//!< スーパークラスのメソッドを実行
	}
	m_pExplosion->setPos(getPos());
	m_pExplosion->render(matView_, matProj_,
		[&](ID3D11DeviceContext* pDeviceContext_) -> void {
		m_pExplosionTex->setup(pDeviceContext_);
	});
}

/**
* @fn void CSpaceship::shot(std::vector<FLOAT> courses_)
* @brief 弾丸のショット
* @param	courses_	発射方向
* @return					無し
*/
void CSpaceship::shot(std::vector<FLOAT> courses_)
{
	if (isInvalid() || countEmptyBullets() < courses_.size())
	{
		return;
	}
	for each (FLOAT course in courses_)
	{
		enumBullets(
			[&](std::shared_ptr<CQuadPolygon> bullet_, size_t index_) -> BOOL {
			if (bullet_->getState() == eWaiting)
			{
				bullet_->setState(eMoving);
				bullet_->setForwardDirection(course + getAngleZ());
				return FALSE; //!< 列挙を中断
			}
			return TRUE; //!< 列挙を続行												
		});
	}
}

/**
* @fn BOOL CSpaceship::collisionBullet(XMFLOAT4 rivalPos_, FLOAT range_)
* @brief 衝突
* @param	rivalPos_	位置
* @param	range_	衝突の範囲
* @return			無し
*/
BOOL CSpaceship::collisionBullet(XMFLOAT4 rivalPos_, FLOAT range_)
{
	BOOL hit = FALSE;
	enumBullets(
		[&](std::shared_ptr<CQuadPolygon> bullet_, size_t index_) -> BOOL {
		if (bullet_->getState() == eMoving)
		{
			if (bullet_->collision(rivalPos_, bullet_->getPos(), range_))
			{
				hit = TRUE;
				bullet_->reset(computeBulletPos(index_));
				return TRUE; //!< 列挙を続行
			}
		}
		return TRUE; //!< 列挙を続行
	});
	return hit;
}

/**
* @fn void CSpaceship::resetAllBullets()
* @brief すべての弾丸を初期状態にする
* @param	無し
* @return	無し
*/
void CSpaceship::resetAllBullets()
{
	enumBullets(
		[&](std::shared_ptr<CQuadPolygon> bullet_, size_t index_) -> BOOL {
		bullet_->reset(computeBulletPos(index_));
		return TRUE; //!< 列挙を続行
	});	
}

/**
* @fn void CSpaceship::enumBullets(std::function<BOOL(std::shared_ptr<CQuadPolygon>, size_t)> callbackEnumeProc_)
* @brief 弾丸の列挙
* @param	callbackEnumeProc_	コールバック<ラムダ式>
* @return	無し
*/
void CSpaceship::enumBullets(std::function<BOOL(std::shared_ptr<CQuadPolygon>, size_t)> callbackEnumeProc_)
{
	for (size_t i = 0; i < _countof (m_pBullets); i++)
	{
		if (m_pBullets[i] != NULL)
		{
			if (callbackEnumeProc_ (m_pBullets[i], i) == FALSE)
			{
				break; //!< 列挙を中断
			}
		}
	}
}

/**
* @fn void CSpaceship::release()
* @brief リリース
* @param	無し
* @return	無し
*/
void CSpaceship::release()
{
	m_pExplosion->release();
	m_pBulletTex->release();
	m_pExplosionTex->release();
	enumBullets(
		[&](std::shared_ptr<CQuadPolygon> bullet_, size_t index_) -> BOOL {
		SAFE_release(bullet_);
		return TRUE; //!< 列挙を続行												
	});
	super::release();	//!< スーパークラスのメソッドを実行
}