/**
* @file CPlayer.h
* @brief プレーヤー
* @author 織戸　喜隆
* @date 日付（2017.02.14）
*/
#pragma once

#include "stdafx.h"
#include "CSpaceship.h"

/*! @struct BulletPlace
@brief  弾丸の構造体 <方向 位置>
*/
struct BulletPlace {
	FLOAT dir;
	FLOAT pos;
};

class CPlayer :
	public CSpaceship
{
	typedef CSpaceship super;

public:
	static std::unique_ptr<CPlayer> create(
		ID3D11Device*			pDevice_,			// デバイス
		ID3D11DeviceContext*	pDeviceContext_,	// デバイスコンテキスト
		D3D11_VIEWPORT			viewPort_[],		// ビューポート
		LPCWSTR					shaderName_,		// シェーダープログラム
		XMFLOAT4				position_,			// ウィンドウの位置
		XMFLOAT2				size_,				// スプライトの大きさ
		int						bulletTexelIndex_);	// 弾丸のテクセルのインデックス（敵と共有）
	CPlayer();
	~CPlayer();
	XMFLOAT4 computeBulletPos(size_t index_) {
		FLOAT x = getPos().x + kBullet_Left;
		FLOAT y = getPos().y + 7.0f;
		if (countSideBullet(x) == 0)
		{
			x = getPos().x + kBullet_Left;
		}
		else {
			x = getPos().x + kBullet_Right;
		}
		return{ x, y, 0, 0 };
	}
	int countSideBullet(FLOAT x_)
	{
		int ans = 0;
		enumBullets([&](std::shared_ptr<CQuadPolygon> bullet_, size_t index_) -> BOOL {
			if ((bullet_->getState() == eWaiting) && (bullet_->getPos().x == x_))
			{
				ans++;
			}
			return TRUE; //!< 列挙を続行
		});
		return ans;
	}
	void init(
		ID3D11Device*			pDevice_,			// デバイス
		ID3D11DeviceContext*	pDeviceContext_,	// デバイスコンテキスト
		D3D11_VIEWPORT			viewPort_[],		// ビューポート
		LPCWSTR					shaderName_,		// シェーダープログラム
		XMFLOAT4				position_,			// ウィンドウの位置
		XMFLOAT2				size_,				// スプライトの大きさ
		int						bulletTexelIndex_);	// 弾丸のテクセルのインデックス（敵と共有）
	void stepAnimation (BOOL enable_);
	void render(XMFLOAT4X4	matView_, XMFLOAT4X4	matProj_, std::function<void(ID3D11DeviceContext* pDeviceContext_)> callbackTextureProc_ );
	void shot(void);
	BOOL isExplosionCompleted()
	{
		return m_pExplosion->isCompleted();
	}
};