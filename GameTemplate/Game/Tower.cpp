#include "stdafx.h"
#include "Tower.h"
#include "Enemy.h"
#include "Game.h"
#include "GameOver.h"
#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

Tower::Tower()
{

}

Tower::~Tower()
{

}

bool Tower::Start()
{
	EffectEngine::GetInstance()->ResistEffect(8, u"Assets/efk/Smoke.efk");
	EffectEngine::GetInstance()->ResistEffect(9, u"Assets/efk/towereffect.efk");
	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/towerbreak.wav");

	m_modelRender.Init("Assets/modelData/tower2/tower2.tkm");
	m_modelRender.SetPosition(m_position);
	//大きさを設定する。
	m_modelRender.SetScale(m_scale);
	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);
	
	m_modelRender2.Init("Assets/modelData/tower2/breaktower.tkm");
	m_modelRender2.SetPosition(m_position);
	//大きさを設定する。
	m_modelRender2.SetScale(m_scale);
	//回転を設定する。
	m_modelRender2.SetRotation(m_rotation);
	//モデルを更新する。
	m_modelRender2.Update();

	m_modelRender3.Init("Assets/modelData/tower2/allbreaktower.tkm");
	m_modelRender3.SetPosition(m_position);
	//大きさを設定する。
	m_modelRender3.SetScale(m_scale);
	//回転を設定する。
	m_modelRender3.SetRotation(m_rotation);
	//モデルを更新する。
	m_modelRender3.Update();


	m_charaCon.Init(
		400.0f,
		300.0f,
		m_position
	);
	TowerEffect();
	m_enemy = FindGO<Enemy>("enemy");
	m_game = FindGO<Game>("game");

	return true;
}


void Tower::Collision()
{
	if (m_towerState == enTowerState_DamageInvalid)
	{
		return;
	}

	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			//Hpを減らす
			m_hp -= 10;
			if (m_hp <= 0) {
				m_game->GameOverNotice();
				TowerBreak = true;
				return;
			}
			else
			{
				m_towerState = enTowerState_DamageInvalid;
				return;
			}
		}
	}
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("enemy_magic");
	for (auto collision : collisions2)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			//Hpを減らす
			m_hp -= 10;
			if (m_hp <= 0) {
				m_game->GameOverNotice();
				TowerBreak = true;
				return;
			}
			else
			{
				m_towerState = enTowerState_DamageInvalid;
				return;
			}
		}
	}
}

void Tower::BreakEffect()
{
	effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(8);
	effectEmitter->SetScale(Vector3(5.0f, 10.0f, 5.0f));
	Vector3 effectPosition = m_position;
	effectEmitter->SetPosition(effectPosition);
	effectPosition.y += 100.0f;
	effectEmitter->Play();

	SoundSource* m_se = NewGO<SoundSource>(0);
	m_se->Init(6);
	m_se->SetVolume(0.5f);
	m_se->Play(false);

}

void  Tower::TowerEffect()
{
	effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(9);
	effectEmitter->SetScale(Vector3(10.0f, 10.0f, 10.0f));
	Vector3 effectPosition = m_position;
	effectPosition.y += 1800.0f;
	effectEmitter->SetPosition(effectPosition);
	effectEmitter->Play();
}

void Tower::DamageInvalid()
{
	if (m_towerState != enTowerState_DamageInvalid)
	{
		return;
	}
	damageinvalidtimer -= g_gameTime->GetFrameDeltaTime();
	if (damageinvalidtimer <= 0.0f)
	{
		damageinvalidtimer = 1.0f;
		m_towerState = enTowerState_Idle;
	}
}

void Tower::ProcessCommonStateTransition()
{
	switch (m_BreakState)
	{
	case Tower::enBreakState_NoBreak:
		if (m_hp <= 100)
		{
			m_BreakState = enBreakState_HelfBreak;
			BreakEffect();
		}
		break;
	case Tower::enBreakState_HelfBreak:
		if (m_hp <= 0)
		{
			m_BreakState = enBreakState_AllBreak;
			BreakEffect();
		}
		else if (m_hp > 100)
		{
			m_BreakState = enBreakState_NoBreak;
		}
		break;
	case Tower::enBreakState_AllBreak:
		break;
	default:
		break;
	}
}

void Tower::Update()
{
	ProcessCommonStateTransition();
	DamageInvalid();
	Collision();
	m_modelRender.Update();
	m_modelRender2.Update();
	m_modelRender3.Update();
}

void Tower::Render(RenderContext& rc)
{
	switch (m_BreakState)
	{
	case Tower::enBreakState_NoBreak:
		m_modelRender.Draw(rc);
		break;
	case Tower::enBreakState_HelfBreak:
		m_modelRender2.Draw(rc);
		break;
	case Tower::enBreakState_AllBreak:
		m_modelRender3.Draw(rc);
		break;
	default:
		break;
	}
}
