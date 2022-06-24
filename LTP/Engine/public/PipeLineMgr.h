#pragma once
#include "Base.h"

BEGIN(Engine)
class CPipeLineMgr :public CBase
{
	DECLARE_SINGLETON(CPipeLineMgr);


private:
	explicit CPipeLineMgr();
	virtual ~CPipeLineMgr() = default;


public:
	HRESULT Set_Transform(TRANSFORMSTATETYPE eStateType, const _fMatrix& TransformMatrix);
	HRESULT Set_Transform(TRANSFORMSTATETYPE eStateType, const _float4x4& TransformMatrix);

	_fMatrix Get_Transform_Matrix(TRANSFORMSTATETYPE eStateType);
	_float4x4 Get_Transform_Float4x4(TRANSFORMSTATETYPE eStateType);
	_fMatrix Get_Transform_Matrix_TP(TRANSFORMSTATETYPE eStateType);
	_float4x4 Get_Transform_Float4x4_TP(TRANSFORMSTATETYPE eStateType);

public:
	HRESULT Set_TargetPostion(TARGETPOSITIONTYPE eStateType, const _fVector& TargetPos);
	HRESULT Set_TargetPostion(TARGETPOSITIONTYPE eStateType, const _float4& TargetPos);

	_fVector Get_TargetPostion_Vector(TARGETPOSITIONTYPE eStateType);
	_float4  Get_TargetPostion_float4(TARGETPOSITIONTYPE eStateType);

public:


private:
	_float4x4		m_TransfromMatrix[PLM_END];
	_float4			m_vTargetPosition[PLV_END];

public:
	virtual void Free() override;

};
END

