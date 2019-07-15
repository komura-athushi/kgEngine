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

class CFont
{
public:
	CFont();
	~CFont();
	//�t�H���g�����[�h����
	void LoadFont(const wchar_t* path) 
	{
		m_spriteFont = m_fontfactory.Load(path);
	}
	/// <summary>
	/// �`�悷��
	/// </summary>
	/// <param name="text">����</param>
	/// <param name="pos">���W�A{0.0f,0.0f}�`{1.0f,1.0f}�̊�</param>
	/// <param name="color">�F</param>
	/// <param name="scale">�傫��</param>
	/// <param name="pivot">�s�{�b�g</param>
	/// <param name="rotation">��]</param>
	/// <param name="effects">�f�t�H���g��DirectX::SpriteEffects_None</param>
	/// <param name="layerDepth">�`�悷�鏇��</param>
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
	/// <summary>
	///�`��A���W���X�N���[�����W�Ŏw�� 
	/// </summary>
	/// <param name="text">����</param>
	/// <param name="pos">�X�N���[�����W</param>
	/// <param name="color">�F</param>
	/// <param name="scale">�傫��</param>
	/// <param name="pivot">�s�{�b�g</param>
	/// <param name="rotation">��]</param>
	/// <param name="effects">�f�t�H���g��DirectX::SpriteEffects_None</param>
	/// <param name="layerDepth">�`�悷�鏇��</param>
	void DrawScreenPos(
		wchar_t const* text,
		const CVector2& pos,
		const CVector4& color = CVector4::White(),
		const CVector2& scale = CVector2::One(),
		const CVector2& pivot = CVector2::Zero(),
		float rotation = 0.0f,
		DirectX::SpriteEffects effects = DirectX::SpriteEffects_None,
		float layerDepth = 0.5f
	);
private:
	DirectX::SpriteBatch* m_spriteBatch = nullptr;
	DirectX::SpriteFont* m_spriteFont = nullptr;
	CVector2 m_screenSize;

	static FontFactory m_fontfactory;

};

