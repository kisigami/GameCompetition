#include "stdafx.h"
#include "Player.h"
#include "Game.h"

#include "MagicBall.h"
#include "Tower.h"

#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

Player::Player()
{

}

Player::~Player()
{

}

bool Player::Start()
{
	//アニメーションを読み込む
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_UseItem].Load("Assets/animData/useitem.tka");
	m_animationClips[enAnimationClip_UseItem].SetLoopFlag(false);
	m_animationClips[enAnimationClip_FirstAttack].Load("Assets/animData/attack1.tka");
	m_animationClips[enAnimationClip_FirstAttack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_SecondAttack].Load("Assets/animData/attack2.tka");
	m_animationClips[enAnimationClip_SecondAttack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_ThirdAttack].Load("Assets/animData/attack3.tka");
	m_animationClips[enAnimationClip_ThirdAttack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Magic].Load("Assets/animData/magicattack.tka");
	m_animationClips[enAnimationClip_Magic].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/damage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_UseHeal].Load("Assets/animData/heal.tka");
	m_animationClips[enAnimationClip_UseHeal].SetLoopFlag(false);
	//モデルを読み込む
	m_modelRender.Init("Assets/modelData/player/player4.tkm"
		,m_animationClips, enAnimationClip_Num
	);

	//座標を設定する
	m_modelRender.SetPosition(m_position);
	//回転を設定する
	m_modelRender.SetRotation(m_rotation);
	//大きさを設定する
	m_modelRender.SetScale(m_scale);
	//キャラクタコントローラーの初期化
	m_charaCon.Init(
		60.0f,
		250.0f,
		m_position
	);

	//画像を読み込む
	m_sprintRender.Init("Assets/sprite/Item/No.dds", 128.0f, 128.0f);
	m_sprintRender.SetPosition(Vector3(850.0f, -450.0f, 0.0f));
	m_sprintRender2.Init("Assets/sprite/Item/Thunder.dds", 128.0f, 128.0f);
	m_sprintRender2.SetPosition(Vector3(850.0f, -450.0f, 0.0f));
	m_sprintRender3.Init("Assets/sprite/Item/Heal.dds", 128.0f, 128.0f);
	m_sprintRender3.SetPosition(Vector3(850.0f, -450.0f, 0.0f));
	m_itemWaku.Init("Assets/sprite/item/itemwaku.dds", 1920.0f, 1080.0f);
	m_itemStop.Init("Assets/sprite/item/itemstop.dds", 1920.0f, 1080.0f);
	m_itemHeal.Init("Assets/sprite/item/itemheal.dds", 1920.0f, 1080.0f);

	//剣のボーンIDを取得する
	m_swordBoneId = m_modelRender.FindBoneID(L"sword");

	//アニメーションイベント用の関数を設定する
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//各エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(0, u"Assets/efk/slash.efk");
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/efk/slash2.efk");
	EffectEngine::GetInstance()->ResistEffect(2, u"Assets/efk/slash3.efk");
	EffectEngine::GetInstance()->ResistEffect(3, u"Assets/efk/item.efk");
	EffectEngine::GetInstance()->ResistEffect(4, u"Assets/efk/use_item_thunder.efk");
	EffectEngine::GetInstance()->ResistEffect(5, u"Assets/efk/heal.efk");

	//各サウンドを読み込む
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/swordfuriorosi.wav");
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/sword2.wav");
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/sword3.wav");
	g_soundEngine->ResistWaveFileBank(4, "Assets/sound/playerreact.wav");
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/kaminari.wav");
	g_soundEngine->ResistWaveFileBank(8, "Assets/sound/heal.wav");
	g_soundEngine->ResistWaveFileBank(31, "Assets/sound/playerdamage.wav");
	g_soundEngine->ResistWaveFileBank(33, "Assets/sound/errer.wav");
	g_soundEngine->ResistWaveFileBank(29, "Assets/sound/sentaku.wav");
	g_soundEngine->ResistWaveFileBank(30, "Assets/sound/kettei.wav");

	m_tower = FindGO<Tower>("tower");
	m_game = FindGO<Game>("game");
	return true;
}

void Player::Update()
{
	if (m_mp > 40)
	{
		m_mp = 40;
	}

	Hit();
	//移動処理
	Move();
	//回転処理
	Rotation();
	//当たり判定
	Collision();
	//攻撃判定
	Attack();
	//連続攻撃
	//アイテム選択
	ChoiseItem();
	//アニメーションの再生
	PlayAnimation();
	//ステートの遷移処理
	ManageState();

	//モデルの更新
	m_modelRender.Update();

	//各画像の更新
	m_sprintRender.Update();
	m_sprintRender2.Update();
	m_sprintRender3.Update();

}

void Player::Hit()
{

	//攻撃ステート1なら
	if (m_playerState == enPlayerState_FirstAttack)
	{
		//タイマーを起動する
		move = 1;
	}
	//攻撃ステート2なら
	if (m_playerState == enPlayerState_SecondAttack)
	{
		//タイマーを起動する
		move2 = 1;
	}

	//攻撃タイマー１を起動
	if (move == 1)
	{
		m_attackTimer += g_gameTime->GetFrameDeltaTime() * 50.0f;
		//攻撃タイマーが0.01以上かつ5.0以内なら
		if (m_attackTimer > 0.001f && m_attackTimer <= 6.0f)
		{
			//攻撃タイプを1にする
			Type = 1;
		}
		//攻撃タイマーがそれ以外なら
		else
		{
			Type = 0;//攻撃タイプを0にする
			m_attackTimer = 0.0f;//タイマーをリセットする
			move = 0;
		}
	}
	if (move2 == 1)
	{
		m_attackTimer2 += g_gameTime->GetFrameDeltaTime() * 50.0f;

		if (m_attackTimer2 > 0.001f && m_attackTimer2 <= 10.0f)
		{
			Type = 2;
		}
		else
		{
			Type = 0;//攻撃タイプを0にする
			m_attackTimer2 = 0.0f;//タイマーをリセットする
			move2 = 0;
		}
	}

	//攻撃を食らったら攻撃タイプを0へ
	if (m_playerState == enPlayerState_ReceiveDamage)
	{
		Type = 0;
		m_attackTimer2 = 0.0f;
		move = 0;
		move2 = 0;
	}
}
void Player::Move()
{
	//移動できない状態ならば、移動処理を行わない
	if (IsEnableMove() == false)
	{
		return;
	}
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	float lStick_x = g_pad[0]->GetLStickXF();
	float lStick_y = g_pad[0]->GetLStickYF();
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	m_moveSpeed += cameraForward * lStick_y * 750.0f;
	m_moveSpeed += cameraRight * lStick_x * 750.0f;

	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {

		m_moveSpeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	modelPosition.y += 3.0f;
	m_modelRender.SetPosition(modelPosition);
}

void Player::Rotation()
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

void Player::Collision()
{
	//被ダメージステートなら
	if (m_playerState == enPlayerState_ReceiveDamage 
		|| m_playerState == enPlayerState_UseItem || m_playerState == enPlayerState_UseHeal)
	{
		//ダメージ判定をしない
		return;
	}
	//敵の攻撃用のコリジョンの配列を取得する
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			SoundSource* m_se = NewGO<SoundSource>(0);
			m_se->Init(31);
			m_se->SetVolume(0.2f);
			m_se->Play(false);
			//被ダメージステートに移行する
			m_playerState = enPlayerState_ReceiveDamage;
			//SEを再生する
		}
	}
	//敵の魔法攻撃のコリジョンを取得する
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("enemy_magic");
	//配列をfor文で回す
	for (auto collision : collisions2)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			SoundSource* m_se = NewGO<SoundSource>(0);
			m_se->Init(31);
			m_se->SetVolume(0.2f);
			m_se->Play(false);
			//被ダメージリステートに移行する
			m_playerState = enPlayerState_ReceiveDamage;
		}
	}

	const auto& collisions3 = g_collisionObjectManager->FindCollisionObjects("heavy_attack");
	//配列をfor文で回す
	for (auto collision : collisions3)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			SoundSource* m_se = NewGO<SoundSource>(0);
			m_se->Init(31);
			m_se->SetVolume(0.2f);
			m_se->Play(false);
			//被ダメージリステートに移行する
			m_playerState = enPlayerState_ReceiveDamage;
		}
	}

	const auto& collisions4 = g_collisionObjectManager->FindCollisionObjects("quick_attack");
	//配列をfor文で回す
	for (auto collision : collisions4)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			SoundSource* m_se = NewGO<SoundSource>(0);
			m_se->Init(31);
			m_se->SetVolume(0.2f);
			m_se->Play(false);
			//被ダメージリステートに移行する
			m_playerState = enPlayerState_ReceiveDamage;
		}
	}
}

void Player::Attack()
{
	//攻撃ステートではないなら
	if (m_playerState != enPlayerState_FirstAttack &&
		m_playerState != enPlayerState_SecondAttack &&
		m_playerState != enPlayerState_ThirdAttack)
	{
		//攻撃処理をしない
		return;
	}
	//攻撃判定中なら
	if (m_isUnderAttack == true)
	{
		//攻撃用のコリジョンを作成する
		MakeAttackCollision();
	}
}

void Player::MakeAttackCollision()
{
	//コリジョンオブジェクトを作成する
	auto collisionObject = NewGO<CollisionObject>(0);
	//座標を少し前にする？
	Vector3 collisionPosition = m_position;
	collisionPosition += m_forward * 50.0f;
	//ボックス状のコリジョンを作成する
	collisionObject->CreateBox(collisionPosition,
		Quaternion::Identity,
		Vector3(10.0f, 200.0f, 10.0f));
	//剣のボーンのワールド行列を取得する
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
	//剣のボーンのワールド行列をコリジョンに適用させる
	collisionObject->SetName("player_attack");
	collisionObject->SetWorldMatrix(matrix);
}

void Player::MakeFirstSlashingEffect()
{
	//エフェクトを作成する
	effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(0);
	effectEmitter->SetScale(Vector3::One * 3.0f);
	Vector3 effectPosition = m_position;
	//座標を少し上&前にする
	effectPosition.y += 210.0f;
	effectPosition += m_forward * 98.0f;
	effectEmitter->SetPosition(effectPosition);
	Quaternion effectRotation = m_rotation;
	//回転させる
	effectRotation.AddRotationDegY(540.0f);
	effectRotation.AddRotationDegX(-70.0f);
	effectEmitter->SetRotation(effectRotation);
	//エフェクトを再生する
	effectEmitter->Play();
	//SEを再生する
	SoundSource* m_se = NewGO<SoundSource>(0);
	m_se->Init(0);
	m_se->SetVolume(0.3f);
	m_se->Play(false);
}

void Player::MakeSecondSlashingEffect()
{
	//エフェクトを作成する
	effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(1);
	effectEmitter->SetScale(Vector3::One * 4.0f);
	Vector3 effectPosition = m_position;
	//エフェクトを少し上&前にする
	effectPosition.y += 175.0f;
	effectPosition += m_forward * 10.0f;
	effectEmitter->SetPosition(effectPosition);
	Quaternion effectRotation = m_rotation;
	//回転させる
	effectRotation.AddRotationDegY(490.0f);
	effectRotation.AddRotationDegX(-180.0f);
	effectRotation.AddRotationDegZ(0.0f);
	effectEmitter->SetRotation(effectRotation);
	//エフェクトを再生する
	effectEmitter->Play();
	//SEを再生する
	SoundSource* m_se = NewGO<SoundSource>(0);
	m_se->Init(1);
	m_se->SetVolume(0.3f);
	m_se->Play(false);
}

//コメント前
void Player::MakeThirdSlashingEffect()
{
	SoundSource* m_se = NewGO<SoundSource>(0);
	m_se->Init(2);
	m_se->SetVolume(0.5f);
	m_se->Play(false);
	effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(2);
	effectEmitter->SetScale(Vector3::One * 4.0f);
	Vector3 effectPosition = m_position;
	effectPosition.y += 175.0f;
	effectPosition += m_forward * 100.0f;
	effectEmitter->SetPosition(effectPosition);
	Quaternion rotation;
	rotation = m_rotation;
	rotation.AddRotationDegY(490.0f);
	rotation.AddRotationDegX(180.0f);
	rotation.AddRotationDegZ(-160.0f);
	effectEmitter->SetRotation(rotation);
	effectEmitter->Play();
}

void Player::ChoiseItem()
{
	wchar_t text[256];
	swprintf_s(text, 256, L"%d", m_repairNum);
	m_fontRender1.SetText(text);
	m_fontRender1.SetPosition(Vector3(808.0f, -430.0f, 0.0f));
	m_fontRender1.SetScale(1.0f);

	wchar_t text3[256];
	swprintf_s(text3, 256, L"%d", m_restraintNum);
	m_fontRender2.SetText(text3);
	m_fontRender2.SetPosition(Vector3(808.0f, -430.0f, 0.0f));
	m_fontRender2.SetScale(1.0f);

	if (m_equipState == enEquipState_Heal) 
	{
		m_fontRender1.SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		m_fontRender2.SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));
		if (g_pad[0]->IsTrigger(enButtonRight) || g_pad[0]->IsTrigger(enButtonLeft))
		{
			SoundSource* m_se = NewGO<SoundSource>(0);
			m_se->Init(29);
			m_se->SetVolume(0.3f);
			m_se->Play(false);
			m_equipState = enEquipState_Thuner;
			return;
		}
	}

	if (m_equipState == enEquipState_Thuner)
	{
		m_fontRender1.SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));
		m_fontRender2.SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		if (g_pad[0]->IsTrigger(enButtonRight) || g_pad[0]->IsTrigger(enButtonLeft))
		{
			SoundSource* m_se = NewGO<SoundSource>(0);
			m_se->Init(29);
			m_se->SetVolume(0.3f);
			m_se->Play(false);
			m_equipState = enEquipState_Heal;
			return;
		}
	}
}


void Player::MakeMagicBall()
{
	//魔法攻撃を作成する
	MagicBall* magicBall = NewGO<MagicBall>(0);
	Vector3 magicBallPosition = m_position;
	//座標を少し上にする
	magicBallPosition.y += 55.0f;
	//座標を少し前にする
	magicBallPosition += m_forward * 300.0f;
	magicBall->SetPosition(magicBallPosition);
	magicBall->SetRotation(m_rotation);
	//使用者はプレイヤー
	magicBall->SetEnUser(MagicBall::enUser_Player);
}

void Player::MakeRangeEffect()
{
	//エフェクトを作成する
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(3);
	effectEmitter->SetScale(Vector3::One * 25.0f);
	Vector3 effectPosition = m_position;
	//座標を少し上にする
	effectPosition.y += 2.0f;
	effectEmitter->SetPosition(effectPosition);
	//エフェクトを再生する
	effectEmitter->Play();
}

void Player::MakeEnemyStopCollision()
{
	//コリジョンオブジェクトを作成する
	CollisionObject* m_collisionObject;
	m_collisionObject = NewGO<CollisionObject>(0);
	//球状のコリジョンを作成する
	m_collisionObject->CreateSphere(m_position,
		Quaternion::Identity,
		650.0f);
	m_collisionObject->SetName("item_thunder");
	m_restraintNum -= 1;
}

void Player::MakeEnemyStopEffect()
{
	//エフェクトを作成する
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(4);
	effectEmitter->SetScale(Vector3::One * 1.0f);
	effectEmitter->SetPosition(m_position);
	//エフェクトを再生する
	effectEmitter->Play();
	SoundSource* m_se = NewGO<SoundSource>(0);
	m_se->Init(7);
	m_se->SetVolume(0.3f);
	m_se->Play(false);
}

void Player::ProcessCommonStateTransition()
{

	if (m_game->m_isBattleStartFade == true)
	{
		m_playerState = enPlayerState_FadeWait;
		return;
	}
	//RBボタンが押される&攻撃タイプが0なら
	if (g_pad[0]->IsTrigger(enButtonRB2) && Type == 0)
	{
		//攻撃１ステートに移行する
		m_playerState = enPlayerState_FirstAttack;
		return;
	}
	//RBボタンが押される&攻撃タイプが1なら
	if (g_pad[0]->IsTrigger(enButtonRB2) && Type == 1)
	{
		//攻撃2ステートに移行する
		m_playerState = enPlayerState_SecondAttack;
		return;
	}
	//RBボタンが押される&攻撃タイプが2なら
	if (g_pad[0]->IsTrigger(enButtonRB2) && Type == 2)
	{
		//攻撃3ステートに移行する
		m_playerState = enPlayerState_ThirdAttack;
		return;
	}


	//RB1ボタンとLB1ボタンが押されたら＆アイテムステートがサンダーなら
	if (g_pad[0]->IsPress(enButtonRB1) && g_pad[0]->IsPress(enButtonLB1)
		&& m_equipState == enEquipState_Thuner && m_restraintNum >= 1)
	{
		//攻撃範囲のエフェクトを作成する
		MakeRangeEffect();
		a = 1;
	}
	//ボタンから指が離れたら
	else if (a == 1)
	{
		//アイテム使用ステートに移行する
		m_playerState = enPlayerState_UseItem;
		a = 0;
		return;
	}


	//RB1ボタンとLB1ボタンが押されたら＆アイテムステートが回復なら
	if (g_pad[0]->IsTrigger(enButtonRB1) && g_pad[0]->IsTrigger(enButtonLB1)
		&& m_equipState == enEquipState_Heal && m_repairNum >= 1)
	{
		if (m_tower->m_hp < 200.0f)
		{
			EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
			effectEmitter->Init(5);
			effectEmitter->SetScale(Vector3::One * 5.0f);
			Vector3 effectPosition = m_tower->m_position;
			m_tower->m_position.y -= 10.0f;
			effectEmitter->SetPosition(m_tower->m_position);
			effectEmitter->Play();

			m_playerState = enPlayerState_UseHeal;
			m_repairNum -= 1;
			m_tower->m_hp += 30.0f;
			SoundSource* m_se = NewGO<SoundSource>(0);
			m_se->Init(8);
			m_se->SetVolume(0.3f);
			m_se->Play(false);
			if (m_tower->m_hp > 200)
			{
				m_tower->m_hp = 200.0f;
			}
		}
		return;
	}
	
	//LB2ボタンが押されたら
	if (m_mp >= 10)
	{
		if (g_pad[0]->IsTrigger(enButtonLB2))
		{
			//魔法攻撃ステートに移行する
			m_playerState = enPlayerState_Magic;
			return;
		}
	}
	//xかzの移動速度があったら
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//移動ステートへ移行する
		m_playerState = enPlayerState_Run;
	}
	//xかzの移動速度がなかったら
	else
	{
		//待機ステートに移行する
		m_playerState = enPlayerState_Idle;
		return;
	}

}

void Player::ProcessIdleStateTransition()
{
	//ステートを遷移する
	ProcessCommonStateTransition();
}

void Player::ProcessRunStateTransition()
{
	//ステートを遷移する
	ProcessCommonStateTransition();
}


void Player::ProcessDamageStateTransition()
{
	//アニメーションが再生がおわったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//ステートを遷移する
		ProcessCommonStateTransition();
	}
}

void Player::ProcessAttackStateTransition()
{
	//アニメーションが再生がおわったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//ステートを遷移する
		ProcessCommonStateTransition();
	}
}
void Player::ProcessAttack2StateTransition()
{
	//アニメーションが再生がおわったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//ステートを遷移する
		ProcessCommonStateTransition();
	}
}

void Player::ProcessAttack3StateTransition()
{
	//アニメーションが再生がおわったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//ステートを遷移する
		ProcessCommonStateTransition();
	}
}

void Player::ProcessMagicStateTransition()
{
	//アニメーションが再生がおわったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//ステートを遷移する
		ProcessCommonStateTransition();
	}
}

void Player::ProcessUseItemStateTransition()
{
	//アニメーションが再生がおわったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//ステートを遷移する
		ProcessCommonStateTransition();
	}
}
void  Player::ProcessFadeWaitStateTransition()
{
	if (m_game->m_isWaitBattleFadeout == false)
	{
		ProcessCommonStateTransition();
	}
}

void Player::ProcessHealStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//ステートを遷移する
		ProcessCommonStateTransition();
	}
}
void Player::ManageState()
{
	switch (m_playerState)
	{
	//待機ステートの時
	case enPlayerState_Idle:
		//待機ステートのステート遷移処理
		ProcessIdleStateTransition();
		break;
	//移動ステートの時
	case enPlayerState_Run:
		//移動ステートのステート遷移処理
		ProcessRunStateTransition();
		break;
	//被ダメージステートの時
	case enPlayerState_ReceiveDamage:
		//被ダメージステートのステート遷移処理
		ProcessDamageStateTransition();
		break;
	//攻撃1ステートの時
	case enPlayerState_FirstAttack:
		//攻撃1ステートのステート遷移処理
		ProcessAttackStateTransition();
		break;
	//攻撃2ステートの時
	case enPlayerState_SecondAttack:
		//攻撃2ステートのステート遷移処理
		ProcessAttack2StateTransition();
		break;
	//攻撃3ステートの時
	case enPlayerState_ThirdAttack:
		//攻撃3ステートのステート遷移処理
		ProcessAttack3StateTransition();
		break;
	//魔法攻撃ステートの時
	case enPlayerState_Magic:
		//魔法攻撃ステートのステート遷移処理
		ProcessMagicStateTransition();
		break;
		//アイテム使用ステートの時
	case enPlayerState_UseItem:
		//アイテム使用ステートのステート遷移処理
		ProcessUseItemStateTransition();
		break;
	case enPlayerState_UseHeal:
		//アイテム使用ステートのステート遷移処理
		ProcessHealStateTransition();
		break;
	case enPlayerState_FadeWait:
		//アイテム使用ステートのステート遷移処理
		ProcessFadeWaitStateTransition();
		break;
	}
}

void Player::PlayAnimation()
{
	switch (m_playerState)
	{
	//待機ステートの時
	case enPlayerState_Idle:
		m_modelRender.SetAnimationSpeed(2.0f);
		//待機アニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.3f);
		break;
	//移動ステートの時
	case enPlayerState_Run:
		m_modelRender.SetAnimationSpeed(1.0f);
		//移動アニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.1f);
		break;
	//被ダメージステートの時
	case enPlayerState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(1.4f);
		//被ダメージアニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
	//攻撃1ステートの時
	case enPlayerState_FirstAttack:
		m_modelRender.SetAnimationSpeed(1.6f);
		m_modelRender.PlayAnimation(enAnimationClip_FirstAttack, 0.3f);
		break;
	//攻撃2ステートの時
	case enPlayerState_SecondAttack:
		m_modelRender.SetAnimationSpeed(1.6f);
		//攻撃2アニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_SecondAttack, 0.3f);
		break;
	//攻撃3ステートの時
	case enPlayerState_ThirdAttack:
		m_modelRender.SetAnimationSpeed(1.7f);
		//攻撃3アニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_ThirdAttack, 0.3f);
		break;
	//魔法攻撃ステートの時
	case enPlayerState_Magic:
		m_modelRender.SetAnimationSpeed(1.8f);
		//魔法攻撃アニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_Magic, 0.3f);
		break;
	//アイテム使用ステートの時
	case enPlayerState_UseItem:
		m_modelRender.SetAnimationSpeed(1.8f);
		//アイテム使用アニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_UseItem, 0.5f);
		break;
	case enPlayerState_FadeWait:
		m_modelRender.SetAnimationSpeed(2.0f);
		//待機アニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.3f);
		break;
	case enPlayerState_UseHeal:
		m_modelRender.SetAnimationSpeed(1.8f);
		m_modelRender.PlayAnimation(enAnimationClip_UseHeal, 0.5f);
		break;
	default:
		break;
	}
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//キーの名前が「attack_start」なら
	if (wcscmp(eventName, L"attack_start") == 0)
	{
		//攻撃中にする
		m_isUnderAttack = true;
	}
	//キーの名前が「attack_end」なら
	else if (wcscmp(eventName, L"attack_end") == 0)
	{
		//攻撃が終わる
		m_isUnderAttack = false;
	}
	//キーの名前が「effect_start」なら
	if (wcscmp(eventName, L"effect_start") == 0)
	{
		//攻撃1のエフェクトを再生する
		MakeFirstSlashingEffect(); 
	}
	//キーの名前が「effect_start2」なら
	if (wcscmp(eventName, L"effect_start2") == 0)
	{
		//攻撃2のエフェクトを再生する
		MakeSecondSlashingEffect();
	}
	//キーの名前が「effect_start3」なら
	if (wcscmp(eventName, L"effect_start3") == 0)
	{
		//攻撃3のエフェクトを再生する
		MakeThirdSlashingEffect();
	}
	//キーの名前が「magic_attack」なら
	else if (wcscmp(eventName, L"magic_attack") == 0)
	{
		m_mp -= 10;
		MakeMagicBall();
	}
	//キーの名前が「thunder3」なら
	if (wcscmp(eventName, L"useitem") == 0)
	{
		//コリジョンを作成する＆エフェクトを再生する
		MakeEnemyStopEffect();
		MakeEnemyStopCollision();

	}

}

void Player::Render(RenderContext& rc)
{
	
	//モデルを描画する
	m_modelRender.Draw(rc);
	//画像を描画する
	m_itemWaku.Draw(rc);
	switch (m_equipState)
	{
	case Player::enEquipState_Heal:
		m_itemHeal.Draw(rc);
		break;
	case Player::enEquipState_Thuner:
		m_itemStop.Draw(rc);
	break;
	}
	m_fontRender1.Draw(rc);
	m_fontRender2.Draw(rc);
}