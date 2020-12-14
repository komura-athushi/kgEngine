#include "KGstdafx.h"
#include "CascadeShadowMap.h"
#include <limits>


CascadeShadowMap::CascadeShadowMap()
{
	static float resTbl[SHADOWMAP_NUM][2] = {
		{2048, 2048},
		{1024, 1024},
		{512,   512}
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

void CascadeShadowMap::Update()
{
	
	//���C�g�̕����ɂ���āA���C�g�J�����̏���������߂�
	CVector3 lightCameraUpAxis;

	if (fabsf(m_lightDir.y) > 0.999998f) {
		//�قڐ^��or�^���������Ă���̂ŁA1,0,0��������Ƃ���
		lightCameraUpAxis = CVector3::AxisX();
	}
	else {
		//����ȊO�̂Ƃ��́A0,1,0��������Ƃ���
		lightCameraUpAxis = CVector3::AxisY();
	}
	m_lightDir.Normalize();

	for (int i = 0; i < Engine().GetGraphicsEngine().GetSplitNumber(); i++) {
		UpdateLightViewProjMatrix(i);
	}
	
}

void CascadeShadowMap::UpdateLightViewProjMatrix(const int splitNumber)
{
	CMatrix	inverseViewMatrix = MainCamera(splitNumber).GetViewMatrix();
	//CMatrix inverseViewMatrix = g_camera3D.GetViewMatrix2();
	inverseViewMatrix.Inverse(inverseViewMatrix);
	float nearClip = 1.0f;

	float shadowAreaTbl[] = {
		m_lightHeight * 0.4f,
		m_lightHeight * 0.8f,
		m_lightHeight * 1.5f,
		0.0f
	};
	float farClip = shadowAreaTbl[0];
	float FOVY = MainCamera(splitNumber).GetViewAngle();
	float FOVX = FOVY * MainCamera(splitNumber).GetAspect();


	//float lightHeight = g_camera3D.GetTarget().y + m_lightHeight;
	CVector3 centerPosTmp = { 0.0f, 0.0f, (nearClip + farClip) * 0.5f };
	for (int i = 0; i < SHADOWMAP_NUM; i++) {
		//�������8�̒��_�����߂�
		//x��y�����̍ő�ŏ������߂�
		float x1 = nearClip * tan(FOVX / 2);
		float x2 = farClip * tan(FOVX / 2);
		float y1 = nearClip * tan(FOVY / 2);
		float y2 = farClip * tan(FOVY / 2);

		//x��y��������ߕ��ʂ�4�̒��_�����߂�
		CVector3 pos[8];
		pos[0] = CVector3(x1, y1, nearClip);
		pos[1] = CVector3(-x1, y1, nearClip);
		pos[2] = CVector3(x1, -y1, nearClip);
		pos[3] = CVector3(-x1, -y1, nearClip);

		//�����ʂ�4�̒��_�����߂�
		pos[4] = CVector3(x2, y2, farClip);
		pos[5] = CVector3(-x2, y2, farClip);
		pos[6] = CVector3(x2, -y2, farClip);
		pos[7] = CVector3(-x2, -y2, farClip);



		CVector3 posSum = CVector3::Zero();
		for (int i = 0; i < 8; i++) {
			//�J�����̋t�r���[�s��������āA�J�����r���[���W�����[���h���W�ɕϊ�����
			inverseViewMatrix.Mul(pos[i]);
			posSum.Add(pos[i]);
			if (pos[i].y >= MainCamera(splitNumber).GetPosition().y + 500.0f) {
				pos[i].y = MainCamera(splitNumber).GetPosition().y + 500.0f;
			}
			else if (pos[i].y <= MainCamera(splitNumber).GetPosition().y - 500.0f) {
				pos[i].y = MainCamera(splitNumber).GetPosition().y - 500.0f;
			}
		}

		inverseViewMatrix.Mul(centerPosTmp);
		//�����̍��W�����߂Ă���
		CVector3 centerPos = posSum / 8;

		//������̒����̍��W�ƃ��C�g�̍��������Ƀ��C�g�̍��W�����߂Ă���
		//���C�g��Y���W��4000.0�ɂ���B
		CVector3 topPos = centerPos;
		topPos.x = 0.0f;
		topPos.y = MainCamera(splitNumber).GetPosition().y + 1000.0f - centerPos.y;
		topPos.z = 0.0f;
		float s = m_lightDir.Dot(topPos);
		CVector3 lightPos = centerPos + m_lightDir * s;

		//������̒��S�ƃ��C�g�̍��W�Ń��C�g�r���[�s������
		CMatrix lightViewMatrix;
		lightViewMatrix.MakeLookAt(
			lightPos,
			centerPos,
			CVector3::AxisY()
		);

		CVector3 vectorMin = CVector3(FLT_MAX, FLT_MAX, FLT_MAX);
		CVector3 vectorMax = CVector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		for (int i = 0; i < 8; i++) {
			//���C�g�r���[�s��������āA���[���h���W�����C�g�J�����r���[���W�ɕϊ�����
			lightViewMatrix.Mul(pos[i]);
			//�ő�ŏ������߂Ă���
			vectorMin.x = min(vectorMin.x, pos[i].x);
			vectorMax.x = max(vectorMax.x, pos[i].x);
			vectorMin.y = min(vectorMin.y, pos[i].y);
			vectorMax.y = max(vectorMax.y, pos[i].y);
			vectorMin.z = min(vectorMin.z, pos[i].z);
			vectorMax.z = max(vectorMax.z, pos[i].z);
		}

		float w = vectorMax.x - vectorMin.x;
		float h = vectorMax.y - vectorMin.y;

		//�v���W�F�N�V�����s��������Ă�����
		CMatrix projMatrix;
		projMatrix.MakeOrthoProjectionMatrix(
			w,
			h,
			1.0f,
			vectorMax.z
		);

		//���C�g�r���[�v���W�F�N�V�����s������߂Ă������`
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
			//�r���[�|�[�g��ݒ�B
			d3dDeviceContext->RSSetViewports(1, m_shadowMapRT[i][j].GetViewport());
			//�V���h�E�}�b�v���N���A�B
			//��ԉ���Z��1.0�Ȃ̂ŁA1.0�œh��Ԃ��B
			float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
			m_shadowMapRT[i][j].ClearRenderTarget(clearColor);
			if (m_shadowCaters.size() >= 1) {
				//�V���h�E�L���X�^�[���V���h�E�}�b�v�Ƀ����_�����O�B
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