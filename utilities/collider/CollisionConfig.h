#pragma once
#include <cstdint>


// プレイヤー
const uint32_t kCollisionAttributePlayer = 0b1;

// マップボックス　床壁
const uint32_t kCollisionAttributeMapBox_Ground = 0b1 << 1;

// マップボックス ダメージ
const uint32_t kCollisionAttributeMapBox_Damage = 0b1 << 2;

// マップボックス スタート
const uint32_t kCollisionAttributeMapBox_State = 0b1 << 3;

// マップボックス ゴール
const uint32_t kCollisionAttributeMapBox_Goal = 0b1 << 4;

