/**
 * @file CScroll.h
 * @brief CScrollクスクロールクラスラス
 * @author 織戸　喜隆
 * @date 日付（2017.02.14）
 */
#pragma once

#include "stdafx.h"
#include "CPicture.h"

/*! @class CScroll
@brief  スクロールクラス
*/
class CScroll
{
private:
	eScrollMode						m_scrollMode = eVirtical;	//!< スクロールする方向
	std::unique_ptr<CPicture>	m_sprite1 = NULL;	//!< スクロールする　スプライト上
	std::unique_ptr<CPicture>	m_sprite2 = NULL;	//!< スクロールする　スプライト下
	XMFLOAT2	m_posLeftTop = { 0.0f, 0.0f }; 			//!< スプライト左上　位置
	XMFLOAT2	m_posRightBottom = { 0.0f, 0.0f }; 		//!< スプライト右下　位置
public:
	static std::unique_ptr<CScroll> create(
		ID3D11Device*			pDevice_,			//!< デバイス
		ID3D11DeviceContext*	pDeviceContext_,	//!< デバイスコンテキスト
		D3D11_VIEWPORT			viewPort_[],		//!< ビューポート
		LPCWSTR					shaderName_,			//!< シェーダープログラム
		LPCWSTR					textureName_,			//!< PNGファイル
		XMFLOAT4				position_,			//!< ウィンドウの位置
		XMFLOAT2				size_,				//!< スプライトの大きさ
		std::vector<QuadrangleTexel>	texels,			//!< テクセルの配列
		eScrollMode				scrollMode_			//!< スクロールする方向
		);	
	CScroll();
	~CScroll();
	void setScrollMode(eScrollMode mode_) { m_scrollMode = mode_; }
	void offsetPos(XMFLOAT4 v4Pos_) {
		m_sprite1->offsetPos(v4Pos_);
		m_sprite2->offsetPos(v4Pos_);
	};
	void init(
		ID3D11Device*			pDevice_,			//!< デバイス
		ID3D11DeviceContext*	pDeviceContext_,	//!< デバイスコンテキスト
		D3D11_VIEWPORT			viewPort_[],		//!< ビューポート
		LPCWSTR					shaderName_,			//!< シェーダープログラム
		LPCWSTR					textureName_,			//!< PNGファイル
		XMFLOAT4				position_,			//!< ウィンドウの位置
		XMFLOAT2				size_,				//!< スプライトの大きさ
		std::vector<QuadrangleTexel>	texels,			//!< テクセルの配列
		eScrollMode				scrollMode_			//!< スクロールする方向
	);

	void scroll(float value_);
	void horizontalScroll(float value_);
	void verticalScroll(float value_);
	void render(XMFLOAT4X4 matView_, XMFLOAT4X4 matProj_);
	void release();
};