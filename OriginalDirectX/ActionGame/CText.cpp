/**
* @file CText.cpp
* @brief テキスト表示のためのクラス
* @author 長谷川　勇太
* @date 日付（2018.11.19）
*/
#include "stdafx.h"
#include "CText.h"

CText::CText() {}
CText::~CText() {}

std::shared_ptr<CText> CText::create(
	ID3D11Device*			pDevice_,			//!< デバイス
	ID3D11DeviceContext*	pDeviceContext_,	//!< デバイスコンテキスト
	D3D11_VIEWPORT          viewPort_[],		//!< ビューポート
	LPCWSTR					shaderName_,		//!< シェーダープログラム
	LPCWSTR					textureName_,			//!< PNGファイル
	XMFLOAT4				pos_,				// 初めの位置
	XMFLOAT2				size_,				//!< スプライトの大きさ
	std::vector<QuadrangleTexel>	texels,		//!< テクセルの配列
	char					str_[])				// 初期テキスト
{
	shared_ptr<CText> object = shared_ptr<CText>(new CText());
	object->init(pDevice_, pDeviceContext_, viewPort_, shaderName_, textureName_, pos_, size_, texels, str_);
	return object;
}

void CText::init(
	ID3D11Device*			pDevice_,			//!< デバイス
	ID3D11DeviceContext*	pDeviceContext_,	//!< デバイスコンテキスト
	D3D11_VIEWPORT          viewPort_[],		//!< ビューポート
	LPCWSTR					shaderName_,		//!< シェーダープログラム
	LPCWSTR					textureName_,			//!< PNGファイル
	XMFLOAT4				pos_,				// 初めの位置
	XMFLOAT2				size_,				//!< スプライトの大きさ
	std::vector<QuadrangleTexel>	texels,		//!< テクセルの配列
	char					str_[])				// 初期テキスト
{
	setText(str_);
	XMFLOAT4 pos = pos_;
	for (int i = 0; i < (int)m_vTextChr.size(); ++i)
	{
		m_vText.push_back(CPicture::create(pDevice_, pDeviceContext_, viewPort_, shaderName_, textureName_, pos, size_, texels));
		pos.x += size_.x;
	}
}

void CText::setText(const char str[])
{
	if ((int)m_vTextChr.size() != 0) { return; }
	for (int i = 0; str[i] != '\0'; ++i)
	{
		m_vTextChr.push_back(str[i]);
	}
}

void CText::setText(const int n)
{
	if ((int)m_vTextChr.size() != 0) { return; }
	string s = to_string(n);
	const char* const str = s.data();
	for (int i = 0; *(str + i) != '\0'; ++i)
	{
		m_vTextChr.push_back(*(str + i));
	}
}

string CText::getText()
{
	string combinedText = "";
	for (int i = 0; i < (int)m_vTextChr.size(); ++i)
	{
		combinedText += m_vTextChr[i];
	}
	return combinedText;
}

void CText::resetTest()
{
	m_vTextChr.clear();
}

void CText::clearText()
{
	release();
	m_vTextChr.clear();
	m_vText.clear();
}

void CText::setBeginPos(XMFLOAT4 pos_)
{
	XMFLOAT4 pos = pos_;
	for (int i = 0; i < (int)m_vText.size(); ++i)
	{
		m_vText[i]->setPos(pos);
		pos.x += m_vText[i]->getSize().x;
	}
}

XMFLOAT4 CText::getBeginPos()
{
	return (*(m_vText.begin()))->getPos();
}

void CText::render(XMFLOAT4X4	matView_, XMFLOAT4X4 matProj_, vector<shared_ptr<CTexture>> vTexture_)
{
	if (m_vTextChr.size() > m_vText.size()) { return; }
	vector<int> charNum;
	conversionC2I(charNum, m_vTextChr);
	for (int i = 0; i < (int)m_vTextChr.size(); ++i)
	{
		m_vText[i]->render(matView_, matProj_, vTexture_[eFontTexture], charNum[i]);
	}
}

void CText::release()
{
	if (m_vText.size() == 0)	return;
	vector<shared_ptr<CPicture>>::iterator itr = m_vText.end() - 1;
	for (; itr != m_vText.begin(); --itr)
	{
		(*itr)->release();
	}
	(*itr)->release();
}

void CText::conversionC2I(vector<int>& vi_, vector<char> str_)
{
	for (int i = 0; i < (int)str_.size(); ++i)
	{
		switch (str_[i])
		{
		case '1':	vi_.push_back(0);	break;
		case '2':	vi_.push_back(1);	break;
		case '3':	vi_.push_back(2);	break;
		case '4':	vi_.push_back(3);	break;
		case '5':	vi_.push_back(4);	break;
		case '6':	vi_.push_back(5);	break;
		case '7':	vi_.push_back(6);	break;
		case '8':	vi_.push_back(7);	break;
		case '9':	vi_.push_back(8);	break;
		case '0':	vi_.push_back(9);	break;
		case 'A':	vi_.push_back(10);	break;
		case 'B':	vi_.push_back(11);	break;
		case 'C':	vi_.push_back(12);	break;
		case 'D':	vi_.push_back(13);	break;
		case 'E':	vi_.push_back(14);	break;
		case 'F':	vi_.push_back(15);	break;
		case 'G':	vi_.push_back(16);	break;
		case 'H':	vi_.push_back(17);	break;
		case 'I':	vi_.push_back(18);	break;
		case 'J':	vi_.push_back(19);	break;
		case 'K':	vi_.push_back(20);	break;
		case 'L':	vi_.push_back(21);	break;
		case 'M':	vi_.push_back(22);	break;
		case 'N':	vi_.push_back(23);	break;
		case 'O':	vi_.push_back(24);	break;
		case 'P':	vi_.push_back(25);	break;
		case 'Q':	vi_.push_back(26);	break;
		case 'R':	vi_.push_back(27);	break;
		case 'S':	vi_.push_back(28);	break;
		case 'T':	vi_.push_back(29);	break;
		case 'U':	vi_.push_back(30);	break;
		case 'V':	vi_.push_back(31);	break;
		case 'W':	vi_.push_back(32);	break;
		case 'X':	vi_.push_back(33);	break;
		case 'Y':	vi_.push_back(34);	break;
		case 'Z':	vi_.push_back(35);	break;
		case 'a':	vi_.push_back(36);	break;
		case 'b':	vi_.push_back(37);	break;
		case 'c':	vi_.push_back(38);	break;
		case 'd':	vi_.push_back(39);	break;
		case 'e':	vi_.push_back(40);	break;
		case 'f':	vi_.push_back(41);	break;
		case 'g':	vi_.push_back(42);	break;
		case 'h':	vi_.push_back(43);	break;
		case 'i':	vi_.push_back(44);	break;
		case 'j':	vi_.push_back(45);	break;
		case 'k':	vi_.push_back(46);	break;
		case 'l':	vi_.push_back(47);	break;
		case 'm':	vi_.push_back(48);	break;
		case 'n':	vi_.push_back(49);	break;
		case 'o':	vi_.push_back(50);	break;
		case 'p':	vi_.push_back(51);	break;
		case 'q':	vi_.push_back(52);	break;
		case 'r':	vi_.push_back(53);	break;
		case 's':	vi_.push_back(54);	break;
		case 't':	vi_.push_back(55);	break;
		case 'u':	vi_.push_back(56);	break;
		case 'v':	vi_.push_back(57);	break;
		case 'w':	vi_.push_back(58);	break;
		case 'x':	vi_.push_back(59);	break;
		case 'y':	vi_.push_back(60);	break;
		case 'z':	vi_.push_back(61);	break;
		case ' ':	vi_.push_back(62);	break;
		default:	break;
		}
	}
}
