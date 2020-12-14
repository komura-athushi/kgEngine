/*****************************************************************//**
 * \file   kgFont.h
 * \brief  CFont�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once

//Sprite��Factory
class FontFactory {
public:
	DirectX::SpriteFont* Load(const wchar_t* path);
private:
	//unique_ptr  �X�}�[�g�|�C���^�Anew�������������w���|�C���^�����݂��Ȃ���Ύ����I��
	//delete�����
	std::unordered_map<int, std::unique_ptr<DirectX::SpriteFont>> m_fontmap;
};

/**
 * \brief �t�H���g��`��.
 */
class CFont
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	CFont();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~CFont();
	/**
	 * \brief �`�悷��.
	 * 
	 * \param text ����
	 * \param pos ���W�A{0.0f,0.0f}�`{1.0f,1.0f}�̊�
	 * \param color �F
	 * \param scale �傫��
	 * \param pivot �s�{�b�g
	 * \param rotation ��]
	 * \param effects �f�t�H���g��DirectX::SpriteEffects_None
	 * \param layerDepth �`�悷�鏇��(���͋@�\���Ă܂���B�B�B)
	 */
	void Draw(
		wchar_t const* text,
		const CVector2& pos,
		const CVector4& color = CVector4::White(),
		const CVector2& scale = CVector2::One(),
		const CVector2& pivot = CVector2::Zero(),
		float rotation = 0.0f,
		DirectX::SpriteEffects effects = DirectX::SpriteEffects_None,
		float layerDepth = 0.5f
	);
	/**
	 * \brief �`��A���W���X�N���[�����W�Ŏw�� .
	 * 
	 * \param text ����
	 * \param pos �X�N���[�����W
	 * \param color �F
	 * \param scale �傫��
	 * \param pivot �s�{�b�g
	 * \param rotation ��]
	 * \param effects �f�t�H���g��DirectX::SpriteEffects_None
	 * \param layerDepth �`�悷�鏇��
	 * \param isIgnoreSplit ��ʕ����������ĉ摜�`��
	 */
	void DrawScreenPos(
		wchar_t const* text,
		const CVector2& pos,
		const CVector4& color = CVector4::White(),
		const CVector2& scale = CVector2::One(),
		const CVector2& pivot = CVector2::Zero(),
		float rotation = 0.0f,
		DirectX::SpriteEffects effects = DirectX::SpriteEffects_None,
		float layerDepth = 0.5f,
		bool isIgnoreSplit = false
	);
private:
	DirectX::SpriteBatch* m_spriteBatch = nullptr;
	DirectX::SpriteFont* m_spriteFont = nullptr;
	CVector2 m_screenSize;
	Shader m_ps;
	static FontFactory m_fontfactory;
	ID3D11Buffer* m_cb = nullptr;							//!<�萔�o�b�t�@�B
};

