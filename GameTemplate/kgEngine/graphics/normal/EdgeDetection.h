#pragma once

#include "graphics/RenderTarget.h"

//�G�b�W���o���΂����肵�Ă������`
class EdgeDetection
{
public:
	EdgeDetection();
	~EdgeDetection();

private:
	RenderTarget m_edgeMapRT;								//�@����`�悷�郌���_�����O�^�[�Q�b�g
};

