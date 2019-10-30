/**
 * @file CEnemy.cpp
 * @brief 敵
 * @author 織戸　喜隆
 * @date 日付（2017.02.14）
 */

#include "stdafx.h"
#include "CEnemy.h"

/**
 * @fn std::unique_ptr<CEnemy> CEnemy::create(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, XMFLOAT4 position_, XMFLOAT2 size_, std::vector<QuadrangleTexel> texels)
 * @brief インスタンスを生成し初期化 【クラスメソッド】
 * @param pDevice_			デバイス
 * @param pDeviceContext_	デバイスコンテキスト
 * @param viewPort_[]		ビューポート
 * @param shaderName_		シェーダープログラム
 * @param position_			ウィンドウの位置
 * @param size_				スプライトの大きさ
 * @param bulletTexelIndex_	弾丸のテクセルのインデックス（プレーヤーと共有）
 * @return					CEnemyのユニークポインタ
 */
std::shared_ptr<CEnemy> CEnemy::create(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, XMFLOAT4 position_, XMFLOAT2 size_, int bulletTexelIndex_)
{
	std::shared_ptr<CEnemy> object = shared_ptr<CEnemy>(new CEnemy());
	object->init(pDevice_, pDeviceContext_, viewPort_, shaderName_, position_, size_, bulletTexelIndex_);
	return object;
}
/**
 * @fn void CEnemy::init(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, XMFLOAT4 position_, XMFLOAT2 size_, std::vector<QuadrangleTexel> texels)
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
void CEnemy::init(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, XMFLOAT4 position_, XMFLOAT2 size_, int bulletTexelIndex_)
{
	super::init(pDevice_, pDeviceContext_, viewPort_, shaderName_, position_, size_, kTexelEnemys, bulletTexelIndex_); //!< スーパークラスのメソッドを実行
	m_moveQuantity = -0.528f * 1.75f;
	enumBullets(
		[=](std::shared_ptr<CQuadPolygon> bullet_, size_t index_) -> BOOL {
		bullet_->setMoveQuantity(-2.5);
		return TRUE; //!< 列挙を続行
	});
}

CEnemy::CEnemy()
{
}

CEnemy::~CEnemy()
{
}

/**
 * @fn BOOL CEnemy::stepAnimation (BOOL enable_)
 * @brief アニメーション
 * @param enable_	アニメーションの有効性
 * @return			継続・中止
 */
void CEnemy::stepAnimation (BOOL enable_)
{
	super::stepAnimation(enable_, kTexelEnemys); //!< スーパークラスのメソッドを実行
}

/**
 * @fn void CEnemy::render(XMFLOAT4X4	matView_, XMFLOAT4X4 matProj_)
 * @brief レンダリング
 * @param matView_	ビュー行列
 * @param matProj_	プロジェクション行列
 * @return			無し
 */
void CEnemy::render(XMFLOAT4X4	matView_, XMFLOAT4X4 matProj_, std::function<void(ID3D11DeviceContext* pDeviceContext_)> callbackTextureProc_)
{
	if (!outOfWindow())
	{
		super::render(matView_, matProj_, callbackTextureProc_, m_animIndex); //!< スーパークラスのメソッドを実行
	}
}

/**
 * @fn void CEnemy::shot(void)
 * @brief 弾丸を発射
 * @param	無し
 * @return	無し
 */
void CEnemy::shot(void)
{
	std::vector<FLOAT> courses = { -20.0f, 0.0f, 20.0f };
	super::shot(courses); //!< スーパークラスのメソッドを実行
}