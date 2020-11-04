/*****************************************************************//**
 * \file   PlayerConst.h
 * \brief  プレイヤー関連の定数
 * * プレイヤーの外で利用する可能性があったので、ヘッダーに移動。
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once

namespace nsPlayerCnst{
    const float StandardSize = 40.0f;

    //移動速度減衰
    const float MoveSpeedAtten = 0.98f;
    const float MoveSpeedAtten2 = 0.3f;
    //重力
    const float GravityMoveSpeed = 800.0f;
    //ジャンプ速度
    const float JumpMoveSpeed = 700.0f;
    //一定以上のyの速度があったらバウンドする～
    const float LimitBoundMoveSpeed = -1000.0f;
    const float BoundMultiply = 0.7f;
    //地面と衝突する前のyベクトルを記憶する
    float MoveSpeedY = 0.0f;
    const float TimeLimit = 0.3f;
    const int CountLimit = 5;
    const float DushSpeed = 5.0f;
    //連続で衝突音を出さないために
    const float CollisionTime = 0.2f;
    const float BrakeVolume = 0.7f;
    const float CollisionVolume = 1.8;
    const float CollisionAtten = 0.5f;
}