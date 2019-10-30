/**
* @file CText.h
* @brief テキスト表示のためのクラス
* @author 長谷川　勇太
* @date 日付（2018.11.19）
*/
#pragma once

#include "stdafx.h"
#include "DotEat.h"
#include "CPicture.h"

class CText
{
	vector<char> m_vTextChr;
	vector<shared_ptr<CPicture>> m_vText;
public:
	CText();
	~CText();
	static std::shared_ptr<CText> create(
		ID3D11Device*			pDevice_,			//!< デバイス
		ID3D11DeviceContext*	pDeviceContext_,	//!< デバイスコンテキスト
		D3D11_VIEWPORT          viewPort_[],		//!< ビューポート
		LPCWSTR					shaderName_,		//!< シェーダープログラム
		LPCWSTR					textureName_,			//!< PNGファイル
		XMFLOAT4				pos_,				// 初めの位置
		XMFLOAT2				size_,				//!< スプライトの大きさ
		std::vector<QuadrangleTexel>	texels,			//!< テクセルの配列
		char					str_[]);			// 初期テキスト
	void init(
		ID3D11Device*			pDevice_,			//!< デバイス
		ID3D11DeviceContext*	pDeviceContext_,	//!< デバイスコンテキスト
		D3D11_VIEWPORT          viewPort_[],		//!< ビューポート
		LPCWSTR					shaderName_,			//!< シェーダープログラム
		LPCWSTR					textureName_,			//!< PNGファイル
		XMFLOAT4				pos_,				// 初めの位置
		XMFLOAT2				size_,				//!< スプライトの大きさ
		std::vector<QuadrangleTexel>	texels,			//!< テクセルの配列
		char					str_[]);			// 初期テキスト
	void setText(const char str[]);
	void setText(const int n);
	string getText();
	void resetTest();
	void clearText();
	void setBeginPos(XMFLOAT4 pos_);
	XMFLOAT4 getBeginPos();
	void conversionC2I(vector<int>& vi_, vector<char> str_);
	void render(XMFLOAT4X4	matView_, XMFLOAT4X4 matProj_, vector<shared_ptr<CTexture>> vTexture_);
	void release();
};

// テキストUV
static std::vector <QuadrangleTexel>  gTextTexel = {
	// 1~0
	QuadrangleTexel{ { k1_26 * 0, k1_3 * 1 },{ k1_26 * 0, k1_3 * 0 },{ k1_26 * 1, k1_3 * 1 },{ k1_26 * 1, k1_3 * 0 } },
	QuadrangleTexel{ { k1_26 * 1, k1_3 * 1 },{ k1_26 * 1, k1_3 * 0 },{ k1_26 * 2, k1_3 * 1 },{ k1_26 * 2, k1_3 * 0 } },
	QuadrangleTexel{ { k1_26 * 2, k1_3 * 1 },{ k1_26 * 2, k1_3 * 0 },{ k1_26 * 3, k1_3 * 1 },{ k1_26 * 3, k1_3 * 0 } },
	QuadrangleTexel{ { k1_26 * 3, k1_3 * 1 },{ k1_26 * 3, k1_3 * 0 },{ k1_26 * 4, k1_3 * 1 },{ k1_26 * 4, k1_3 * 0 } },
	QuadrangleTexel{ { k1_26 * 4, k1_3 * 1 },{ k1_26 * 4, k1_3 * 0 },{ k1_26 * 5, k1_3 * 1 },{ k1_26 * 5, k1_3 * 0 } },
	QuadrangleTexel{ { k1_26 * 5, k1_3 * 1 },{ k1_26 * 5, k1_3 * 0 },{ k1_26 * 6, k1_3 * 1 },{ k1_26 * 6, k1_3 * 0 } },
	QuadrangleTexel{ { k1_26 * 6, k1_3 * 1 },{ k1_26 * 6, k1_3 * 0 },{ k1_26 * 7, k1_3 * 1 },{ k1_26 * 7, k1_3 * 0 } },
	QuadrangleTexel{ { k1_26 * 7, k1_3 * 1 },{ k1_26 * 7, k1_3 * 0 },{ k1_26 * 8, k1_3 * 1 },{ k1_26 * 8, k1_3 * 0 } },
	QuadrangleTexel{ { k1_26 * 8, k1_3 * 1 },{ k1_26 * 8, k1_3 * 0 },{ k1_26 * 9, k1_3 * 1 },{ k1_26 * 9, k1_3 * 0 } },
	QuadrangleTexel{ { k1_26 * 9, k1_3 * 1 },{ k1_26 * 9, k1_3 * 0 },{ k1_26 * 10, k1_3 * 1 },{ k1_26 * 10, k1_3 * 0 } },
	// A~Z
	QuadrangleTexel{ { k1_26 * 0, k1_3 * 2 },{ k1_26 * 0, k1_3 * 1 },{ k1_26 * 1, k1_3 * 2 },{ k1_26 * 1, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 1, k1_3 * 2 },{ k1_26 * 1, k1_3 * 1 },{ k1_26 * 2, k1_3 * 2 },{ k1_26 * 2, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 2, k1_3 * 2 },{ k1_26 * 2, k1_3 * 1 },{ k1_26 * 3, k1_3 * 2 },{ k1_26 * 3, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 3, k1_3 * 2 },{ k1_26 * 3, k1_3 * 1 },{ k1_26 * 4, k1_3 * 2 },{ k1_26 * 4, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 4, k1_3 * 2 },{ k1_26 * 4, k1_3 * 1 },{ k1_26 * 5, k1_3 * 2 },{ k1_26 * 5, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 5, k1_3 * 2 },{ k1_26 * 5, k1_3 * 1 },{ k1_26 * 6, k1_3 * 2 },{ k1_26 * 6, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 6, k1_3 * 2 },{ k1_26 * 6, k1_3 * 1 },{ k1_26 * 7, k1_3 * 2 },{ k1_26 * 7, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 7, k1_3 * 2 },{ k1_26 * 7, k1_3 * 1 },{ k1_26 * 8, k1_3 * 2 },{ k1_26 * 8, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 8, k1_3 * 2 },{ k1_26 * 8, k1_3 * 1 },{ k1_26 * 9, k1_3 * 2 },{ k1_26 * 9, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 9, k1_3 * 2 },{ k1_26 * 9, k1_3 * 1 },{ k1_26 * 10, k1_3 * 2 },{ k1_26 * 10, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 10, k1_3 * 2 },{ k1_26 * 10, k1_3 * 1 },{ k1_26 * 11, k1_3 * 2 },{ k1_26 * 11, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 11, k1_3 * 2 },{ k1_26 * 11, k1_3 * 1 },{ k1_26 * 12, k1_3 * 2 },{ k1_26 * 12, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 12, k1_3 * 2 },{ k1_26 * 12, k1_3 * 1 },{ k1_26 * 13, k1_3 * 2 },{ k1_26 * 13, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 13, k1_3 * 2 },{ k1_26 * 13, k1_3 * 1 },{ k1_26 * 14, k1_3 * 2 },{ k1_26 * 14, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 14, k1_3 * 2 },{ k1_26 * 14, k1_3 * 1 },{ k1_26 * 15, k1_3 * 2 },{ k1_26 * 15, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 15, k1_3 * 2 },{ k1_26 * 15, k1_3 * 1 },{ k1_26 * 16, k1_3 * 2 },{ k1_26 * 16, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 16, k1_3 * 2 },{ k1_26 * 16, k1_3 * 1 },{ k1_26 * 17, k1_3 * 2 },{ k1_26 * 17, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 17, k1_3 * 2 },{ k1_26 * 17, k1_3 * 1 },{ k1_26 * 18, k1_3 * 2 },{ k1_26 * 18, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 18, k1_3 * 2 },{ k1_26 * 18, k1_3 * 1 },{ k1_26 * 19, k1_3 * 2 },{ k1_26 * 19, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 19, k1_3 * 2 },{ k1_26 * 19, k1_3 * 1 },{ k1_26 * 20, k1_3 * 2 },{ k1_26 * 20, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 20, k1_3 * 2 },{ k1_26 * 20, k1_3 * 1 },{ k1_26 * 21, k1_3 * 2 },{ k1_26 * 21, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 21, k1_3 * 2 },{ k1_26 * 21, k1_3 * 1 },{ k1_26 * 22, k1_3 * 2 },{ k1_26 * 22, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 22, k1_3 * 2 },{ k1_26 * 22, k1_3 * 1 },{ k1_26 * 23, k1_3 * 2 },{ k1_26 * 23, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 23, k1_3 * 2 },{ k1_26 * 23, k1_3 * 1 },{ k1_26 * 24, k1_3 * 2 },{ k1_26 * 24, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 24, k1_3 * 2 },{ k1_26 * 24, k1_3 * 1 },{ k1_26 * 25, k1_3 * 2 },{ k1_26 * 25, k1_3 * 1 } },
	QuadrangleTexel{ { k1_26 * 25, k1_3 * 2 },{ k1_26 * 25, k1_3 * 1 },{ k1_26 * 26, k1_3 * 2 },{ k1_26 * 26, k1_3 * 1 } },
	// a~z
	QuadrangleTexel{ { k1_26 * 0, k1_3 * 3 },{ k1_26 * 0, k1_3 * 2 },{ k1_26 * 1, k1_3 * 3 },{ k1_26 * 1, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 1, k1_3 * 3 },{ k1_26 * 1, k1_3 * 2 },{ k1_26 * 2, k1_3 * 3 },{ k1_26 * 2, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 2, k1_3 * 3 },{ k1_26 * 2, k1_3 * 2 },{ k1_26 * 3, k1_3 * 3 },{ k1_26 * 3, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 3, k1_3 * 3 },{ k1_26 * 3, k1_3 * 2 },{ k1_26 * 4, k1_3 * 3 },{ k1_26 * 4, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 4, k1_3 * 3 },{ k1_26 * 4, k1_3 * 2 },{ k1_26 * 5, k1_3 * 3 },{ k1_26 * 5, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 5, k1_3 * 3 },{ k1_26 * 5, k1_3 * 2 },{ k1_26 * 6, k1_3 * 3 },{ k1_26 * 6, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 6, k1_3 * 3 },{ k1_26 * 6, k1_3 * 2 },{ k1_26 * 7, k1_3 * 3 },{ k1_26 * 7, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 7, k1_3 * 3 },{ k1_26 * 7, k1_3 * 2 },{ k1_26 * 8, k1_3 * 3 },{ k1_26 * 8, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 8, k1_3 * 3 },{ k1_26 * 8, k1_3 * 2 },{ k1_26 * 9, k1_3 * 3 },{ k1_26 * 9, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 9, k1_3 * 3 },{ k1_26 * 9, k1_3 * 2 },{ k1_26 * 10, k1_3 * 3 },{ k1_26 * 10, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 10, k1_3 * 3 },{ k1_26 * 10, k1_3 * 2 },{ k1_26 * 11, k1_3 * 3 },{ k1_26 * 11, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 11, k1_3 * 3 },{ k1_26 * 11, k1_3 * 2 },{ k1_26 * 12, k1_3 * 3 },{ k1_26 * 12, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 12, k1_3 * 3 },{ k1_26 * 12, k1_3 * 2 },{ k1_26 * 13, k1_3 * 3 },{ k1_26 * 13, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 13, k1_3 * 3 },{ k1_26 * 13, k1_3 * 2 },{ k1_26 * 14, k1_3 * 3 },{ k1_26 * 14, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 14, k1_3 * 3 },{ k1_26 * 14, k1_3 * 2 },{ k1_26 * 15, k1_3 * 3 },{ k1_26 * 15, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 15, k1_3 * 3 },{ k1_26 * 15, k1_3 * 2 },{ k1_26 * 16, k1_3 * 3 },{ k1_26 * 16, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 16, k1_3 * 3 },{ k1_26 * 16, k1_3 * 2 },{ k1_26 * 17, k1_3 * 3 },{ k1_26 * 17, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 17, k1_3 * 3 },{ k1_26 * 17, k1_3 * 2 },{ k1_26 * 18, k1_3 * 3 },{ k1_26 * 18, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 18, k1_3 * 3 },{ k1_26 * 18, k1_3 * 2 },{ k1_26 * 19, k1_3 * 3 },{ k1_26 * 19, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 19, k1_3 * 3 },{ k1_26 * 19, k1_3 * 2 },{ k1_26 * 20, k1_3 * 3 },{ k1_26 * 20, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 20, k1_3 * 3 },{ k1_26 * 20, k1_3 * 2 },{ k1_26 * 21, k1_3 * 3 },{ k1_26 * 21, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 21, k1_3 * 3 },{ k1_26 * 21, k1_3 * 2 },{ k1_26 * 22, k1_3 * 3 },{ k1_26 * 22, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 22, k1_3 * 3 },{ k1_26 * 22, k1_3 * 2 },{ k1_26 * 23, k1_3 * 3 },{ k1_26 * 23, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 23, k1_3 * 3 },{ k1_26 * 23, k1_3 * 2 },{ k1_26 * 24, k1_3 * 3 },{ k1_26 * 24, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 24, k1_3 * 3 },{ k1_26 * 24, k1_3 * 2 },{ k1_26 * 25, k1_3 * 3 },{ k1_26 * 25, k1_3 * 2 } },
	QuadrangleTexel{ { k1_26 * 25, k1_3 * 3 },{ k1_26 * 25, k1_3 * 2 },{ k1_26 * 26, k1_3 * 3 },{ k1_26 * 26, k1_3 * 2 } },
	// スペース
	QuadrangleTexel{ { k1_26 * 10, k1_3 * 1 },{ k1_26 * 10, k1_3 * 0 },{ k1_26 * 11, k1_3 * 1 },{ k1_26 * 11, k1_3 * 0 } },
};
