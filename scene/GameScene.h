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

#include "Map.h"
#include "Function.h"

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
	/// マップデータの読み込み
	/// </summary>
	void LoadMapData(const char* fileName);

	/// <summary>
	/// マップ生成スクリプト実行
	/// </summary>
	void UpdateMapData();

	/// <summary>
	/// マップを生成する
	/// </summary>
	void GeneratedMap(Vector3 pos);


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;


	// 3Dモデル
	Model* model_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;




	/* ----- マップ Map ----- */
	// マップ
	Map* map_ = nullptr;
	// マップリスト
	std::list<Map*> mapBoxs_;
	// マップ生成コマンド
	std::stringstream mapBoxCreateCommands_;
	// テクスチャ
	uint32_t mapBoxTextureHandle_ = 0;
	StageData stage_;



	/* ----- Camera カメラ ----- */
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;



	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
