#pragma once

//�N���X�錾
class Player;          //�v���C���[�N���X
class Tower;           //�^���[�N���X
class Game;            //�Q�[���N���X

/// <summary>
/// �G�l�~�[
/// </summary>
class Enemy:public IGameObject
{
public:
	/// <summary>
	/// �G�l�~�[�X�e�[�g
	/// </summary>
	enum EnEnemyState {
		enEnemyState_Idle,              //�ҋ@�X�e�[�g
		enEnemyState_Assault,           //�ˌ��X�e�[�g
		enEnemyState_Attack,            //�U���X�e�[�g
		enEnemyState_ReceiveDamage,     //��_���[�W�X�e�[�g
		enEnemyState_ReceiveRestraint,  //�S���X�e�[�g
		enEnemyState_Down,              //�_�E���X�e�[�g
	};
public:
	Enemy();
	~Enemy();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// �傫����ݒ�
	/// </summary>
	/// <param name="scale">�傫��</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	/// <param name="rotation">��]</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// HP��ݒ�
	/// </summary>
	/// <param name="hp">HP</param>
	void SetHP(const int hp)
	{
		m_hp = hp;
	}

private:
	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();
	/// <summary>
	/// ��]����
	/// </summary>
	void Rotation();
	/// <summary>
	/// �U������
	/// </summary>
	void Attack();
	/// <summary>
	/// �v���C���[�̍U���Ƃ̓����蔻�菈��
	/// </summary>
	void Collision();
	/// <summary>
	/// �U���p�̓����蔻����쐬����
	/// </summary>
	void MakeAttackCollision();
	/// <summary>
	/// �S������
	/// </summary>
	void Restraint();
	/// <summary>
	/// ��_���[�W�G�t�F�N�g
	/// </summary>
	void MakeDamageEffect();
	/// <summary>
	/// �A�j���[�V�����C�x���g�p�̊֐�
	/// </summary>
	/// <param name="clipName">�A�j���[�V�����̖��O</param>
	/// <param name="eventName">�A�j���[�V�����C�x���g�̃L�[�̖��O</param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// �e�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ManageState();
	/// <summary>
	/// ���ʂ̃X�e�[�g�J�ڏ���
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// �ҋ@�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// �ˌ��X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessAssultStateTransition();
	/// <summary>
	/// �U���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// ��_���[�W�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// �_�E���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessDownStateTransition();
	/// <summary>
	/// �S���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessReceiveRestraintStateTransition();

	/// <summary>
	/// �v���C���[���U���ł��邩
	/// </summary>
	/// <returns>�U���ł���Ȃ�true</returns>
	const bool IsCanPlayerAttack() const;
	/// <summary>
	/// �����U���ł��邩
	/// </summary>
	/// <returns>�U���ł���Ȃ�true</returns>
	const bool IsCanTowerAttack() const;
	/// <summary>
	///�A�j���[�V�����N���b�v
	/// </summary>
	enum EnAnimationClip {    	           //�A�j���[�V�����N���b�v
		enAnimationClip_Idle,              //�ҋ@�A�j���[�V����
		enAnimationClip_Assult,            //�ˌ��A�j���[�V����
		enAnimationClip_Attack,            //�U���A�j���[�V����
		enAnimationClip_ReceiveDamage,     //��_���[�W�A�j���[�V����
		enAnimationClip_ReceiveRestraint,  //�S���A�j���[�V����
		enAnimationClip_Down,              //�_�E���A�j���[�V����
		enAnimationClip_Num,	           //�A�j���[�V�����̐�
	};
	AnimationClip         m_animationClips[enAnimationClip_Num];    //�A�j���[�V�����N���b�v 
	ModelRender           m_modelRender;                            //���f�������_�[
	CharacterController	  m_charaCon;						        //�L�����N�^�[�R���g���[���[
	Vector3               m_position;                               //���W
	Vector3               m_moveSpeed;                              //�ړ����x
	Vector3	              m_scale = Vector3::One;                   //�傫��
	Vector3               m_forward = Vector3::AxisZ;               //�G�l�~�[�̐��ʃx�N�g��
	Quaternion            m_rotation;                               //��]
	bool                  m_isUnderAttack = false;                  //�U�������H
	EnEnemyState          m_enemyState = enEnemyState_Assault;      //�G�l�~�[�X�e�[�g
	int                   m_swordBoneId = -1;                       //�\�[�h�{�[��ID
	int                   m_hp =3;                                  //HP
	float                 m_restraintTimer = 8.0f;                  //�S���^�C�}�[
	Game*                 m_game = nullptr;                         //�Q�[��
	Tower*                m_tower = nullptr;                        //�^���[
	Player*               m_player = nullptr;                       //�v���C���[
};

