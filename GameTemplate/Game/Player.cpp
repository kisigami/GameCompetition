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
	m_animationClips[enAnimationClip_UseHeal].Load("Assets/animData/heal.tka");
	m_animationClips[enAnimationClip_UseHeal].SetLoopFlag(false);
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
	m_itemWaku.Init("Assets/sprite/item/itemwaku.dds", 1920.0f, 1080.0f);
	m_itemStop.Init("Assets/sprite/item/itemstop.dds", 1920.0f, 1080.0f);
	m_itemHeal.Init("Assets/sprite/item/itemheal.dds", 1920.0f, 1080.0f);

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
	EffectEngine::GetInstance()->ResistEffect(5, u"Assets/efk/heal.efk");

	//�e�T�E���h��ǂݍ���
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
			Type = 0;//�U���^�C�v��0�ɂ���
			m_attackTimer2 = 0.0f;//�^�C�}�[�����Z�b�g����
			move2 = 0;
		}
	}

	//�U����H�������U���^�C�v��0��
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
	if (m_playerState == enPlayerState_ReceiveDamage 
		|| m_playerState == enPlayerState_UseItem || m_playerState == enPlayerState_UseHeal)
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
			SoundSource* m_se = NewGO<SoundSource>(0);
			m_se->Init(31);
			m_se->SetVolume(0.2f);
			m_se->Play(false);
			//��_���[�W�X�e�[�g�Ɉڍs����
			m_playerState = enPlayerState_ReceiveDamage;
			//SE���Đ�����
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
			SoundSource* m_se = NewGO<SoundSource>(0);
			m_se->Init(31);
			m_se->SetVolume(0.2f);
			m_se->Play(false);
			//��_���[�W���X�e�[�g�Ɉڍs����
			m_playerState = enPlayerState_ReceiveDamage;
		}
	}

	const auto& collisions3 = g_collisionObjectManager->FindCollisionObjects("heavy_attack");
	//�z���for���ŉ�
	for (auto collision : collisions3)
	{
		//�R���W�����ƃL�����R�����Փ˂�����
		if (collision->IsHit(m_charaCon))
		{
			SoundSource* m_se = NewGO<SoundSource>(0);
			m_se->Init(31);
			m_se->SetVolume(0.2f);
			m_se->Play(false);
			//��_���[�W���X�e�[�g�Ɉڍs����
			m_playerState = enPlayerState_ReceiveDamage;
		}
	}

	const auto& collisions4 = g_collisionObjectManager->FindCollisionObjects("quick_attack");
	//�z���for���ŉ�
	for (auto collision : collisions4)
	{
		//�R���W�����ƃL�����R�����Փ˂�����
		if (collision->IsHit(m_charaCon))
		{
			SoundSource* m_se = NewGO<SoundSource>(0);
			m_se->Init(31);
			m_se->SetVolume(0.2f);
			m_se->Play(false);
			//��_���[�W���X�e�[�g�Ɉڍs����
			m_playerState = enPlayerState_ReceiveDamage;
		}
	}
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
		Vector3(10.0f, 200.0f, 10.0f));
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
	//���@�U�����쐬����
	MagicBall* magicBall = NewGO<MagicBall>(0);
	Vector3 magicBallPosition = m_position;
	//���W��������ɂ���
	magicBallPosition.y += 55.0f;
	//���W�������O�ɂ���
	magicBallPosition += m_forward * 300.0f;
	magicBall->SetPosition(magicBallPosition);
	magicBall->SetRotation(m_rotation);
	//�g�p�҂̓v���C���[
	magicBall->SetEnUser(MagicBall::enUser_Player);
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
	m_restraintNum -= 1;
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
		&& m_equipState == enEquipState_Thuner && m_restraintNum >= 1)
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
		return;
	}


	//RB1�{�^����LB1�{�^���������ꂽ�灕�A�C�e���X�e�[�g���񕜂Ȃ�
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
	case enPlayerState_UseHeal:
		//�A�C�e���g�p�X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessHealStateTransition();
		break;
	case enPlayerState_FadeWait:
		//�A�C�e���g�p�X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessFadeWaitStateTransition();
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
		m_modelRender.SetAnimationSpeed(1.7f);
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
	case enPlayerState_FadeWait:
		m_modelRender.SetAnimationSpeed(2.0f);
		//�ҋ@�A�j���[�V�������Đ�����
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
		m_mp -= 10;
		MakeMagicBall();
	}
	//�L�[�̖��O���uthunder3�v�Ȃ�
	if (wcscmp(eventName, L"useitem") == 0)
	{
		//�R���W�������쐬���違�G�t�F�N�g���Đ�����
		MakeEnemyStopEffect();
		MakeEnemyStopCollision();

	}

}

void Player::Render(RenderContext& rc)
{
	
	//���f����`�悷��
	m_modelRender.Draw(rc);
	//�摜��`�悷��
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