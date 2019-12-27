#pragma once

class DepthValueMap
{
public:
	DepthValueMap();
	~DepthValueMap();
	//�V���h�E�L���X�^�[��ǉ�
	void RegistSkinModel(SkinModel* model)
	{
		m_modelList.push_back(model);
	}
	//�V���h�E�}�b�v��SRV���擾
	ID3D11ShaderResourceView* GetDepthValueMapSRV()
	{
		return m_depthValueMapRT.GetRenderTargetSRV();
	}
	void RenderDepthValueMap();

private:
	RenderTarget m_depthValueMapRT;								//�@����`�悷�郌���_�����O�^�[�Q�b�g
	std::vector<SkinModel*> m_modelList;				//���f���̔z��
};

