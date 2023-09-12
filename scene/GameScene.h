#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

#include "Function.h"
#include "Map.h"
#include "Skydome.h"
#include "GameCamera.h"
#include "CollisionManager.h"
#include "CollisionConfig.h"
#include "Collider.h"
#include "Player.h"

#include <list>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollision();


	/// <summary>
	/// ステージを変更する
	/// </summary>
	void ChangeStage(int index);


	/// <summary>
	/// マップデータの読み込み
	/// </summary>
	void LoadMapData(const char* fileName, int index);

	/// <summary>
	/// マップ生成スクリプト実行
	/// </summary>
	void UpdateMapData(int index);

	/// <summary>
	/// マップの座標を決める
	/// </summary>
	Vector3 CreateMapVector(int indexX, int indexY);

	/// <summary>
	/// マップを生成する
	/// </summary>
	void GeneratedMap(Vector3 pos, uint32_t mapBoxTextureHandle, uint32_t filter);


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;


	// 3Dモデル
	Model* model_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;



	/* ----- コライダー ----- */
	// コライダー
	CollisionManager* collisionManager_ = nullptr;



	/* ----- Player 自キャラ ----- */
	// 自キャラ
	Player* player_ = nullptr;



	/* ----- マップ Map ----- */
	// マップ
	Map* map_ = nullptr;
	// マップリスト
	std::list<Map*> mapBoxs_;
	// マップ生成コマンド
	std::stringstream mapBoxCreateCommands_[4];
	// テクスチャ
	StageData stage_;
	// 現在進行中のマップ
	int nowMap_ = 0;


	/* ----- SkyDome 天球 ----- */
	// 天球
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;


	/* ----- Camera カメラ ----- */
	// ゲームカメラ
	GameCamera* gameCamera_ = nullptr;
	WorldTransform gameCameraTransform_;
	
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;



	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
