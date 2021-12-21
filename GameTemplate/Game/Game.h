#pragma once

#include "Level3DRender/LevelRender.h"

class Player;
class Background;
class GameCamera;
class Enemy;
class PlayerAttackEnemy;
class Tower;
class BreakTower;
class Hp;
class Fade;
class GameOver;
class GameClear;
class MagicEnemy;
class Stuck;
class Heal;
class LongSwordEnemy;
class TwoSwordEnemy;
class TowerUi;
class PlayerUi;


class Game : public IGameObject
{
public:
	enum EnGameState {
		enGameState_Battle,//戦闘中
		enGameState_Idle, //待機中
		enGameState_Menu,
	};
public:
	enum EnWaveState {
		enWaveState_0,//最初
		enWaveState_1,//ウェーブ１
		enWaveState_2,//ウェーブ２
		enWaveState_3,
		enWaveState_4,
	};
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void GameOverNotice();
	void GameClearNotice();
	void Render(RenderContext& rc);
	void ProcessCommonStateTransition();
	void Idle();
	void Battle();

	bool m_isDrawInstancing = true;

	const bool IsWannihilationEnemy()const
	{
		return 	m_AllEnemynum == m_DeadEnemynum;

	}
	int a = 0;
	bool ExitEnemy = false;
	ModelRender m_modelRender;
	LevelRender m_levelRender0;
	LevelRender m_levelRender1;
	LevelRender m_levelRender2;
	LevelRender m_levelRender3;
	LevelRender m_levelRender4;
	LevelRender m_levelRender5;
	Player* m_player = nullptr;
	Background* m_background = nullptr;
	GameCamera* m_gameCamera = nullptr;
	SkyCube* m_skyCube = nullptr;
	Enemy* m_enemy = nullptr;
	Tower* m_tower = nullptr;;
	Hp* m_hp = nullptr;;
	Fade* m_fade = nullptr;
	bool           m_isWaitFadeout = false;
	GameOver* m_gameOver = nullptr;
	GameClear* m_gameclear = nullptr;
	PlayerAttackEnemy* m_playerattackenemy = nullptr;
	MagicEnemy* m_magicenemy = nullptr;
	Stuck* m_stuck = nullptr;;
	Heal* m_heal = nullptr;;
	FontRender timeFont1;
	FontRender timeFont2;
	FontRender timeFont3;
	//float gametimeFont = 15.0f;
	float gametimeFont = 5.0f;
	LongSwordEnemy* m_longswordenemy = nullptr;;
	TwoSwordEnemy* m_twoswordenemy = nullptr;;
	int m_AllEnemynum = 0;
	int m_DeadEnemynum = 0;
	bool GameClearFlag = false;
	bool GameOverFlag = false;
	SoundSource* m_bgm = nullptr;
	TowerUi* m_towerui = nullptr;
	PlayerUi* m_playerui = nullptr;
	EnGameState m_gameState = enGameState_Idle;
	EnWaveState m_waveState = enWaveState_0;
	float battletime = 0.0f;
	BreakTower* m_breaktower = nullptr;
};

