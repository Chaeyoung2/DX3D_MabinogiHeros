#pragma once

#include "Constant.h"


#define KEYMGR CKeyMgr::GetInstance()

class CKeyMgr
{
	DECLARE_SINGLETON_NOTBASE(CKeyMgr)

public:
	bool	m_bKeyDown[256];	// Ű�� ���ȴ��� üũ�� �迭
	bool	m_bKeyUp[256];	// Ű�� �������� üũ�� �迭

private:
	CKeyMgr(void);
	~CKeyMgr(void);

public:
	bool StayKeyDown(INT nKey); // Ű�� ������ �ִ��� üũ
	bool OnceKeyDown(INT nKey); // Ű�� �ѹ� ���ȴ��� üũ
	bool OnceKeyUp(INT nKey);	// Ű�� �ѹ� ���ȴ� �������� üũ
	bool IsToggleKey(INT nKey);	// �ѹ� ���������� on off�� �ٲٵ���

};