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
	/// AABBを求める
	/// </summary>
	void CalcAABB();

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





private:

	WorldTransform worldTransform_; // ワールド変換データ

	Model* model_ = nullptr; // モデル

	uint32_t playerTextureHandle_ = 0u; // テクスチャハンドル

	Input* input_ = nullptr; // キーボード入力

	const float radius_ = 2.0f; // 半径

	AABB aabb_;
};