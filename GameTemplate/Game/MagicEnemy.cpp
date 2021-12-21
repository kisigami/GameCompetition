#include "stdafx.h"
#include "MagicEnemy.h"

#include "Game.h"
#include "Player.h"
#include "Tower.h"
#include "IceBall.h"

#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"

MagicEnemy::MagicEnemy()
{

}

MagicEnemy::~MagicEnemy()
{

}

bool MagicEnemy::Start()
{
	//アニメーションの読み込み
	//待機
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/magic/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Assult].Load("Assets/animData/magic/run.tka");
	m_animationClips[enAnimationClip_Assult].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/magic/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_ReceiveDamage].Load("Assets/animData/magic/damage.tka");
	m_animationClips[enAnimationClip_ReceiveDamage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/magic/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	m_animationClips[enAnimationClip_ElectricShock].Load("Assets/animData/magic/eletric.tka");
	m_animationClips[enAnimationClip_ElectricShock].SetLoopFlag(false);

	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/enemy/magic/magicenemy.tkm",
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
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	m_player = FindGO<Player>("player");
	m_tower = FindGO<Tower>("tower");
	m_game = FindGO<Game>("game");

	return true;
}

void MagicEnemy::Update()
{
	Electric();
	Assult();
	Collision();
	Rotation();
	PlayAnimation();
	ManageState();
	m_modelRender.Update();
}

void MagicEnemy::Electric()
{
	if (m_enemyState != enMagicEnemyState_ElectricShock)
	{
		return;
	}
	electrictimer -= g_gameTime->GetFrameDeltaTime();
}

void MagicEnemy::MakeMagicBall()
{
	IceBall* iceBall = NewGO<IceBall>(0);
	Vector3 iceBallPosition = m_position;
	iceBallPosition.y += 55.0f;
	iceBallPosition += m_forward * 300.0f;
	iceBall->SetPosition(iceBallPosition);
	iceBall->SetRotation(m_rotation);
	iceBall->SetEnUser(IceBall::enUser_Enemy);
}


void MagicEnemy::Collision()
{
	if (m_enemyState == enMagicEnemyState_ReceiveDamage ||
		m_enemyState == enMagicEnemyState_Down)
	{
		return;
	}

	const auto& collisions3 = g_collisionObjectManager->FindCollisionObjects("item_thunder");
	for (auto collision : collisions3)
	{
		if (collision->IsHit(m_charaCon))
		{
			m_enemyState = enMagicEnemyState_ElectricShock;
		}
	}

	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			//Hpを減らす
			m_hp -= 1;
			if (m_player->m_mp < 40)
			{
				m_player->m_mp += 10;
			}

			//もし０なら
			if (m_hp <= 0)
			{
				//ダウンステートへ
				m_enemyState = enMagicEnemyState_Down;
			}
			//０以外なら
			else
			{
				//被ダメージステートへ
				m_enemyState = enMagicEnemyState_ReceiveDamage;
			}
		}
	}

	{
		const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("player_magic");
		for (auto collision : collisions2)
		{
			if (collision->IsHit(m_charaCon))
			{

				m_hp -= 1;
				//HPが0になったら。
				if (m_hp == 0)
				{

					m_enemyState = enMagicEnemyState_Down;
				}
				else {

					m_enemyState = enMagicEnemyState_ReceiveDamage;
				}
				return;
			}
		}
	}
}


void MagicEnemy::Assult()
{
	//突撃ステートではなかったら
	if (m_enemyState != enMagicEnemyState_Assult)
	{
		//何もしない
		return;
	}

	m_moveSpeed.y -= 10000.0f * g_gameTime->GetFrameDeltaTime();

	Vector3 modelPosition = m_position;
	//ちょっとだけモデルの座標を挙げる
	modelPosition.y += 2.5f;


	//塔へ進む
	Vector3 diff = m_tower->GetPosition() - m_position;
	diff.Normalize();
	m_moveSpeed.x = diff.x * 300.0f;
	m_moveSpeed.z = diff.z * 300.0f;	
	
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	m_modelRender.SetPosition(m_position);
}

void MagicEnemy::Rotation()
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

const bool MagicEnemy::SearchPlayer() const
{
	//プレイヤーを見つける
	Vector3 diff = m_player->GetPosition() - m_position;
	if (diff.LengthSq() <= 600.0 * 600.0f)
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

const bool MagicEnemy::IsCanTowerAttack() const
{
	Vector3 diff = m_tower->GetPosition() - m_position;
	if (diff.LengthSq() <= 1000.0f * 1000.0f)
	{
		return true;
	}
	return false;
}


const bool MagicEnemy::IsCanPlayerAttack() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	if (diff.LengthSq() <= 1000.0f * 1000.0f)
	{
		return true;
	}
	return false;
}


void MagicEnemy::PlayAnimation()
{
	switch (m_enemyState)
	{
		//待機
	case enMagicEnemyState_Idle:
		m_modelRender.SetAnimationSpeed(5.5f);
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
	case enMagicEnemyState_Assult:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Assult, 0.1f);
		break;
	case enMagicEnemyState_Attack:
		m_modelRender.SetAnimationSpeed(0.7f);
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
	case enMagicEnemyState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(1.7f);
		m_modelRender.PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
		break;
	case enMagicEnemyState_ElectricShock:
		m_modelRender.SetAnimationSpeed(0.5f);
		m_modelRender.PlayAnimation(enAnimationClip_ElectricShock, 0.1f);
		break;
	case enMagicEnemyState_Down:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	default:
		break;
	}
}

void MagicEnemy::ManageState()
{
	switch (m_enemyState)
	{
		//待機
	case enMagicEnemyState_Idle:
		ProcessIdleStateTransition();
		break;
	case enMagicEnemyState_Assult:
		ProcessAssultStateTransition();
		break;
	case enMagicEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
	case enMagicEnemyState_ReceiveDamage:
		ProcessReceiveDamageTransition();
		break;
	case enMagicEnemyState_ElectricShock:
		ProcessElectricShockStateTransition();
		break;
	case enMagicEnemyState_Down:
		ProcessDownTransition();
		break;
	default:
		break;
	}
}

void MagicEnemy::ProcessCommonStateTransition()
{
	//エネミーはプレイヤーへの攻撃より塔への攻撃を優先する
	Vector3 diff_player = m_player->GetPosition() - m_position;
	Vector3 diff_tower = m_tower->GetPosition() - m_position;

	//プレイヤーへの攻撃範囲内だったら
	if (IsCanPlayerAttack() == true)
	{
		//プレイヤーにベクトルを向ける
		diff_player.Normalize();
		m_moveSpeed = diff_player * 300.0f;
		int ram = rand() % 100;
		if (ram > 10)
		{
			//攻撃する
			m_enemyState = enMagicEnemyState_Attack;
			return;
		}
		else
		{
			//待機する
			m_enemyState = enMagicEnemyState_Idle;
			return;
		}
	}


	if (IsCanTowerAttack() == true)
	{
		//プレイヤーにベクトルを向ける
		diff_tower.Normalize();
		m_moveSpeed = diff_tower * 300.0f;
		int ram = rand() % 100;
		if (ram > 10)
		{
			//攻撃する
			m_enemyState = enMagicEnemyState_Attack;
			return;
		}
		else
		{
			//待機する
			m_enemyState = enMagicEnemyState_Idle;
			return;
		}
	}
	else
	{
		m_enemyState = enMagicEnemyState_Assult;
		return;
	}
}

void MagicEnemy::ProcessIdleStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{ 		//他のステートに遷移する
		ProcessCommonStateTransition();
		return;
	}
}

void MagicEnemy::ProcessAssultStateTransition()
{
	//他のステートに遷移する
	ProcessCommonStateTransition();
	return;
}

void MagicEnemy::ProcessAttackStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する
		ProcessCommonStateTransition();
		return;
	}
}

void MagicEnemy::ProcessReceiveDamageTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		if (electrictimer < 9.0f)
		{
			m_enemyState = enMagicEnemyState_ElectricShock;
			return;
		}
		else {
			ProcessCommonStateTransition();
			return;
		}
	}
}

void MagicEnemy::ProcessElectricShockStateTransition()
{
	if (electrictimer <= 0.0f)
	{
		ProcessCommonStateTransition();
		electrictimer = 9.0f;
		return;
	}
}

void MagicEnemy::ProcessDownTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する
		DeleteGO(this);
		m_game->m_DeadEnemynum++;
		return;
	}
}

void MagicEnemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"magic_attack") == 0)
	{
		MakeMagicBall();
	}


}


void MagicEnemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
