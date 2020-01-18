#pragma once
#include "graphics/RenderTarget.h"
#include "OffScreenCamera.h"
#include "Object/ObjectData.h"

class Fade;
class Collection : public IGameObject
{
public:
	Collection();
	~Collection();
	bool Start() override;
	void Draw() override;
private:
	//サンプルステート初期化
	void InitSamplerState();
	//モデル描画していく
	void OffScreenRender();
private:
	const static int W_NUMBER = 4;						//横幅
	const static int H_NUMBER = 4;						//縦幅
	RenderTarget m_offRenderTarget;						//オフスクリーンのレンダーターゲット
	OffScreenCamera m_offScreenCamera;					//オフスクリーン用のカメラ
	CVector3 m_position = CVector3::Zero();				//モデルの座標
	CVector3 m_scale = CVector3::One();					//大きさ
	CQuaternion m_rot = CQuaternion::Identity();		//回転
	Shader m_vs;										//何もしないシェーダー
	Shader m_ps;										//何もしないシェーダー
	ID3D11SamplerState* m_samplerState = nullptr;		//サンプラステート。
	PostEffect m_postEffect[W_NUMBER * H_NUMBER];		//ポストエフェクト、1ページのモデル分
	ID3D11RasterizerState* m_rasterizerState = NULL;	//ラスタライザステート。
	std::map<float, SkinModel_ObjData*> m_modelList;	//モデルのマップ
	float m_degree = 0.0f;								//モデルの回転の角度
	int m_page = 1;										//何ページ目か
	int m_listSize = 0;									//モデルの数
	CSprite m_sprite;									//画像
	CSprite m_haikei;									//画像
	CSprite m_collectionCursor;							//カーソル
	int m_cursorNumber = 1;								//カーソルが何番目か
	PostEffect m_mainPostEffect;						//左の大きいやつ用のポストエフェクト
	int m_maximumPage = 1;								//ページの最大数
	int m_mimmumPage = 1;								//ページの最小数
	int m_finalPageNumber = 0;							//最後のページ
	CFont m_font;										//文字
	int m_hitNumber = 0;								//巻き込んだものの数
	SkinModel m_model;									//左端用のモデル
	bool m_isWaitFadeout = false;
	Fade* m_fade = nullptr;				
};

