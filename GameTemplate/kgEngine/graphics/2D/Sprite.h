#pragma once
#include "graphics/Shader.h"
//�X�v���C�g
class CSprite {
public:
	//�f�t�H���g��UV���W
	static const CVector2 DEFAULT_PIVOT;
	//�R���X�g���N�^
	CSprite();
	//�f�X�g���N�^
	~CSprite();
	//������
	void Init(const wchar_t* fileName);
	//�X�N���[�����W�w��ŉ摜��`��
	void DrawScreenPos(const CVector2& pos,
		const CVector2& scale = CVector2::One(),
		const CVector2& pivot = CVector2::Zero(),
		float rotation = 0.0f,
		const CVector4 & color = CVector4::White(),
		DirectX::SpriteEffects effects = DirectX::SpriteEffects_None,
		float layerDepth = 0.5f);
private:
	struct ConstantBuffer {
		CVector4 mulColor;
	};
	CVector2 m_screenSize = CVector2::Zero();
	DirectX::SpriteBatch* m_spriteBatch = nullptr;
	ID3D11ShaderResourceView* m_srv = nullptr;
	ID3D11Resource* m_tex = nullptr;
	unsigned int m_width, m_height;
	ID3D11Buffer* m_cb = nullptr;							//!<�萔�o�b�t�@�B
	Shader m_ps;
	Shader m_vs;
	CVector4 m_mulColor = CVector4::Blue();				//��Z�J���[
};