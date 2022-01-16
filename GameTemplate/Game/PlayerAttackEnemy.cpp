#include "stdafx.h"
#include "PlayerAttackEnemy.h"

#include "Game.h"
#include "Player.h"

#include "collision/CollisionObject.h"

namespace
{
	const float GRAVITY = 6000.0f;               //重力
	const float MOVE_SPEED = 300.0f;             //移動速度
	const float PLAYER_ATTACK_RANGE = 500.0f;    //プレイヤーを攻撃できる距離
	const float RESTRAINT_TIME = 8.0f;           //拘束時間
}

PlayerAttackEnemy::PlayerAttackEnemy()
{

}

PlayerAttackEnemy::~PlayerAttackEnemy()
{

}																																																																																																										

bool PlayerAttackEnemy::Start()
{
	//アニメーションの読み込み
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/playerattackenemy/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Assault].Load("Assets/animData/playerattackenemy/run.tka");
	m_animationClips[enAnimationClip_Assault].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/playerattackenemy/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_ReceiveDamage].Load("Assets/animData/playerattackenemy/damage.tka");
	m_animationClips[enAnimationClip_ReceiveDamage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/playerattackenemy/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	m_animationClips[enAnimationClip_ReceiveRestraint].Load("Assets/animData/playerattackenemy/electric.tka");
	m_animationClips[enAnimationClip_ReceiveRestraint].SetLoopFlag(false);

	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/enemy/playerattackenemy/playerattackenemy.tkm",
		m_animationClips,
		enAnimationClip_Num,
		enModelUpAxisZ
	);

	//座標、回転、大きさを設定
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);

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

	//SEを再生する
	g_soundEngine->ResistWaveFileBank(32, "Assets/sound/enemydamage.wav");

	m_player = FindGO<Player>("player");
	m_game = FindGO<Game>("game");
	return true;
}

void PlayerAttackEnemy::Update()
{
	//移動処理
	Move();
	//回転処理
	Rotation();
	//攻撃処理
	Attack();
	//当たり判定
	Collision();
	//拘束処理
	Restraint();
	//アニメーションの再生
	PlayAnimation();
	//ステートの遷移処理
	ManageState();

	//モデルの更新
	m_modelRender.Update();
}

void PlayerAttackEnemy::Move()
{
	//突撃ステートではなかったら
	if (m_playerattackenemyState != enPlayerAttackEnemyState_Assault)
	{
		//何もしない
		return;
	}

	//重力
	m_moveSpeed.y -= 10000.0f * g_gameTime->GetFrameDeltaTime();

	//プレイヤーへの突撃処理
	Vector3 diff = m_player->GetPosition() - m_position;
	diff.Normalize();
	//移動速度
	m_moveSpeed.x = diff.x * 300.0f;
	m_moveSpeed.z = diff.z * 300.0f;

	//座標の更新
	m_modelRender.SetPosition(m_position);
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
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

void PlayerAttackEnemy::Attack()
{
	//攻撃ステートではなかったら
	if (m_playerattackenemyState != enPlayerAttackEnemyState_Attack)
	{
		//何もしない
		return;
	}
	//フラグがtrueなら
	if (m_isUnderAttack == true)
	{
		//攻撃の当たり判定を作成する
		MakeAttackCollision();
	}
}

void PlayerAttackEnemy::MakeAttackCollision()
{
	//当たり判定を作成する
	auto collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	//座標を少し前にする
	collisionPosition += m_forward * 50.0f;
	//箱状の当たり判定を作成する
	collisionObject->CreateBox(collisionPosition,
		Quaternion::Identity,
		Vector3(170.0f, 20.0f, 20.0f));
	//コリジョンに名前を付ける
	collisionObject->SetName("enemy_attack");
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
	collisionObject->SetWorldMatrix(matrix);
}

void PlayerAttackEnemy::Collision()
{
	//被ダメージステートかダウンステートだったら
	if (m_playerattackenemyState == enPlayerAttackEnemyState_ReceiveDamage ||
		m_playerattackenemyState == enPlayerAttackEnemyState_Down)
	{
		//何もしない
		return;
	}

	//プレイヤーの拘束魔法の当たり判定
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("item_thunder");
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			//拘束ステートへ
			m_playerattackenemyState = enPlayerAttackEnemyState_ReceiveRestraint;
		}
	}

	//プレイヤーの近接攻撃の当たり判定
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("player_attack");
	for (auto collision : collisions2)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			//SEを再生する
			SoundSource* m_se = NewGO<SoundSource>(0);
			m_se->Init(32);
			m_se->SetVolume(0.3f);
			m_se->Play(false);
			//Hpを1減らす
			m_hp -= 1;
			//被ダメージエフェクトを再生する
			MakeDamageEffect();
			//プレイヤーのMpが40以下だったら
			if (m_player->m_mp < 40)
			{
				//プレイヤーのMpを5増やす
				m_player->m_mp += 5;
			}
			//Hpが0だったら
			if (m_hp == 0)
			{
				//ダウンステートへ
				m_playerattackenemyState = enPlayerAttackEnemyState_Down;
			}
			//Hpが0ではなかったら
			else
			{
				//被ダメージステートへ
				m_playerattackenemyState = enPlayerAttackEnemyState_ReceiveDamage;
			}
		}
	}

	//プレイヤーの魔法攻撃の当たり判定
	const auto& collisions3 = g_collisionObjectManager->FindCollisionObjects("player_magic");
	for (auto collision : collisions3)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			//SEを再生する
			SoundSource* m_se = NewGO<SoundSource>(0);
			m_se->Init(32);
			m_se->SetVolume(0.3f);
			m_se->Play(false);
			//Hpを1減らす
			m_hp -= 1;
			//Hpが0だったら
			if (m_hp == 0)
			{
				//ダウンステートへ
				m_playerattackenemyState = enPlayerAttackEnemyState_Down;
			}
			//Hpが0ではなかったら
			else 
			{
				//被ダメージステートへ
				m_playerattackenemyState = enPlayerAttackEnemyState_ReceiveDamage;
			}
		}
	}
}

void PlayerAttackEnemy::Restraint()
{
	//拘束ステートではなかったら
	if (m_playerattackenemyState != enPlayerAttackEnemyState_ReceiveRestraint)
	{
		//何もしない
		return;
	}
	//拘束タイマーを減らす
	electrictimer -= g_gameTime->GetFrameDeltaTime();
}

void PlayerAttackEnemy::MakeDamageEffect()
{
	//エフェクトを作成する
	EffectEmitter* m_effectEmitter;
	m_effectEmitter = NewGO<EffectEmitter>(0);
	Vector3 EffectPosition = m_position;
	Vector3 EffectScale = m_scale;
	//座標を少し上にする
	EffectPosition.y += 180.0f;
	m_effectEmitter->SetPosition(EffectPosition);
	//大きさを設定する
	m_effectEmitter->SetScale(Vector3(0.5f, 1.0f, 1.0f));
	//エフェクトを再生する
	m_effectEmitter->Init(11);
	m_effectEmitter->Play();
}

void PlayerAttackEnemy::PlayAnimation()
{
	switch (m_playerattackenemyState)
	{
		//待機ステートの時
	case enPlayerAttackEnemyState_Idle:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.0f);
		//待機アニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
		//突撃ステートの時
	case enPlayerAttackEnemyState_Assault:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.0f);
		//突撃アニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_Assault, 0.1f);
		break;
		//攻撃ステートの時
	case enPlayerAttackEnemyState_Attack:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.0f);
		//攻撃アニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.4f);
		break;
		//被ダメージステートの時
	case enPlayerAttackEnemyState_ReceiveDamage:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.5f);
		//被ダメージアニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
		break;
		//拘束ステートの時
	case enPlayerAttackEnemyState_ReceiveRestraint:
		//再生速度
		m_modelRender.SetAnimationSpeed(0.5f);
		//拘束アニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_ReceiveRestraint, 0.1f);
		break;
		//ダウンステートの時
	case enPlayerAttackEnemyState_Down:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.0f);
		//ダウンステートを再生する
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	default:
		break;
	}
}

void PlayerAttackEnemy::ManageState()
{
	switch (m_playerattackenemyState)
	{
		//待機ステートの時
	case enPlayerAttackEnemyState_Idle:
		//待機ステートのステート遷移処理
		ProcessIdleStateTransition();
		break;
		//突撃ステートの時
	case enPlayerAttackEnemyState_Assault:
		//突撃ステートのステート遷移処理
		ProcessAssultStateTransition();
		break;
	case enPlayerAttackEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
	case enPlayerAttackEnemyState_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
	case enPlayerAttackEnemyState_ReceiveRestraint:
		ProcessReceiveRestraintStateTransition();
		break;
	case enPlayerAttackEnemyState_Down:
		ProcessDownStateTransition();
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
		m_playerattackenemyState = enPlayerAttackEnemyState_Assault;
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
			m_playerattackenemyState = enPlayerAttackEnemyState_Attack;
			m_isUnderAttack = false;
			return;
		}
		else
		{
			//待機する
			m_playerattackenemyState = enPlayerAttackEnemyState_Idle;
			return;
		}
	}
}

void PlayerAttackEnemy::ProcessReceiveRestraintStateTransition()
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

void PlayerAttackEnemy::ProcessReceiveDamageStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		if (electrictimer < 9.0f)
		{
			m_playerattackenemyState = enPlayerAttackEnemyState_ReceiveRestraint;
			return;
		}
		else {
			ProcessCommonStateTransition();
			return;
		}
	}
}

void PlayerAttackEnemy::ProcessDownStateTransition()
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

const bool PlayerAttackEnemy::IsCanPlayerAttack() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	if (diff.LengthSq() <= 300.0f * 300.0f)
	{
		return true;
	}
	return false;
}

void PlayerAttackEnemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);

}
