#pragma once

#include "Defines.h"

//abstract : �߻�Ŭ������ ����� �ִ� Ű����.
class CMyMouse abstract
{
public:
	static D3DXVECTOR3 GetMousePos(void)
	{
		D3DXVECTOR3 vPos;

		POINT ptMouse;

		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		vPos = D3DXVECTOR3((float)ptMouse.x, (float)ptMouse.y, 0.f);

		return vPos;
	}

};
