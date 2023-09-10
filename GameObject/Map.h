#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ImGuiManager.h>
#include <list>
#include <Sprite.h>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>


// GameSceneの前方宣言
class GameScene;



// Map
class Map {

public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Model* model, Vector3 position,  uint32_t textureHandle);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(ViewProjection viewProjection);


	/// <summary>
	/// GameSceneの取得
	/// </summary>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	/// <summary>
	/// マップのボックスのサイズの取得
	/// </summary>
	float GetMap_BoxSize() { return map_BoxSize_; }


private:

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャ
	uint32_t textureHandle_ = 0;

	// 背景スプライト
	Sprite* bagGroundSprite_ = nullptr;

	// マップ生成コマンド
	std::stringstream mapCreateCommands_;

	// マップのサイズ
	static const int map_Width_ = 100;
	static const int map_Height_ = 100;

	// マップボックスのサイズ
	float map_BoxSize_ = 2.0f;
};