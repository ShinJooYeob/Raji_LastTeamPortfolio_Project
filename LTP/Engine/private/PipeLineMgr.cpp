#include "..\Public\PipeLineMgr.h"

IMPLEMENT_SINGLETON(CPipeLineMgr);


CPipeLineMgr::CPipeLineMgr()
{

}

HRESULT CPipeLineMgr::Set_Transform(TRANSFORMSTATETYPE eStateType, const _fMatrix& TransformMatrix)
{
	m_TransfromMatrix[eStateType] = TransformMatrix;
	return S_OK;
}

HRESULT CPipeLineMgr::Set_Transform(TRANSFORMSTATETYPE eStateType, const _float4x4 & TransformMatrix)
{
	memcpy((m_TransfromMatrix[eStateType].m), (TransformMatrix.m), sizeof(_float) * 16);
	return S_OK;
}

_fMatrix CPipeLineMgr::Get_Transform_Matrix(TRANSFORMSTATETYPE eStateType)
{
	return m_TransfromMatrix[eStateType].XMatrix();
}

_float4x4 CPipeLineMgr::Get_Transform_Float4x4(TRANSFORMSTATETYPE eStateType)
{
	return m_TransfromMatrix[eStateType];
}

_fMatrix CPipeLineMgr::Get_Transform_Matrix_TP(TRANSFORMSTATETYPE eStateType)
{
	return m_TransfromMatrix[eStateType].TransposeXMatrix();
}

_float4x4 CPipeLineMgr::Get_Transform_Float4x4_TP(TRANSFORMSTATETYPE eStateType)
{
	return m_TransfromMatrix[eStateType].TransposeXMatrix();
}

HRESULT CPipeLineMgr::Set_TargetPostion(TARGETPOSITIONTYPE eStateType, const _fVector & TargetPos)
{
	m_vTargetPosition[eStateType] = TargetPos;
	return S_OK;
}

HRESULT CPipeLineMgr::Set_TargetPostion(TARGETPOSITIONTYPE eStateType, const _float4 & TargetPos)
{
	m_vTargetPosition[eStateType] = TargetPos;
	return S_OK;
}

_fVector CPipeLineMgr::Get_TargetPostion_Vector(TARGETPOSITIONTYPE eStateType)
{
	return m_vTargetPosition[eStateType].XMVector();
}

_float4 CPipeLineMgr::Get_TargetPostion_float4(TARGETPOSITIONTYPE eStateType)
{
	return m_vTargetPosition[eStateType];
}


void CPipeLineMgr::Free()
{

}
