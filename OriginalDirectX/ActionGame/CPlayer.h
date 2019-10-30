#pragma once

#include "stdafx.h"
#include "CMapChip.h"
#include "MediaResource.h"

typedef enum eGravityDirection {
	eGDown = 0,
	eGLeft = 1,
	eGRight = 2,
	eGUp = 3,
	eGNone
}eGravityDirection;


class CPlayer : public CMapChip {
	typedef CMapChip supre;

	PartiallyCol m_partCol						= {};		// 部分的な衝突判定
	FLOAT m_jumpPower							= 0.0f;		// ジャンプの初速
	const FLOAT kGravityScaleMax				= 3.0f;		// 重力の最大値
	BOOL m_isGround								= FALSE;	// 地面に立っているか
	eGravityDirection m_gravityDirection		= eGNone;	// 重力が働く方向
	int m_rattlingFrame							= 0;		// 地面に落ちた時震えるフレーム数
	int m_shakeSign								= 1;		// 揺らすときの符号
	vector<unique_ptr<CPicture>> m_debugRect;				// デバッグ用判定可視化
public:
	static std::unique_ptr<CPlayer> create(
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
	// 重力演算
	void gravityProc();
	// 当たり判定を自分の位置に合わせる
	void recomposeColPos()
	{
		m_partCol.foot.pos = { getPos().x, getPos().y - getSize().y / 2.0f,
			0.0f, 0.0f };
		m_partCol.head.pos = { getPos().x, getPos().y + getSize().y / 2.0f,
			0.0f, 0.0f };
		m_partCol.right.pos = { getPos().x + getSize().x / 2.0f, getPos().y,
			0.0f,0.0f };
		m_partCol.left.pos = { getPos().x - getSize().x / 2.0f, getPos().y,
			0.0f,0.0f };
	}
	// ジャンプ
	void jump();
	// 横移動(重力が上下のどちらかの時)
	void moveHorizone(int x_);
	// 縦移動(重力が左右のどちらかの時)
	void moveVertical(int y_);
	// 引数で渡された値で地面に立っているか判定し、オフセットする
	void checkAndOffset(XMFLOAT4 check_);
	// 呼ばれている間揺らす
	void shake();
	// 震えるフレーム数をカウントする
	void CountUpRattlingFrame() {
		++m_rattlingFrame;
		if (m_rattlingFrame > 90)	++m_rattlingFrame;
		if (m_rattlingFrame > 150)	++m_rattlingFrame;
	}
	int getRattlingFrame()const { return m_rattlingFrame; }
	PartiallyCol getPartCol()const { return m_partCol; }
	void setGround(BOOL isGrd_) { m_isGround = isGrd_; }
	BOOL getGround()const { return m_isGround; }
	void setGravityDirection(eGravityDirection dir_) { m_gravityDirection = dir_;	m_animIndex = dir_; }
	eGravityDirection getGravityDirection() { return m_gravityDirection; }
	int getAnimIndex()const { return m_animIndex; }
	void debugRectRender(XMFLOAT4X4 matView_, XMFLOAT4X4 matProj_, shared_ptr<CTexture> texture_)
	{
		vector<unique_ptr<CPicture>>::iterator itr = m_debugRect.begin();
		for (int i = 0; i < (int)m_debugRect.size(); ++i)
		{
			switch (i)
			{
			case 0:
				itr[i]->setPos(m_partCol.foot.pos);
				break;
			case 1:
				itr[i]->setPos(m_partCol.right.pos);
				break;
			case 2:
				itr[i]->setPos(m_partCol.left.pos);
				break;
			case 3:
				itr[i]->setPos(m_partCol.head.pos);
				break;
			default:
				break;
			}
			itr[i]->render(matView_, matProj_, texture_);
		}
	}
};
