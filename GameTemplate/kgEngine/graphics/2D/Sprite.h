#pragma once
#include "graphics/Shader.h"
//�X�v���C�g
class CSprite {
public:
	//�R���X�g���N�^
	CSprite();
	//�f�X�g���N�^
	~CSprite();
	//������
	void Init(const wchar_t* fileName, bool isCircleGauge  = false);
	//������
	void Init(ID3D11ShaderResourceView* srv);

	/// <summary>
	/// �X�N���[�����W�w��ŕ`��
	/// </summary>
	/// <param name="pos">���W�A(0.0f,0.0f)�ō���A�f�t�H���g�͒���</param>
	/// <param name="scale">�傫���A(1.0f,1.0f)��1�{</param>
	/// <param name="pivot">���j�e�B��̃s�{�b�g</param>
	/// <param name="rotation">��]</param>
	/// <param name="color">�����x(z)</param>
	/// <param name="effects">�f�t�H���g��DirectX::SpriteEffects_None</param>
	/// <param name="layerDepth">�`�悷�鏇��(���͋@�\���Ă܂���B�B�B)</param>
	/// <param name="degree">�s�N�Z����\������p�x</param>
	/// <param name="isIgnoreSplit">��ʕ����������ĉ摜�`��</param>
	void DrawScreenPos(const CVector2& pos = { 1280.0f / 2, 720.0f / 2},
		const CVector2& scale = CVector2::One(),
		const CVector2& pivot = CVector2(0.5f,0.5f),
		float rotation = 0.0f,
		const CVector4 & color = CVector4::White(),
		DirectX::SpriteEffects effects = DirectX::SpriteEffects_None,
		float layerDepth = 0.5f,
		float degree = 0.0f,
		bool isIgnoreSplit = false
	);
	void Draw();
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
	ID3D11Buffer* m_dg = nullptr;							//�萔�o�b�t�@(�p�x)
	Shader m_ps;
	float m_degree = 0.0f;
	bool m_isCircleGauge = false;							//�~�`�Q�[�W�ɂ��邩�ǂ���
	RECT m_sourceRectangle;									//�摜�T�C�Y
	bool m_isNormal = false;
};