/**
 * @file CExplosion.cpp
 * @brief 爆発
 * @author 織戸　喜隆
 * @date 日付（2017.02.14）
 */

#include "stdafx.h"
#include "CExplosion.h"

/**
 * @fn std::unique_ptr<CExplosion> CExplosion::create(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, XMFLOAT4 position_, XMFLOAT2 size_, std::vector<QuadrangleTexel> texels)
 * @brief インスタンスを生成し初期化 【クラスメソッド】
 * @param pDevice_			デバイス
 * @param pDeviceContext_	デバイスコンテキスト
 * @param viewPort_[]		ビューポート
 * @param shaderName_		シェーダープログラム
 * @param position_			ウィンドウの位置
 * @param size_				スプライトの大きさ
 * @param bulletTexelIndex_	弾丸のテクセルのインデックス（プレーヤーと共有）
 * @return					CPlayerのユニークポインタ
 */
std::shared_ptr<CExplosion> CExplosion::create(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, XMFLOAT4 position_, XMFLOAT2 size_, std::vector<QuadrangleTexel> texels, int framesOfWait_)
{
	std::shared_ptr<CExplosion> object = unique_ptr<CExplosion>(new CExplosion());
	object->init(pDevice_, pDeviceContext_, viewPort_, shaderName_, position_, size_, texels, framesOfWait_);
	return object;
}

/**
 * @fn void CExplosion::init(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, XMFLOAT4 position_, XMFLOAT2 size_, std::vector<QuadrangleTexel> texels)
 * @brief インスタンスを初期化
 * @param pDevice_			デバイス
 * @param pDeviceContext_	デバイスコンテキスト
 * @param viewPort_[]		ビューポート
 * @param shaderName_			シェーダープログラム
 * @param position_			ウィンドウの位置
 * @param size_				スプライトの大きさ
 * @param bulletTexelIndex_	弾丸のテクセルのインデックス（プレーヤーと共有）
 * @return					無し
 */

void CExplosion::init(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, XMFLOAT4 position_, XMFLOAT2 size_, std::vector<QuadrangleTexel> texels, int framesOfWait_)
{
	super::init(pDevice_, pDeviceContext_, viewPort_, shaderName_, position_, size_, texels); //!< スーパークラスのメソッドを実行
	m_framesOfWait	= framesOfWait_;
	reset();
}

CExplosion::CExplosion()
{
}

CExplosion::~CExplosion()
{
}

/**
 * @fn void CExplosion::render(XMFLOAT4X4	matView_, XMFLOAT4X4 matProj_)
 * @brief レンダリング
 * @param matView_	ビュー行列
 * @param matProj_	プロジェクション行列
 * @param texelIndex_	弾丸の順序
 * @return			無し
 */
void CExplosion::render(XMFLOAT4X4	matView_, XMFLOAT4X4 matProj_, std::function<void(ID3D11DeviceContext* pDeviceContext_)> callbackTextureProc_, size_t texelIndex_)
{
	if (isInvalid() || m_started == FALSE)
	{
		return;
	}

	if (m_TexelIndex < m_pVertexBuffer.size())
	{
		super::render(matView_, matProj_, callbackTextureProc_, m_TexelIndex); //!< スーパークラスのメソッドを実行
		m_waitCounter++;
		if (m_waitCounter > m_framesOfWait)
		{
			m_waitCounter = 0;
			m_TexelIndex++;
		}
	} else {
		m_completed = TRUE;
	}
}

/**
 * @fn void CExplosion::reset()
 * @brief 弾丸を初期状態にする
 * @param	無し
 * @return	無し
 */
void CExplosion::reset()
{
	m_started	= FALSE;
	m_TexelIndex		= 0;
	m_waitCounter	= 0;
}