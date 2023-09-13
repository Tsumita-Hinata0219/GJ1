#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <Input.h>
#include <ImGuiManager.h>

#include "Function.h"
#include "Struct.h"
#include "CollisionConfig.h"
#include "Collider.h"
#include "BoxType.h"

#include <list>



class Player : public Collider {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Model* model, Vector3 position);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 衝突判定処理
	/// </summary>
	void onCollision(int num) override;

	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	Vector3 GetWorldPosition() override;

	/// <summary>
	/// AABBの取得
	/// </summary>
	AABB GetAABB() override;

	/// <summary>
	/// ボックスを出す処理
	/// </summary>
	void Box();

	/// <summary>
	/// 速度を設定する
	/// </summary>
	void SetVelocity(const Vector3& newVelocity);

	/// <summary>
	/// プレイヤー座標とボックスの座標がどうのこうのする処理
	/// </summary>
	void AddJustPositionOnBox(float newPosY);

	/// <summary>
	/// プレイヤーとボックスの衝突処理
	/// </summary>
	void HitBox();

	/// <summary>
	/// プレイヤーの座標の設定
	/// </summary>
	void SetWorldPosition(Vector3 position) { worldTransform_.translation_ = position; }


private:

	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr; 

	// テクスチャハンドル
	uint32_t playerTextureHandle_ = 0u; 

	// キーボード入力
	Input* input_ = nullptr;

	// 半径
	const float radius_ = 2.0f; 

	// AABB
	AABB aabb_;

	// ジョイスティック
	XINPUT_STATE joyState;

	// なんかのタイム
	uint32_t tim_ = 20;

	// バレット？ボックス？
	Vector3 bulletOffset_ = {0.0f, 0.0f, 0.0f};

	// Bボタンのなんか
	bool bButtonReleased_ = true;

	// わからん
	bool lLetGo_ = true;
	bool rLetGo_ = true;

	// ボックス : 赤
	BoxType* RED_ = nullptr;
	std::list<BoxType*> REDs_;
	uint32_t redBox_ = TextureManager::Load("/picture/red.png");
	float boxSpeedRed_ = 0.0f;
	bool redMove_ = true;
	int TIM_ = 180;
	
	// ボックス : 青
	BoxType* BLUE_ = nullptr;
	std::list<BoxType*> BLUEs_;
	uint32_t blueBox_ = TextureManager::Load("/picture/blue.png");
	float boxSpeedBlue_ = 0.0f;
	bool blueMove_ = true;
	bool RL_ = true;

	// 移動速度
	Vector3 acce_ = {0.0f, -0.8f, 0.0f};
	Vector3 velocity_ = {0.0f, 0.0f, 0.0f};

	// 多分プレイヤーがどっち向いているかのフラグ
	bool playerRotateLeftFlag = false;
	bool playerRotateRightFlag = false;

	// プレイヤーのサイズ
	float playerWidth_ = 2.0f;
	float playerHeight_ = 2.0f;
	float playerDepth_ = 2.0f;

	// プレイヤーの移動速度
	float kPlayerSpeed_ = 0.2f;



	// マップボックスのビット
	MapBit mapBit_;

	// Mapリスト


};

