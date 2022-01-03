#include "stdafx.h"
#include "PlayerAttackEnemy.h"

#include "Game.h"
#include "Player.h"

#include "collision/CollisionObject.h"



PlayerAttackEnemy::PlayerAttackEnemy()
{

}

PlayerAttackEnemy::~PlayerAttackEnemy()
{

}																																																																																																										

bool PlayerAttackEnemy::Start()
{
	//アニメーションの読み込み
	//待機
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/playerattackenemy/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Assult].Load("Assets/animData/playerattackenemy/run.tka");
	m_animationClips[enAnimationClip_Assult].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/playerattackenemy/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_ReceiveDamage].Load("Assets/animData/playerattackenemy/damage.tka");
	m_animationClips[enAnimationClip_ReceiveDamage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/playerattackenemy/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	m_animationClips[enAnimationClip_ElectricShock].Load("Assets/animData/playerattackenemy/electric.tka");
	m_animationClips[enAnimationClip_ElectricShock].SetLoopFlag(false);

	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/enemy/playerattackenemy/playerattackenemy.tkm",
		m_animationClips,
		enAnimationClip_Num,
		enModelUpAxisZ
	);

	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	m_modelRender.Update();

	//キャラクターコントローラー
	m_charaCon.Init(
		70.0f,			//半径
		200.0f,			//高さ
		m_position      //座標
	);

	//剣のボーンのIDを取得
	m_swordBoneId = m_modelRender.FindBoneID(L"sword");

	//アニメーションイベント用の関数を設定する
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	g_soundEngine->ResistWaveFileBank(32, "Assets/sound/enemydamage.wav");

	m_player = FindGO<Player>("player");
	m_game = FindGO<Game>("game");
	return true;
}

void PlayerAttackEnemy::Update()
{
	Electric();
	Attack();
	Assult();
	Collision();
	Rotation();
	PlayAnimation();
	ManageState();
	m_modelRender.Update();
}

void PlayerAttackEnemy::Electric()
{
	if (m_enemyState != enPlayerAttackEnemyState_ElectricShock)
	{
		return;
	}
	electrictimer -= g_gameTime->GetFrameDeltaTime();
}

void PlayerAttackEnemy::Attack()
{
	if (m_enemyState != enPlayerAttackEnemyState_Attack)
	{
		return;
	}
	if (m_isUnderAttack == true)
	{
		MakeAttackCollision();
	}
}

void PlayerAttackEnemy::MakeAttackCollision()
{
	auto collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	collisionPosition += m_forward * 50.0f;
	collisionObject->CreateBox(collisionPosition,
		Quaternion::Identity,
		Vector3(170.0f, 20.0f, 20.0f));
	collisionObject->SetName("enemy_attack");
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
	collisionObject->SetWorldMatrix(matrix);
}

void PlayerAttackEnemy::MakeDamageEffect()
{
	EffectEmitter* m_effectEmitter;
	m_effectEmitter = NewGO<EffectEmitter>(0);
	Vector3 EffectPosition = m_position;
	EffectPosition.y += 180.0f;
	m_effectEmitter->SetPosition(EffectPosition);
	m_effectEmitter->SetRotation(m_rotation);
	Vector3 EffectScale = m_scale;
	m_effectEmitter->SetScale(Vector3(0.5f, 1.0f, 1.0f));
	m_effectEmitter->Init(11);
	m_effectEmitter->Play();
}

void PlayerAttackEnemy::Collision()
{
	if (m_enemyState == enPlayerAttackEnemyState_ReceiveDamage ||
		m_enemyState == enPlayerAttackEnemyState_Down)
	{
		return;
	}

	const auto& collisions3 = g_collisionObjectManager->FindCollisionObjects("item_thunder");
	for (auto collision : collisions3)
	{
		if (collision->IsHit(m_charaCon))
		{
			m_enemyState = enPlayerAttackEnemyState_ElectricShock;
		}
	}

	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			SoundSource* m_se = NewGO<SoundSource>(0);
			m_se->Init(32);
			m_se->SetVolume(0.3f);
			m_se->Play(false);
			//Hpを減らす
			m_hp -= 1;
			MakeDamageEffect();
			if (m_player->m_mp < 40)
			{
				m_player->m_mp += 5;
			}
			//もし０なら
			if (m_hp <= 0)
			{
				//ダウンステートへ
				m_enemyState = enPlayerAttackEnemyState_Down;
			}
			//０以外なら
			else
			{
				//被ダメージステートへ
				m_enemyState = enPlayerAttackEnemyState_ReceiveDamage;
			}
		}
	}

	{
		const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("player_magic");
		for (auto collision : collisions2)
		{
			if (collision->IsHit(m_charaCon))
			{
				SoundSource* m_se = NewGO<SoundSource>(0);
				m_se->Init(32);
				m_se->SetVolume(0.3f);
				m_se->Play(false);

				m_hp -= 1;
				//HPが0になったら。
				if (m_hp == 0)
				{
					m_enemyState = enPlayerAttackEnemyState_Down;
				
				}
				else {

					m_enemyState = enPlayerAttackEnemyState_ReceiveDamage;
				}
				return;
			}
		}
	}
}


void PlayerAttackEnemy::Assult()
{
	//突撃ステートではなかったら
	if (m_enemyState != enPlayerAttackEnemyState_Assult)
	{
		//何もしない
		return;
	}

	m_moveSpeed.y -= 10000.0f * g_gameTime->GetFrameDeltaTime();
	Vector3 modelPosition = m_position;
	modelPosition.y += 0.5f;

	Vector3 diff = m_player->GetPosition() - m_position;
	diff.Normalize();
	m_moveSpeed.x = diff.x * 300.0f;
	m_moveSpeed.z = diff.z * 300.0f;

	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	m_modelRender.SetPosition(m_position);
}

void PlayerAttackEnemy::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		return;
	}

	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	m_rotation.SetRotationY(-angle);

	m_modelRender.SetRotation(m_rotation);

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

const bool PlayerAttackEnemy::SearchPlayer() const
{
	//プレイヤーを見つける
	Vector3 diff = m_player->GetPosition() - m_position;
	if (diff.LengthSq() <= 4000.0 * 4000.0f)
	{
		diff.Normalize();
		float cos = m_forward.Dot(diff);
		float angle = acosf(cos);
		if (angle <= (Math::PI / 180.0f) * 120.0f)
		{
			return true;
		}
	}
	//プレイヤーを見つけてない
	return false;
}

const bool PlayerAttackEnemy::IsCanPlayerAttack() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	if (diff.LengthSq() <= 300.0f * 300.0f)
	{
		return true;
	}
	return false;
}


void PlayerAttackEnemy::PlayAnimation()
{
	switch (m_enemyState)
	{
		//待機
	case enPlayerAttackEnemyState_Idle:
		m_modelRender.SetAnimationSpeed(4.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
	case enPlayerAttackEnemyState_Assult:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Assult, 0.1f);
		break;
	case enPlayerAttackEnemyState_Attack:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.4f);
		break;
	case enPlayerAttackEnemyState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(1.5f);
		m_modelRender.PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
		break;
	case enPlayerAttackEnemyState_ElectricShock:
		m_modelRender.SetAnimationSpeed(0.5f);
		m_modelRender.PlayAnimation(enAnimationClip_ElectricShock, 0.1f);
		break;
	case enPlayerAttackEnemyState_Down:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	default:
		break;
	}
}

void PlayerAttackEnemy::ManageState()
{
	switch (m_enemyState)
	{
		//待機
	case enPlayerAttackEnemyState_Idle:
		ProcessIdleStateTransition();
		break;
	case enPlayerAttackEnemyState_Assult:
		ProcessAssultStateTransition();
		break;
	case enPlayerAttackEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
	case enPlayerAttackEnemyState_ReceiveDamage:
		ProcessReceiveDamageTransition();
		break;
	case enPlayerAttackEnemyState_ElectricShock:
		ProcessElectricShockTransition();
		break;
	case enPlayerAttackEnemyState_Down:
		ProcessDownTransition();
		break;
	default:
		break;
	}
}

void PlayerAttackEnemy::ProcessCommonStateTransition()
{
	//エネミーはプレイヤーへの攻撃を優先する
	Vector3 diff_player = m_player->GetPosition() - m_position;
	//Vector3 diff_tower = m_tower->GetPosition() - m_position;

	//プレイヤーへの攻撃ができない距離だったら
	if (IsCanPlayerAttack() == false)
	{
		//プレイヤーを追いかける
		m_enemyState = enPlayerAttackEnemyState_Assult;
		return;
	}
	//攻撃できる距離なら
	else
	{
		//プレイヤーを向く
		diff_player.Normalize();
		m_moveSpeed = diff_player * 300.0f;
		int ram = rand() % 100;
		if (ram > 80)
		{
			//攻撃する
			m_enemyState = enPlayerAttackEnemyState_Attack;
			m_isUnderAttack = false;
			return;
		}
		else
		{
			//待機する
			m_enemyState = enPlayerAttackEnemyState_Idle;
			return;
		}
	}
}

void PlayerAttackEnemy::ProcessElectricShockTransition()
{
	if (electrictimer <= 0.0f)
	{
		ProcessCommonStateTransition();
		electrictimer = 9.0f;
		return;
	}
}

void PlayerAttackEnemy::ProcessIdleStateTransition()
{
	//アニメーションの再生が終わったら
	//if (m_modelRender.IsPlayingAnimation() == false)
	//{ 		//他のステートに遷移する
		ProcessCommonStateTransition();
		return;
	//}
}

void PlayerAttackEnemy::ProcessAssultStateTransition()
{
	//攻撃範囲に塔、プレイヤーがいるなら
	//if (IsCanPlayerAttack() == true || IsCanTowerAttack() == true)
	//{
		//他のステートに遷移する
		ProcessCommonStateTransition();
		return;
	//}
}

void PlayerAttackEnemy::ProcessAttackStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する
		ProcessCommonStateTransition();
		return;
	}
}

void PlayerAttackEnemy::ProcessReceiveDamageTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		if (electrictimer < 9.0f)
		{
			m_enemyState = enPlayerAttackEnemyState_ElectricShock;
			return;
		}
		else {
			ProcessCommonStateTransition();
			return;
		}
	}
}

void PlayerAttackEnemy::ProcessDownTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する
		DeleteGO(this);
		m_game->m_DeadEnemynum++;
		return;
	}
}

void PlayerAttackEnemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"attack_start") == 0)
	{
		m_isUnderAttack = true;
	}

	else if (wcscmp(eventName, L"attack_end") == 0)
	{
		m_isUnderAttack = false;
	}
}


void PlayerAttackEnemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);

}
