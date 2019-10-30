/**
* @file CEnemy.h
* @brief 敵
* @author 織戸　喜隆
* @date 日付（2017.02.14）
*/
#pragma once

#include "stdafx.h"
#include "CSpaceship.h"

class CEnemy :
	public CSpaceship
{
	typedef CSpaceship super;

public:
	static std::shared_ptr<CEnemy> create(
		ID3D11Device*			pDevice_,			// デバイス
		ID3D11DeviceContext*	pDeviceContext_,	// デバイスコンテキスト
		D3D11_VIEWPORT          viewPort_[],		// ビューポート
		LPCWSTR					shaderName_,		// シェーダープログラム
		XMFLOAT4				position_,			// ウィンドウの位置
		XMFLOAT2				size_,				// スプライトの大きさ
		int						bulletTexelIndex_);	// 弾丸のテクセルのインデックス（敵と共有）
	CEnemy();
	~CEnemy();
	XMFLOAT4 computeBulletPos(size_t index_) {
		FLOAT x = getPos().x;
		FLOAT y = getPos().y - 8.0f;
		return{ x, y, 0, 0 };
	}
	void init(
		ID3D11Device*			pDevice_,			// デバイス
		ID3D11DeviceContext*	pDeviceContext_,	// デバイスコンテキスト
		D3D11_VIEWPORT          viewPort_[],		// ビューポート
		LPCWSTR					shaderName_,		// シェーダープログラム
		XMFLOAT4				position_,			// ウィンドウの位置
		XMFLOAT2				size_,				// スプライトの大きさ
		int						bulletTexelIndex_);	// 弾丸のテクセルのインデックス（プレーヤーと共有）
	void stepAnimation (BOOL enable_);
	void render(XMFLOAT4X4	matView_, XMFLOAT4X4 matProj_, std::function<void(ID3D11DeviceContext* pDeviceContext_)> callbackTextureProc_);
	void shot(void);
};