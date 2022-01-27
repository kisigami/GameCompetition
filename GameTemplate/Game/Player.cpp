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
	const float CHARACON_RADIUS = 60.0f;                          //�L�����R���̔��a
	const float CHARACON_HEIGHT = 250.0f;                         //�L�����R���̍���
	const float MOVE_SPEED = 750.0f;                              //�ړ����x
	const float ATTACK_COLLISION_FORWARD_POSITION = 50.0f;        //�U���p�̃R���W�����̑O�����̃x�N�g��
	const float COLLISION_BOX_SIZE_X = 10.0f;                     //�U���p�̃R���W������X�̑傫��
	const float COLLISION_BOX_SIZE_Y = 50.0f;                     //�U���p�̃R���W������Y�̑傫��
	const float COLLISION_BOX_SIZE_Z = 10.0f;                     //�U���p�̃R���W������Z�̑傫��
	const float TIMER_SPEED = 50.0f;                              //�A���U������^�C�}�[�̑��x
	const float SECOND_ATTACK_TIMER = 6.0f;                       //2��ڂ̍U���̃{�^�����͂̐�������
	const float THIRD_ATTACK_TIMER = 10.0f;                       //3��ڂ̍U���̃{�^�����͂̐�������
	const int FIRST_ATTACK_NUMBER = 0;                            //1��ڂ̍U���̍U���ԍ�
	const int SECOND_ATTACK_NUMBER = 1;                           //2��ڂ̍U���̍U���ԍ�
	const int THIRD_ATTACK_NUMBER = 2;                            //3��ڂ̍U���̍U���ԍ�
	const float FIRST_SLASHING_EFFECT_SCALE = 3.0f;               //1��ڂ̍U���G�t�F�N�g�̑傫��
	const float FIRST_SLASHING_EFFECT_POSITION_Y = 210.0f;        //1��ڂ̍U���G�t�F�N�g��Y���W
	const float FIRST_SLASHING_EFFECT_POSITION_FORWARD = 98.0f;   //1��ڂ̍U���G�t�F�N�g�̑O�����̍��W
	const float FIRST_SLASHING_EFFECT_ROTATION_X = -70.0f;        //1��ڂ̍U���G�t�F�N�g��X���̉�]
	const float FIRST_SLASHING_EFFECT_ROTATION_Y = 540.0f;        //1��ڂ̍U���G�t�F�N�g��Y���̉�]
	const float SECOND_SLASHING_EFFECT_SCALE = 4.0f;              //2��ڂ̍U���G�t�F�N�g�̑傫��
	const float SECOND_SLASHING_EFFECT_POSITION_Y = 175.0f;       //2��ڂ̍U���G�t�F�N�g��Y���W
	const float SECOND_SLASHING_EFFECT_FORWARD = 10.0f;           //2��ڂ̍U���G�t�F�N�g�̑O�����̍��W
	const float SECOND_SLASHING_EFFECT_ROTATION_X = -180.0f;      //2��ڂ̍U���G�t�F�N�g��X���̉�]
	const float SECOND_SLASHING_EFFECT_ROTATION_Y = 490.0f;       //2��ڂ̍U���G�t�F�N�g��Y���̉�]
	const float SECOND_SLASHING_EFFECT_ROTATION_Z = 0.0f;         //2��ڂ̍U���G�t�F�N�g��Z���̉�]
	const float THIRD_SLASHING_EFFECT_SCALE = 4.0f;               //3��ڂ̍U���G�t�F�N�g�̑傫��
	const float THIRD_SLASHING_EFFECT_FORWARD = 100.0f;           //3��ڂ̍U���G�t�F�N�g�̑O�����̍��W
	const float THIRD_SLASHING_EFFECT_ROTATION_X = -180.0f;       //3��ڂ̍U���G�t�F�N�g��X���̉�]
	const float THIRD_SLASHING_EFFECT_ROTATION_Y = 490.0f;        //3��ڂ̍U���G�t�F�N�g��Y���̉�]
	const float THIRD_SLASHING_EFFECT_ROTATION_Z = -160.0f;       //3��ڂ̍U���G�t�F�N�g��Z���̉�]
	const float MAGICBALL_POSITION_Y = 55.0f;                     //���@�U����Y���W
	const float MAGICBALL_POSITION_FORWARD = 300.0f;              //���@�U���̑O�����̍��W
	const float RESTRAINT_RANGE_SCALE = 25.0f;                    //�S�����@�̌��ʔ͈͂̑傫��
	const float RESTRAINT_COLLISION_SPHRE_RADIUS = 650.0f;        //�S�����@�̓����蔻��̃R���W�����̔��a
	const float RESTRAINT_EFFECT_SIZE = 1.0f;                     //�S�����@�̃G�t�F�N�g�̑傫��
	const float REPAIR_EFFECT_SIZE = 5.0f;                        //�^���[�C���̃G�t�F�N�g�̑傫��
	const float TOWER_DURABILITY_MAX = 200.0f;                    //�^���[�̑ϋv�l�̍ő�l
	const float REPAIR_DURABILITY_RECOVERY = 30.0f;               //�^���[�C���̑ϋv�l�̉񕜗�
	const float MAGICATTACK_CONSUME_MP = 10.0f;                   //���@�U���̏���MP
}

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
	
	//���f����ǂݍ���
	m_modelRender.Init("Assets/modelData/player/player4.tkm"
		,m_animationClips, enAnimationClip_Num
	);

	//���W�A��]�A�傫����ݒ肷��
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);

	//�L�����N�^�R���g���[���[�̏�����
	m_charaCon.Init(
		CHARACON_RADIUS,
		CHARACON_HEIGHT,
		m_position
	);

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
	//�ړ�����
	Move();
	//��]����
	Rotation();
	//�����蔻��
	Collision();
	//�A���U������
	SelectSkill();
	//�U������
	Attack();
	//�A�j���[�V�����̍Đ�
	PlayAnimation();
	//�X�e�[�g�̑J�ڏ���
	ManageState();

	//���f���̍X�V
	m_modelRender.Update();
}

void Player::Move()
{
	//�ړ��ł��Ȃ���ԂȂ�΁A�ړ��������s��Ȃ�
	if (IsEnableMove() == false)
	{
		return;
	}
	//�ړ����x
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	//�X�e�b�N�̓��͗ʂ��擾
	float lStick_x = g_pad[0]->GetLStickXF();
	float lStick_y = g_pad[0]->GetLStickYF();
	//camera�̑O�����ƉE�������擾
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//XZ���ʂł̑O�������A�E�����ɕϊ�����
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//XZ�����̈ړ����x���N���A
	m_moveSpeed += cameraForward * lStick_y * MOVE_SPEED;
	m_moveSpeed += cameraRight * lStick_x * MOVE_SPEED;
	//�L�����R�����g�p���āA���W���X�V����
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
	//��]��ݒ肷��
	m_modelRender.SetRotation(m_rotation);
	//�v���C���[�̐��ʃx�N�g�����v�Z��
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Player::Collision()
{
	//��_���[�W�X�e�[�g�Ȃ�
	if (m_playerState == enPlayerState_ReceiveDamage
		|| m_playerState == enPlayerState_Restraint || m_playerState == enPlayerState_Repair)
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
			//SE���Đ�����
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(31);
			se->SetVolume(0.2f);
			se->Play(false);
			//��_���[�W�X�e�[�g��
			m_playerState = enPlayerState_ReceiveDamage;
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
			//SE���Đ�����
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(31);
			se->SetVolume(0.2f);
			se->Play(false);
			//��_���[�W���X�e�[�g��
			m_playerState = enPlayerState_ReceiveDamage;
		}
	}
	//�{�X�̒x���U���̃R���W�������擾����
	const auto& collisions3 = g_collisionObjectManager->FindCollisionObjects("heavy_attack");
	//�z���for���ŉ�
	for (auto collision : collisions3)
	{
		//�R���W�����ƃL�����R�����Փ˂�����
		if (collision->IsHit(m_charaCon))
		{
			//SE���Đ�����
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(31);
			se->SetVolume(0.2f);
			se->Play(false);
			//��_���[�W���X�e�[�g�Ɉڍs����
			m_playerState = enPlayerState_ReceiveDamage;
		}
	}
	//�{�X�̑����U���̃R���W�������擾����
	const auto& collisions4 = g_collisionObjectManager->FindCollisionObjects("quick_attack");
	//�z���for���ŉ�
	for (auto collision : collisions4)
	{
		//�R���W�����ƃL�����R�����Փ˂�����
		if (collision->IsHit(m_charaCon))
		{
			//SE���Đ�����
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(31);
			se->SetVolume(0.2f);
			se->Play(false);
			//��_���[�W���X�e�[�g��
			m_playerState = enPlayerState_ReceiveDamage;
		}
	}
}

void Player::Hit()
{
	//1���ڂ̍U���X�e�[�g��������
	if (m_playerState == enPlayerState_FirstAttack)
	{
		//2���ڂ̍U���t���O��true�ɂ���
		m_secondAttackTimerFrag = true;
	}
	//2���ڂ̍U���X�e�[�g��������
	if (m_playerState == enPlayerState_SecondAttack)
	{
		//3���ڂ̍U���t���O��true�ɂ���
		m_secondAttackTimerFrag = true;
	}
	//2���ڂ̍U���t���O��true��������
	if (m_secondAttackTimerFrag == true)
	{
		//2���ڂ̍U���^�C�}�[�𑝉�������
		m_secondAttackTimer += g_gameTime->GetFrameDeltaTime() * TIMER_SPEED;
		//2���ڂ̍U���^�C�}�[��0.001f�ȏォ6.0f�ȓ��Ȃ�
		if (m_secondAttackTimer > 0.001f && m_secondAttackTimer <= SECOND_ATTACK_TIMER)
		{
			//�U���^�C�v��1�ɂ���
			m_attackNumber = SECOND_ATTACK_NUMBER;;
		}
		//2���ڂ̍U���^�C�}�[������ȊO�Ȃ�
		else
		{
			//�U���ԍ���1���ڂɂ���
			m_attackNumber = FIRST_ATTACK_NUMBER;
			//�^�C�}�[�����Z�b�g����
			m_secondAttackTimer = 0.0f;
			//2���ڂ̍U���t���O��false�ɂ���
			m_secondAttackTimerFrag = false;
		}
	}
	//3���ڂ̍U���t���O��true�ɂ���
	if (m_thirdAttackTimerFrag == true)
	{
		//3���ڂ̍U���^�C�}�[�𑝉�������
		m_thirdAttackTimer += g_gameTime->GetFrameDeltaTime() * TIMER_SPEED;
		//3���ڂ̍U���^�C�}�[��0.001f���傫��10.0f��菬����������
		if (m_thirdAttackTimer > 0.001f && m_thirdAttackTimer <= THIRD_ATTACK_TIMER)
		{
			//�U���ԍ���3���ڂɂ���
			m_attackNumber = THIRD_ATTACK_NUMBER;
		}
		else
		{
			//�U���ԍ���1���ڂɂ���
			m_attackNumber = FIRST_ATTACK_NUMBER;
			//�^�C�}�[�����Z�b�g����
			m_thirdAttackTimer = 0.0f;
			//3���ڂ̍U���t���O��false�ɂ���
			m_thirdAttackTimerFrag = false;
		}
	}
	//�U����H�������U���^�C�v��0��
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
	//���W�������O�ɂ���
	Vector3 collisionPosition = m_position;
	collisionPosition += m_forward * ATTACK_COLLISION_FORWARD_POSITION;
	//�{�b�N�X��̃R���W�������쐬����
	collisionObject->CreateBox(collisionPosition,
		Quaternion::Identity,
		Vector3(COLLISION_BOX_SIZE_X,COLLISION_BOX_SIZE_Y,COLLISION_BOX_SIZE_Z)
	);
	//���̃{�[���̃��[���h�s����擾����
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
	//���̃{�[���̃��[���h�s����R���W�����ɓK�p������
	collisionObject->SetName("player_attack");
	collisionObject->SetWorldMatrix(matrix);
}

void Player::MakeFirstSlashingEffect()
{
	//�G�t�F�N�g���Đ�����
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(0);
	effectEmitter->SetScale(Vector3::One * FIRST_SLASHING_EFFECT_SCALE);
	//���W��������&�O�ɂ���
	Vector3 effectPosition = m_position;
	effectPosition.y += FIRST_SLASHING_EFFECT_POSITION_Y;
	effectPosition += m_forward * FIRST_SLASHING_EFFECT_POSITION_FORWARD;
	effectEmitter->SetPosition(effectPosition);
	//��]��ݒ肷��
	Quaternion effectRotation = m_rotation;
	effectRotation.AddRotationDegX(FIRST_SLASHING_EFFECT_ROTATION_X);
	effectRotation.AddRotationDegY(FIRST_SLASHING_EFFECT_ROTATION_Y);
	effectEmitter->SetRotation(effectRotation);
	effectEmitter->Play();
}

void Player::MakeSecondSlashingEffect()
{
	//�G�t�F�N�g���Đ�����
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(1);
	effectEmitter->SetScale(Vector3::One * SECOND_SLASHING_EFFECT_SCALE);
	//���W��������&�O�ɂ���
	Vector3 effectPosition = m_position;
	effectPosition.y += SECOND_SLASHING_EFFECT_POSITION_Y;
	effectPosition += m_forward * SECOND_SLASHING_EFFECT_FORWARD;
	effectEmitter->SetPosition(effectPosition);
	//��]��ݒ肷��
	Quaternion effectRotation = m_rotation;
	effectRotation.AddRotationDegX(SECOND_SLASHING_EFFECT_ROTATION_X);
	effectRotation.AddRotationDegY(SECOND_SLASHING_EFFECT_ROTATION_Y);
	effectRotation.AddRotationDegZ(SECOND_SLASHING_EFFECT_ROTATION_Z);
	effectEmitter->SetRotation(effectRotation);
	effectEmitter->Play();
}

void Player::MakeThirdSlashingEffect()
{
	//�G�t�F�N�g���Đ�����
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(2);
	effectEmitter->SetScale(Vector3::One * THIRD_SLASHING_EFFECT_SCALE);
	//���W��������&�O�ɂ���
	Vector3 effectPosition = m_position;
	effectPosition.y += SECOND_SLASHING_EFFECT_POSITION_Y;
	effectPosition += m_forward * THIRD_SLASHING_EFFECT_FORWARD;
	effectEmitter->SetPosition(effectPosition);
	//��]��ݒ肷��
	Quaternion rotation = m_rotation;
	rotation.AddRotationDegX(THIRD_SLASHING_EFFECT_ROTATION_X);
	rotation.AddRotationDegY(THIRD_SLASHING_EFFECT_ROTATION_Y);
	rotation.AddRotationDegZ(THIRD_SLASHING_EFFECT_ROTATION_Z);
	effectEmitter->SetRotation(rotation);
	effectEmitter->Play();
}

void Player::MakeMagicBall()
{
	//���@�U�����쐬����
	MagicBall* magicBall = NewGO<MagicBall>(0);
	Vector3 magicBallPosition = m_position;
	//���W��������ɂ���
	magicBallPosition.y += MAGICBALL_POSITION_Y;
	//���W�������O�ɂ���
	magicBallPosition += m_forward * MAGICBALL_POSITION_FORWARD;
	magicBall->SetPosition(magicBallPosition);
	magicBall->SetRotation(m_rotation);
	//�g�p�҂̓v���C���[
	magicBall->SetEnUser(MagicBall::enUser_Player);
}

void Player::MakeRestraintRangeEffect()
{
	//�G�t�F�N�g���Đ�����
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(3);
	effectEmitter->SetScale(Vector3::One * RESTRAINT_RANGE_SCALE);
	Vector3 effectPosition = m_position;
	effectEmitter->SetPosition(effectPosition);
	effectEmitter->Play();
}

void Player::MakeRestraintCollision()
{
	//�R���W�����I�u�W�F�N�g���쐬����
	CollisionObject* collisionObject;
	collisionObject = NewGO<CollisionObject>(0);
	//����̃R���W�������쐬����
	collisionObject->CreateSphere(m_position,
		Quaternion::Identity,
		RESTRAINT_COLLISION_SPHRE_RADIUS);
	collisionObject->SetName("item_thunder");
}

void Player::MakeRestraintEffect()
{
	//�G�t�F�N�g���Đ�����
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(4);
	effectEmitter->SetScale(Vector3::One * RESTRAINT_EFFECT_SIZE);
	effectEmitter->SetPosition(m_position);
	effectEmitter->Play();
}

void Player::MakeRepairEffect()
{
	//�G�t�F�N�g���Đ�����
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(5);
	effectEmitter->SetScale(Vector3::One * REPAIR_EFFECT_SIZE);
	Vector3 effectPosition = m_tower->GetPosition();
	effectEmitter->SetPosition(effectPosition);
	effectEmitter->Play();
}

void Player::SelectSkill()
{
	//�X�L���X�e�[�g���^���[�C���X�e�[�g��������
	if (m_skillState == enSkillState_Repair)
	{
		//�E�{�^�������{�^���������ꂽ��
		if (g_pad[0]->IsTrigger(enButtonRight) || g_pad[0]->IsTrigger(enButtonLeft))
		{
			//SE���Đ�����
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(29);
			se->SetVolume(0.3f);
			se->Play(false);
			//�S�����@�X�e�[�g��
			m_skillState = enSkillState_Restraint;
			return;
		}
	}
	//�X�L���X�e�[�g���S�����@�X�e�[�g��������
	if (m_skillState == enSkillState_Restraint)
	{
		//�E�{�^�������{�^���������ꂽ��
		if (g_pad[0]->IsTrigger(enButtonRight) || g_pad[0]->IsTrigger(enButtonLeft))
		{
			//SE���Đ�����
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(29);
			se->SetVolume(0.3f);
			se->Play(false);
			//�^���[�C���X�e�[�g��
			m_skillState = enSkillState_Repair;
			return;
		}
	}
}


void Player::ProcessCommonStateTransition()
{
	//�t�F�[�h���Ȃ�
	if (m_game->m_isBattleStartFade == true)
	{
		//�t�F�[�h�ҋ@�X�e�[�g��
		m_playerState = enPlayerState_FadeWait;
		return;
	}
	//RB�{�^�����������&�U���^�C�v��0�Ȃ�
	if (g_pad[0]->IsTrigger(enButtonRB2) && m_attackNumber == FIRST_ATTACK_NUMBER)
	{
		//�U���P�X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_FirstAttack;
		return;
	}
	//RB�{�^�����������&�U���^�C�v��1�Ȃ�
	if (g_pad[0]->IsTrigger(enButtonRB2) && m_attackNumber == SECOND_ATTACK_NUMBER)
	{
		//�U��2�X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_SecondAttack;
		return;
	}
	//RB�{�^�����������&�U���^�C�v��2�Ȃ�
	if (g_pad[0]->IsTrigger(enButtonRB2) && m_attackNumber == THIRD_ATTACK_NUMBER)
	{
		//�U��3�X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_ThirdAttack;
		return;
	}

	//RB1�{�^����LB1�{�^���������ꂽ�灕
	//�X�L���X�e�[�g���S�����@�X�e�[�g�������灕
	//�S�����@�̎g�p�񐔂�1���傫��������
	if (g_pad[0]->IsPress(enButtonRB1) && g_pad[0]->IsPress(enButtonLB1)
		&& m_skillState == enSkillState_Restraint && m_restraintNum >= 1)
	{
		//�U���͈͂̃G�t�F�N�g���쐬����
		MakeRestraintRangeEffect();
		//�v���X�{�^���t���O��true�ɂ���
		m_pressButtonFrag = true;
	}
	//�v���X�{�^���t���O��true��������
	//�{�^������w�����ꂽ��
	else if (m_pressButtonFrag == true)
	{
		//�S�����@�X�e�[�g��
		m_playerState = enPlayerState_Restraint;
		//�S�����@�̎g�p�񐔂�1���炷
		m_restraintNum -= 1;
		//�v���X�{�^���t���O��false�ɂ���
		m_pressButtonFrag = false;
		return;
	}

	//RB1�{�^����LB1�{�^���������ꂽ�灕
	//�X�L���X�e�[�g���^���[�C���X�e�[�g�������灕
	//�^���[�C���̎g�p�񐔂�1���傫��������
	if (g_pad[0]->IsTrigger(enButtonRB1) && g_pad[0]->IsTrigger(enButtonLB1)
		&& m_skillState == enSkillState_Repair && m_repairNum >= 1)
	{
		//�^���[�̑ϋv�l���ő�l��菬����������
		if (m_tower->GetDurability() < TOWER_DURABILITY_MAX)
		{
			//SE���Đ�����
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(8);
			se->SetVolume(0.3f);
			se->Play(false);
			//�^���[�C���̃G�t�F�N�g���Đ�����
			MakeRepairEffect();
			//�^���[�C���X�e�[�g��
			m_playerState = enPlayerState_Repair;
			//�^���[�C���̎g�p�񐔂�1���炷
			m_repairNum -= 1;
			//�^���[�̑ϋv�l��30�񕜂���
			float towerdurability = m_tower->GetDurability();
			towerdurability += REPAIR_DURABILITY_RECOVERY;
			m_tower->SetDurability(towerdurability);
			//�^���[�̑ϋv�l���ő�l���傫��������
			if (m_tower->GetDurability() > TOWER_DURABILITY_MAX)
			{
				//�^���[�̑ϋv�l���ő�l�ɂ���
				towerdurability = m_tower->GetDurability(); 
				towerdurability = TOWER_DURABILITY_MAX;
				m_tower->SetDurability(towerdurability);
			}
		}
		return;
	}
	
	//MP��10���傫��������
	if (m_mp >= MAGICATTACK_CONSUME_MP)
	{
		//LB2�{�^���������ꂽ��
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
	//���̃X�e�[�g��J�ڂ���
	ProcessCommonStateTransition();
}

void Player::ProcessRunStateTransition()
{
	//���̃X�e�[�g��J�ڂ���
	ProcessCommonStateTransition();
}

void Player::ProcessFristAttackStateTransition()
{
	//�A�j���[�V�������Đ������������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g��J�ڂ���
		ProcessCommonStateTransition();
	}
}

void Player::ProcessSecondAttackStateTransition()
{
	//�A�j���[�V�������Đ������������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g��J�ڂ���
		ProcessCommonStateTransition();
	}
}

void Player::ProcessThirdAttackStateTransition()
{
	//�A�j���[�V�������Đ������������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g��J�ڂ���
		ProcessCommonStateTransition();
	}
}

void Player::ProcessMagicStateTransition()
{
	//�A�j���[�V�������Đ������������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g��J�ڂ���
		ProcessCommonStateTransition();
	}
}

void Player::ProcessDamageStateTransition()
{
	//�A�j���[�V�������Đ������������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g��J�ڂ���
		ProcessCommonStateTransition();
	}
}

void Player::ProcessRepairStateTransition()
{
	//�A�j���[�V�������Đ������������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g��J�ڂ���
		ProcessCommonStateTransition();
	}
}

void Player::ProcessRestraintStateTransition()
{
	//�A�j���[�V�������Đ������������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g��J�ڂ���
		ProcessCommonStateTransition();
	}
}

void  Player::ProcessFadeWaitStateTransition()
{
	//�o�g���X�^�[�g�t�F�[�h���I�������
	if (m_game->m_isWaitBattleFadeout == false)
	{
		//���̃X�e�[�g��J�ڂ���
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
		//1���ڂ̍U���X�e�[�g�̎�
	case enPlayerState_FirstAttack:
		//�U��1�X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessFristAttackStateTransition();
		break;
		//2���ڂ̍U���X�e�[�g�̎�
	case enPlayerState_SecondAttack:
		//�U��2�X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessSecondAttackStateTransition();
		break;
		//3���ڂ̍U���X�e�[�g�̎�
	case enPlayerState_ThirdAttack:
		//�U��3�X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessThirdAttackStateTransition();
		break;
		//���@�U���X�e�[�g�̎�
	case enPlayerState_Magic:
		//���@�U���X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessMagicStateTransition();
		break;
		//�S�����@�X�e�[�g�̎�
	case enPlayerState_Restraint:
		//�S�����@�X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessRestraintStateTransition();
		break;
		//�^���[�̏C���X�e�[�g�̎�
	case enPlayerState_Repair:
		//�^���[�̏C���X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessRepairStateTransition();
		break;
		//�t�F�[�h�ҋ@�X�e�[�g�̎�
	case enPlayerState_FadeWait:
		//�t�F�[�h�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ���
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
		//�Đ����x
		m_modelRender.SetAnimationSpeed(2.0f);
		//�ҋ@�A�j���[�V�������Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.3f);
		break;
	//�ړ��X�e�[�g�̎�
	case enPlayerState_Run:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(1.0f);
		//�ړ��A�j���[�V�������Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.1f);
		break;
	//��_���[�W�X�e�[�g�̎�
	case enPlayerState_ReceiveDamage:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(1.4f);
		//��_���[�W�A�j���[�V�������Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
	//1���ڂ̍U���X�e�[�g�̎�
	case enPlayerState_FirstAttack:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(1.6f);
		//1���ڂ̍U���A�j���[�V�������Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_FirstAttack, 0.3f);
		break;
	//2���ڂ̍U���X�e�[�g�̎�
	case enPlayerState_SecondAttack:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(1.6f);
		//2���ڂ̍U���A�j���[�V�������Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_SecondAttack, 0.3f);
		break;
	//3���ڂ̍U���X�e�[�g�̎�
	case enPlayerState_ThirdAttack:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(1.7f);
		//3���ڂ̍U���A�j���[�V�������Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_ThirdAttack, 0.3f);
		break;
	//���@�U���X�e�[�g�̎�
	case enPlayerState_Magic:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(1.8f);
		//���@�U���A�j���[�V�������Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_Magic, 0.3f);
		break;
	//�S�����@�X�e�[�g�̎�
	case enPlayerState_Restraint:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(1.8f);
		//�S�����@�A�j���[�V�������Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_Restraint, 0.5f);
		break;
		//�^���[�C���X�e�[�g�̎�
	case enPlayerState_Repair:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(1.8f);
		//�^���[�C���A�j���[�V�������Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_Repair, 0.5f);
		break;
		//�t�F�[�h�ҋ@�X�e�[�g�̎�
	case enPlayerState_FadeWait:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(2.0f);
		//�ҋ@�A�j���[�V�������Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.3f);
		break;
	}
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	if (wcscmp(eventName, L"attack_start") == 0)
	{
		//�U�����ɂ���
		m_isUnderAttack = true;
	}
	else if (wcscmp(eventName, L"attack_end") == 0)
	{
		//�U�����I���
		m_isUnderAttack = false;
	}
	if (wcscmp(eventName, L"effect_start") == 0)
	{
		//SE���Đ�����
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(0);
		se->SetVolume(0.3f);
		se->Play(false);
		//�U��1�̃G�t�F�N�g���Đ�����
		MakeFirstSlashingEffect(); 
	}
	if (wcscmp(eventName, L"effect_start2") == 0)
	{
		//SE���Đ�����
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(1);
		se->SetVolume(0.3f);
		se->Play(false);
		//�U��2�̃G�t�F�N�g���Đ�����
		MakeSecondSlashingEffect();
	}
	if (wcscmp(eventName, L"effect_start3") == 0)
	{
		//SE���Đ�����
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(2);
		se->SetVolume(0.5f);
		se->Play(false);
		//�U��3�̃G�t�F�N�g���Đ�����
		MakeThirdSlashingEffect();
	}
	else if (wcscmp(eventName, L"magic_attack") == 0)
	{
		//Mp��10���炷
		m_mp -= MAGICATTACK_CONSUME_MP;
		//���@�U�����쐬����
		MakeMagicBall();
	}
	if (wcscmp(eventName, L"useitem") == 0)
	{
		//�S�����@�̃G�t�F�N�g���Đ�����
		MakeRestraintEffect();
		//�S�����@�p�̓����蔻��R���W�������쐬����
		MakeRestraintCollision();
		//SE���Đ�����
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(7);
		se->SetVolume(0.3f);
		se->Play(false);
	}
}

void Player::Render(RenderContext& rc)
{
	//���f����`�悷��
	m_modelRender.Draw(rc);
}