#include "stdafx.h"
#include "Game.h"

#include "Player.h"
#include "Tower.h"
#include "Background.h"
#include "GameCamera.h"

#include "Enemy.h"
#include "PlayerAttackEnemy.h"
#include "MagicEnemy.h"
#include "TwoSwordEnemy.h"
#include "BattleStartFade.h"

#include "Fade.h"
#include "GameClear.h"
#include "GameOver.h"
#include "TowerUi.h"
#include "PlayerUi.h"
#include "HowToPlay.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

Game::Game()
{

}

Game::~Game()
{
	//�I�u�W�F�N�g���폜
	DeleteGO(m_gameCamera);
	DeleteGO(m_skyCube);
	DeleteGO(m_player);
	DeleteGO(m_background);
	DeleteGO(m_tower);
	DeleteGO(m_twoswordenemy);
	DeleteGO(m_bgm);
	DeleteGO(m_towerui);
	DeleteGO(m_playerui);
	const auto& enemys = FindGOs<Enemy>("enemy");
	for (auto enemy : enemys)
	{
		DeleteGO(enemy);
	}
	const auto& playerattackenemys = FindGOs<PlayerAttackEnemy>("playerattackenemy");
	for (auto playerattackenemy : playerattackenemys)
	{
		DeleteGO(playerattackenemy);
	}
	const auto& magicenemys = FindGOs<MagicEnemy>("magicenemy");
	for (auto magicenemy : magicenemys)
	{
		DeleteGO(magicenemy);
	}
}

bool Game::Start()
{
	round.Init("Assets/sprite/round.dds",1920.0f, 1080.0f);
	round.SetPosition(Vector3(0.0f, -10.0f, 0.0f));
	enemynum.Init("Assets/sprite/enemynum.dds", 1920.0f, 1080.0f);
	enemynum.SetPosition(Vector3(0.0f, -10.0f, 0.0f));
	buttom.Init("Assets/sprite/roundbuttom.dds", 1920.0f, 1080.0f);

	g_soundEngine->ResistWaveFileBank(22, "Assets/sound/gamebgm.wav");
	m_gameclear = FindGO<GameClear>("gameclear");
	m_towerui = NewGO<TowerUi>(0, "towerui");
	m_playerui = NewGO<PlayerUi>(0, "playerui");
	m_player = FindGO<Player>("player");
	m_tower = FindGO<Tower>("tower");
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetLuminance(0.3f);
	g_sceneLight->SetIBLTextureForAmbient(m_skyCube->GetTextureFilePath(), 0.1f);
	g_camera3D->SetFar(15000.0f);
	
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");
	m_gameCamera = FindGO<GameCamera>("gamecamera");

	//�^���[�ƃv���C���[�Ɣw�i
	m_levelRender0.Init("Assets/level3D/stage0.tkl", [&](LevelObjectData& objData)
		{
			if (objData.EqualObjectName(L"player4") == true) {
				//�v���C���[�̃I�u�W�F�N�g���쐬
				m_player = NewGO<Player>(0, "player");
				//���W��ݒ�
				m_player->SetPosition(objData.position);
				//�傫����ݒ�
				m_player->SetScale(objData.scale);
				//��]��ݒ�
				m_player->SetRotation(objData.rotation);
				return true;
			}
			else if (objData.EqualObjectName(L"background2") == true) {
				//�w�i�I�u�W�F�N�g���쐬
				m_background = NewGO<Background>(0, "background");
				//���W��ݒ�
				m_background->SetPosition(objData.position);
				//�傫����ݒ�
				m_background->SetScale(objData.scale);
				//��]��ݒ�
				m_background->SetRotation(objData.rotation);
				return true;
			}
			else if (objData.EqualObjectName(L"tower2") == true) {
				//�^���[���쐬
				m_tower = NewGO<Tower>(0, "tower");
				//���W��ݒ�
				m_tower->SetPosition(objData.position);
				//�傫����ݒ�
				m_tower->SetScale(objData.scale);
				//��]��ݒ�
				m_tower->SetRotation(objData.rotation);
				return true;
			}
			return true;
		});

	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(22);
	m_bgm->Play(true);
	m_bgm->SetVolume(0.1f);
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();
	m_battlestartfade = FindGO<BattleStartFade>("battlestartfade");
	m_gameOver = FindGO<GameOver>("gameover");

	return true;
}


void Game::Update()
{

	if (m_isWaitFadeout) {
		if (!m_fade->IsFade()) {
			if (m_GameClearFlag == true) {
				NewGO<GameClear>(0, "gameclear");
				DeleteGO(this);
				return;
			}
			if (m_GameOverFlag == true)
			{
				NewGO<GameOver>(0, "gameover");
				DeleteGO(this);
				return;
			}
		}
	}

	Idle();
	Battle();

	wchar_t text1[256];
	int minute1 = (int)m_gametimeFont / 60;
	int sec1 = (int)m_gametimeFont % 60;
	swprintf_s(text1, 256, L"%02d:%02d", minute1, sec1);
	timeFont1.SetText(text1);
	timeFont1.SetPosition(Vector3(600.0f, 400.0f, 0.0f));
	timeFont1.SetScale(2.3f);
	timeFont1.SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	wchar_t text[256];
	swprintf_s(text, 256, L"%d", m_AllEnemynum - m_DeadEnemynum);
	timeFont2.SetText(text);
	timeFont2.SetPosition(Vector3(790.0f, 480.0f, 0.0f));
	timeFont2.SetScale(1.8f);
	timeFont2.SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	wchar_t text3[256];
	int wave = 0;
	wave = m_waveState + 1;
	swprintf_s(text3, 256, L"��%d���E���h", wave);
	timeFont3.SetText(text3);
	timeFont3.SetPosition(Vector3(340.0f, 470.0f, 0.0f));
	timeFont3.SetScale(1.4f);
	timeFont3.SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	wchar_t text4[256];
	swprintf_s(text4, 256, L"�ҋ@��...");
	idlefont.SetText(text4);
	idlefont.SetPosition(Vector3(470.0f, 500.0f, 0.0f));
	idlefont.SetScale(2.0f);
	idlefont.SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	round.Update();
	enemynum.Update();
}

void Game::Idle()
{
	if (m_gameState != enGameState_Idle)
	{
		return;
	}

	m_battletime = 1.0f;
	m_gametimeFont -= g_gameTime->GetFrameDeltaTime();
	
	if (m_gametimeFont <= 1.0f)
	{
		m_battlestartfade->StartFadeOut();
		m_isBattleStartFade = true;
	}

	//�ҋ@�^�C�}�[��0�ɂȂ�����
	if (m_gametimeFont <= 0.0f)
	{
		//�E�F�[�u�P
		if (m_waveState == enWaveState_0) 
		{
			m_levelRender1.Init("Assets/level3D/stage1.tkl", [&](LevelObjectData& objData)
				{
					if (objData.EqualObjectName(L"normal2") == true) {
						//�G�l�~�[���쐬
						m_enemy = NewGO<Enemy>(0, "enemy");
						//���W��ݒ�
						m_enemy->SetPosition(objData.position);
						//�傫����ݒ�
						m_enemy->SetScale(objData.scale);
						//��]��ݒ�
						m_enemy->SetRotation(objData.rotation);
						m_AllEnemynum++;
						return true;
					}
					else if (objData.EqualObjectName(L"playerattackenemy") == true) {
						//�v���C���[�U���G�l�~�[���쐬
						m_playerattackenemy = NewGO<PlayerAttackEnemy>(0, "playerattackenemy");
						//���W��ݒ�
						m_playerattackenemy->SetPosition(objData.position);
						//�傫����ݒ�
						m_playerattackenemy->SetScale(objData.scale);
						//��]��ݒ�
						m_playerattackenemy->SetRotation(objData.rotation);
						m_AllEnemynum++;
						return true;
					}
					else if (objData.EqualObjectName(L"magicenemy") == true) {
						//���@�G�l�~�[���쐬
						m_magicenemy = NewGO<MagicEnemy>(0, "magicenemy");
						//���W��ݒ�
						m_magicenemy->SetPosition(objData.position);
						//�傫����ݒ�
						m_magicenemy->SetScale(objData.scale);
						//��]��ݒ�
						m_magicenemy->SetRotation(objData.rotation);
						m_AllEnemynum++;
						return true;
					}
					return true;
				});
		}
		//�E�F�[�u�Q
		else if (m_waveState == enWaveState_1)
		{
			m_levelRender2.Init("Assets/level3D/stage2.tkl", [&](LevelObjectData& objData)
				{
					if (objData.EqualObjectName(L"normal2") == true) {
						//�G�l�~�[���쐬
						m_enemy = NewGO<Enemy>(0, "enemy");
						//���W��ݒ�
						m_enemy->SetPosition(objData.position);
						//�傫����ݒ�
						m_enemy->SetScale(objData.scale);
						//��]��ݒ�
						m_enemy->SetRotation(objData.rotation);
						m_AllEnemynum++;
						return true;
					}
					else if (objData.EqualObjectName(L"playerattackenemy") == true) {
						//�v���C���[�U���G�l�~�[���쐬
						m_playerattackenemy = NewGO<PlayerAttackEnemy>(0, "playerattackenemy");
						//���W��ݒ�
						m_playerattackenemy->SetPosition(objData.position);
						//�傫����ݒ�
						m_playerattackenemy->SetScale(objData.scale);
						//��]��ݒ�
						m_playerattackenemy->SetRotation(objData.rotation);
						m_AllEnemynum++;
						return true;
					}
					else if (objData.EqualObjectName(L"magicenemy") == true) {
						//���@�G�l�~�[���쐬
						m_magicenemy = NewGO<MagicEnemy>(0, "magicenemy");
						//���W��ݒ�
						m_magicenemy->SetPosition(objData.position);
						//�傫����ݒ�
						m_magicenemy->SetScale(objData.scale);
						//��]��ݒ�
						m_magicenemy->SetRotation(objData.rotation);
						m_AllEnemynum++;
						return true;
					}
					return true;
				});
		}
		else if (m_waveState == enWaveState_2)
		{
			m_levelRender3.Init("Assets/level3D/stage3.tkl", [&](LevelObjectData& objData)
				{
					if (objData.EqualObjectName(L"normal2") == true) {
						//�G�l�~�[���쐬
						m_enemy = NewGO<Enemy>(0, "enemy");
						//���W��ݒ�
						m_enemy->SetPosition(objData.position);
						//�傫����ݒ�
						m_enemy->SetScale(objData.scale);
						//��]��ݒ�
						m_enemy->SetRotation(objData.rotation);
						m_AllEnemynum++;
						return true;
					}
					else if (objData.EqualObjectName(L"playerattackenemy") == true) {
						//�v���C���[�U���G�l�~�[���쐬
						m_playerattackenemy = NewGO<PlayerAttackEnemy>(0, "playerattackenemy");
						//���W��ݒ�
						m_playerattackenemy->SetPosition(objData.position);
						//�傫����ݒ�
						m_playerattackenemy->SetScale(objData.scale);
						//��]��ݒ�
						m_playerattackenemy->SetRotation(objData.rotation);
						m_AllEnemynum++;
						return true;
					}
					else if (objData.EqualObjectName(L"magicenemy") == true) {
						//���@�G�l�~�[���쐬
						m_magicenemy = NewGO<MagicEnemy>(0, "magicenemy");
						//���W��ݒ�
						m_magicenemy->SetPosition(objData.position);
						//�傫����ݒ�
						m_magicenemy->SetScale(objData.scale);
						//��]��ݒ�
						m_magicenemy->SetRotation(objData.rotation);
						m_AllEnemynum++;
						return true;
					}
					return true;
				});
		}
		else if (m_waveState == enWaveState_3)
		{
			m_levelRender4.Init("Assets/level3D/stage4.tkl", [&](LevelObjectData& objData)
			{
				if (objData.EqualObjectName(L"normal2") == true) {
					//�G�l�~�[���쐬
					m_enemy = NewGO<Enemy>(0, "enemy");
					//���W��ݒ�
					m_enemy->SetPosition(objData.position);
					//�傫����ݒ�
					m_enemy->SetScale(objData.scale);
					//��]��ݒ�
					m_enemy->SetRotation(objData.rotation);
					m_AllEnemynum++;
					return true;
				}
				else if (objData.EqualObjectName(L"playerattackenemy") == true) {
					//�v���C���[�U���G�l�~�[���쐬
					m_playerattackenemy = NewGO<PlayerAttackEnemy>(0, "playerattackenemy");
					//���W��ݒ�
					m_playerattackenemy->SetPosition(objData.position);
					//�傫����ݒ�
					m_playerattackenemy->SetScale(objData.scale);
					//��]��ݒ�
					m_playerattackenemy->SetRotation(objData.rotation);
					m_AllEnemynum++;
					return true;
				}
				else if (objData.EqualObjectName(L"magicenemy") == true) {
					//���@�G�l�~�[���쐬
					m_magicenemy = NewGO<MagicEnemy>(0, "magicenemy");
					//���W��ݒ�
					m_magicenemy->SetPosition(objData.position);
					//�傫����ݒ�
					m_magicenemy->SetScale(objData.scale);
					//��]��ݒ�
					m_magicenemy->SetRotation(objData.rotation);
					m_AllEnemynum++;
					return true;
				}
				return true;
			});
		}
		else if (m_waveState == enWaveState_4)
		{
			m_levelRender5.Init("Assets/level3D/stage5.tkl", [&](LevelObjectData& objData)
			{
				if (objData.EqualObjectName(L"normal2") == true) {
					//�G�l�~�[���쐬
					m_enemy = NewGO<Enemy>(0, "enemy");
					//���W��ݒ�
					m_enemy->SetPosition(objData.position);
					//�傫����ݒ�
					m_enemy->SetScale(objData.scale);
					//��]��ݒ�
					m_enemy->SetRotation(objData.rotation);
					m_AllEnemynum++;
					return true;
				}
				else if (objData.EqualObjectName(L"playerattackenemy") == true) {
					//�v���C���[�U���G�l�~�[���쐬
					m_playerattackenemy = NewGO<PlayerAttackEnemy>(0, "playerattackenemy");
					//���W��ݒ�
					m_playerattackenemy->SetPosition(objData.position);
					//�傫����ݒ�
					m_playerattackenemy->SetScale(objData.scale);
					//��]��ݒ�
					m_playerattackenemy->SetRotation(objData.rotation);
					m_AllEnemynum++;
					return true;
				}
				else if (objData.EqualObjectName(L"magicenemy") == true) {
					//���@�G�l�~�[���쐬
					m_magicenemy = NewGO<MagicEnemy>(0, "magicenemy");
					//���W��ݒ�
					m_magicenemy->SetPosition(objData.position);
					//�傫����ݒ�
					m_magicenemy->SetScale(objData.scale);
					//��]��ݒ�
					m_magicenemy->SetRotation(objData.rotation);
					m_AllEnemynum++;
					return true;
				}
				else if (objData.EqualObjectName(L"2sword") == true) {
					//���@�G�l�~�[���쐬
					m_twoswordenemy = NewGO<TwoSwordEnemy>(0, "twoswordenemy");
					//���W��ݒ�
					m_twoswordenemy->SetPosition(objData.position);
					//�傫����ݒ�
					m_twoswordenemy->SetScale(objData.scale);
					//��]��ݒ�
					m_twoswordenemy->SetRotation(objData.rotation);
					m_AllEnemynum++;
					return true;
				}
				return true;
			});
		}

		m_gameState = enGameState_Battle();
		m_battlestartfade->StartFadeIn();
	}
}


void Game::Battle()
{
	if (m_gameState != enGameState_Battle)
	{
		return;
	}

	m_battletime -= g_gameTime->GetFrameDeltaTime();
	if(m_battletime <= 0.0f)
	{
		m_isBattleStartFade = false;
	}

	if (IsWannihilationEnemy() == true)
	{
		if (m_waveState == enWaveState_0)
		{
			m_waveState = enWaveState_1;
			m_gameState = enGameState_Idle;
			m_gametimeFont = 8.0f;
		}
		else if (m_waveState == enWaveState_1)
		{
			m_waveState = enWaveState_2;
			m_gameState = enGameState_Idle;
			m_gametimeFont = 8.0f;
		}
		else if (m_waveState == enWaveState_2)
		{
			m_waveState = enWaveState_3;
			m_gameState = enGameState_Idle;
			m_gametimeFont = 8.0f;
		}
		else if (m_waveState == enWaveState_3)
		{
			m_waveState = enWaveState_4;
			m_gameState = enGameState_Idle;
			m_gametimeFont = 8.0f;
		}
		else
		{
			//GameClearNotice();
		}
	}
}

void Game::GameOverNotice()
{
	m_isWaitFadeout = true;
	m_fade->StartFadeOut();
	m_GameOverFlag = true;
}

void Game::GameClearNotice()
{
	m_isWaitFadeout = true;
	m_fade->StartFadeOut();
	m_GameClearFlag = true;
}

void Game::Render(RenderContext& rc)
{
	buttom.Draw(rc);
	if (m_gameState == enGameState_Battle)
	{
		timeFont2.Draw(rc);
		timeFont3.Draw(rc);
		round.Draw(rc);
		enemynum.Draw(rc);
	}
	else if (m_gameState == enGameState_Idle)
	{
		idlefont.Draw(rc);
	}
}

