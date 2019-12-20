#pragma once

#include "Skeleton.h"
#include "ToonMap.h"
const int NUM_DIRECTION_LIG = 4;
/*!
*@brief	ディレクションライト。
*/
struct SDirectionLight {
	CVector4 direction[NUM_DIRECTION_LIG];		//ライトの方向。
	//CVector4 direction;
	CVector4 lightcolor[NUM_DIRECTION_LIG];			//ライトのカラー。
	CVector4 ambientlight = CVector4(0.3f, 0.3f, 0.3f, 1.0f);
	CVector3 eyePos;				//カメラの視点。
	float specPow = 0.3f;			//スペキュラライトの絞り。
	CVector3 m_eyeDir;
	int isToomShader = 1;
	CVector4 color = CVector4::White();
};

/*!
*@brief	FBXの上方向。
*/
enum EnFbxUpAxis {
	enFbxUpAxisY,		//Y-up
	enFbxUpAxisZ,		//Z-up
};
/*!
*@brief	スキンモデルクラス。
*/
class SkinModel
{
public:
	//メッシュが見つかったときのコールバック関数。
	using OnFindMesh = std::function<void(const std::unique_ptr<DirectX::ModelMeshPart>&)>;
	SkinModel();
	/*!
	*@brief	デストラクタ。
	*/
	~SkinModel();
	
	/*!
	*@brief	初期化。
	*@param[in]	filePath		ロードするcmoファイルのファイルパス。
	*@param[in] enFbxUpAxis		fbxの上軸。デフォルトはenFbxUpAxisZ。
	*/
	void Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis = enFbxUpAxisZ);
	//インスタンシング描画のデータの数を設定
	void SetInstancingNumber(const int& numInstance)
	{
		m_maxInstance = numInstance;
	}
	//インスタンシング描画のデータの初期化
	void InitInstancingData();
	/*!
	*@brief	モデルをワールド座標系に変換するためのワールド行列を更新する。
	*@param[in]	position	モデルの座標。
	*@param[in]	rotation	モデルの回転。
	*@param[in]	scale		モデルの拡大率。
	*/
	void UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale);
	/*!
	*@brief	ボーンを検索。
	*@param[in]		boneName	ボーンの名前。
	*@return	見つかったボーン。見つからなかった場合はnullptrを返します。
	*/
	Bone* FindBone(const wchar_t* boneName)
	{
		int boneId = m_skeleton.FindBoneID(boneName);
		return m_skeleton.GetBone(boneId);
	}
	/*!
	*@brief	モデルを描画。
	*/
	void Draw(CMatrix viewMatrix, CMatrix projMatrix, EnRenderMode renderMode = enRenderMode_Normal);
	//座標、回転、拡大を指定してモデルをドロー
	//void Draw(const CVector3& m_position, const CQuaternion& rot, const CVector3& scale, CMatrix viewMatrix, CMatrix projMatrix, EnRenderMode renderMode = enRenderMode_Normal);
	/*!
	*@brief	スケルトンの取得。
	*/
	Skeleton& GetSkeleton()
	{
		return m_skeleton;
	}
	/*!
	*@brief	メッシュを検索する。
	*@param[in] onFindMesh		メッシュが見つかったときのコールバック関数
	*/
	void FindMesh(OnFindMesh onFindMesh) const
	{
		for (auto& modelMeshs : m_modelDx->meshes) {
			for (std::unique_ptr<DirectX::ModelMeshPart>& mesh : modelMeshs->meshParts) {
				onFindMesh(mesh);
			}
		}
	}
	//シャドウキャスターを設定
	void SetShadowCaster(bool caster)
	{
		m_isShadowCaster = caster;
	}
	//シャドウキャスターかどうか
	bool isShadowCaster() const
	{
		return m_isShadowCaster;
	}
	//シャドウレシーバーを設定
	void SetShadowReceiver(bool receiver)
	{
		m_isShadowReceiver = receiver;
	}
	//ワールド行列を取得
	CMatrix GetWorldMatrix()  const
	{
		return m_worldMatrix;
	}
	//ワールド行列を設定
	void SetWorldMatrix(const CMatrix& worldmatirx)
	{
		m_worldMatrix = worldmatirx;
	}
	//インスタンシング描画の数を取得
	int GetInstanceNumber()
	{
		return m_numInstance;
	}
	//インスタンシング描画をするかどうかを取得
	bool GetisInstancing()
	{
		return m_isInstancing;
	}
	/*!
	*@brief	SRVのレジスタ番号。
	*/
	enum EnSkinModelSRVReg {
		enSkinModelSRVReg_DiffuseTexture = 0,		//!<ディフューズテクスチャ。
		enSkinModelSRVReg_BoneMatrix,				//!<ボーン行列。
	};
	bool IsFbxZUp() const
	{
		return m_enFbxUpAxis == enFbxUpAxisZ;
	}
	/// <summary>
	/// インスタンシング用データの更新
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡大</param>
	/// <param name="enUpdateAxis">fbxの上方向</param>
	void UpdateInstancingData(
		const CVector3& pos,
		const CQuaternion& rot,
		const CVector3& scale,
		EnFbxUpAxis enUpdateAxis = enFbxUpAxisZ
	);
	//インスタンシング用データの更新、行列を直接設定する用
	void UpdateInstancingData(const CMatrix& worldMatrix);
	//インスタンスの数を初期化する、インスタンシングデータの更新前に呼び出す
	void BeginUpdateInstancingData()
	{
		if (m_maxInstance >= 1) {
			m_numInstance = 0;
		}
	}
	//インスタンシングデータの更新が終わったら呼び出す、インスタンスの数を確定する
	void EndUpdateInstancingData()
	{

	}
	/*!
	*@brief	サンプラステートの初期化。
	*/
	void InitSamplerState();
	/*!
	*@brief	定数バッファの作成。
	*/
	void InitConstantBuffer();
	/*!
	*@brief	スケルトンの初期化。
	*@param[in]	filePath		ロードするcmoファイルのファイルパス。
	*/
	void InitSkeleton(const wchar_t* filePath);
	//トゥーンシェーダーをオフ
	void SetOffToonShader()
	{
		m_isToonShader = false;
	}
	//色を設定
	void SetColor(const CVector4& color)
	{
		m_dirLight.color = color;
	}
private:
	//定数バッファ。
	struct SVSConstantBuffer {
		CMatrix mWorld;
		CMatrix mView;
		CMatrix mProj;
		CMatrix mLightView;		//todo ライトビュー行列。
		CMatrix mLightProj;		//todo ライトプロジェクション行列。
		int isShadowReciever;	//todo シャドウレシーバーのフラグ。
	};
	EnFbxUpAxis			m_enFbxUpAxis = enFbxUpAxisZ;	//!<FBXの上方向。
	ID3D11Buffer*		m_cb = nullptr;					//!<定数バッファ。
	Skeleton			m_skeleton;						//!<スケルトン。
	CMatrix				m_worldMatrix;					//!<ワールド行列。
	DirectX::Model*		m_modelDx;						//!<DirectXTKが提供するモデルクラス。
	ID3D11SamplerState* m_samplerState = nullptr;		//!<サンプラステート。
	ID3D11Buffer* m_lightCb = nullptr;	//!<ライト用の定数バッファ。
	ID3D11Buffer* m_isuvscroll = nullptr;
	ID3D11Buffer* m_uvscroll = nullptr;
	SDirectionLight	m_dirLight;				//!<ディレクションライト。
	bool m_isShadowCaster = true;
	bool m_isShadowReceiver = true;
	std::unique_ptr<CMatrix[]>	m_instancingData;		//!<インスタンシング描画用のデータ。
	ID3D11Buffer* m_instancingDataSB = nullptr;					//!<インスタンシング描画用の定数バッファ
	ID3D11ShaderResourceView* m_srv = nullptr;
	int m_maxInstance = 0;
	int m_numInstance = 0;
	bool m_isInstancing = false;
	int m_isUVscroll = true;
	float m_UVscroll = 0.0f;
	ToonMap* m_toonMap = nullptr;
	bool m_isToonShader = true;
};

