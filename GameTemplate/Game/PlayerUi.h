#pragma once

//�N���X�錾
class Player;      //�v���C���[�N���X

/// <summary>
/// �v���C���[UI
/// </summary>
class PlayerUi : public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// MP�̉摜�̑傫���̏���
	/// </summary>
	void MpScale();
	/// <summary>
	/// �X�L���̎g�p�񐔕\���̏���
	/// </summary>
	void SkillFont();

	FontRender   m_skillNumFont;            //�X�L���̎g�p�񐔂̕���
	SpriteRender m_playerIcon;              //�v���C���[�A�C�R���̉摜
	SpriteRender m_mp;                      //MP�̉摜
	SpriteRender m_mpFarame;                //MP�̘g�摜
	SpriteRender m_mpButtom;                //MP�̒�̉摜
	SpriteRender m_skillFrame;              //�X�L���̘g�̉摜
	SpriteRender m_restraint;               //�S�����@�̉摜
	SpriteRender m_repair;                  //�^���[�C���̉摜
	float        m_mpSpriteScale = 1.0f;    //MP�̉摜�̑傫��
	Player*      m_player = nullptr;        //�v���C���[
};

