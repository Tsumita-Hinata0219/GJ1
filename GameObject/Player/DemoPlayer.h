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

	void Initialize(Model* model, Vector3 position);

	void Update();

	void Draw(ViewProjection viewProjection);

	void Move();

	void CalcAABB();

	void SetIsHit(bool falg) { isHit = falg; }

	void onCollision() override;

	Vector3 GetWorldPosition() override;

	AABB GetAABB() override;

private:
	WorldTransform worldTransform_; // ワールド変換データ

	Model* model_ = nullptr; // モデル

	uint32_t playerTextureHandle_ = 0u; // テクスチャハンドル

	Input* input_ = nullptr; // キーボード入力

	const float radius_ = 2.0f; // 半径

	AABB aabb_;

	bool isHit;
};