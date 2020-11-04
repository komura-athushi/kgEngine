/*****************************************************************//**
 * \file   PlayerConst.h
 * \brief  �v���C���[�֘A�̒萔
 * * �v���C���[�̊O�ŗ��p����\�����������̂ŁA�w�b�_�[�Ɉړ��B
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once

namespace nsPlayerCnst{
    const float StandardSize = 40.0f;

    //�ړ����x����
    const float MoveSpeedAtten = 0.98f;
    const float MoveSpeedAtten2 = 0.3f;
    //�d��
    const float GravityMoveSpeed = 800.0f;
    //�W�����v���x
    const float JumpMoveSpeed = 700.0f;
    //���ȏ��y�̑��x����������o�E���h����`
    const float LimitBoundMoveSpeed = -1000.0f;
    const float BoundMultiply = 0.7f;
    //�n�ʂƏՓ˂���O��y�x�N�g�����L������
    float MoveSpeedY = 0.0f;
    const float TimeLimit = 0.3f;
    const int CountLimit = 5;
    const float DushSpeed = 5.0f;
    //�A���ŏՓˉ����o���Ȃ����߂�
    const float CollisionTime = 0.2f;
    const float BrakeVolume = 0.7f;
    const float CollisionVolume = 1.8;
    const float CollisionAtten = 0.5f;
}