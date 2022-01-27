#include "stdafx.h"
#include "Player.h"
#include "Game.h"
#include "PlayerUi.h"
#include "MagicBall.h"
#include "Tower.h"

#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"

namespace
{
	const float CHARACON_RADIUS = 60.0f;                          //キャラコンの半径
	const float CHARACON_HEIGHT = 250.0f;                         //キャラコンの高さ
	const float MOVE_SPEED = 750.0f;                              //移動速度
	const float ATTACK_COLLISION_FORWARD_POSITION = 50.0f;        //攻撃用のコリジョンの前方向のベクトル
	const float COLLISION_BOX_SIZE_X = 10.0f;                     //攻撃用のコリジョンのXの大きさ
	const float COLLISION_BOX_SIZE_Y = 50.0f;                     //攻撃用のコリジョンのYの大きさ
	const float COLLISION_BOX_SIZE_Z = 10.0f;                     //攻撃用のコリジョンのZの大きさ
	const float TIMER_SPEED = 50.0f;                              //連続攻撃判定タイマーの速度
	const float SECOND_ATTACK_TIMER = 6.0f;                       //2回目の攻撃のボタン入力の制限時間
	const float THIRD_ATTACK_TIMER = 10.0f;                       //3回目の攻撃のボタン入力の制限時間
	const int FIRST_ATTACK_NUMBER = 0;                            //1回目の攻撃の攻撃番号
	const int SECOND_ATTACK_NUMBER = 1;                           //2回目の攻撃の攻撃番号
	const int THIRD_ATTACK_NUMBER = 2;                            //3回目の攻撃の攻撃番号
	const float FIRST_SLASHING_EFFECT_SCALE = 3.0f;               //1回目の攻撃エフェクトの大きさ
	const float FIRST_SLASHING_EFFECT_POSITION_Y = 210.0f;        //1回目の攻撃エフェクトのY座標
	const float FIRST_SLASHING_EFFECT_POSITION_FORWARD = 98.0f;   //1回目の攻撃エフェクトの前方向の座標
	const float FIRST_SLASHING_EFFECT_ROTATION_X = -70.0f;        //1回目の攻撃エフェクトのX軸の回転
	const float FIRST_SLASHING_EFFECT_ROTATION_Y = 540.0f;        //1回目の攻撃エフェクトのY軸の回転
	const float SECOND_SLASHING_EFFECT_SCALE = 4.0f;              //2回目の攻撃エフェクトの大きさ
	const float SECOND_SLASHING_EFFECT_POSITION_Y = 175.0f;       //2回目の攻撃エフェクトのY座標
	const float SECOND_SLASHING_EFFECT_FORWARD = 10.0f;           //2回目の攻撃エフェクトの前方向の座標
	const float SECOND_SLASHING_EFFECT_ROTATION_X = -180.0f;      //2回目の攻撃エフェクトのX軸の回転
	const float SECOND_SLASHING_EFFECT_ROTATION_Y = 490.0f;       //2回目の攻撃エフェクトのY軸の回転
	const float SECOND_SLASHING_EFFECT_ROTATION_Z = 0.0f;         //2回目の攻撃エフェクトのZ軸の回転
	const float THIRD_SLASHING_EFFECT_SCALE = 4.0f;               //3回目の攻撃エフェクトの大きさ
	const float THIRD_SLASHING_EFFECT_FORWARD = 100.0f;           //3回目の攻撃エフェクトの前方向の座標
	const float THIRD_SLASHING_EFFECT_ROTATION_X = -180.0f;       //3回目の攻撃エフェクトのX軸の回転
	const float THIRD_SLASHING_EFFECT_ROTATION_Y = 490.0f;        //3回目の攻撃エフェクトのY軸の回転
	const float THIRD_SLASHING_EFFECT_ROTATION_Z = -160.0f;       //3回目の攻撃エフェクトのZ軸の回転
	const float MAGICBALL_POSITION_Y = 55.0f;                     //魔法攻撃のY座標
	const float MAGICBALL_POSITION_FORWARD = 300.0f;              //魔法攻撃の前方向の座標
	const float RESTRAINT_RANGE_SCALE = 25.0f;                    //拘束魔法の効果範囲の大きさ
	const float RESTRAINT_COLLISION_SPHRE_RADIUS = 650.0f;        //拘束魔法の当たり判定のコリジョンの半径
	const float RESTRAINT_EFFECT_SIZE = 1.0f;                     //拘束魔法のエフェクトの大きさ
	const float REPAIR_EFFECT_SIZE = 5.0f;                        //タワー修理のエフェクトの大きさ
	const float TOWER_DURABILITY_MAX = 200.0f;                    //タワーの耐久値の最大値
	const float REPAIR_DURABILITY_RECOVERY = 30.0f;               //タワー修理の耐久値の回復量
	const float MAGICATTACK_CONSUME_MP = 10.0f;                   //魔法攻撃の消費MP
}

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
	m_animationClips[enAnimationClip_Restraint].Load("Assets/animData/useitem.tka");
	m_animationClips[enAnimationClip_Restraint].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Repair].Load("Assets/animData/heal.tka");
	m_animationClips[enAnimationClip_Repair].SetLoopFlag(false);
	
	//モデルを読み込む
	m_modelRender.Init("Assets/modelData/player/player4.tkm"
		,m_animationClips, enAnimationClip_Num
	);

	//座標、回転、大きさを設定する
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);

	//キャラクタコントローラーの初期化
	m_charaCon.Init(
		CHARACON_RADIUS,
		CHARACON_HEIGHT,
		m_position
	);

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
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/playerreact.wav");
	g_soundEngine->ResistWaveFileBank(4, "Assets/sound/kaminari.wav");
	g_soundEngine->ResistWaveFileBank(5, "Assets/sound/heal.wav");
	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/playerdamage.wav");
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/sentaku.wav");

	m_tower = FindGO<Tower>("tower");
	m_game = FindGO<Game>("game");
	return true;
}

void Player::Update()
{
	Hit();
	//移動処理
	Move();
	//回転処理
	Rotation();
	//当たり判定
	Collision();
	//連続攻撃判定
	SelectSkill();
	//攻撃判定
	Attack();
	//アニメーションの再生
	PlayAnimation();
	//ステートの遷移処理
	ManageState();

	//モデルの更新
	m_modelRender.Update();
}

void Player::Move()
{
	//移動できない状態ならば、移動処理を行わない
	if (IsEnableMove() == false)
	{
		return;
	}
	//移動速度
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	//ステックの入力量を取得
	float lStick_x = g_pad[0]->GetLStickXF();
	float lStick_y = g_pad[0]->GetLStickYF();
	//cameraの前方向と右方向を取得
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//XZ平面での前方方向、右方向に変換する
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//XZ成分の移動速度をクリア
	m_moveSpeed += cameraForward * lStick_y * MOVE_SPEED;
	m_moveSpeed += cameraRight * lStick_x * MOVE_SPEED;
	//キャラコンを使用して、座標を更新する
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {

		m_moveSpeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
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
	//回転を設定する
	m_modelRender.SetRotation(m_rotation);
	//プレイヤーの正面ベクトルを計算す
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Player::Collision()
{
	//被ダメージステートなら
	if (m_playerState == enPlayerState_ReceiveDamage
		|| m_playerState == enPlayerState_Restraint || m_playerState == enPlayerState_Repair)
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
			//SEを再生する
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(31);
			se->SetVolume(0.2f);
			se->Play(false);
			//被ダメージステートへ
			m_playerState = enPlayerState_ReceiveDamage;
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
			//SEを再生する
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(31);
			se->SetVolume(0.2f);
			se->Play(false);
			//被ダメージリステートへ
			m_playerState = enPlayerState_ReceiveDamage;
		}
	}
	//ボスの遅い攻撃のコリジョンを取得する
	const auto& collisions3 = g_collisionObjectManager->FindCollisionObjects("heavy_attack");
	//配列をfor文で回す
	for (auto collision : collisions3)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			//SEを再生する
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(31);
			se->SetVolume(0.2f);
			se->Play(false);
			//被ダメージリステートに移行する
			m_playerState = enPlayerState_ReceiveDamage;
		}
	}
	//ボスの速い攻撃のコリジョンを取得する
	const auto& collisions4 = g_collisionObjectManager->FindCollisionObjects("quick_attack");
	//配列をfor文で回す
	for (auto collision : collisions4)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			//SEを再生する
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(31);
			se->SetVolume(0.2f);
			se->Play(false);
			//被ダメージリステートへ
			m_playerState = enPlayerState_ReceiveDamage;
		}
	}
}

void Player::Hit()
{
	//1撃目の攻撃ステートだったら
	if (m_playerState == enPlayerState_FirstAttack)
	{
		//2撃目の攻撃フラグをtrueにする
		m_secondAttackTimerFrag = true;
	}
	//2撃目の攻撃ステートだったら
	if (m_playerState == enPlayerState_SecondAttack)
	{
		//3撃目の攻撃フラグをtrueにする
		m_secondAttackTimerFrag = true;
	}
	//2撃目の攻撃フラグがtrueだったら
	if (m_secondAttackTimerFrag == true)
	{
		//2撃目の攻撃タイマーを増加させる
		m_secondAttackTimer += g_gameTime->GetFrameDeltaTime() * TIMER_SPEED;
		//2撃目の攻撃タイマーが0.001f以上か6.0f以内なら
		if (m_secondAttackTimer > 0.001f && m_secondAttackTimer <= SECOND_ATTACK_TIMER)
		{
			//攻撃タイプを1にする
			m_attackNumber = SECOND_ATTACK_NUMBER;;
		}
		//2撃目の攻撃タイマーがそれ以外なら
		else
		{
			//攻撃番号を1撃目にする
			m_attackNumber = FIRST_ATTACK_NUMBER;
			//タイマーをリセットする
			m_secondAttackTimer = 0.0f;
			//2撃目の攻撃フラグをfalseにする
			m_secondAttackTimerFrag = false;
		}
	}
	//3撃目の攻撃フラグをtrueにする
	if (m_thirdAttackTimerFrag == true)
	{
		//3撃目の攻撃タイマーを増加させる
		m_thirdAttackTimer += g_gameTime->GetFrameDeltaTime() * TIMER_SPEED;
		//3撃目の攻撃タイマーが0.001fより大きく10.0fより小さかったら
		if (m_thirdAttackTimer > 0.001f && m_thirdAttackTimer <= THIRD_ATTACK_TIMER)
		{
			//攻撃番号を3撃目にする
			m_attackNumber = THIRD_ATTACK_NUMBER;
		}
		else
		{
			//攻撃番号を1撃目にする
			m_attackNumber = FIRST_ATTACK_NUMBER;
			//タイマーをリセットする
			m_thirdAttackTimer = 0.0f;
			//3撃目の攻撃フラグをfalseにする
			m_thirdAttackTimerFrag = false;
		}
	}
	//攻撃を食らったら攻撃タイプを0へ
	if (m_playerState == enPlayerState_ReceiveDamage)
	{
		m_attackNumber = FIRST_ATTACK_NUMBER;
		m_thirdAttackTimer = 0.0f;
		m_secondAttackTimerFrag = false;
		m_thirdAttackTimerFrag = false;
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
	//座標を少し前にする
	Vector3 collisionPosition = m_position;
	collisionPosition += m_forward * ATTACK_COLLISION_FORWARD_POSITION;
	//ボックス状のコリジョンを作成する
	collisionObject->CreateBox(collisionPosition,
		Quaternion::Identity,
		Vector3(COLLISION_BOX_SIZE_X,COLLISION_BOX_SIZE_Y,COLLISION_BOX_SIZE_Z)
	);
	//剣のボーンのワールド行列を取得する
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
	//剣のボーンのワールド行列をコリジョンに適用させる
	collisionObject->SetName("player_attack");
	collisionObject->SetWorldMatrix(matrix);
}

void Player::MakeFirstSlashingEffect()
{
	//エフェクトを再生する
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(0);
	effectEmitter->SetScale(Vector3::One * FIRST_SLASHING_EFFECT_SCALE);
	//座標を少し上&前にする
	Vector3 effectPosition = m_position;
	effectPosition.y += FIRST_SLASHING_EFFECT_POSITION_Y;
	effectPosition += m_forward * FIRST_SLASHING_EFFECT_POSITION_FORWARD;
	effectEmitter->SetPosition(effectPosition);
	//回転を設定する
	Quaternion effectRotation = m_rotation;
	effectRotation.AddRotationDegX(FIRST_SLASHING_EFFECT_ROTATION_X);
	effectRotation.AddRotationDegY(FIRST_SLASHING_EFFECT_ROTATION_Y);
	effectEmitter->SetRotation(effectRotation);
	effectEmitter->Play();
}

void Player::MakeSecondSlashingEffect()
{
	//エフェクトを再生する
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(1);
	effectEmitter->SetScale(Vector3::One * SECOND_SLASHING_EFFECT_SCALE);
	//座標を少し上&前にする
	Vector3 effectPosition = m_position;
	effectPosition.y += SECOND_SLASHING_EFFECT_POSITION_Y;
	effectPosition += m_forward * SECOND_SLASHING_EFFECT_FORWARD;
	effectEmitter->SetPosition(effectPosition);
	//回転を設定する
	Quaternion effectRotation = m_rotation;
	effectRotation.AddRotationDegX(SECOND_SLASHING_EFFECT_ROTATION_X);
	effectRotation.AddRotationDegY(SECOND_SLASHING_EFFECT_ROTATION_Y);
	effectRotation.AddRotationDegZ(SECOND_SLASHING_EFFECT_ROTATION_Z);
	effectEmitter->SetRotation(effectRotation);
	effectEmitter->Play();
}

void Player::MakeThirdSlashingEffect()
{
	//エフェクトを再生する
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(2);
	effectEmitter->SetScale(Vector3::One * THIRD_SLASHING_EFFECT_SCALE);
	//座標を少し上&前にする
	Vector3 effectPosition = m_position;
	effectPosition.y += SECOND_SLASHING_EFFECT_POSITION_Y;
	effectPosition += m_forward * THIRD_SLASHING_EFFECT_FORWARD;
	effectEmitter->SetPosition(effectPosition);
	//回転を設定する
	Quaternion rotation = m_rotation;
	rotation.AddRotationDegX(THIRD_SLASHING_EFFECT_ROTATION_X);
	rotation.AddRotationDegY(THIRD_SLASHING_EFFECT_ROTATION_Y);
	rotation.AddRotationDegZ(THIRD_SLASHING_EFFECT_ROTATION_Z);
	effectEmitter->SetRotation(rotation);
	effectEmitter->Play();
}

void Player::MakeMagicBall()
{
	//魔法攻撃を作成する
	MagicBall* magicBall = NewGO<MagicBall>(0);
	Vector3 magicBallPosition = m_position;
	//座標を少し上にする
	magicBallPosition.y += MAGICBALL_POSITION_Y;
	//座標を少し前にする
	magicBallPosition += m_forward * MAGICBALL_POSITION_FORWARD;
	magicBall->SetPosition(magicBallPosition);
	magicBall->SetRotation(m_rotation);
	//使用者はプレイヤー
	magicBall->SetEnUser(MagicBall::enUser_Player);
}

void Player::MakeRestraintRangeEffect()
{
	//エフェクトを再生する
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(3);
	effectEmitter->SetScale(Vector3::One * RESTRAINT_RANGE_SCALE);
	Vector3 effectPosition = m_position;
	effectEmitter->SetPosition(effectPosition);
	effectEmitter->Play();
}

void Player::MakeRestraintCollision()
{
	//コリジョンオブジェクトを作成する
	CollisionObject* collisionObject;
	collisionObject = NewGO<CollisionObject>(0);
	//球状のコリジョンを作成する
	collisionObject->CreateSphere(m_position,
		Quaternion::Identity,
		RESTRAINT_COLLISION_SPHRE_RADIUS);
	collisionObject->SetName("item_thunder");
}

void Player::MakeRestraintEffect()
{
	//エフェクトを再生する
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(4);
	effectEmitter->SetScale(Vector3::One * RESTRAINT_EFFECT_SIZE);
	effectEmitter->SetPosition(m_position);
	effectEmitter->Play();
}

void Player::MakeRepairEffect()
{
	//エフェクトを再生する
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(5);
	effectEmitter->SetScale(Vector3::One * REPAIR_EFFECT_SIZE);
	Vector3 effectPosition = m_tower->GetPosition();
	effectEmitter->SetPosition(effectPosition);
	effectEmitter->Play();
}

void Player::SelectSkill()
{
	//スキルステートがタワー修理ステートだったら
	if (m_skillState == enSkillState_Repair)
	{
		//右ボタンか左ボタンが押されたら
		if (g_pad[0]->IsTrigger(enButtonRight) || g_pad[0]->IsTrigger(enButtonLeft))
		{
			//SEを再生する
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(29);
			se->SetVolume(0.3f);
			se->Play(false);
			//拘束魔法ステートへ
			m_skillState = enSkillState_Restraint;
			return;
		}
	}
	//スキルステートが拘束魔法ステートだったら
	if (m_skillState == enSkillState_Restraint)
	{
		//右ボタンか左ボタンが押されたら
		if (g_pad[0]->IsTrigger(enButtonRight) || g_pad[0]->IsTrigger(enButtonLeft))
		{
			//SEを再生する
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(29);
			se->SetVolume(0.3f);
			se->Play(false);
			//タワー修理ステートへ
			m_skillState = enSkillState_Repair;
			return;
		}
	}
}


void Player::ProcessCommonStateTransition()
{
	//フェード中なら
	if (m_game->m_isBattleStartFade == true)
	{
		//フェード待機ステートへ
		m_playerState = enPlayerState_FadeWait;
		return;
	}
	//RBボタンが押される&攻撃タイプが0なら
	if (g_pad[0]->IsTrigger(enButtonRB2) && m_attackNumber == FIRST_ATTACK_NUMBER)
	{
		//攻撃１ステートに移行する
		m_playerState = enPlayerState_FirstAttack;
		return;
	}
	//RBボタンが押される&攻撃タイプが1なら
	if (g_pad[0]->IsTrigger(enButtonRB2) && m_attackNumber == SECOND_ATTACK_NUMBER)
	{
		//攻撃2ステートに移行する
		m_playerState = enPlayerState_SecondAttack;
		return;
	}
	//RBボタンが押される&攻撃タイプが2なら
	if (g_pad[0]->IsTrigger(enButtonRB2) && m_attackNumber == THIRD_ATTACK_NUMBER)
	{
		//攻撃3ステートに移行する
		m_playerState = enPlayerState_ThirdAttack;
		return;
	}

	//RB1ボタンとLB1ボタンが押されたら＆
	//スキルステートが拘束魔法ステートだったら＆
	//拘束魔法の使用回数が1より大きかったら
	if (g_pad[0]->IsPress(enButtonRB1) && g_pad[0]->IsPress(enButtonLB1)
		&& m_skillState == enSkillState_Restraint && m_restraintNum >= 1)
	{
		//攻撃範囲のエフェクトを作成する
		MakeRestraintRangeEffect();
		//プレスボタンフラグをtrueにする
		m_pressButtonFrag = true;
	}
	//プレスボタンフラグがtrueだったら
	//ボタンから指が離れたら
	else if (m_pressButtonFrag == true)
	{
		//拘束魔法ステートへ
		m_playerState = enPlayerState_Restraint;
		//拘束魔法の使用回数を1減らす
		m_restraintNum -= 1;
		//プレスボタンフラグをfalseにする
		m_pressButtonFrag = false;
		return;
	}

	//RB1ボタンとLB1ボタンが押されたら＆
	//スキルステートがタワー修理ステートだったら＆
	//タワー修理の使用回数が1より大きかったら
	if (g_pad[0]->IsTrigger(enButtonRB1) && g_pad[0]->IsTrigger(enButtonLB1)
		&& m_skillState == enSkillState_Repair && m_repairNum >= 1)
	{
		//タワーの耐久値が最大値より小さかったら
		if (m_tower->GetDurability() < TOWER_DURABILITY_MAX)
		{
			//SEを再生する
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(8);
			se->SetVolume(0.3f);
			se->Play(false);
			//タワー修理のエフェクトを再生する
			MakeRepairEffect();
			//タワー修理ステートへ
			m_playerState = enPlayerState_Repair;
			//タワー修理の使用回数を1減らす
			m_repairNum -= 1;
			//タワーの耐久値を30回復する
			float towerdurability = m_tower->GetDurability();
			towerdurability += REPAIR_DURABILITY_RECOVERY;
			m_tower->SetDurability(towerdurability);
			//タワーの耐久値が最大値より大きかったら
			if (m_tower->GetDurability() > TOWER_DURABILITY_MAX)
			{
				//タワーの耐久値を最大値にする
				towerdurability = m_tower->GetDurability(); 
				towerdurability = TOWER_DURABILITY_MAX;
				m_tower->SetDurability(towerdurability);
			}
		}
		return;
	}
	
	//MPが10より大きかったら
	if (m_mp >= MAGICATTACK_CONSUME_MP)
	{
		//LB2ボタンが押されたら
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
	//他のステートを遷移する
	ProcessCommonStateTransition();
}

void Player::ProcessRunStateTransition()
{
	//他のステートを遷移する
	ProcessCommonStateTransition();
}

void Player::ProcessFristAttackStateTransition()
{
	//アニメーションが再生がおわったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートを遷移する
		ProcessCommonStateTransition();
	}
}

void Player::ProcessSecondAttackStateTransition()
{
	//アニメーションが再生がおわったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートを遷移する
		ProcessCommonStateTransition();
	}
}

void Player::ProcessThirdAttackStateTransition()
{
	//アニメーションが再生がおわったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートを遷移する
		ProcessCommonStateTransition();
	}
}

void Player::ProcessMagicStateTransition()
{
	//アニメーションが再生がおわったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートを遷移する
		ProcessCommonStateTransition();
	}
}

void Player::ProcessDamageStateTransition()
{
	//アニメーションが再生がおわったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートを遷移する
		ProcessCommonStateTransition();
	}
}

void Player::ProcessRepairStateTransition()
{
	//アニメーションが再生がおわったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートを遷移する
		ProcessCommonStateTransition();
	}
}

void Player::ProcessRestraintStateTransition()
{
	//アニメーションが再生がおわったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートを遷移する
		ProcessCommonStateTransition();
	}
}

void  Player::ProcessFadeWaitStateTransition()
{
	//バトルスタートフェードが終わったら
	if (m_game->m_isWaitBattleFadeout == false)
	{
		//他のステートを遷移する
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
		//1撃目の攻撃ステートの時
	case enPlayerState_FirstAttack:
		//攻撃1ステートのステート遷移処理
		ProcessFristAttackStateTransition();
		break;
		//2撃目の攻撃ステートの時
	case enPlayerState_SecondAttack:
		//攻撃2ステートのステート遷移処理
		ProcessSecondAttackStateTransition();
		break;
		//3撃目の攻撃ステートの時
	case enPlayerState_ThirdAttack:
		//攻撃3ステートのステート遷移処理
		ProcessThirdAttackStateTransition();
		break;
		//魔法攻撃ステートの時
	case enPlayerState_Magic:
		//魔法攻撃ステートのステート遷移処理
		ProcessMagicStateTransition();
		break;
		//拘束魔法ステートの時
	case enPlayerState_Restraint:
		//拘束魔法ステートのステート遷移処理
		ProcessRestraintStateTransition();
		break;
		//タワーの修理ステートの時
	case enPlayerState_Repair:
		//タワーの修理ステートのステート遷移処理
		ProcessRepairStateTransition();
		break;
		//フェード待機ステートの時
	case enPlayerState_FadeWait:
		//フェード待機ステートのステート遷移処理
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
		//再生速度
		m_modelRender.SetAnimationSpeed(2.0f);
		//待機アニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.3f);
		break;
	//移動ステートの時
	case enPlayerState_Run:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.0f);
		//移動アニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.1f);
		break;
	//被ダメージステートの時
	case enPlayerState_ReceiveDamage:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.4f);
		//被ダメージアニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
	//1撃目の攻撃ステートの時
	case enPlayerState_FirstAttack:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.6f);
		//1撃目の攻撃アニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_FirstAttack, 0.3f);
		break;
	//2撃目の攻撃ステートの時
	case enPlayerState_SecondAttack:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.6f);
		//2撃目の攻撃アニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_SecondAttack, 0.3f);
		break;
	//3撃目の攻撃ステートの時
	case enPlayerState_ThirdAttack:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.7f);
		//3撃目の攻撃アニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_ThirdAttack, 0.3f);
		break;
	//魔法攻撃ステートの時
	case enPlayerState_Magic:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.8f);
		//魔法攻撃アニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_Magic, 0.3f);
		break;
	//拘束魔法ステートの時
	case enPlayerState_Restraint:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.8f);
		//拘束魔法アニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_Restraint, 0.5f);
		break;
		//タワー修理ステートの時
	case enPlayerState_Repair:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.8f);
		//タワー修理アニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_Repair, 0.5f);
		break;
		//フェード待機ステートの時
	case enPlayerState_FadeWait:
		//再生速度
		m_modelRender.SetAnimationSpeed(2.0f);
		//待機アニメーションを再生する
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.3f);
		break;
	}
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	if (wcscmp(eventName, L"attack_start") == 0)
	{
		//攻撃中にする
		m_isUnderAttack = true;
	}
	else if (wcscmp(eventName, L"attack_end") == 0)
	{
		//攻撃が終わる
		m_isUnderAttack = false;
	}
	if (wcscmp(eventName, L"effect_start") == 0)
	{
		//SEを再生する
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(0);
		se->SetVolume(0.3f);
		se->Play(false);
		//攻撃1のエフェクトを再生する
		MakeFirstSlashingEffect(); 
	}
	if (wcscmp(eventName, L"effect_start2") == 0)
	{
		//SEを再生する
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(1);
		se->SetVolume(0.3f);
		se->Play(false);
		//攻撃2のエフェクトを再生する
		MakeSecondSlashingEffect();
	}
	if (wcscmp(eventName, L"effect_start3") == 0)
	{
		//SEを再生する
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(2);
		se->SetVolume(0.5f);
		se->Play(false);
		//攻撃3のエフェクトを再生する
		MakeThirdSlashingEffect();
	}
	else if (wcscmp(eventName, L"magic_attack") == 0)
	{
		//Mpを10減らす
		m_mp -= MAGICATTACK_CONSUME_MP;
		//魔法攻撃を作成する
		MakeMagicBall();
	}
	if (wcscmp(eventName, L"useitem") == 0)
	{
		//拘束魔法のエフェクトを再生する
		MakeRestraintEffect();
		//拘束魔法用の当たり判定コリジョンを作成する
		MakeRestraintCollision();
		//SEを再生する
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(7);
		se->SetVolume(0.3f);
		se->Play(false);
	}
}

void Player::Render(RenderContext& rc)
{
	//モデルを描画する
	m_modelRender.Draw(rc);
}