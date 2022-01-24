#pragma once
//�΂˃J�������g������"camera/SpringCamera.h"���C���N���[�h����
#include "camera/SpringCamera.h"

//�N���X�錾
class Player;        //�v���C���[�N���X

/// <summary>
/// �J����
/// </summary>
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	/// <summary>
	/// ���W�Ǝ��_���X�V����
	/// </summary>
	void UpdatePositionAndTarget();

private:
	Vector3       m_toCameraPos = Vector3::One;        //�����_���王�_�Ɍ������x�N�g��
	SpringCamera  m_springCamera;                      //�X�v�����O�J����
	Quaternion    m_rotation;                          //��]
	Player*       m_player = nullptr;                  //�v���C���[

};

