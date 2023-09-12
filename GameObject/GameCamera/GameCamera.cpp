#include "GameCamera.h"



// 初期化処理
void GameCamera::Initialize(WorldTransform worldTransform) {

	// ワールド変換データの初期設定
	worldTransform_ = worldTransform;
	worldTransform_.Initialize();


	// ビュープロジェクションの初期化処理
	viewProjection_.farZ = 1200.0f;
	viewProjection_.Initialize();
}



// 更新処理
void GameCamera::Update() {

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);


#ifdef _DEBUG

	ImGui::Begin("GameCamera");

	ImGui::DragFloat3("Scale", &worldTransform_.scale_.x, 0.01f);
	ImGui::DragFloat3("Rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("Translation", &worldTransform_.translation_.x, 0.01f);

	ImGui::End();


#endif


}

