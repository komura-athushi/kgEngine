#pragma once

#define BUILD_LEVEL_DEBUG		0							//デバッグ
#define BUILD_LEVEL_PREVIEW		1							//プレビュー
#define BUILD_LEVEL_MASTER		2							//リリース

#ifdef  _DEBUG
#define BUILD_LEVEL		BUILD_LEVEL_DEBUG
#elif _RELEASE
#define BUILD_LEVEL		BUILD_LEVEL_MASTER
#else
#define BUILD_LEVEL		BUILD_LEVEL_PREVIEW
#endif

#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <math.h>
#include <vector>
#include <list>
#include <D3Dcompiler.h>
#include <Windows.h>
#include <d3d11.h>
#include <map>
#include <functional>
#include <algorithm>

#include <xaudio2.h>
#include <x3daudio.h>
#include <xaudio2fx.h>

#include "bulletPhysics/src/btBulletDynamicsCommon.h"
#include "bulletPhysics/src/BulletCollision\CollisionDispatch\btGhostObject.h"

//DirectXTKのインクルードファイル。
#include "DirectXTK/Inc/Model.h"
#include "DirectXTK/Inc/Effects.h"	
#include "DirectXTK/Inc/CommonStates.h"
#include "DirectXTK/Inc/SpriteFont.h"
#include "DirectXTK/Inc/DDSTextureLoader.h"

#include "physics/Physics.h"
#include "HID/Pad.h"

#include "math/Vector.h"
#include "math/Matrix.h"
#include "graphics/GraphicsEngine.h"

#include "graphics/SkinModel.h"
#include "graphics/Skeleton.h"

#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "graphics/Camera.h"

#include "KgEngine.h"
#include "Noncopyable.h"
#include "gameobject/kgGameObject.h"
#include "gameobject/kgGameObjectManager.h"
#include "graphics/kgSkinModelRender.h"
#include "timer/GameTime.h"
#include "timer/StopWatch.h"
#include "graphics/shadow/kgShadowMap.h"
#include "graphics/RenderTarget.h"
#include "graphics/2D/Sprite.h"
const float FRAME_BUFFER_W = 1280.0f;				//フレームバッファの幅。
const float FRAME_BUFFER_H = 720.0f;				//フレームバッファの高さ。

static const int MAX_BONE = 512;	//!<ボーンの最大数。
