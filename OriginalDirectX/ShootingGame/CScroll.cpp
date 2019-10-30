/**
 * @file CScroll.cpp
 * @brief スクロールクラス
 * @author 織戸　喜隆
 * @date 日付（2017.02.14）
 */

#include "stdafx.h"
#include "CScroll.h"

/**
 * @fn void CScroll::create(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, LPCWSTR textureName_, XMFLOAT4 position_, XMFLOAT2 size_, std::vector<QuadrangleTexel> texels)
 * @brief インスタンスを取得 【クラスメソッド】
 * @param pDevice_			デバイス
 * @param pDeviceContext_	デバイスコンテキスト
 * @param viewPort_[]		ビューポート
 * @param shaderName_			シェーダープログラム
 * @param textureName	PNGファイル_
 * @param position_			ウィンドウの位置
 * @param size_				スプライトの大きさ
 * @param texels			テクセルの配列
 * @param scrollMode_		スクロールの方向（縦/横）
 * @return					CScrollのユニークポインタ
 */
std::unique_ptr<CScroll> CScroll::create(
	ID3D11Device*			pDevice_,
	ID3D11DeviceContext*	pDeviceContext_,
	D3D11_VIEWPORT			viewPort_[],
	LPCWSTR					shaderName_,
	LPCWSTR					textureName_,
	XMFLOAT4				position_,
	XMFLOAT2				size_,
	std::vector<QuadrangleTexel>	texels,
	eScrollMode				scrollMode_)
{
	std::unique_ptr<CScroll> object = unique_ptr<CScroll>(new CScroll());
	object->init(pDevice_, pDeviceContext_, viewPort_, shaderName_, textureName_, position_, size_, texels, scrollMode_);
	return object;
}

/**
 * @fn void CScroll::init(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, LPCWSTR textureName_, XMFLOAT4 position_, XMFLOAT2 size_, std::vector<QuadrangleTexel> texels)
 * @brief インスタンスを初期化
 * @param pDevice_			デバイス
 * @param pDeviceContext_	デバイスコンテキスト
 * @param viewPort_[]		ビューポート
 * @param shaderName_		シェーダープログラム
 * @param textureName	PNGファイル_
 * @param position_			ウィンドウの位置
 * @param size_				スプライトの大きさ
 * @param texels			テクセルの配列
 * @param scrollMode_		スクロールの方向（縦/横）
 * @return					無し
 */
void CScroll::init(
	ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[],
	LPCWSTR shaderName_,
	LPCWSTR textureName_,
	XMFLOAT4 position_,
	XMFLOAT2 size_,
	std::vector<QuadrangleTexel> texels,
	eScrollMode scrollMode_)
{
	m_scrollMode = scrollMode_;
	if (m_scrollMode == eHorizontal)
	{
		m_posLeftTop = { size_.x / 2 ,  -0 };
		m_posRightBottom = { -size_.x / 2 , 240 };
	}
	else {
		m_posLeftTop = { 0.0 ,  size_.y / 2 };
		m_posRightBottom = { 0.0 , -size_.y / 2 };
	}

	/**
	 *スプライトの作成
	 */
	m_sprite1 = CPicture::create(pDevice_, pDeviceContext_, viewPort_, shaderName_, textureName_, position_, { size_.x, size_.y /*+ 2*/ }, texels);
	m_sprite2 = CPicture::create(pDevice_, pDeviceContext_, viewPort_, shaderName_, textureName_, position_, { size_.x, size_.y /*+ 2*/ }, texels);

	/**
	 *スプライトの位置を初期化
	 */
	m_sprite1->setPos({ 0.0,  m_posLeftTop.y, 0.0f, 0.0f });
	m_sprite2->setPos({ 0.0,  m_posRightBottom.y, 0.0f, 0.0f });
}

CScroll::CScroll()
{
}

CScroll::~CScroll()
{
}

/**
* @fn void CScroll::horizontalScroll(float value_)
* @brief スクロール
* @param value_	スクロール量
* @return		無し
*/
void CScroll::scroll(float value_)
{
	if (m_scrollMode == eHorizontal)
	{
		horizontalScroll(value_);
	}
	else {
		verticalScroll(value_);
	}
}

/**
* @fn void CScroll::horizontalScroll(float value_)
* @brief スクロール
* @param value_	スクロール量
* @return		無し
*/
void CScroll::horizontalScroll(float value_)
{
	/**
	* 指定された分だけスクロール
	*/
	m_posLeftTop.x += value_;
	m_posRightBottom.x += value_;

	/**
	* 画面の範囲外なら最初の位置にする
	*/
	m_posLeftTop.x = (
		m_posLeftTop.x < -m_sprite1->getSize().x + 2 ? m_sprite1->getSize().x : m_posLeftTop.x);
	m_posRightBottom.x = (m_posRightBottom.x < -m_sprite2->getSize().x + 2 ? m_sprite2->getSize().x : m_posRightBottom.x);

	/**
	* 位置を決定する
	*/
	m_sprite1->setPos({ m_posLeftTop.x, m_sprite1->getPos().y, m_sprite1->getPos().z, m_sprite1->getPos().w });
	m_sprite2->setPos({ m_posRightBottom.x, m_sprite1->getPos().y, m_sprite1->getPos().z, m_sprite1->getPos().w });
}

/**
 * @fn void CScroll::verticalScroll(float value_)
 * @brief スクロール
 * @param value_	スクロール量
 * @return		無し
 */
void CScroll::verticalScroll(float value_)
{
	/**
	 * 指定された分だけスクロール
	 */
	m_posLeftTop.y += value_;
	m_posRightBottom.y += value_;

	/**
	 * 画面の範囲外なら最初の位置にする
	 */
	m_posLeftTop.y = (m_posLeftTop.y < -m_sprite1->getSize ().y + 2 ? m_sprite1->getSize ().y : m_posLeftTop.y);
	m_posRightBottom.y = (m_posRightBottom.y < -m_sprite2->getSize ().y + 2 ? m_sprite2->getSize ().y : m_posRightBottom.y);

	/**
	 * 位置を決定する
	 */
	m_sprite1->setPos({ m_sprite1->getPos().x, m_posLeftTop.y, m_sprite1->getPos().z, m_sprite1->getPos().w });
	m_sprite2->setPos({ m_sprite1->getPos().x, m_posRightBottom.y, m_sprite1->getPos().z, m_sprite1->getPos().w });
}

/**
 * @fn void CScroll::render(XMFLOAT4X4 matView_, XMFLOAT4X4 matProj_)
 * @brief レンダリング
 * @param matView_	ビュー行列
 * @param matProj_	プロジェクション行列
 * @return     無し
 */
void CScroll::render(XMFLOAT4X4 matView_, XMFLOAT4X4 matProj_)
{
	m_sprite1->render(matView_, matProj_);
	m_sprite2->render(matView_, matProj_);
}

/**
 * @fn void CScroll::release()
 * @brief リリース
 * @param	無し
 * @return   無し
 */
//リリース
void CScroll::release()
{
	m_sprite1->release();
	m_sprite2->release();
}