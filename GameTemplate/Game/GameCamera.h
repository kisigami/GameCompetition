#pragma once
//�΂˃J�������g�p�������ꍇ�́Aspringcamera���C���N���[�h����
#include "camera/SpringCamera.h"

class Player;        //�v���C���[�N���X
//�Q�[���J�����N���X
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	//���W�Ǝ��_���X�V����
	void UpdatePositionAndTarget();
private:
	Vector3       m_toCameraPos = Vector3::One;        //�����_���王�_�Ɍ������x�N�g��
	SpringCamera  m_springCamera;                      //�X�v�����O�J����
	Quaternion    m_rotation;                          //��]
	Player*       m_player = nullptr;                  //�v���C���[

};

