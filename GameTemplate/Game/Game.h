#pragma once

#include "Level3DRender/LevelRender.h"

class Player;
class Background;
class GameCamera;
class Enemy;
class PlayerAttackEnemy;
class Tower;
class Fade;
class GameOver;
class GameClear;
class MagicEnemy;
class LongSwordEnemy;
class TwoSwordEnemy;
class TowerUi;
class PlayerUi;
class Shop;

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

	
	ModelRender          m_modelRender;
	LevelRender          m_levelRender0;
	LevelRender          m_levelRender1;
	LevelRender          m_levelRender2;
	LevelRender          m_levelRender3;
	LevelRender          m_levelRender4;
	LevelRender          m_levelRender5;
	FontRender           timeFont1;
	FontRender           timeFont2;
	FontRender           timeFont3;
	SoundSource*         m_bgm = nullptr;
	Player*              m_player = nullptr;
	Background*          m_background = nullptr;
	GameCamera*          m_gameCamera = nullptr;
	SkyCube*             m_skyCube = nullptr;
	Tower*               m_tower = nullptr;;
	Fade*                m_fade = nullptr;
	GameOver*            m_gameOver = nullptr;
	GameClear*           m_gameclear = nullptr;
	TowerUi*             m_towerui = nullptr;
	PlayerUi*            m_playerui = nullptr;
	Enemy*               m_enemy = nullptr;
	PlayerAttackEnemy*   m_playerattackenemy = nullptr;
	MagicEnemy*          m_magicenemy = nullptr;
	LongSwordEnemy*      m_longswordenemy = nullptr;;
	TwoSwordEnemy*       m_twoswordenemy = nullptr;;
	Shop*                m_shop = nullptr;

	bool                 m_isWaitFadeout = false;

	int m_AllEnemynum = 0;
	int m_DeadEnemynum = 0;

	bool m_GameClearFlag = false;
	bool m_GameOverFlag = false;

	float  m_battletime = 0.0f;
	float  m_gametimeFont = 15.0f;

	EnGameState m_gameState = enGameState_Idle;
	EnWaveState m_waveState = enWaveState_0;
};

