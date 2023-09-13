#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <Input.h>
#include <ImGuiManager.h>

#include "Function.h"


class GameScene;

class BoxType {

public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Model* model, Vector3& position, uint32_t textureHandle);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(Vector3 move);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(const ViewProjection& viewProjection_);

	/// <summary>
	/// AABBの取得
	/// </summary>
	AABB GetAABB();

private:

	uint32_t Box_ = 0;
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	XINPUT_STATE joyState;

	float boxWidth_ = 2.0f;
	float boxHeight_ = 2.0f;
	float boxDepth_ = 2.0f;


};