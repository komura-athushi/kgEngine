#pragma once
//�R�s�[���֎~����N���X�Ɍp��������A�\���̂��p���ł���A�f�t�H���g�ł�public�p���炵��
struct Noncopyable{
	Noncopyable() = default;
	Noncopyable(const Noncopyable&) = delete;
	Noncopyable& operator=(const Noncopyable&) = delete;
};

//�N���X���`�����ۂɈÖٓI�ɒ�`�����֐������邪�A�����𐧌䂷�� = default �� = delete ���g���Ă���
// = default�͈Öْ�`�����֐����g�p����
// = delete�͈Öْ�`�����֐��̎g�p���֎~����

