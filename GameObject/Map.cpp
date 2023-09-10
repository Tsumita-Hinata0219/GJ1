#include "Map.h"
#include "GameScene.h"


/// <summary>
/// 初期化処理
/// </summary>
void Map::Initialize(Model* model, Vector3 position, uint32_t textureHandle) {

	// NULLポインタチェック
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;

	worldTransform_.translation_ = position;
	worldTransform_.Initialize();
}



/// <summary>
/// 更新処理
/// </summary>
void Map::Update() {

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}



/// <summary>
/// 描画処理
/// </summary>
void Map::Draw(ViewProjection viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
