#include "KGstdafx.h"
#include "NormalMap.h"


NormalMap::NormalMap()
{
	//�V���h�E�}�b�v�����p�̃����_�����O�^�[�Q�b�g���쐬�B
//�𑜓x��2048�~2048�B
//�e�N�X�`���̃t�H�[�}�b�g��R�����݂̂�32bit���������_�^�B
	m_normalMapRT.Create(
		FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		DXGI_FORMAT_R16G16B16A16_FLOAT
	);
}

NormalMap::~NormalMap()
{

}

void NormalMap::RenderNormalMap(Camera* camera, SkinModel* skinModel)
{

	auto d3dDeviceContext = Engine().GetGraphicsEngine().GetD3DDeviceContext();
	//�����_�����O�^�[�Q�b�g��؂�ւ���B
	ID3D11RenderTargetView* rts[] = {
		m_normalMapRT.GetRenderTargetView()
	};
	d3dDeviceContext->OMSetRenderTargets(1, rts, m_normalMapRT.GetDepthStensilView());
	//�r���[�|�[�g��ݒ�B
	d3dDeviceContext->RSSetViewports(1, m_normalMapRT.GetViewport());
	//�V���h�E�}�b�v���N���A�B
	//��ԉ���Z��1.0�Ȃ̂ŁA1.0�œh��Ԃ��B
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
	m_normalMapRT.ClearRenderTarget(clearColor);

	if (skinModel != nullptr) {
		skinModel->Draw(
			camera->GetViewMatrix(),
			camera->GetProjectionMatrix(),
			enRenderMode_NormalMap
		);
	}
	else if (m_modelList.size() >= 1) {
		//�V���h�E�L���X�^�[���V���h�E�}�b�v�Ƀ����_�����O�B
		for (auto& caster : m_modelList) {
			caster->Draw(
				MainCamera().GetViewMatrix(),
				MainCamera().GetProjectionMatrix(),
				enRenderMode_NormalMap
			);
		}
	}
	//�L���X�^�[���N���A�B
	m_modelList.clear();
}