/*****************************************************************//**
 * \file   PlayerConst.h
 * \brief  プレイヤー関連の定数
 * * プレイヤーの外で利用する可能性があったので、ヘッダーに移動。
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once

namespace nsPlayerCnst{
    const float standardSize = 40.0f;

    //移動速度減衰
    const float moveSpeedAtten = 0.98f;
    const float moveSpeedAtten2 = 0.3f;
    //重力
    const float gravityMoveSpeed = 800.0f;
    //ジャンプ速度
    const float jumpMoveSpeed = 700.0f;
    //一定以上のyの速度があったらバウンドする～
    const float limitBoundMoveSpeed = -1000.0f;
    const float boundMultiply = 0.7f;
    //地面と衝突する前のyベクトルを記憶する
    float moveSpeedY = 0.0f;
    const float timeLimit = 0.3f;
    const int countLimit = 5;
    const float dushSpeed = 5.0f;
    //連続で衝突音を出さないために
    const float collisionTime = 0.2f;
    const float brakeVolume = 0.7f;
    const float collisionVolume = 1.8;
    const float collisionAtten = 0.5f;
    //塊の体積を加算する時の減衰率
    const float volumeMultiply = 0.9f;
    const float stopTime = 1.5f;
    //リスポーンまでの時間
    const float respawnTime = 3.0f;
    //リスポーンの時にyを高くする
    const float respawnHeight = 50.f;
    //ダッシュの初速時にループして速度を加算する
    const float roopDush = 30;
    const float speed = 5.0f * 0.9f;
    const float brakeSpeed = 55.0f;
    const float brakeSpeed2 = 0.8f;
    const float brakeAngle = 0.5f;
    const float effectScale = 4.0f;
}