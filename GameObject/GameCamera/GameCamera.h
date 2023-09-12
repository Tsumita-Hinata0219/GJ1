#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "ImGuiManager.h"

#include "Function.h"


/// <summary>
/// ゲームカメラ
/// </summary>
class GameCamera {

public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(WorldTransform worldTransform);


	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();


	/// <summary>
	/// ビュープロジェクションの取得
	/// </summary>
	ViewProjection& GetViewProjection() { return viewProjection_; }

	/// <summary>
	/// ワールド変換データ
	/// </summary>
	WorldTransform& GetWorldTransform() { return worldTransform_; }


private:

	// ワールド変換データ
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection viewProjection_;
};
