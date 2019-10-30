/**
 * @file CCamera.cpp
 * @brief カメラ
 * @author 織戸　喜隆
 * @date 日付（2017.02.14）
 */

#include "stdafx.h"
#include "CCamera.h"


CCamera::CCamera ()
{
}


CCamera::~CCamera ()
{
}

/**
 * @fn void CCamera::init(XMFLOAT3 vEyePt_, XMFLOAT3 vLookatPt_, XMFLOAT3 vUpVec_, FLOAT	angle_, FLOAT	aspect_, FLOAT	near_, FLOAT	far_)
 * @brief 初期化
 * @param vEyePt_		カメラ（視点）位置
 * @param vLookatPt_	注視位置
 * @param vUpVec_		上方位置
 * @param angle_		視野角
 * @param aspect_		アスペクト比
 * @param near_			前方クリップ
 * @param far_			後方クリップ
 * @return				無し
 */
void CCamera::init(XMFLOAT3 vEyePt_, XMFLOAT3 vLookatPt_, XMFLOAT3 vUpVec_, FLOAT	angle_, FLOAT	aspect_, FLOAT	near_, FLOAT	far_)
{
	m_vEyePt	= vEyePt_;		//!< カメラ（視点）位置
	m_vLookatPt	= vLookatPt_;	//!< 注視位置
	m_vUpVec	= vUpVec_;		//!< 上方位置
	m_angle		= angle_;		//!< 視野角
	m_aspect	= aspect_;		//!< アスペクト比
	m_near		= near_;		//!< 前方クリップ
	m_far		= far_;			//!< 後方クリップ
	XMStoreFloat4x4(&m_matView, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matProj, XMMatrixIdentity());
	update();
}

/**
 * @fn void CCamera::update()
 * @brief ビュートランスフォーム、プロジェクショントランスフォームの更新
 * @param	無し
 * @param	無し
 * @return	無し
 */
void CCamera::update()
{
	/**
	 * ビュートランスフォーム（視点座標変換）
	 */
	XMStoreFloat4x4(&m_matView, XMMatrixLookAtLH(
		XMLoadFloat3(&m_vEyePt),	//!< カメラ（視点）位置
		XMLoadFloat3(&m_vLookatPt),	//!< 注視位置
		XMLoadFloat3(&m_vUpVec)));	//!< 上方位置
	/**
	 * プロジェクショントランスフォーム（射影変換）
	 */
	XMStoreFloat4x4(&m_matProj, XMMatrixPerspectiveFovLH(
		m_angle,	//!< 視野角
		m_aspect,	//!< アスペクト比
		m_near,		//!< 前方クリップ
		m_far));	//!< 後方クリップ
}
