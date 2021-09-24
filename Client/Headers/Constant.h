#pragma once

const unsigned int g_iBackCX = 1280;
const unsigned int g_iBackCY = 768;

enum SCENE {SCENE_STATIC, SCENE_LOGO, SCENE_STAGE, SCENE_BOSS, SCENE_OFF, SCENE_END};
enum CHANNEL_TYPE { CHANNEL_PLAYER, CHANNEL_PLAYER_DAMAGE, CHANNEL_PLAYER_EFFECT, CHANNEL_MONSTER, CHANNEL_MONSTER_WALK, CHANNEL_MONSTER_SHOUT, CHANNEL_MONSTER_BLADE, CHANNEL_MONSTER_BLADE2, CHANNEL_MONSTER_DAMAGE, CHANNEL_EFFECT, CHANNEL_BACKGROUND, CHANNEL_BLADE, CHANNEL_BLADE2, CHANNEL_END };

#define NO_COPY(ClassName)				\
private:								\
	ClassName(const ClassName&);				\
	ClassName& operator = (const ClassName&);	\

#define DECLARE_SINGLETON_NOTBASE(ClassName)	\
	NO_COPY(ClassName)					\
private:								\
	static ClassName* m_pInstance;		\
public:									\
	static ClassName* GetInstance()		\
	{									\
	if(NULL == m_pInstance)			\
	m_pInstance = new ClassName;\
	return m_pInstance;			\
	}									\
	void DestroyInstance()				\
	{									\
	if(m_pInstance)					\
		{								\
		delete m_pInstance;			\
		m_pInstance = NULL;			\
		}								\
	}

#define IMPLEMENT_SINGLETON_NOTBASE(ClassName)			\
	ClassName* ClassName::m_pInstance = NULL;