/**
 * @file CMapChip.cpp
 * @brief マップチップ
 * @author 織戸　喜隆
 * @date 日付（2017.10.13）
 */
#include "stdafx.h"
#include "CMapChip.h"

/**
 * @fn std::unique_ptr<CMapChip> CMapChip::create(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, LPCWSTR textureName_,　XMFLOAT4 position_, XMFLOAT2 size_, MapIndex index_, std::vector<QuadrangleTexel> texels)
 * @brief インスタンスを生成し初期化 【クラスメソッド】
 * @param pDevice_			デバイス
 * @param pDeviceContext_	デバイスコンテキスト
 * @param viewPort_[]		ビューポート
 * @param shaderName_		シェーダープログラム
 * @param textureName_		PNGファイル
 * @param position_			ウィンドウの位置
 * @param size_				スプライトの大きさ
 * @param index_			マップの位置(x, y)
 * @param texels			マップチップのテクセルの配列
 * @return					CMapChipのユニークポインタ
 */
std::unique_ptr<CMapChip> CMapChip::create(
	ID3D11Device* pDevice_,
	ID3D11DeviceContext* pDeviceContext_,
	D3D11_VIEWPORT viewPort_[],
	LPCWSTR shaderName_,
	LPCWSTR textureName_,
	XMFLOAT2 size_,
	MapIndex index_,
	std::vector<QuadrangleTexel> texels)
{
	std::unique_ptr<CMapChip> object = unique_ptr<CMapChip>(new CMapChip());
	object->init(pDevice_, pDeviceContext_, viewPort_, shaderName_, textureName_, size_, index_, texels);
	return object;
}

CMapChip::CMapChip()
{
}


CMapChip::~CMapChip()
{
}

/**
 * @fn void CMapChip::init(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, LPCWSTR textureName_,　XMFLOAT4 position_, XMFLOAT2 size_,MapIndex index_, std::vector<QuadrangleTexel> texels)
 * @brief インスタンスを初期化
 * @param pDevice_			デバイス
 * @param pDeviceContext_	デバイスコンテキスト
 * @param viewPort_[]		ビューポート
 * @param shaderName_		シェーダープログラム
 * @param textureName_		PNGファイル
 * @param index_			マップの位置(x, y)
 * @param texels			テクセルの配列
 * @return					無し
 */
void CMapChip::init(
	ID3D11Device* pDevice_,
	ID3D11DeviceContext* pDeviceContext_,
	D3D11_VIEWPORT viewPort_[],
	LPCWSTR shaderName_,
	LPCWSTR textureName_,
	XMFLOAT2 size_,
	MapIndex index_,
	std::vector<QuadrangleTexel> texels)
{
	setPosFromIndex(index_);
	super::init(pDevice_, pDeviceContext_, viewPort_, shaderName_, textureName_, m_position.pos, size_, texels); //!< スーパークラスのメソッドを実行
}

/**
 * @fn void CMapChip::setPosFromIndex(MapIndex index_, XMFLOAT2 size_)
 * @brief マップ上の位置を設定する
 * @param index_		マップの位置(x, y)
 * @param size_		マップチップの大きさ＜デフォルト値＞
 * @return			無し
 */

void CMapChip::setPosFromIndex(MapIndex index_, XMFLOAT2 size_)
{
	m_index = index_;
	m_position.pos.x = (FLOAT)(index_.x * size_.x);
	m_position.pos.y = (FLOAT)(index_.y * size_.y);
	m_position.pos.z = 0.0f;
	m_position.pos.w = 0.0f;
}