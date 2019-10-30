/**
* @file CSingleSprite.cpp
* @brief 弾丸・クラス
* @author 織戸　喜隆
* @date 日付（2018.07.23）
*/
#include "stdafx.h"
#include "CSingleSprite.h"

//!< 【クラス変数】
ID3D11SamplerState*			CSingleSprite::g_pSampleLinear	= NULL;			//!< 【クラス変数】テクスチャーのサンプラー
ID3D11ShaderResourceView*	CSingleSprite::g_pTexture		= NULL;			//!< 【クラス変数】テクスチャー

/**
* @fn std::unique_ptr<CSingleSprite> CSingleSprite::create(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, LPCWSTR textureName_, XMFLOAT4 position_, XMFLOAT2 size_, std::vector<UVCoord> uvArray_)
* @brief インスタンスを生成し初期化 【クラスメソッド】
* @param pDevice_			デバイス
* @param pDeviceContext_	デバイスコンテキスト
* @param viewPort_[]		ビューポート
* @param shaderName_			シェーダープログラム
* @param textureName_		画像ファイル
* @param position_			ウィンドウの位置
* @param size_				スプライトの大きさ
* @param uvArray_			爆発のUV配列
* @return					CSingleSpriteのユニークポインタ
*/
std::unique_ptr<CSingleSprite> CSingleSprite::create(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, LPCWSTR textureName_, XMFLOAT4 position_, XMFLOAT2 size_, std::vector<UVCoord>	uvArray_)
{
	std::unique_ptr<CSingleSprite> object = unique_ptr<CSingleSprite>(new CSingleSprite());
	object->init(pDevice_, pDeviceContext_, viewPort_, shaderName_, textureName_, position_, size_, uvArray_);
	return object;
}


CSingleSprite::CSingleSprite()
{
}


CSingleSprite::~CSingleSprite()
{
}

/**
* @fn void CSingleSprite::init(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, LPCWSTR textureName_, XMFLOAT4 position_, XMFLOAT2 size_, std::vector<UVCoord> uvArray_)
* @brief インスタンスを初期化
* @param pDevice_			デバイス
* @param pDeviceContext_	デバイスコンテキスト
* @param viewPort_[]		ビューポート
* @param shaderName_			シェーダープログラム
* @param textureName_		画像ファイル
* @param position_			ウィンドウの位置
* @param size_				スプライトの大きさ
* @param uvArray_			爆発のUV配列
* @return					無し
*/
void CSingleSprite::init(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, LPCWSTR textureName_, XMFLOAT4 position_, XMFLOAT2 size_, std::vector<UVCoord>	uvArray_)
{
	super::init(pDevice_, pDeviceContext_, viewPort_, shaderName_, position_, size_, uvArray_);
	/**
	* テクスチャー作成(サンプラーを含む) DirectXTex <WindowsSDK 10>
	*/
	createSamplerAndTexture(pDevice_, textureName_);
}

/**
* @fn void CSingleSprite::createTexture (LPCWSTR textureName_)
* @brief テクスチャをファイルから生成
* @param pDevice_			デバイス
* @param textureName_		画像ファイル
* @return					無し
*/
void CSingleSprite::createSamplerAndTexture(ID3D11Device*pDevice_, LPCWSTR textureName_)
{

	if ((textureName_ == NULL) || (g_pTexture))	//!< 【クラス変数】
	{
		return;
	}

	/**
	* テクスチャー用サンプラー作成
	*/
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC; //!< D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 2;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;

	pDevice_->CreateSamplerState(&samplerDesc, &g_pSampleLinear);
	/**
	* 【クラス変数】
	* テクスチャー作成 DirectXTex <WindowsSDK 10>
	*/
	if (FAILED(CreateWICTextureFromFile(pDevice_, textureName_, NULL, &g_pTexture)))
	{
		//!< 【クラス変数】 テクスチャファイル名を付加する
		TCHAR   str[256];
		wsprintf(str, L"CSingleSprite::init <CreateWICTextureFromFile>テクスチャー作成失敗 %s", textureName_);
		throw str;
	}
}

/**
* @fn void CSingleSprite::render(XMFLOAT4X4	matView_, XMFLOAT4X4 matProj_, size_t uvIndex_, XMFLOAT4 diffuse_)
* @brief レンダリング
* @param matView_	ビュー行列
* @param matProj_	プロジェクション行列
* @param uvIndex_	テクスチャ配列の位置
* @param diffuse_	ディフューズ
* @return			無し
*/
void CSingleSprite::render(XMFLOAT4X4	matView_, XMFLOAT4X4 matProj_, size_t uvIndex_, XMFLOAT4 diffuse_)
{
	if (!m_active)
	{
		return;
	}
	if (m_damaged)
	{
		m_fadeOutFrames--;
	}
	if (m_fadeOutFrames <= 0)
	{
		m_active = FALSE;
		return;
	}
	computeMatrixFromPos();

	//! シェーダー　定数バッファ
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(m_pDeviceContext->Map(
		m_pConstantBuffer,			//!< マップするリソース
		0, 							//!< サブリソースのインデックス番号
		D3D11_MAP_WRITE_DISCARD,	//!< 書き込みアクセス
		0,
		&pData)))					//!< データの書き込み先ポインタ
	{
		ShaderGlobal_0 sg;
		XMStoreFloat4x4(&sg.matWVP, XMLoadFloat4x4(&m_mFinalWorld) * XMLoadFloat4x4(&matView_) * XMLoadFloat4x4(&matProj_));
		XMStoreFloat4x4(&sg.matWVP, XMMatrixTranspose(XMLoadFloat4x4(&sg.matWVP)));
		sg.diffuse = diffuse_;
		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(ShaderGlobal_0));

		m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
	}
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);									//!< 頂点インプットレイアウトをセット
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	//!< プリミティブ・トポロジーをセット
	UINT stride = sizeof(VertexWithTex<XMFLOAT3>);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVBArray[uvIndex_], &stride, &offset);//!< バーテックスバッファーをセット
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);							//!< 使用するシェーダーの登録　（effectの”テクニック”に相当）
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);					//!< VSに定数バッファを設定
	m_pDeviceContext->RSSetViewports(1, m_viewPort);									//!< RSにビューポートを設定
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);								//!< PSにピクセル・シェーダを設定
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);					//!< PSに定数バッファを設定
																						//! サンプラーとテクスチャーの設定
	setSamplerAndTexture();
	m_pDeviceContext->Draw(4, 0);														//!<  プリミティブをレンダリング
}

/**
* @fn BOOL CSingleSprite::setSamplerAndTexture()
* @brief		テクスチャをクラス変数として共有する
* @param		無し
* @return		無し
*/
void CSingleSprite::setSamplerAndTexture()
{
	m_pDeviceContext->PSSetSamplers(0, 1, &g_pSampleLinear);							//!< 【クラス変数】PSにサンプラーを設定
	m_pDeviceContext->PSSetShaderResources(												//!< 【クラス変数】テクスチャーをシェーダーに渡す
		0, //!< 設定する最初のスロット番号
		1, //!< 設定するシェーダ・リソース・ビューの数
		&g_pTexture);	//!< 設定するシェーダ・リソース・ビューの配列
}

/**
* @fn void CSingleSprite::release(void)
* @brief リリース
* @param	無し
* @return	無し
*/
void CSingleSprite::release(void)
{
	/**
	*  必ず生成したのと逆の順番でリリース
	*/
	SAFE_RELEASE(g_pTexture);
	SAFE_RELEASE(g_pSampleLinear);
	super::release();
}
