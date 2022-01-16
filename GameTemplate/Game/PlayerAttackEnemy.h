#pragma once

//�N���X�錾
class Player;       //�v���C���[�N���X
class Game;         //�Q�[���N���X

//�v���C���[�A�^�b�N�G�l�~�[
class PlayerAttackEnemy:public IGameObject
{
public:
	//�G�l�~�[�X�e�[�g
	enum EnPlayerAttackEnemyState {
		enPlayerAttackEnemyState_Idle,               //�ҋ@��
		enPlayerAttackEnemyState_Assault,            //�ˌ�
		enPlayerAttackEnemyState_Attack,             //�U��
		enPlayerAttackEnemyState_ReceiveDamage,      //��_���[�W
		enPlayerAttackEnemyState_ReceiveRestraint,   //�S��
		enPlayerAttackEnemyState_Down,               //�_�E��
	};
public:
	PlayerAttackEnemy();
	~PlayerAttackEnemy();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	//���W��ݒ�
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	//���W���擾
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	//�傫����ݒ�
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	//��]��ݒ�
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	//HP��ݒ�
	void SetHP(const int hp)
	{
		m_hp = hp;
	}

	//�ړ�����
	void Move();
	//��]����
	void Rotation();
	//�U������
	void Attack();
	//�v���C���[�Ƃ̓����蔻�菈��
	void Collision();
	//�U���p�̓����蔻����쐬����
	void MakeAttackCollision();
	//�S������
	void Restraint();
	//��_���[�W�G�t�F�N�g
	void MakeDamageEffect();
	//�A�j���[�V�����C�x���g�p�̊֐�
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//�A�j���[�V�����̍Đ�
	void PlayAnimation();
	//�e�X�e�[�g�̑J�ڏ���
	void ManageState();
	//���ʂ̃X�e�[�g�J�ڏ���
	void ProcessCommonStateTransition();
	//�ҋ@�X�e�[�g�̑J�ڏ���
	void ProcessIdleStateTransition();
	//�ˌ��X�e�[�g�̑J�ڏ���
	void ProcessAssultStateTransition();
	//�U���X�e�[�g�̑J�ڏ���
	void ProcessAttackStateTransition();
	//��_���[�W�X�e�[�g�̑J�ڏ���
	void ProcessReceiveDamageStateTransition();
	//�_�E���X�e�[�g�̑J�ڏ���
	void ProcessDownStateTransition();
	//�S���X�e�[�g�̑J�ڏ���
	void ProcessReceiveRestraintStateTransition();

	//�v���C���[���U���ł��邩
	const bool IsCanPlayerAttack() const;

	enum EnAnimationClip {    	              //�A�j���[�V�����N���b�v
		enAnimationClip_Idle,                 //�ҋ@�A�j���[�V����
		enAnimationClip_Assault,              //�ˌ��A�j���[�V����
		enAnimationClip_Attack,               //�U���A�j���[�V����
		enAnimationClip_ReceiveDamage,        //��_���[�W�A�j���[�V����
		enAnimationClip_ReceiveRestraint,     //�S���A�j���[�V����
		enAnimationClip_Down,                 //�_�E���A�j���[�V����
		enAnimationClip_Num,	              //�A�j���[�V�����̐�
	};

	AnimationClip                  m_animationClips[enAnimationClip_Num];		                 //�A�j���[�V�����N���b�v
	ModelRender                    m_modelRender;                                                //���f�������_�[
	CharacterController	           m_charaCon;								                     //�L�����R��
	Vector3                        m_position;                                                   //���W
	Vector3                        m_moveSpeed;                                                  //�ړ����x
	Vector3                        m_scale = Vector3::One;                                       //�傫��
	Vector3                        m_forward = Vector3::AxisZ;                                   //�v���C���[�A�^�b�N�G�l�~�[�̐��ʃx�N�g��
	Quaternion                     m_rotation;                                                   //��]
	bool                           m_isUnderAttack = false;                                      //�U�������H
	EnPlayerAttackEnemyState       m_playerattackenemyState = enPlayerAttackEnemyState_Assault;  //�v���C���[�A�^�b�N�G�l�~�[�X�e�[�g
	int                            m_swordBoneId = -1;                                           //�\�[�h�{�[��ID
	int                            m_hp = 2;                                                     //HP
	float                          electrictimer = 8.0f;                                         //�S���^�C�}�[
	Player*                        m_player = nullptr;                                           //�v���C���[
	Game*                          m_game = nullptr;                                             //�Q�[��
};
