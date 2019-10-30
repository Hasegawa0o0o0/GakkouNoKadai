/**
 * @file CDotEatApp.cpp
 * @brief DirectX11をベースにしたアプリケーションクラス
 * @author 織戸　喜隆
 * @date 日付（2017.02.14）
 */

#include "stdafx.h"
#include "CDotEatApp.h"

CDotEatApp::CDotEatApp()
{
}

CDotEatApp::~CDotEatApp()
{
}

/**
 * @fn void CDotEatApp::initCameraInfo()
 * @brief カメラの設定
 * @param	無し
 * @return	無し
 */
void CDotEatApp::initCameraInfo()
{
	const FLOAT x = 32.0f * 5.5;
	const FLOAT y = 32.0f * 4;
	m_vEyePt	= XMFLOAT3{ x, y,  -400.0f };		//!< カメラ（視点）位置
	m_vLookatPt = XMFLOAT3{ x, y,     0.0f };		//!< 注視位置
	m_vUpVec	= XMFLOAT3{ 0.0f, 1.0f,     0.0f };	//!< 上方位置
	m_angle		= (FLOAT)XM_PI / 4;					//!< 視野角
	m_aspect	= ASPECT;							//!< アスペクト比
	m_near		= 0.1f;								//!< 前方クリップ
	m_far		= 1000.0f;							//!< 後方クリップ
}

/**
 * @fn bool CDotEatApp::createSprites(void)
 * @brief スプライトの生成
 * @param	無し
 * @return	成功・失敗
 */
bool CDotEatApp::createSprites(void)
{
	super::createSprites(); //!< スーパークラスのメソッドを実行
	try {
		vector <QuadrangleTexel>  iTexel1_1 = {
			QuadrangleTexel{ { 0, 1.0f },{ 0, 0.0f },{ 1, 1.0f },{ 1, 0.0f } }
		};
		m_pOpening		= CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, Tex_Opening, { 180.0f, 112.0f, 110.0f, 0.0f }, { WINDOW_WIDTH , WINDOW_HEIGHT }, iTexel1_1);
		m_pClear		= CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, Tex_Clear, { 200.0f, 134.0f, 100.0f, 0.0f }, { WINDOW_WIDTH , WINDOW_HEIGHT }, iTexel1_1);
		m_pOver			= CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, Tex_Over, { 226.0f, 106.0f, 110.0f, 0.0f }, { WINDOW_WIDTH , WINDOW_HEIGHT }, iTexel1_1);
		m_pBackground	= CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, Tex_Background, { 180.0f, 112.0f, 110.0f, 0.0f }, { WINDOW_WIDTH , WINDOW_HEIGHT }, iTexel1_1);
		
		createMap();		 //!< Mapを生成
		return true;
	}
	catch (LPCWSTR str) {
		MessageBox(0, str, NULL, MB_OK);
		return false;
	}
}

/**
 * @fn void CDotEatApp::createMap()
 * @brief ドットイートのマップを作製
 * @param	無し
 * @return	無し
 */
void CDotEatApp::createMap()
{
	/**
	 * ゲームの初期化
	 */
	//m_pMapManager = unique_ptr<CMapManager>(new CMapManager());
	//m_pMapManager->init(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL);
	m_pMapManager = CMapManager::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL);
}

/**
 * @fn void CDotEatApp::doOpenning()
 * @brief オープニング
 * @param	無し
 * @return	無し
 */
void CDotEatApp::doOpenning()
{
	if (m_pDirectInput->isKeyPressed(DIK_X)) {
		m_gameStatus = ePlaying;
	}
}

/**
 * @fn void CDotEatApp::doPlaying()
 * @brief プレー中
 * @param	無し
 * @return	無し
 */
void CDotEatApp::doPlaying()
{
	try
	{
		m_frameCounter++;
		m_frameCounter = m_frameCounter > 65536 ? 0 : m_frameCounter;

		eDirection dir = eNone;
		if (m_pDirectInput->isPressedOnce(DIK_LEFT))	dir = eLeft;
		if (m_pDirectInput->isPressedOnce(DIK_RIGHT))	dir = eRight;
		if (m_pDirectInput->isPressedOnce(DIK_UP))		dir = eDown;
		if (m_pDirectInput->isPressedOnce(DIK_DOWN))	dir = eUp;

		//m_pMapManager->update(m_frameCounter, 0, FALSE);

	}// end of try
	catch (eGameStatus status_)
	{
		if (status_ == eGameClear)
		{
			m_gameStatus = eGameClear;
			//MessageBox(0, L"ゲームクリアです。", NULL, MB_OK);
		}
		else if (status_ == eGameOver)
		{
			m_gameStatus = eGameOver;
			//MessageBox(0, L"ゲームオーバーです。", NULL, MB_OK);
		}
	}//end of catch
}

/**
 * @fn void CDotEatApp::doGameClear()
 * @brief ゲームクリア
 * @param	無し
 * @return	無し
 */

void CDotEatApp::doGameClear()
{
	if (m_pDirectInput->isKeyPressed(DIK_X)) {
		m_gameStatus = ePlaying;
		createMap();
	}
}

/**
 * @fn void CDotEatApp::doGameOver()
 * @brief ゲームオーバー
 * @param	無し
 * @return	無し
 */
void CDotEatApp::doGameOver()
{
	if (m_pDirectInput->isKeyPressed(DIK_X)) {
		m_gameStatus = ePlaying;
		createMap();
	}
}

/**
 * @fn void CDotEatApp::doEnding()
 * @brief エンディング
 * @param	無し
 * @return	無し
 */
void CDotEatApp::doEnding()
{
}

/**
 * @fn void CDotEatApp::renderSprite(XMFLOAT4X4 matView_, XMFLOAT4X4 matProj_)
 * @brief レンダー
 * @param	matView_	ビュー行列
 * @param	matProj_	プロジェクション行列
 * @return				無し
 */
void CDotEatApp::render(XMFLOAT4X4 matView_, XMFLOAT4X4 matProj_)
{
	//switch (m_gameStatus)
	//{
	//case eOpenning:
	//	m_pOpening->render(matView_, matProj_/*,
	//		[&](ID3D11DeviceContext* pDeviceContext_) -> void {
	//		m_pOpeningTex->setup(pDeviceContext_);
	//	}*/);
	//	break;
	//case ePlaying:
	//	m_pMapManager->render(matView_, matProj_);
	//	m_pBackground->render(matView_, matProj_);
	//	break;
	//case eGameClear:
	//	m_pClear->render(matView_, matProj_/*,
	//		[&](ID3D11DeviceContext* pDeviceContext_) -> void {
	//		m_pClearTex->setup(pDeviceContext_);
	//	}*/);
	//	break;
	//case eGameOver:
	//	m_pOver->render(matView_, matProj_/*,
	//		[&](ID3D11DeviceContext* pDeviceContext_) -> void {
	//		m_pOverTex->setup(pDeviceContext_);
	//	}*/);
	//	break;
	//case eEnding:
	//	break;
	//default:
	//	break;
	//}

	/**
	 * デバッグ情報
	 */
	//debugString(m_hWnd, 10, 10, L"スペースシップⅡ");
	OutputDebugString(L"スペースシップⅡ\n");
}

/**
 * @fn void CDotEatApp::release()
 * @brief 解放
 * @param	無し
 * @return	無し
 */
void CDotEatApp::release()
{
	m_pOver->release();
	m_pClear->release();
	m_pOpening->release();
	m_pMapManager->release();
	super::release();			//!< スーパークラスのメソッドを実行
}