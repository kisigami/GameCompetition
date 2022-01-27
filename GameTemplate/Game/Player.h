#pragma once

//�N���X�錾
class Game;           //�Q�[���N���X
class MagicBall;      //���@�U���N���X
class Tower;          //�^���[�N���X
class PlayerUi;       //�v���C���[UI

/// <summary>
/// �v���C���[
/// </summary>
class Player : public IGameObject
{
public:
	/// <summary>
	/// �v���C���[�X�e�[�g
	/// </summary>
	enum EnPlayerState {
		enPlayerState_FadeWait,         //�t�F�[�h�ҋ@�X�e�[�g
		enPlayerState_Idle,             //�ҋ@�X�e�[�g
		enPlayerState_Run,              //����X�e�[�g
		enPlayerState_Repair,           //�^���[�C���g�p�X�e�[�g
		enPlayerState_Restraint,        //�S�����@�g�p�X�e�[�g
		enPlayerState_FirstAttack,      //1���ڂ̍U���X�e�[�g
		enPlayerState_SecondAttack,     //2���ڂ̍U���X�e�[�g
		enPlayerState_ThirdAttack,      //3���ڂ̍U���X�e�[�g
		enPlayerState_Magic,            //���@�U���X�e�[�g
		enPlayerState_ReceiveDamage,    //��_���[�W�X�e�[�g
	};
public:
	/// <summary>
	/// �X�L���X�e�[�g
	/// </summary>
	enum EnSkillState {
		enSkillState_Repair,              //�^���[�C���X�e�[�g
		enSkillState_Restraint,           //�S�����@�X�e�[�g
	};
public:
	Player();
	~Player();
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
	/// �傫�����擾
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
	/// Mp��ݒ�
	/// </summary>
	/// <param name="mp">MP</param>
	void SetMp(const float& mp)
	{
		m_mp = mp;
	}
	/// <summary>
	/// Mp���擾
	/// </summary>
	/// <returns>MP</returns>
	const float& GetMp() const
	{
		return m_mp;
	}
	/// <summary>
	/// �������Ԃ��ǂ����擾
	/// </summary>
	/// <returns>������X�e�[�g�Ȃ�true</returns>
	bool IsEnableMove() const
	{
		return
			m_playerState != enPlayerState_FadeWait&&
			m_playerState != enPlayerState_FirstAttack &&
			m_playerState != enPlayerState_SecondAttack &&
			m_playerState != enPlayerState_ThirdAttack &&
			m_playerState != enPlayerState_Restraint &&
			m_playerState != enPlayerState_Repair &&
			m_playerState != enPlayerState_ReceiveDamage &&
			m_playerState != enPlayerState_Magic;
	}
	/// <summary>
	/// �^���[�C���X�e�[�g���H
	/// </summary>
	/// <returns>�^���[�C���X�e�[�g�Ȃ�true</returns>
	const bool SelectRepair() const
	{
		return m_skillState == enSkillState_Repair;
	}
	/// <summary>
	/// �S�����@�X�e�[�g���H
	/// </summary>
	/// <returns>�S�����@�X�e�[�g�Ȃ�true</returns>
	const bool SelectRestraint() const
	{
		return m_skillState == enSkillState_Restraint;
	}
	/// <summary>
	/// �^���[�C���̎g�p�񐔂��擾
	/// </summary>
	/// <returns>�^���[�C���̎g�p��</returns>
	const int& GetRepairNum() const
	{
		return m_repairNum;
	}
	/// <summary>
	/// �S�����@�̎g�p�񐔂��擾
	/// </summary>
	/// <returns>�S�����@�̎g�p��</returns>
	const int& GetRestraintNum() const
	{
		return m_restraintNum;
	}
private:
	/// <summary>
	/// �X�L���I������
	/// </summary>
	void SelectSkill();
	/// <summary>
	/// �A���U���̔��菈��
	/// </summary>
	void  Hit();
	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();
	/// <summary>
	/// ��]����
	/// </summary>
	void Rotation();
	/// <summary>
	/// �G�̍U���Ƃ̓����蔻�菈��
	/// </summary>
	void Collision();
	/// <summary>
	/// �U������
	/// </summary>
	void Attack();
	/// <summary>
	/// �U���p�̓����蔻��p�̃R���W�������쐬����
	/// </summary>
	void MakeAttackCollision();
	/// <summary>
	/// 1���ڂ̍U���G�t�F�N�g
	/// </summary>
	void MakeFirstSlashingEffect();
	//// <summary>
	/// 2���ڂ̍U���G�t�F�N�g
	/// </summary>
	void MakeSecondSlashingEffect();
	/// <summary>
	/// 3���ڂ̍U���G�t�F�N�g
	/// </summary>
	void MakeThirdSlashingEffect();
	/// <summary>
	/// ���@�U�����쐬����
	/// </summary>
	void MakeMagicBall();
	/// <summary>
	///	�S�����@�̌��ʔ͈͂̃G�t�F�N�g
	/// </summary>
	void MakeRestraintRangeEffect();
	/// <summary>
	/// �S�����@�̃G�t�F�N�g
	/// </summary>
	void MakeRestraintEffect();
	/// <summary>
	/// �S�����@�̃R���W����
	/// </summary>
	void MakeRestraintCollision();
	/// <summary>
	/// �^���[�C���̃G�t�F�N�g
	/// </summary>
	void MakeRepairEffect();
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// �e�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ManageState();
	/// <summary>
	/// �A�j���[�V�������O�C�x���g
	/// </summary>
	/// <param name="clipName">�A�j���[�V�����̖��O</param>
	/// <param name="eventName">�A�j���[�V�����C�x���g�L�[�̖��O</param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	/// <summary>
	/// ���ʂ̃X�e�[�g�J�ڏ���
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// �ҋ@�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// ����X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessRunStateTransition();
	/// <summary>
	/// ��_���[�W�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessDamageStateTransition();
	/// <summary>
	/// 1���ڂ̍U���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessFristAttackStateTransition();
	/// <summary>
	/// 2���ڂ̍U���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessSecondAttackStateTransition();
	/// <summary>
	/// 3���ڂ̍U���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessThirdAttackStateTransition();
	/// <summary>
	/// ���@�U���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessMagicStateTransition();
	/// <summary>
	/// �S�����@�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessRestraintStateTransition();
	/// <summary>
	/// �t�F�[�h�ҋ@�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessFadeWaitStateTransition();
	/// <summary>
	/// �^���[�C���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessRepairStateTransition();

	//�A�j���[�V�����N���b�v
	enum EnAnimationClip {
		enAnimationClip_Idle,              //�ҋ@�A�j���[�V����
		enAnimationClip_Run,               //����A�j���[�V����
		enAnimationClip_Damage,            //��_���[�W�A�j���[�V����
		enAnimationClip_FirstAttack,       //1���ڂ̍U���A�j���[�V����
		enAnimationClip_SecondAttack,      //2���ڂ̍U���A�j���[�V����
		enAnimationClip_ThirdAttack,       //3���ڂ̍U���A�j���[�V����
		enAnimationClip_Magic,             //���@�U���A�j���[�V����
		enAnimationClip_Restraint,		   //�S�����@�A�j���[�V����
		enAnimationClip_Repair,            //�^���[�C���A�j���[�V����
		enAnimationClip_Num                //�A�j���[�V�����̐�
	};
    
	ModelRender          m_modelRender;                          //���f�������_�[
	AnimationClip		 m_animationClips[enAnimationClip_Num];  //�A�j���[�V�����N���b�v//���f�������_�[
	Vector3              m_position;                             //���W
	Vector3              m_scale = Vector3::One;                 //�傫��
	Vector3              m_moveSpeed;                            //�ړ����x
	Vector3              m_forward = Vector3::AxisZ;             //�v���C���[�̑O�����̃x�N�g��
	Quaternion           m_rotation;                             //��]
	CharacterController  m_charaCon;                             //�L�����N�^�[�R���g���[���[
	EnPlayerState        m_playerState = enPlayerState_Idle;     //�v���C���[�X�e�[�g
	EnSkillState         m_skillState = enSkillState_Restraint;  //�X�L���X�e�[�g
	bool                 m_isUnderAttack = false;                //�U�������H
	int                  m_swordBoneId = -1;                     //�\�[�h�{�[��ID
	float                m_mp = 40;                              //MP
	int                  m_attackNumber = 0;                     //�U���ԍ�
	bool                 m_secondAttackTimerFrag = false;        //1���ڂ���̃^�C�}�[�̃t���O
	bool                 m_thirdAttackTimerFrag = false;         //2���ڂ���̃^�C�}�[�̃t���O
	float                m_secondAttackTimer = 0.0f;             //1���ڂ���̃^�C�}�[
	float                m_thirdAttackTimer = 0.0f;              //2���ڂ���̃^�C�}�[
	bool                 m_pressButtonFrag = false;              //�v���X�{�^���t���O
	int                  m_restraintNum = 1;                     //�S�����@�̎g�p��
	int                  m_repairNum = 1;                        //�^���[�C���̎g�p��
	Game*                m_game = nullptr;                       //�Q�[��
	Tower*               m_tower = nullptr;                      //�^���[
	PlayerUi*            m_playerUi = nullptr;                   //�v���C���[UI
};
