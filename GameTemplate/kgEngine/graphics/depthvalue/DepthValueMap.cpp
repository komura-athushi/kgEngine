#include "KGstdafx.h"
#include "DepthValueMap.h"


DepthValueMap::DepthValueMap()
{
	//�V���h�E�}�b�v�����p�̃����_�����O�^�[�Q�b�g���쐬�B
//�𑜓x��2048�~2048�B
//�e�N�X�`���̃t�H�[�}�b�g��R�����݂̂�32bit���������_�^�B
	m_depthValueMapRT.Create(
		FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		DXGI_FORMAT_R32_FLOAT
	);
}

DepthValueMap::~DepthValueMap()
{

}

void  DepthValueMap::RenderDepthValueMap()
{

	auto d3dDeviceContext = Engine().GetGraphicsEngine().GetD3DDeviceContext();
	//�����_�����O�^�[�Q�b�g��؂�ւ���B
	ID3D11RenderTargetView* rts[] = {
		m_depthValueMapRT.GetRenderTargetView()
	};
	d3dDeviceContext->OMSetRenderTargets(1, rts, m_depthValueMapRT.GetDepthStensilView());
	//�r���[�|�[�g��ݒ�B
	d3dDeviceContext->RSSetViewports(1, m_depthValueMapRT.GetViewport());
	//�V���h�E�}�b�v���N���A�B
	//��ԉ���Z��1.0�Ȃ̂ŁA1.0�œh��Ԃ��B
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
	m_depthValueMapRT.ClearRenderTarget(clearColor);

	if (m_modelList.size() >= 1) {
		//�V���h�E�L���X�^�[���V���h�E�}�b�v�Ƀ����_�����O�B
		for (auto& caster : m_modelList) {
			caster->Draw(
				MainCamera().GetViewMatrix(),
				MainCamera().GetProjectionMatrix(),
				enRenderMode_DepthValueMap
			);
		}
	}
	//�L���X�^�[���N���A�B
	m_modelList.clear();
}