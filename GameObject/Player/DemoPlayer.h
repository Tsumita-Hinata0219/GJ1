#pragma once
#include <ImGuiManager.h>
#include <Input.h>
#include <Model.h>
#include <WorldTransform.h>

#include "Collider.h"
#include "CollisionConfig.h"
#include "Function.h"

#include <list>

class DemoPlayer : public Collider {

public:
	DemoPlayer();
	~DemoPlayer();

	// 初期化処理
	void Initialize(Model* model, Vector3 position);

	// 更新処理
	void Update();

	// 描画処理
	void Draw(ViewProjection viewProjection);

	// 移動処理
	void Move();

	// 当たり判定に必要なAABBを求める
	void CalcAABB();

	// 衝突時処理
	void onCollision(int num) override;

	// ワールド座標の取得
	Vector3 GetWorldPosition() override;

	// AABBの取得
	AABB GetAABB() override;

	// プレイヤーの座標の設定
	void SetWorldPosition(Vector3 position);

	// 衝突フラグのリセット
	void IsCollisionStateReset();

private:
	WorldTransform worldTransform_; // ワールド変換データ

	Model* model_ = nullptr; // モデル

	uint32_t playerTextureHandle_ = 0u; // テクスチャハンドル

	Input* input_ = nullptr; // キーボード入力

	const float radius_ = 2.0f; // 半径

	AABB aabb_;

	bool isGroundHit;
	bool isDamageHit;
	bool isStartHit;
	bool isGoalHit;
};