#include "stdafx.h"
#include "Player.h"
#include "Game.h"

#include "Enemy.h"

#include "IceBall.h"
#include "ExplosionBall.h"

#include "Stuck.h"
#include "Heal.h"
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
	//�A�j���[�V������ǂݍ���
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
	m_animationClips[enAnimationClip_Win].Load("Assets/animData/win.tka");
	m_animationClips[enAnimationClip_Win].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	
	//���f����ǂݍ���
	m_modelRender.Init("Assets/modelData/player/player4.tkm"
		,m_animationClips, enAnimationClip_Num
	);

	//���W��ݒ肷��
	m_modelRender.SetPosition(m_position);
	//��]��ݒ肷��
	m_modelRender.SetRotation(m_rotation);
	//�傫����ݒ肷��
	m_modelRender.SetScale(m_scale);
	//�L�����N�^�R���g���[���[�̏�����
	m_charaCon.Init(
		60.0f,
		250.0f,
		m_position
	);

	//�摜��ǂݍ���
	m_sprintRender.Init("Assets/sprite/Item/No.dds", 128.0f, 128.0f);
	m_sprintRender.SetPosition(Vector3(850.0f, -450.0f, 0.0f));
	m_sprintRender2.Init("Assets/sprite/Item/Thunder.dds", 128.0f, 128.0f);
	m_sprintRender2.SetPosition(Vector3(850.0f, -450.0f, 0.0f));
	m_sprintRender3.Init("Assets/sprite/Item/Heal.dds", 128.0f, 128.0f);
	m_sprintRender3.SetPosition(Vector3(850.0f, -450.0f, 0.0f));

	//���̃{�[��ID���擾����
	m_swordBoneId = m_modelRender.FindBoneID(L"sword");

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//�e�G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(0, u"Assets/efk/slash.efk");
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/efk/slash2.efk");
	EffectEngine::GetInstance()->ResistEffect(2, u"Assets/efk/slash3.efk");
	EffectEngine::GetInstance()->ResistEffect(3, u"Assets/efk/item.efk");
	EffectEngine::GetInstance()->ResistEffect(4, u"Assets/efk/use_item_thunder.efk");

	//�e�T�E���h��ǂݍ���
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/swordfuriorosi.wav");
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/sword2.wav");
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/sword3.wav");
	g_soundEngine->ResistWaveFileBank(4, "Assets/sound/playerreact.wav");

	m_enemy = FindGO<Enemy>("enemy");
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
	//�ړ�����
	Move();
	//��]����
	Rotation();
	//�����蔻��
	Collision();
	//�U������
	Attack();
	//�A���U��
	//�A�C�e���I��
	ChoiseItem();
	//�A�j���[�V�����̍Đ�
	PlayAnimation();
	//�X�e�[�g�̑J�ڏ���
	ManageState();

	//���f���̍X�V
	m_modelRender.Update();
	//�e�摜�̍X�V
	m_sprintRender.Update();
	m_sprintRender2.Update();
	m_sprintRender3.Update();
}

void Player::Hit()
{
	//�U���X�e�[�g1�Ȃ�
	if (m_playerState == enPlayerState_FirstAttack)
	{
		//�^�C�}�[���N������
		move = 1;
	}
	//�U���X�e�[�g2�Ȃ�
	if (m_playerState == enPlayerState_SecondAttack)
	{
		//�^�C�}�[���N������
		move2 = 1;
	}

	//�U���^�C�}�[�P���N��
	if (move == 1)
	{
		m_attackTimer += g_gameTime->GetFrameDeltaTime() * 50.0f;
		//�U���^�C�}�[��0.01�ȏォ��5.0�ȓ��Ȃ�
		if (m_attackTimer > 0.001f && m_attackTimer <= 6.0f)
		{
			//�U���^�C�v��1�ɂ���
			Type = 1;
		}
		//�U���^�C�}�[������ȊO�Ȃ�
		else
		{
			Type = 0;//�U���^�C�v��0�ɂ���
			m_attackTimer = 0.0f;//�^�C�}�[�����Z�b�g����
			move = 0;
		}
		//�U����H�������U���^�C�v��0��
		if (m_playerState == enPlayerState_ReceiveDamage)
		{
			Type = 0;
			m_attackTimer = 0.0f;
			move = 0;
		}
	}
	if (move2 == 1)
	{
		m_attackTimer2 += g_gameTime->GetFrameDeltaTime() * 50.0f;

		if (m_attackTimer2 > 0.001f && m_attackTimer2 <= 8.0f)
		{
			Type = 2;
		}
		else
		{
			Type = 0;//�U���^�C�v��0�ɂ���
			m_attackTimer2 = 0.0f;//�^�C�}�[�����Z�b�g����
			move2 = 0;
		}
		//�U����H�������U���^�C�v��0��
		if (m_playerState == enPlayerState_ReceiveDamage)
		{
			Type = 0;
			m_attackTimer2 = 0.0f;
			move2 = 0;
		}
	}
}
void Player::Move()
{
	//�ړ��ł��Ȃ���ԂȂ�΁A�ړ��������s��Ȃ�
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
	//��_���[�W�X�e�[�g�Ȃ�
	if (m_playerState == enPlayerState_ReceiveDamage)
	{
		//�_���[�W��������Ȃ�
		return;
	}
	//�G�̍U���p�̃R���W�����̔z����擾����
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����
		if (collision->IsHit(m_charaCon))
		{
			//HP�����炷
			m_hp -= 5;
			//HP��0�Ȃ�
			if (m_hp == 0)
			{

			}
			//HP��0�ł͂Ȃ�������
			else {
				//��_���[�W�X�e�[�g�Ɉڍs����
				m_playerState = enPlayerState_ReceiveDamage;
				//SE���Đ�����
			}
		}
	}
	//�G�̖��@�U���̃R���W�������擾����
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("enemy_magic");
	//�z���for���ŉ�
	for (auto collision : collisions2)
	{
		//�R���W�����ƃL�����R�����Փ˂�����
		if (collision->IsHit(m_charaCon))
		{
			//Hp�����炷
			m_hp -= 5;
			if (m_hp == 0)
			{

			}
			//HP��0�ł͂Ȃ�������B
			else {
				//��_���[�W���X�e�[�g�Ɉڍs����
				m_playerState = enPlayerState_ReceiveDamage;
			}
		}
	}

	//const auto& collisions3 = g_collisionObjectManager->FindCollisionObjects("enemy_point");
	//�z���for���ŉ�
	//for (auto collision : collisions3)
	//{
		//�R���W�����ƃL�����R�����Փ˂�����
	//	if (collision->IsHit(m_charaCon))
	//	{
	//		DeleteGO(collision);
	//		HavePoint += 100;
	//	}
	//}
}

void Player::Attack()
{
	//�U���X�e�[�g�ł͂Ȃ��Ȃ�
	if (m_playerState != enPlayerState_FirstAttack &&
		m_playerState != enPlayerState_SecondAttack &&
		m_playerState != enPlayerState_ThirdAttack)
	{
		//�U�����������Ȃ�
		return;
	}
	//�U�����蒆�Ȃ�
	if (m_isUnderAttack == true)
	{
		//�U���p�̃R���W�������쐬����
		MakeAttackCollision();
	}
}

void Player::MakeAttackCollision()
{
	//�R���W�����I�u�W�F�N�g���쐬����
	auto collisionObject = NewGO<CollisionObject>(0);
	//���W�������O�ɂ���H
	Vector3 collisionPosition = m_position;
	collisionPosition += m_forward * 50.0f;
	//�{�b�N�X��̃R���W�������쐬����
	collisionObject->CreateBox(collisionPosition,
		Quaternion::Identity,
		Vector3(20.0f, 220.0f, 20.0f));
	//���̃{�[���̃��[���h�s����擾����
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
	//���̃{�[���̃��[���h�s����R���W�����ɓK�p������
	collisionObject->SetName("player_attack");
	collisionObject->SetWorldMatrix(matrix);
}

void Player::MakeFirstSlashingEffect()
{
	//�G�t�F�N�g���쐬����
	effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(0);
	effectEmitter->SetScale(Vector3::One * 3.0f);
	Vector3 effectPosition = m_position;
	//���W��������&�O�ɂ���
	effectPosition.y += 210.0f;
	effectPosition += m_forward * 98.0f;
	effectEmitter->SetPosition(effectPosition);
	Quaternion effectRotation = m_rotation;
	//��]������
	effectRotation.AddRotationDegY(540.0f);
	effectRotation.AddRotationDegX(-70.0f);
	effectEmitter->SetRotation(effectRotation);
	//�G�t�F�N�g���Đ�����
	effectEmitter->Play();
	//SE���Đ�����
	SoundSource* m_se = NewGO<SoundSource>(0);
	m_se->Init(0);
	m_se->SetVolume(0.3f);
	m_se->Play(false);
}

void Player::MakeSecondSlashingEffect()
{
	//�G�t�F�N�g���쐬����
	effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(1);
	effectEmitter->SetScale(Vector3::One * 4.0f);
	Vector3 effectPosition = m_position;
	//�G�t�F�N�g��������&�O�ɂ���
	effectPosition.y += 175.0f;
	effectPosition += m_forward * 10.0f;
	effectEmitter->SetPosition(effectPosition);
	Quaternion effectRotation = m_rotation;
	//��]������
	effectRotation.AddRotationDegY(490.0f);
	effectRotation.AddRotationDegX(-180.0f);
	effectRotation.AddRotationDegZ(0.0f);
	effectEmitter->SetRotation(effectRotation);
	//�G�t�F�N�g���Đ�����
	effectEmitter->Play();
	//SE���Đ�����
	SoundSource* m_se = NewGO<SoundSource>(0);
	m_se->Init(1);
	m_se->SetVolume(0.3f);
	m_se->Play(false);
}

//�R�����g�O
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
	if (m_equipState == enEquipState_No && g_pad[0]->IsTrigger(enButtonRight))
	{
		m_equipState = enEquipState_Thuner;
		return;
	}
	if (m_equipState == enEquipState_Thuner && g_pad[0]->IsTrigger(enButtonRight)
		&& Healnum >= 1)
	{
		m_equipState = enEquipState_Heal;
		return;
	}
	if (m_equipState == enEquipState_Heal && g_pad[0]->IsTrigger(enButtonRight)
		&& Thundernum >=1)
	{
		m_equipState = enEquipState_Thuner;
		return;
	}
	if (m_equipState == enEquipState_No && g_pad[0]->IsTrigger(enButtonLeft))
	{
		m_equipState = enEquipState_Heal;
		return;
	}
	if (m_equipState == enEquipState_Thuner && g_pad[0]->IsTrigger(enButtonLeft) && Healnum >= 1)
	{
		m_equipState = enEquipState_Heal;
		return;
	}
	if (m_equipState == enEquipState_Heal && g_pad[0]->IsTrigger(enButtonLeft) && Thundernum >= 1)
	{
		m_equipState = enEquipState_Thuner;
		return;
	}
	
	switch (m_equipState)
	{
	case Player::enEquipState_No:
		m_alpha = 1.0f;
		m_alpha2 = 0.0f;
		m_alpha3 = 0.0f;
		break;
	case Player::enEquipState_Heal:
		m_alpha = 0.0f;
		m_alpha2 = 0.0f;
		m_alpha3 = 1.0f;
		break;
	case Player::enEquipState_Thuner:
		m_alpha = 0.0f;
		m_alpha2 = 1.0f;
		m_alpha3 = 0.0f;
		break;
	default:
		break;
	}
	//No
	m_sprintRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f,(m_alpha)));
	//thunder
	m_sprintRender2.SetMulColor(Vector4(1.0f, 1.0f, 1.0f,(m_alpha2)));
	//heal
	m_sprintRender3.SetMulColor(Vector4(1.0f, 1.0f, 1.0f,(m_alpha3)));
}


void Player::MakeMagicBall()
{
	IceBall* iceBall = NewGO<IceBall>(0);
	Vector3 iceBallPosition = m_position;
	iceBallPosition.y += 55.0f;
	iceBallPosition += m_forward * 300.0f;
	iceBall->SetPosition(iceBallPosition);
	iceBall->SetRotation(m_rotation);
	iceBall->SetEnUser(IceBall::enUser_Player);
}

void Player::MakeExplosionBall()
{
	ExplosionBall* explosionBall = NewGO<ExplosionBall>(0);
	Vector3 explosionBallPosition = m_position;
	explosionBallPosition.y += 55.0f;
	explosionBallPosition += m_forward * 300.0f;
	explosionBall->SetPosition(explosionBallPosition);
	explosionBall->SetRotation(m_rotation);
}
void Player::MakeRangeEffect()
{
	//�G�t�F�N�g���쐬����
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(3);
	effectEmitter->SetScale(Vector3::One * 25.0f);
	Vector3 effectPosition = m_position;
	//���W��������ɂ���
	effectPosition.y += 2.0f;
	effectEmitter->SetPosition(effectPosition);
	//�G�t�F�N�g���Đ�����
	effectEmitter->Play();
}

void Player::MakeEnemyStopCollision()
{
	//�R���W�����I�u�W�F�N�g���쐬����
	CollisionObject* m_collisionObject;
	m_collisionObject = NewGO<CollisionObject>(0);
	//����̃R���W�������쐬����
	m_collisionObject->CreateSphere(m_position,
		Quaternion::Identity,
		650.0f);
	m_collisionObject->SetName("item_thunder");
}

void Player::MakeEnemyStopEffect()
{
	//�G�t�F�N�g���쐬����
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(4);
	effectEmitter->SetScale(Vector3::One * 1.0f);
	effectEmitter->SetPosition(m_position);
	//�G�t�F�N�g���Đ�����
	effectEmitter->Play();
}

void Player::ProcessCommonStateTransition()
{
	//if (m_game->IsWannihilationEnemy())
	//{
		//�N���A�X�e�[�g�Ɉڍs����B
	//	m_playerState = enPlayerState_Win;
	//	return;
	//}

	//RB�{�^�����������&�U���^�C�v��0�Ȃ�
	if (g_pad[0]->IsTrigger(enButtonRB2) && Type == 0)
	{
		//�U���P�X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_FirstAttack;
		return;
	}
	//RB�{�^�����������&�U���^�C�v��1�Ȃ�
	if (g_pad[0]->IsTrigger(enButtonRB2) && Type == 1)
	{
		//�U��2�X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_SecondAttack;
		return;
	}
	//RB�{�^�����������&�U���^�C�v��2�Ȃ�
	if (g_pad[0]->IsTrigger(enButtonRB2) && Type == 2)
	{
		//�U��3�X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_ThirdAttack;
		return;
	}


	//RB1�{�^����LB1�{�^���������ꂽ�灕�A�C�e���X�e�[�g���T���_�[�Ȃ�
	if (g_pad[0]->IsPress(enButtonRB1) && g_pad[0]->IsPress(enButtonLB1)
		&& m_equipState == enEquipState_Thuner && Thundernum >= 1)
	{
		//�U���͈͂̃G�t�F�N�g���쐬����
		MakeRangeEffect();
		a = 1;
	}
	//�{�^������w�����ꂽ��
	else if (a == 1)
	{
		//�A�C�e���g�p�X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_UseItem;
		a = 0;
		Thundernum -= 1;
		return;
	}

	//RB1�{�^����LB1�{�^���������ꂽ�灕�A�C�e���X�e�[�g���񕜂Ȃ�
	if (g_pad[0]->IsTrigger(enButtonRB1) && g_pad[0]->IsTrigger(enButtonLB1)
		&& m_equipState == enEquipState_Heal && Healnum >= 1)
	{
		if (m_tower->m_hp < 200.0f) {
			Healnum -= 1;
			m_tower->m_hp += 50.0f;
			if (m_tower->m_hp > 200)
			{
				m_tower->m_hp = 200.0f;
			}
		}
	}
	//LB2�{�^���������ꂽ��
	if (m_mp >= 10)
	{
		if (g_pad[0]->IsTrigger(enButtonLB2))
		{
			//���@�U���X�e�[�g�Ɉڍs����
			m_playerState = enPlayerState_Magic;
			return;
		}
	}
	//x��z�̈ړ����x����������
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//�ړ��X�e�[�g�ֈڍs����
		m_playerState = enPlayerState_Run;
	}
	//x��z�̈ړ����x���Ȃ�������
	else
	{
		//�ҋ@�X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_Idle;
		return;
	}

}

void Player::ProcessIdleStateTransition()
{
	//�X�e�[�g��J�ڂ���
	ProcessCommonStateTransition();
}

void Player::ProcessRunStateTransition()
{
	//�X�e�[�g��J�ڂ���
	ProcessCommonStateTransition();
}


void Player::ProcessDamageStateTransition()
{
	//�A�j���[�V�������Đ������������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���
		ProcessCommonStateTransition();
	}
}

void Player::ProcessAttackStateTransition()
{
	//�A�j���[�V�������Đ������������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���
		ProcessCommonStateTransition();
	}
}
void Player::ProcessAttack2StateTransition()
{
	//�A�j���[�V�������Đ������������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���
		ProcessCommonStateTransition();
	}
}

void Player::ProcessAttack3StateTransition()
{
	//�A�j���[�V�������Đ������������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���
		ProcessCommonStateTransition();
	}
}

void Player::ProcessMagicStateTransition()
{
	//�A�j���[�V�������Đ������������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���
		ProcessCommonStateTransition();
	}
}

void Player::ProcessUseItemStateTransition()
{
	//�A�j���[�V�������Đ������������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���
		ProcessCommonStateTransition();
	}
}

void Player::ProcessWinStateTransition()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_game->GameEndFlag = true;
		m_game->GameClearNotice();
	}
}

void Player::ProcessDownStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���
		ProcessCommonStateTransition();
	}
}

void Player::ManageState()
{
	switch (m_playerState)
	{
	//�ҋ@�X�e�[�g�̎�
	case enPlayerState_Idle:
		//�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessIdleStateTransition();
		break;
	//�ړ��X�e�[�g�̎�
	case enPlayerState_Run:
		//�ړ��X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessRunStateTransition();
		break;
	//��_���[�W�X�e�[�g�̎�
	case enPlayerState_ReceiveDamage:
		//��_���[�W�X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessDamageStateTransition();
		break;
	//�U��1�X�e�[�g�̎�
	case enPlayerState_FirstAttack:
		//�U��1�X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessAttackStateTransition();
		break;
	//�U��2�X�e�[�g�̎�
	case enPlayerState_SecondAttack:
		//�U��2�X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessAttack2StateTransition();
		break;
	//�U��3�X�e�[�g�̎�
	case enPlayerState_ThirdAttack:
		//�U��3�X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessAttack3StateTransition();
		break;
	//���@�U���X�e�[�g�̎�
	case enPlayerState_Magic:
		//���@�U���X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessMagicStateTransition();
		break;
		//�A�C�e���g�p�X�e�[�g�̎�
	case enPlayerState_UseItem:
		//�A�C�e���g�p�X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessUseItemStateTransition();
		break;
	case enPlayerState_Win:
		ProcessWinStateTransition();
		break;
	case enPlayerState_Down:
		ProcessDownStateTransition();
		break;
	}
}

void Player::PlayAnimation()
{
	switch (m_playerState)
	{
	//�ҋ@�X�e�[�g�̎�
	case enPlayerState_Idle:
		m_modelRender.SetAnimationSpeed(2.0f);
		//�ҋ@�A�j���[�V�������Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.3f);
		break;
	//�ړ��X�e�[�g�̎�
	case enPlayerState_Run:
		m_modelRender.SetAnimationSpeed(1.0f);
		//�ړ��A�j���[�V�������Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.1f);
		break;
	//��_���[�W�X�e�[�g�̎�
	case enPlayerState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(1.4f);
		//��_���[�W�A�j���[�V�������Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
	//�U��1�X�e�[�g�̎�
	case enPlayerState_FirstAttack:
		m_modelRender.SetAnimationSpeed(1.6f);
		m_modelRender.PlayAnimation(enAnimationClip_FirstAttack, 0.3f);
		break;
	//�U��2�X�e�[�g�̎�
	case enPlayerState_SecondAttack:
		m_modelRender.SetAnimationSpeed(1.6f);
		//�U��2�A�j���[�V�������Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_SecondAttack, 0.3f);
		break;
	//�U��3�X�e�[�g�̎�
	case enPlayerState_ThirdAttack:
		m_modelRender.SetAnimationSpeed(1.6f);
		//�U��3�A�j���[�V�������Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_ThirdAttack, 0.3f);
		break;
	//���@�U���X�e�[�g�̎�
	case enPlayerState_Magic:
		m_modelRender.SetAnimationSpeed(1.8f);
		//���@�U���A�j���[�V�������Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_Magic, 0.3f);
		break;
	//�A�C�e���g�p�X�e�[�g�̎�
	case enPlayerState_UseItem:
		m_modelRender.SetAnimationSpeed(1.8f);
		//�A�C�e���g�p�A�j���[�V�������Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_UseItem, 0.5f);
		break;
	case enPlayerState_Win:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Win, 0.5f);
		break;
	case enPlayerState_Down:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.5f);
		break;
	default:
		break;
	}
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//�L�[�̖��O���uattack_start�v�Ȃ�
	if (wcscmp(eventName, L"attack_start") == 0)
	{
		//�U�����ɂ���
		m_isUnderAttack = true;
	}
	//�L�[�̖��O���uattack_end�v�Ȃ�
	else if (wcscmp(eventName, L"attack_end") == 0)
	{
		//�U�����I���
		m_isUnderAttack = false;
	}
	//�L�[�̖��O���ueffect_start�v�Ȃ�
	if (wcscmp(eventName, L"effect_start") == 0)
	{
		//�U��1�̃G�t�F�N�g���Đ�����
		MakeFirstSlashingEffect(); 
	}
	//�L�[�̖��O���ueffect_start2�v�Ȃ�
	if (wcscmp(eventName, L"effect_start2") == 0)
	{
		//�U��2�̃G�t�F�N�g���Đ�����
		MakeSecondSlashingEffect();
	}
	//�L�[�̖��O���ueffect_start3�v�Ȃ�
	if (wcscmp(eventName, L"effect_start3") == 0)
	{
		//�U��3�̃G�t�F�N�g���Đ�����
		MakeThirdSlashingEffect();
	}
	//�L�[�̖��O���umagic_attack�v�Ȃ�
	else if (wcscmp(eventName, L"magic_attack") == 0)
	{
		//���@�U�����쐬����
		MakeMagicBall();
		//MakeExplosionBall();
		m_mp -= 10;
	}
	//�L�[�̖��O���uthunder3�v�Ȃ�
	if (wcscmp(eventName, L"useitem") == 0)
	{
		//�R���W�������쐬���違�G�t�F�N�g���Đ�����
		MakeEnemyStopEffect();

	}
	if (wcscmp(eventName, L"collision") == 0)
	{
		MakeEnemyStopCollision();

	}
}

void Player::Render(RenderContext& rc)
{
	//���f����`�悷��
	m_modelRender.Draw(rc);
	//�摜��`�悷��
	m_sprintRender.Draw(rc);
	m_sprintRender2.Draw(rc);
	m_sprintRender3.Draw(rc);

}