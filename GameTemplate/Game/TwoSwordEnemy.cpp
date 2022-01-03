#include "stdafx.h"
#include "TwoSwordEnemy.h"
#include "Player.h"
#include "Tower.h"
#include "Game.h"

TwoSwordEnemy::TwoSwordEnemy()
{

}

TwoSwordEnemy::~TwoSwordEnemy()
{

}

bool TwoSwordEnemy::Start()
{
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/2sword/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Assult].Load("Assets/animData/2sword/run.tka");
	m_animationClips[enAnimationClip_Assult].SetLoopFlag(true);
	m_animationClips[enAnimationClip_QuickAttack].Load("Assets/animData/2sword/quickattack.tka");
	m_animationClips[enAnimationClip_QuickAttack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_HeavyAttack].Load("Assets/animData/2sword/heavyattack.tka");
	m_animationClips[enAnimationClip_HeavyAttack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_ReceiveDamage].Load("Assets/animData/2sword/damage.tka");
	m_animationClips[enAnimationClip_ReceiveDamage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/2sword/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	m_animationClips[enAnimationClip_ElectricShock].Load("Assets/animData/2sword/electric.tka");
	m_animationClips[enAnimationClip_ElectricShock].SetLoopFlag(false);

	m_modelRender.Init("Assets/modelData/enemy/2sword/2sword.tkm",
		m_animationClips,
		enAnimationClip_Num,
		enModelUpAxisZ
	);

	m_charaCon.Init(
		120.0f,			//半径
		220.0f,			//高さ
		m_position      //座標
	);

	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale(m_scale);
	m_modelRender.Update();

	m_swordBoneId = m_modelRender.FindBoneID(L"Rsword");
	m_swordBoneId2 = m_modelRender.FindBoneID(L"Lsword");
	//アニメーションイベント用の関数を設定する
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	g_soundEngine->ResistWaveFileBank(32, "Assets/sound/enemydamage.wav");

	m_player = FindGO<Player>("player");
	m_tower = FindGO<Tower>("tower");
	m_game = FindGO<Game>("game");
	return true;
}

void TwoSwordEnemy::Update()
{
	ElectricShock();
	Assult();
	Rotation();
	Collision();
	Attack();
	ManageState();
	PlayAnimation();
	m_modelRender.Update();
}

const bool TwoSwordEnemy::IsCanPlayerAttack() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	if (diff.LengthSq() <= 600.0f * 600.0f)
	{
		return true;
	}
	return false;
}
const bool TwoSwordEnemy::IsCanTowerAttack() const
{
	Vector3 diff = m_tower->GetPosition() - m_position;
	if (diff.LengthSq() <= 800.0f * 800.0f)
	{
		return true;
	}
	return false;
}

const bool TwoSwordEnemy::IsCanQuickAttack() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	if (diff.LengthSq() <= 450.0f * 450.0f)
	{
		return true;
	}
	return false;
}

void TwoSwordEnemy::ElectricShock()
{
	if (m_enemyState != enEnemyState_ElectricShock)
	{
		return;
	}
	electricshocktimer -= g_gameTime->GetFrameDeltaTime();
}

void TwoSwordEnemy::Assult()
{
	//突撃ステートではなかったら
	if (m_enemyState != enEnemyState_Assult)
	{
		//何もしない
		return;
	}
	Vector3 modelPosition = m_position;
	Vector3 diff_tower = m_tower->GetPosition() - m_position;
	diff_tower.Normalize();
	m_moveSpeed.x = diff_tower.x * 300.0f;
	m_moveSpeed.z = diff_tower.z * 300.0f;
	m_moveSpeed.y -= 6000.0f * g_gameTime->GetFrameDeltaTime();
	m_modelRender.SetPosition(m_position);
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
}

void TwoSwordEnemy::Rotation()
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

void TwoSwordEnemy::Attack()
{
	if (m_enemyState != enEnemyState_QuickAttack &&
		m_enemyState != enAnimationClip_HeavyAttack)
	{
		return;
	}
	if (m_isUnderAttack == true)
	{
		if (m_enemyState == enEnemyState_QuickAttack)
		{
			MakeQuickAttackCollision();
		}
		else {
			MakeHeavyAttackCollision();
		}
	}
}

void TwoSwordEnemy::MakeQuickAttackCollision()
{
	auto collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	collisionObject->CreateBox(collisionPosition,
		Quaternion::Identity,
		Vector3(120.0f, 240.0f, 20.0f));
	collisionObject->SetName("quick_attack");
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
	collisionObject->SetWorldMatrix(matrix);
}

void TwoSwordEnemy::MakeHeavyAttackCollision()
{
	auto collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	collisionObject->CreateBox(collisionPosition,
		Quaternion::Identity,
		Vector3(120.0f, 400.0f, 20.0f));
	collisionObject->SetName("heavy_attack");
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId2)->GetWorldMatrix();
	collisionObject->SetWorldMatrix(matrix);
}

void TwoSwordEnemy::Collision()
{
	if (m_enemyState == enEnemyState_ReceiveDamage ||
		m_enemyState == enEnemyState_Down)
	{
		return;
	}

	const auto& collisions3 = g_collisionObjectManager->FindCollisionObjects("item_thunder");
	for (auto collision : collisions3)
	{
		if (collision->IsHit(m_charaCon))
		{
			m_enemyState = enEnemyState_ElectricShock;
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
			if (m_player->m_mp < 40)
			{
				m_player->m_mp += 5;
			}
			if (m_hp <= 0)
			{
				//ダウンステートへ
				m_enemyState = enEnemyState_Down;
		
			}
			else
			{
				//被ダメージステートへ
				m_enemyState = enEnemyState_ReceiveDamage;
			}
		}
	}

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
				m_enemyState = enEnemyState_Down;
		
			}
			else
			{
				m_enemyState = enEnemyState_ReceiveDamage;
			}
		}
	}
}

void TwoSwordEnemy::PlayAnimation()
{
	switch (m_enemyState)
	{
		//待機
	case enEnemyState_Idle:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
	case enEnemyState_Assult:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Assult, 0.1f);
		break;
	case enEnemyState_QuickAttack:
		m_modelRender.SetAnimationSpeed(1.1f);
		m_modelRender.PlayAnimation(enAnimationClip_QuickAttack, 0.1f);
		break;
	case enEnemyState_HeavyAttack:
		m_modelRender.SetAnimationSpeed(0.7f);
		m_modelRender.PlayAnimation(enAnimationClip_HeavyAttack, 0.1f);
		break;
	case enEnemyState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(1.4f);
		m_modelRender.PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
		break;
	case enEnemyState_Down:
		m_modelRender.SetAnimationSpeed(1.7f);
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	case enEnemyState_ElectricShock:
		m_modelRender.SetAnimationSpeed(0.5f);
		m_modelRender.PlayAnimation(enAnimationClip_ElectricShock, 0.1f);
		break;
	default:
		break;
	}
}

void TwoSwordEnemy::ManageState()
{
	switch (m_enemyState)
	{
		//待機
	case enEnemyState_Idle:
		ProcessIdleStateTransition();
		break;
	case enEnemyState_Assult:
		ProcessAssultStateTransition();
		break;
	case enEnemyState_QuickAttack:
		ProcessAttackStateTransition();
		break;
	case enEnemyState_HeavyAttack:
		ProcessAttackStateTransition();
		break;
	case enEnemyState_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
	case enEnemyState_Down:
		ProcessDownTransition();
		break;
	case enEnemyState_ElectricShock:
		ProcessElectricShockStateTransition();
	default:
		break;
	}
}

void TwoSwordEnemy::ProcessCommonStateTransition()
{
	Vector3 diff_player = m_player->GetPosition() - m_position;
	diff_player.Normalize();
	Vector3 diff_tower = m_tower->GetPosition() - m_position;
	diff_tower.Normalize();
	//プレイヤーを攻撃できるなら
	if (IsCanPlayerAttack() == true)
	{
		m_moveSpeed = diff_player * 300.0f;
		//プレイヤーが速い攻撃の攻撃範囲にいたら
		if (IsCanQuickAttack() == true)
		{
			int ram = rand() % 100;
			if (ram > 50)
			{
				//速い攻撃
				m_enemyState = enEnemyState_QuickAttack;
				m_isUnderAttack = false;
				return;
			}
			else
			{
				//待機
				m_enemyState = enEnemyState_Idle;
				return;
			}
		}
		//プレイヤーが攻撃範囲にいる＆遅い攻撃範囲にいる
		else
		{
			int ram = rand() % 100;
			if (ram > 50)
			{
				//遅い攻撃
				m_enemyState = enEnemyState_HeavyAttack;
				m_isUnderAttack = false;
				return;
			}
			else
			{
				//待機
				m_enemyState = enEnemyState_Idle;
				return;
			}
		}
	}
	//プレイヤーを攻撃出来ないなら
	else
	{
		if (IsCanTowerAttack() == true)
		{
			//塔を攻撃する
			m_moveSpeed = diff_tower * 300.0f;
			ram = rand() % 100;
			if (ram > 50)
			{
				m_enemyState = enEnemyState_QuickAttack;
				return;
			}
			else
			{
				m_enemyState = enEnemyState_HeavyAttack;
				return;
			}
		}

		m_enemyState = enEnemyState_Assult;
		return;
	}
}

void TwoSwordEnemy::ProcessAssultStateTransition()
{
	ProcessCommonStateTransition();
}

void TwoSwordEnemy::ProcessAttackStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する
		ProcessCommonStateTransition();
		return;
	}
}

void TwoSwordEnemy::ProcessReceiveDamageStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		if (electricshocktimer < 3.0f)
		{
			m_enemyState = enEnemyState_ElectricShock;
			return;
		}
		else {
			ProcessCommonStateTransition();
			return;
		}
	}
}

void TwoSwordEnemy::ProcessIdleStateTransition()
{
	ProcessCommonStateTransition();
	return;
}

void TwoSwordEnemy::ProcessDownTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		DeleteGO(this);
		m_game->m_DeadEnemynum++;
		return;
	}
}
void TwoSwordEnemy::ProcessElectricShockStateTransition()
{
	if (electricshocktimer <= 0.0f)
	{
		ProcessCommonStateTransition();
		electricshocktimer = 3.0f;
		return;
	}
}

void TwoSwordEnemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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

void TwoSwordEnemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}