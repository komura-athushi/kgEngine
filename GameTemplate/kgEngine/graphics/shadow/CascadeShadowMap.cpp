#include "KGstdafx.h"
#include "CascadeShadowMap.h"
#include <limits>


CascadeShadowMap::CascadeShadowMap()
{
	static float resTbl[SHADOWMAP_NUM][2] = {
		{4096, 4096},
		{2048, 2048},
		{1024, 1024},
	};

	/*static float resTbl[SHADOWMAP_NUM][2] = {
		{500, 500},
		{500, 500},
		{500, 500},
	};*/

	for (int i = 0; i < SHADOWMAP_NUM; i++) {
		m_shadowMapRT[0][i].Create(
			resTbl[i][0],
			resTbl[i][1],
			DXGI_FORMAT_R32_FLOAT
		);

		m_shadowMapRT[1][i].Create(
			resTbl[i][0],
			resTbl[i][1],
			DXGI_FORMAT_R32_FLOAT
		);
	}

	/*float clearColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_shadowMapRT.Create(
		FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		1,
		1,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);*/
}

void CascadeShadowMap::UpdateFromLightDirection(const CVector3& lightCameraPos, const CVector3& lightDir)
{
	//ライトの方向によって、ライトカメラの上方向を決める
	CVector3 lightCameraUpAxis;

	if (fabsf(lightDir.y) > 0.999998f) {
		//ほぼ真上or真下を向いているので、1,0,0を上方向とする
		lightCameraUpAxis = CVector3::AxisX();
	}
	else {
		//それ以外のときは、0,1,0を上方向とする
		lightCameraUpAxis = CVector3::AxisY();
	}
	m_lightViewMatrix.MakeLookAt(
		m_lightCameraPosition,
		m_lightCamerataraget,
		lightCameraUpAxis
	);

	m_lightProjMatrix.MakeOrthoProjectionMatrix(
		500,
		500,
		10.0f,
		1500.0f
	);
}

void CascadeShadowMap::UpdateFromLightTaraget(const CVector3& lightCameraPos,const CVector3& lightCameraTarget)
{
	m_lightCameraPosition = lightCameraPos;
	m_lightCamerataraget = lightCameraTarget;
	//ライトの方向を計算する
	auto lightDir = m_lightCamerataraget - m_lightCameraPosition;
	if (lightDir.Length() < 0.00001f) {
		//ライトカメラの座標と注視点が近すぎるのでクラッシュさせる
		std::abort();
	}

	//正規化して、方向ベクトルに変換する
	lightDir.Normalize();

	UpdateFromLightDirection(lightCameraPos, lightDir);
}

void CascadeShadowMap::Update()
{
	
	//ライトの方向によって、ライトカメラの上方向を決める
	CVector3 lightCameraUpAxis;

	if (fabsf(m_lightDir.y) > 0.999998f) {
		//ほぼ真上or真下を向いているので、1,0,0を上方向とする
		lightCameraUpAxis = CVector3::AxisX();
	}
	else {
		//それ以外のときは、0,1,0を上方向とする
		lightCameraUpAxis = CVector3::AxisY();
	}
	m_lightDir.Normalize();

	for (int i = 0; i < Engine().GetGraphicsEngine().GetSplitNumber(); i++) {
		UpdateLightViewProjMatrix(i);
	}
	
}

void CascadeShadowMap::UpdateLightViewProjMatrix(const int splitNumber)
{
	CMatrix lightViewRot;
	{
		//シーンをレンダリング使用としているカメラを使って、ライトカメラの回転を求める。
		auto cameraDir = MainCamera(splitNumber).GetFront();
		if (fabs(cameraDir.x) < FLT_EPSILON && fabsf(cameraDir.z) < FLT_EPSILON) {
			//ほぼ真上をむいている。
			return;
		}
		//ライトビュー行列の回転成分を計算する。
		CVector3 lightViewForward = m_lightDir;
		CVector3 lightViewUp;
		if (fabsf(lightViewForward.y) > 0.999f) {
			//ほぼ真上。
			lightViewUp.Cross(lightViewForward, CVector3::Right());
		}
		else {
			lightViewUp.Cross(lightViewForward, CVector3::Up());
		}
		lightViewUp.Normalize();
		CVector3 lgihtViewRight;
		lgihtViewRight.Cross(lightViewUp, lightViewForward);
		lgihtViewRight.Normalize();


		//ライトビューの横を設定する。
		lightViewRot.m[0][0] = lgihtViewRight.x;
		lightViewRot.m[0][1] = lgihtViewRight.y;
		lightViewRot.m[0][2] = lgihtViewRight.z;
		lightViewRot.m[0][3] = 0.0f;
		//ライトビューの上を設定する。
		lightViewRot.m[1][0] = lightViewUp.x;
		lightViewRot.m[1][1] = lightViewUp.y;
		lightViewRot.m[1][2] = lightViewUp.z;
		lightViewRot.m[1][3] = 0.0f;
		//ライトビューの前を設定する。
		lightViewRot.m[2][0] = lightViewForward.x;
		lightViewRot.m[2][1] = lightViewForward.y;
		lightViewRot.m[2][2] = lightViewForward.z;
		lightViewRot.m[2][3] = 0.0f;



	}

	CMatrix	inverseViewMatrix = MainCamera(splitNumber).GetViewMatrix();
	//CMatrix inverseViewMatrix = g_camera3D.GetViewMatrix2();
	inverseViewMatrix.Inverse(inverseViewMatrix);
	float nearClip = 1.0f;

	float shadowAreaTbl[] = {
		m_lightHeight * 0.4f,
		m_lightHeight * 1.2f,
		m_lightHeight * 2.4f,
		0.0f
	};
	float farClip = shadowAreaTbl[0];
	float FOVY = MainCamera(splitNumber).GetViewAngle();
	float FOVX = FOVY * MainCamera(splitNumber).GetAspect();


	//float lightHeight = g_camera3D.GetTarget().y + m_lightHeight;
	CVector3 centerPosTmp = { 0.0f, 0.0f, (nearClip + farClip) * 0.5f };
	for (int i = 0; i < SHADOWMAP_NUM; i++) {
		//視錘台の8つの頂点を求める
		//xとy成分の最大最小を求める
		float x1 = nearClip * tan(FOVX / 2);
		float x2 = farClip * tan(FOVX / 2);
		float y1 = nearClip * tan(FOVY / 2);
		float y2 = farClip * tan(FOVY / 2);

		//xとy成分から近平面の4つの頂点を求める
		CVector3 pos[8];
		pos[0] = CVector3(x1, y1, nearClip);
		pos[1] = CVector3(-x1, y1, nearClip);
		pos[2] = CVector3(x1, -y1, nearClip);
		pos[3] = CVector3(-x1, -y1, nearClip);

		//遠平面の4つの頂点を求める
		pos[4] = CVector3(x2, y2, farClip);
		pos[5] = CVector3(-x2, y2, farClip);
		pos[6] = CVector3(x2, -y2, farClip);
		pos[7] = CVector3(-x2, -y2, farClip);



		CVector3 posSum = CVector3::Zero();
		for (int i = 0; i < 8; i++) {
			//カメラの逆ビュー行列をかけて、カメラビュー座標をワールド座標に変換する
			inverseViewMatrix.Mul(pos[i]);
			posSum.Add(pos[i]);
		}

		inverseViewMatrix.Mul(centerPosTmp);
		//中央の座標を求めていく
		CVector3 centerPos = posSum / 8;

		//視推台の中央の座標とライトの高さを元にライトの座標を決めていく
		//ライトのY座標は4000.0にする。
		CVector3 topPos = centerPos;
		topPos.x = 0.0f;
		topPos.y = 2000.0f - centerPos.y;
		topPos.z = 0.0f;
		float s = m_lightDir.Dot(topPos);
		CVector3 lightPos = centerPos + m_lightDir * s;
		CMatrix lightViewMatrix = lightViewRot;
		//ライトの座標を代入して、ライトビュー行列完成
		lightViewMatrix.m[3][0] = lightPos.x;
		lightViewMatrix.m[3][1] = lightPos.y;
		lightViewMatrix.m[3][2] = lightPos.z;
		lightViewMatrix.m[3][3] = 1.0f;
		lightViewMatrix.Inverse(lightViewMatrix);	//ライトビュー完成。

		CVector3 vectorMin = CVector3(FLT_MAX, FLT_MAX, FLT_MAX);
		CVector3 vectorMax = CVector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		for (int i = 0; i < 8; i++) {
			//ライトビュー行列をかけて、ワールド座標をライトカメラビュー座標に変換する
			lightViewMatrix.Mul(pos[i]);
			//最大最小を求めていく
			vectorMin.x = min(vectorMin.x, pos[i].x);
			vectorMax.x = max(vectorMax.x, pos[i].x);
			vectorMin.y = min(vectorMin.y, pos[i].y);
			vectorMax.y = max(vectorMax.y, pos[i].y);
			vectorMin.z = min(vectorMin.z, pos[i].z);
			vectorMax.z = max(vectorMax.z, pos[i].z);
		}

		float w = vectorMax.x - vectorMin.x;
		float h = vectorMax.y - vectorMin.y;

		//プロジェクション行列をつくっていくぅ
		CMatrix projMatrix;
		projMatrix.MakeOrthoProjectionMatrix(
			w,
			h,
			1.0f,
			vectorMax.z
		);

		//ライトビュープロジェクション行列を求めていくぅ～
		m_lightVieProjMatrix[splitNumber][i].Mul(lightViewMatrix, projMatrix);

		m_farList[splitNumber][i] = farClip;

		m_lightViewMatrixInv[splitNumber][i].Inverse(lightViewMatrix);

		nearClip = farClip;
		farClip = shadowAreaTbl[i + 1];
	}
}

void CascadeShadowMap::RenderToShadowMap()
{
	auto d3dDeviceContext = Engine().GetGraphicsEngine().GetD3DDeviceContext();
	for (int i = 0; i < Engine().GetGraphicsEngine().GetSplitNumber(); i++) {
		for (int j = 0; j < SHADOWMAP_NUM; j++) {
			m_shadowMapNumber = j;
			ID3D11RenderTargetView* rt[] = {
				m_shadowMapRT[i][j].GetRenderTargetView()
			};
			d3dDeviceContext->OMSetRenderTargets(1, rt, m_shadowMapRT[i][j].GetDepthStensilView());
			//ビューポートを設定。
			d3dDeviceContext->RSSetViewports(1, m_shadowMapRT[i][j].GetViewport());
			//シャドウマップをクリア。
		//一番奥のZは1.0なので、1.0で塗りつぶす。
			float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
			m_shadowMapRT[i][j].ClearRenderTarget(clearColor);
			if (m_shadowCaters.size() >= 1) {
				//シャドウキャスターをシャドウマップにレンダリング。
				for (auto& caster : m_shadowCaters) {
					caster->Draw(
						&MainCamera(i),
						enRenderMode_CreateCascadeShadowMap,
						true,
						i
					);
				}
			}
		}
	}

	m_shadowCaters.clear();
}