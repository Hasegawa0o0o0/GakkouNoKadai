/**
* @file CExplosion.h
* @brief 爆発
* @author 織戸　喜隆
* @date 日付（2017.02.14）
*/
#pragma once

#include "stdafx.h"
#include "UVAnimation.h"
#include "CQuadPolygon.h"

// Const

class CExplosion :
	public CQuadPolygon
{
	typedef CQuadPolygon super;

	BOOL m_started		= FALSE;	// 爆発開始
	BOOL m_completed	= FALSE;	// 完了
	size_t m_TexelIndex	= 0;		// アニメーションのインデックス
	int m_waitCounter	= 0;		// アニメーション1枚当たりのカウンタ
	int m_framesOfWait	= 0;		// アニメーション1枚当たりのフレーム数
public:
	static std::shared_ptr<CExplosion> CExplosion::create(
		ID3D11Device*			pDevice_,			// デバイス
		ID3D11DeviceContext*	pDeviceContext_,	// デバイスコンテキスト
		D3D11_VIEWPORT			viewPort_[],		// ビューポート
		LPCWSTR					shaderName_,		// シェーダープログラム
		XMFLOAT4				position_,			// ウィンドウの位置
		XMFLOAT2				size_,				// スプライトの大きさ
		std::vector<QuadrangleTexel>	texels,			// テクセルの配列
		int						framesOfWait_);		// ポーズするフレーム数
	CExplosion();
	~CExplosion();

	void explode()		{ m_started = TRUE; }
	void init(
		ID3D11Device*			pDevice_,			// デバイス
		ID3D11DeviceContext*	pDeviceContext_,	// デバイスコンテキスト
		D3D11_VIEWPORT			viewPort_[],		// ビューポート
		LPCWSTR					shaderName_,		// シェーダープログラム
		XMFLOAT4				position_,			// ウィンドウの位置
		XMFLOAT2				size_,				// スプライトの大きさ
		std::vector<QuadrangleTexel>	texels,			// テクセルの配列
		int						framesOfWait_);		// ポーズするフレーム数
	void render(XMFLOAT4X4 matView_, XMFLOAT4X4 matProj_, std::function<void(ID3D11DeviceContext* pDeviceContext_)> callbackTextureProc_, size_t texelIndex_ = 0);
	void reset();
	BOOL isCompleted() { return m_started && m_completed; };
};
