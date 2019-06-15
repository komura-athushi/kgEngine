#pragma once
#include "graphics/Shader.h"
//スプライト
class CSprite {
public:
	//デフォルトのUV座標
	static const CVector2 DEFAULT_PIVOT;
	//コンストラクタ
	CSprite();
	//デストラクタ
	~CSprite();
	/*!
	* @brief	初期化。
	*@param[in]	filePath	ファイルパス
	*@param[in]	w		幅。
	*@param[in]	h		高さ。
	*/
private:
	struct SSpriteCB {
		CMatrix WVP;				//ワールドビュープロジェクション行列
		CVector4 mulColor;			//乗算カラー
	};
		CVector3 _position = CVector3::Zero();
		CQuaternion m_rotation = CQuaternion::Identity();
		CVector3 m_scale = CVector3::One();
		CMatrix m_world = CMatrix::Identity();
		Shader* m_ps;
		Shader* m_vs;
		CVector4 m_mulcolor = CVector4::White();


};