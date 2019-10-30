/**
* @file CMapChip.h
* @brief マップチップ
* @author 織戸　喜隆
* @date 日付（2017.10.13）
*/
#pragma once

#include "stdafx.h"
#include "CPicture.h"
#include "DotEat.h"

class CMapChip :
	public CPicture
{
	typedef CPicture super;

private:
	MapIndex m_index = { 0, 0 };
	string m_objectName = "";		// 生成オブジェクトの名前

public:
	static std::unique_ptr<CMapChip> create(
		ID3D11Device*			pDevice_,			//!< デバイス
		ID3D11DeviceContext*	pDeviceContext_,	//!< デバイスコンテキスト
		D3D11_VIEWPORT          viewPort_[],		//!< ビューポート
		LPCWSTR					shaderName_,		//!< シェーダープログラム
		LPCWSTR					textureName_,			//!< PNGファイル
		XMFLOAT2				size_,				//!< スプライトの大きさ
		MapIndex				index_,				//!< マップの位置(x, y)
		std::vector<QuadrangleTexel>	texels);			//!< テクセルの配列
	void init(
		ID3D11Device*			pDevice_,			//!< デバイス
		ID3D11DeviceContext*	pDeviceContext_,	//!< デバイスコンテキスト
		D3D11_VIEWPORT          viewPort_[],		//!< ビューポート
		LPCWSTR					shaderName_,			//!< シェーダープログラム
		LPCWSTR					textureName_,			//!< PNGファイル
		XMFLOAT2				size_,				//!< スプライトの大きさ
		MapIndex				index_,				//!< マップの位置(x, y)
		std::vector<QuadrangleTexel>	texels);			//!< テクセルの配列
	CMapChip();
	~CMapChip();
	MapIndex getIndex()				{ return m_index;	}
	void setIndex(MapIndex index_)	{
		m_index = index_;
		setPosFromIndex(index_);
	}
	void setPosFromIndex(MapIndex index_, XMFLOAT2 size_ = { 32.0f, 32.0f });
	// 名前
	void setName(string name_) { m_objectName = name_; }
	string getName()const { return m_objectName; }
};