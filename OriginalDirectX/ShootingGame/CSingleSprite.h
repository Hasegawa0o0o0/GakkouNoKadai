/**
* @file CSingleSprite.h
* @brief 弾丸 テクスチャをクラス変数として共有する
* @author 織戸　喜隆
* @date 日付（2018.07.23）
*/
#pragma once

#include "stdafx.h"
#include "CommonType.h"
#include "CPolygon.h"
#include "CXMMath.h"

class CSingleSprite :
	public CPolygon
{
	typedef CPolygon super;
private:
	static ID3D11SamplerState*			g_pSampleLinear;	//!< 【クラス変数】テクスチャーのサンプラー
	static ID3D11ShaderResourceView*	g_pTexture;			//!< 【クラス変数】テクスチャー

public:
	static std::unique_ptr<CSingleSprite> create(//!< 【クラスメソッド】
		ID3D11Device*			pDevice_,		//!< デバイス
		ID3D11DeviceContext*	pDeviceContext_,//!< デバイスコンテキスト
		D3D11_VIEWPORT			viewPort_[],	//!< ビューポート
		LPCWSTR					shaderName_,	//!< シェーダープログラム
		LPCWSTR					textureName_,	//!< 爆発の画像ファイル
		XMFLOAT4				position_,		//!< ウィンドウの位置
		XMFLOAT2				size_,			//!< スプライトの大きさ
		std::vector<UVCoord>	uvArray_);		//!< 爆発のUV配列

	CSingleSprite();
	~CSingleSprite();
	virtual void init(
		ID3D11Device*			pDevice_,		//!< デバイス
		ID3D11DeviceContext*	pDeviceContext_,//!< デバイスコンテキスト
		D3D11_VIEWPORT			viewPort_[],	//!< ビューポート
		LPCWSTR					shaderName_,	//!< シェーダープログラム
		LPCWSTR					textureName_,	//!< 爆発の画像ファイル
		XMFLOAT4				position_,		//!< ウィンドウの位置
		XMFLOAT2				size_,			//!< スプライトの大きさ
		std::vector<UVCoord>	uvArray_);		//!< 爆発のUV配列
	void createSamplerAndTexture(ID3D11Device*pDevice_, LPCWSTR textureName_);
	virtual void setSamplerAndTexture();
	virtual void render(XMFLOAT4X4 matView_, XMFLOAT4X4 matProj_, size_t uvIndex_ = 0, XMFLOAT4 diffuse_ = { 0, 0, 0, 0 }); //!< テクスチャをクラス変数として共有する
	void release(void);
};

