#pragma once
/*
	���ҽ��� �ɼ� �ִ°͵�?
	�ؽ���,�ε�������,���ؽ�����,�ִϸ��̼�����,��(�ؽ���+�ε�������+�ε�������+�ִϸ��̼�����)

	�ϳ��� ��������  �ʿ��� ���ҽ����� �ε����Ŀ� �� �ν��Ͻ��� ���ҽ����� �����Ͽ� �ν��Ͻ��������
	���� ������ �� ���ҽ����� �ݴ´�.
*/
namespace Sophia
{



class cReferenceCounter
{
public:
	cReferenceCounter(void) {m_RefCounter=0;}
	~cReferenceCounter(void) {}

	UINT	m_RefCounter;
	
public:
	UINT	AddRef();
	UINT	GetRefCounter() const { return m_RefCounter; }
	UINT	Release();	
	virtual void Free()=0;	
};

//template <typename T> 
class cResource:
	public cReferenceCounter
{
public:
	cResource( void )
	{

	}

	~cResource( void )
	{

	}
protected:
	std::string			m_strUniqeKey;			// ���ҽ� �Ŵ����� ���� ���� Ű	

	//T					m_key;
public:
	
	const  std::string&	GetUniqueKey() const { return m_strUniqeKey; }
	void			SetUniqueKey(std::string& val) { m_strUniqeKey = val; }
	virtual BOOL	Create()=0;		// AddReference ȣ���� ���� ���ҽ��� �ش�Ǵ� D3D API�� ȣ��

	/*
	void SetKey(T& key) {m_key = key; }
	const T GetKey() { return m_key; }
	*/
};


}