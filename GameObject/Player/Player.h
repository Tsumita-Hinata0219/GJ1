#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <Input.h>
#include <ImGuiManager.h>

#include "Function.h"
#include "CollisionConfig.h"
#include "Collider.h"

#include <list>



class Player : public Collider {

public:

	Player();
	~Player();

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