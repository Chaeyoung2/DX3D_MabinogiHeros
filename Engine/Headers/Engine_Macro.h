#pragma once

#define MSG_BOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("System Error"), MB_OK)

#define DECLARE_SINGLETON(CLASSNAME)					\
public:													\
static CLASSNAME* Get_Instance(void);					\
unsigned long Destroy_Instance(void);					\
private:												\
static CLASSNAME* m_pInstance;

#define IMPLEMENT_SINGLETON(CLASSNAME)					\
CLASSNAME* CLASSNAME::m_pInstance = nullptr;			\
CLASSNAME* CLASSNAME::Get_Instance(void){				\
if(nullptr == m_pInstance) m_pInstance = new CLASSNAME;	\
return m_pInstance;	}									\
unsigned long CLASSNAME::Destroy_Instance(void){		\
unsigned long dwRefCnt = 0;								\
if(nullptr != m_pInstance) {							\
dwRefCnt = m_pInstance->Release();						\
if(0 != dwRefCnt) return dwRefCnt;						\
m_pInstance = nullptr; }								\
return dwRefCnt; }

#define BEGIN(NAMESPACE) namespace NAMESPACE {
#define END }
#define USING(NAMESPACE) using namespace NAMESPACE;

#ifdef ENGINE_EXPORTS
#define DLL_EXPORT _declspec(dllexport) 
#else
#define DLL_EXPORT _declspec(dllimport) 
#endif



// #define NO_COPY(CLASSNAME)							\
// 	private:										\
// 	CLASSNAME(const CLASSNAME&);					\
// 	CLASSNAME& operator = (const CLASSNAME&);		
// 
// #define DECLARE_SINGLETON_(CLASSNAME)				\
// 	NO_COPY(CLASSNAME)								\
// 	private:										\
// 	static CLASSNAME*	m_pInstance;				\
// 	public:											\
// 	static CLASSNAME*	GetInstance( void );		\
// 	static void DestroyInstance( void );			
// 
// #define IMPLEMENT_SINGLETON_(CLASSNAME)				\
// 	CLASSNAME*	CLASSNAME::m_pInstance = NULL;		\
// 	CLASSNAME*	CLASSNAME::GetInstance( void )	{	\
// 		if(NULL == m_pInstance) {					\
// 			m_pInstance = new CLASSNAME;			\
// 		}											\
// 		return m_pInstance;							\
// 	}												\
// 	void CLASSNAME::DestroyInstance( void ) {		\
// 		if(NULL != m_pInstance)	{					\
// 			delete m_pInstance;						\
// 			m_pInstance = NULL;						\
// 		}											\
// 	}
