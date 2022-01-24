#include "stdafx.h"
#include "Tower.h"
#include "Enemy.h"
#include "Game.h"
#include "GameOver.h"
#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

namespace
{
	const float ENEMYDAMAGE = 10.0f;
}

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
	g_soundEngine->ResistWaveFileBank(44, "Assets/sound/towerdamage.wav");
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
	//TowerEffect();
	m_enemy = FindGO<Enemy>("enemy");
	m_game = FindGO<Game>("game");

	return true;
}


void Tower::Collision()
{
	//ダメージ無効ステートだったら
	if (m_towerState == enTowerState_DamageInvalid)
	{
		//何もしない
		return;
	}

	//敵の攻撃の当たり判定
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			//Hpを10減らす
			float towerdurability = GetDurability();
			towerdurability -= ENEMYDAMAGE;
			SetDurability(towerdurability);

			//耐久値が0.0fより小さかったら
			if (GetDurability() <= 0.0f) 
			{
				//SEを再生する
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(44);
				se->SetVolume(0.4f);
				se->Play(false);
				//ダメージ無効ステートへ
				m_towerState = enTowerState_DamageInvalid;
				return;
			}
			//耐久値が0.0fより大きかったら
			else {
				//ゲームオーバーを通知する
				m_game->GameOverNotice();
				//タワー破壊フラグをtrueにする
				m_towerBreakFrag = true;
				return;
			}
		}
	}

	//敵の魔法攻撃の当たり判定
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("enemy_magic");
	for (auto collision : collisions2)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			//Hpを減らす
			float towerdurability = GetDurability();
			towerdurability -= ENEMYDAMAGE;
			SetDurability(towerdurability);

			//タワーの耐久値が0.0fより小さかったら
			if (GetDurability() <= 0.0f) 
			{
				//ゲームオーバーを通知する
				m_game->GameOverNotice();
				//タワー破壊フラグがtrueにする
				m_towerBreakFrag = true;
				return;
			}
			//タワーの耐久値が0.0fより大きかったら
			else
			{
				//SEを再生する
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(44);
				se->SetVolume(0.4f);
				se->Play(false);
				//ダメージ無効ステートへ
				m_towerState = enTowerState_DamageInvalid;
				return;
			}
		}
	}

	//ボスの早い攻撃の当たり判定
	const auto& collisions3 = g_collisionObjectManager->FindCollisionObjects("quick_attack");
	for (auto collision : collisions3)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			//Hpを減らす
			float towerdurability = GetDurability();
			towerdurability -= 15;
			SetDurability(towerdurability);
			if (GetDurability() <= 0.0f) {
				m_game->GameOverNotice();
				m_towerBreakFrag = true;
				return;
			}
			else
			{
				SoundSource* m_se = NewGO<SoundSource>(0);
				m_se->Init(44);
				m_se->SetVolume(0.4f);
				m_se->Play(false);
				m_towerState = enTowerState_DamageInvalid;
				return;
			}
		}
	}

	//ボスの遅い攻撃の当たり判定
	const auto& collisions4 = g_collisionObjectManager->FindCollisionObjects("heavy_attack");
	for (auto collision : collisions4)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			//Hpを減らす
			float towerdurabiity = GetDurability();
			towerdurabiity -= 15.0f;
			SetDurability(towerdurabiity);

			if ( <= 0) {
				m_game->GameOverNotice();
				TowerBreak = true;
				return;
			}
			else
			{
				SoundSource* m_se = NewGO<SoundSource>(0);
				m_se->Init(44);
				m_se->SetVolume(0.4f);
				m_se->Play(false);
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
	//ダメージ無効ステート以外だったら
	if (m_towerState != enTowerState_DamageInvalid)
	{
		//何もしない
		return;
	}
	//ダメージ無効タイマーを減少させる
	damageinvalidtimer -= g_gameTime->GetFrameDeltaTime();
	//ダメージ無効タイマーが0.0fより小さかったら
	if (damageinvalidtimer <= 0.0f)
	{
		//タイマーを初期化する
		damageinvalidtimer = 1.0f;
		//待機ステートへ
		m_towerState = enTowerState_Idle;
	}
}

void Tower::ManageState()
{
	switch (m_BreakState)
	{
		//破壊されていないステートの時
	case enBreakState_NoBreak:
		//耐久値が100.0fより小さかったら
		if (GetDurability() <= 100)
		{
			//半壊ステートへ
			m_BreakState = enBreakState_HelfBreak;
			//破壊エフェクトを再生
			BreakEffect();
		}
		break;
		//半壊ステートの時
	case enBreakState_HelfBreak:
		//耐久値が0.0fより小さかったら
		if (GetDurability() <= 0)
		{
			//全壊ステートへ
			m_BreakState = enBreakState_AllBreak;
			//破壊エフェクトを再生する
			BreakEffect();
		}
		//耐久値が100.0fより大きかったら
		else if (GetDurability() > 100)
		{
			//破壊されていないステートへ
			m_BreakState = enBreakState_NoBreak;
		}
		break;
		//全壊ステートの時
	case Tower::enBreakState_AllBreak:
		break;
	}
}

void Tower::Update()
{
	/// <summary>
	/// 各ステートの遷移処理
	/// </summary>
	ManageState();
	/// <summary>
	/// ダメージ無効処理
	/// </summary>
	DamageInvalid();
	/// <summary>
	/// 敵の攻撃当たり判定処理
	/// </summary>
	Collision();

	//モデルの更新処理
	m_modelRender.Update();
	m_modelRender2.Update();
	m_modelRender3.Update();
}

void Tower::Render(RenderContext& rc)
{
	switch (m_BreakState)
	{
		//破壊されていないステートの時
	case Tower::enBreakState_NoBreak:
		//破壊されていないモデルを描画する
		m_modelRender.Draw(rc);
		break;
		//半壊ステートの時
	case Tower::enBreakState_HelfBreak:
		//半壊しているモデルを描画する
		m_modelRender2.Draw(rc);
		break;
		//全壊ステートの時
	case Tower::enBreakState_AllBreak:
		//全壊しているモデルを描画する
		m_modelRender3.Draw(rc);
		break;
	}
}
