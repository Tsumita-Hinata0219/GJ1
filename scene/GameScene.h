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
#include "DemoPlayer.h"

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
	///	シーンの変更時の処理
	/// </summary>
	void ChangeScene(Scene scene, int nextMap, Vector3 IniPos);


	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollision();


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
	void GeneratedMap(int index, Vector3 pos, uint32_t mapBoxTextureHandle, uint32_t filter);


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;


	// 3Dモデル
	Model* model_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;



	/* ----- Scene シーン ----- */
	// シーン
	Scene scene_ = TITLE;
	// 現在選択しているステージ
	uint32_t nowStageSelect_ = 1;


	/* ----- 背景スプライト ----- */
	// 背景スプライト
	Sprite* titleSprite_ = nullptr;
	Sprite* stageSelectSprite_ = nullptr;
	Sprite* gameClearSprite_ = nullptr;
	Sprite* gameOverSprite_ = nullptr;
	Sprite* pushASprite_ = nullptr;
	Sprite* pushBSprite_ = nullptr;
	Sprite* pushXSprite_ = nullptr;
	Sprite* pushYSprite_ = nullptr;
	Sprite* pushLeftSprite_ = nullptr;
	Sprite* pushRightSprite_ = nullptr;
	// テクスチャ
	uint32_t titleTextureHandle_;
	uint32_t stageSelectTextureHandle_;
	uint32_t gameClearTextureHandle_;
	uint32_t gameOverTextureHandle_;
	uint32_t pushATextureHandle_;
	uint32_t pushBTextureHandle_;
	uint32_t pushXTextureHandle_;
	uint32_t pushYTextureHandle_;
	uint32_t pushLeftTextureHandle_;
	uint32_t pushRightTextureHandle_;


	/* ----- CollisionManager コライダー ----- */
	// コライダー
	CollisionManager* collisionManager_ = nullptr;



	/* ----- Player 自キャラ ----- */
	// 自キャラ
	Player* player_ = nullptr;
	DemoPlayer* demoPlayer_ = nullptr;
	// プレイヤーの初期ポジション
	Vector3 IniPosStage1_;
	Vector3 IniPosStage2_;
	Vector3 IniPosStage3_;
	Vector3 IniPosStage4_;



	/* ----- マップ Map ----- */
	// マップ
	Map* map_ = nullptr;
	// マップリスト
	std::list<Map*> mapBoxs_[4];
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


	/* ----- GamePad ゲームパッド ----- */
	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState_;



	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
