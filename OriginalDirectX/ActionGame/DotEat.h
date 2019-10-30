/**
* @file DotEat.h
* @brief ドットイートで使用する型
* @author 織戸　喜隆
* @date 日付（2017.02.14）
*/
#pragma once

#include "stdafx.h"
#include "Common3DType.h"

/*! @struct eDirection
@brief  方向 <列挙>
*/
typedef enum eDirection {
	eLeft,
	eRight,
	eUp,
	eDown,
	eNone
}eDirection;

/*! @struct FontInfo
@brief  フォント <位置>
*/
typedef struct FontInfo{
	char	ch;
	QuadrangleTexel	coord;
}FontInfo;

/*! @struct eChipType
@brief  マップチップ <列挙>
*/
typedef enum eChipType
{
	eEmpty,
	eBlock,
	eDot,
	ePlayer,
	eGoal,
	eEnemy
}eChipType;

/*! @struct Size
@brief  位置・サイズ <2D>
*/
typedef struct MapIndex {
	INT32 x;	//!< x　/　横
	INT32 y;	//!< y　/　縦
	
	BOOL operator== (MapIndex index_)
	{
		return (x == index_.x) && (y == index_.y);
	}

	BOOL operator!= (MapIndex index_)
	{
		return (x != index_.x) || (y != index_.y);
	}
}Size;