#pragma once

#include "graphics/RenderTarget.h"

class NormalMap
{
public:
	NormalMap();
	~NormalMap();
	//�V���h�E�L���X�^�[��ǉ�
	void RegistSkinModel(SkinModel* model)
	{
		m_modelList.push_back(model);
	}
	//�V���h�E�}�b�v��SRV���擾
	ID3D11ShaderResourceView* GetNormalMapSRV()
	{
		return m_normalMapRT.GetRenderTargetSRV();
	}
	void RenderNormalMap();
private:
	RenderTarget m_normalMapRT;								//�@����`�悷�郌���_�����O�^�[�Q�b�g
	std::vector<SkinModel*> m_modelList;				//���f���̔z��

};

