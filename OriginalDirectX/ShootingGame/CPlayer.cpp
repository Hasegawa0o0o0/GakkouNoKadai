/**
 * @file CPlayer.cpp
 * @brief プレーヤー
 * @author 織戸　喜隆
 * @date 日付（2017.02.14）
 */

#include "stdafx.h"
#include "CPlayer.h"

/**
 * @fn std::unique_ptr<CPlayer> CPlayer::create(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, XMFLOAT4 position_, XMFLOAT2 size_, std::vector<QuadrangleTexel> texels)
 * @brief インスタンスを生成し初期化 【クラスメソッド】
 * @param pDevice_			デバイス
 * @param pDeviceContext_	デバイスコンテキスト
 * @param viewPort_[]		ビューポート
 * @param shaderName_		シェーダープログラム
 * @param position_			ウィンドウの位置
 * @param size_				スプライトの大きさ
 * @param bulletTexelIndex_	弾丸のテクセルのインデックス（プレーヤーと共有）
 * @return					CPlayerのユニークポインタ
 */
std::unique_ptr<CPlayer> CPlayer::create(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, XMFLOAT4 position_, XMFLOAT2 size_, int bulletTexelIndex_)
{
	std::unique_ptr<CPlayer> object = unique_ptr<CPlayer>(new CPlayer());
	object->init(pDevice_, pDeviceContext_, viewPort_, shaderName_, position_, size_, bulletTexelIndex_);
	return object;
}

/**
 * @fn void CPlayer::init(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, XMFLOAT4 position_, XMFLOAT2 size_, std::vector<QuadrangleTexel> texels)
 * @brief インスタンスを初期化
 * @param pDevice_			デバイス
 * @param pDeviceContext_	デバイスコンテキスト
 * @param viewPort_[]		ビューポート
 * @param shaderName_		シェーダープログラム
 * @param position_			ウィンドウの位置
 * @param size_				スプライトの大きさ
 * @param bulletTexelIndex_	弾丸のテクセルのインデックス（プレーヤーと共有）
 * @return					無し
 */
void CPlayer::init( ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, XMFLOAT4 position_, XMFLOAT2 size_, int bulletTexelIndex_)
{
	super::init(pDevice_, pDeviceContext_, viewPort_, shaderName_, position_, size_, kTexelPlayers, bulletTexelIndex_); //!< スーパークラスのメソッドを実行
	m_moveQuantity = 0.528f * 1.75f;
	enumBullets(
		[=](std::shared_ptr<CQuadPolygon> bullet_, size_t index_) -> BOOL {
		bullet_->setMoveQuantity(2.5);
		return TRUE; //!< 列挙を続行
	});
}

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
}

/**
 * @fn BOOL CPlayer::stepAnimation (BOOL enable_)
 * @brief アニメーション
 * @param enable_	アニメーションの有効性
 * @return			継続・中止
 */
void CPlayer::stepAnimation (BOOL enable_)
{
	super::stepAnimation(enable_, kTexelPlayers); //!< スーパークラスのメソッドを実行
}

/**
 * @fn void CPlayer::render(XMFLOAT4X4	matView_, XMFLOAT4X4 matProj_)
 * @brief レンダリング
 * @param matView_	ビュー行列
 * @param matProj_	プロジェクション行列
 * @return			無し
 */
void CPlayer::render(XMFLOAT4X4 matView_, XMFLOAT4X4 matProj_, std::function<void(ID3D11DeviceContext* pDeviceContext_)> callbackTextureProc_)
{
	if (!outOfWindow())
	{
		if (m_damaged)
		{
			super::render(matView_, matProj_, callbackTextureProc_, m_animIndex, { 1, 0, 0, 0 }); //!< スーパークラスのメソッドを実行
		}
		else
		{
			super::render(matView_, matProj_, callbackTextureProc_, m_animIndex, { 0, 0, 0, 0 }); //!< スーパークラスのメソッドを実行
		}
	}
}

/**
 * @fn void CPlayer::shot(void)
 * @brief 弾丸を発射
 * @param	無し
 * @return	無し
 */
void CPlayer::shot(void)
{
	std::vector<BulletPlace> bulletsPlace = {	
		{ 0.0f, kBullet_Left },		//!< 左
		{ 0.0f, kBullet_Right }		//!< 右
	};

	if (isInvalid() || countEmptyBullets() < bulletsPlace.size())
	{
		return;
	}
	for each (BulletPlace obj in bulletsPlace)
	{	
		enumBullets([&](std::shared_ptr<CQuadPolygon> bullet_, size_t index_) -> BOOL {
			if ( (bullet_->getState() == eWaiting) && (bullet_->getPos().x < getPos().x) || 
				 (bullet_->getState() == eWaiting) && (bullet_->getPos().x > getPos().x) )
			{
				bullet_->setPos({ getPos().x + obj.pos, bullet_->getPos().y, bullet_->getPos().z, bullet_->getPos().w });
				bullet_->setState(eMoving);
				bullet_->setAngleZ(getAngleZ());
				bullet_->setForwardDirection(obj.dir + getAngleZ());
				return FALSE; //!< 列挙を中断
			}
			return TRUE; //!< 列挙を続行
		});
	}
}