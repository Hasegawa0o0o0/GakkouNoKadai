/**
* @file CSpaceship.h
* @brief スペースシップ（抽象クラス）
* @author 織戸　喜隆
* @date 日付（2017.02.14）
*/
#pragma once

#include "stdafx.h"
#include "CExplosion.h"
#include "CTexture.h"
#include "MediaResource.h"

class CSpaceship :
	public CQuadPolygon
{
	typedef CQuadPolygon super;
protected:
	std::shared_ptr<CQuadPolygon>	m_pBullets[32] = { NULL,NULL };
	int								m_bulletTexelIndex = 0;
	std::shared_ptr<CExplosion>		m_pExplosion = NULL;
	int								m_hp = kMaxHP;
	std::shared_ptr<CTexture>		m_pBulletTex = NULL;	//!< テクセル
	std::shared_ptr<CTexture>		m_pExplosionTex = NULL;	//!< テクセル
public:
	CSpaceship();
	~CSpaceship();
	size_t countEmptyBullets ()
	{
		int ret_ = 0;
		enumBullets(	
			[&](std::shared_ptr<CQuadPolygon> bullet_, size_t index_) -> BOOL {
			if (bullet_->getState() == eWaiting)
			{
				ret_++;
			}
			return TRUE; //!< 列挙を続行
		});
		return ret_;
	}
	virtual XMFLOAT4 computeBulletPos(size_t index_) {
		return{ 0, 0, 0, 0 };	// 【重要】必ずサブクラスでオーバーライドすること
	}
	virtual void init(
		ID3D11Device*			pDevice_,			// デバイス
		ID3D11DeviceContext*	pDeviceContext_,	// デバイスコンテキスト
		D3D11_VIEWPORT			viewPort_[],		// ビューポート
		LPCWSTR					shaderName_,		// シェーダープログラム
		XMFLOAT4				position_,			// ウィンドウの位置
		XMFLOAT2				size_,				// スプライトの大きさ
		std::vector<QuadrangleTexel>	texels,			// スペースシップのテクセルの配列
		int						bulletTexelIndex_);	// 弾丸のテクセルのインデックス（プレーヤーと共有）
	void release();
	virtual void render(XMFLOAT4X4	matView_, XMFLOAT4X4 matProj_, std::function<void(ID3D11DeviceContext* pDeviceContext_)> callbackTextureProc_, size_t texelIndex_ = 0, XMFLOAT4 v4Diffuse_ = { 0, 0, 0, 0 });
	virtual void shot(void) = 0;
	void shot(std::vector<FLOAT> directions_);
	void shotOpt(BOOL ready_) {
		if (ready_)
			shot();
	};
	BOOL collisionBullet(XMFLOAT4 pos_, FLOAT range_);
	void explode()						{ m_pExplosion->explode(); };
	void resetAllBullets();
	void enumBullets(std::function<BOOL(std::shared_ptr<CQuadPolygon>, size_t)> callbackEnumeProc_);
	void stepAnimation(BOOL enable_, std::vector <QuadrangleTexel> animations_)
	{
		if (enable_)
		{
			m_animIndex++;
			m_animIndex = m_animIndex >= animations_.size() ? 0 : m_animIndex;
		}
	}
	BOOL receiveDamage() {
		m_hp--;
		if (m_hp <= 0)
		{
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
};

