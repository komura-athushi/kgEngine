/*****************************************************************//**
 * \file   PlayerConst.h
 * \brief  �v���C���[�֘A�̒萔
 * * �v���C���[�̊O�ŗ��p����\�����������̂ŁA�w�b�_�[�Ɉړ��B
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once

namespace nsPlayerCnst{
    const float standardSize = 40.0f;

    //�ړ����x����
    const float moveSpeedAtten = 0.98f;
    const float moveSpeedAtten2 = 0.3f;
    //�d��
    const float gravityMoveSpeed = 800.0f;
    //�W�����v���x
    const float jumpMoveSpeed = 700.0f;
    //���ȏ��y�̑��x����������o�E���h����`
    const float limitBoundMoveSpeed = -1000.0f;
    const float boundMultiply = 0.7f;
    //�n�ʂƏՓ˂���O��y�x�N�g�����L������
    float moveSpeedY = 0.0f;
    const float timeLimit = 0.3f;
    const int countLimit = 5;
    const float dushSpeed = 5.0f;
    //�A���ŏՓˉ����o���Ȃ����߂�
    const float collisionTime = 0.2f;
    const float brakeVolume = 0.7f;
    const float collisionVolume = 1.8;
    const float collisionAtten = 0.5f;
    //��̑̐ς����Z���鎞�̌�����
    const float volumeMultiply = 0.9f;
    const float stopTime = 1.5f;
    //���X�|�[���܂ł̎���
    const float respawnTime = 3.0f;
    //���X�|�[���̎���y����������
    const float respawnHeight = 50.f;
    //�_�b�V���̏������Ƀ��[�v���đ��x�����Z����
    const float roopDush = 30;
    const float speed = 5.0f * 0.9f;
    const float brakeSpeed = 55.0f;
    const float brakeSpeed2 = 0.8f;
    const float brakeAngle = 0.5f;
    const float effectScale = 4.0f;
}