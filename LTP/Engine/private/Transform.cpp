#include "..\Public\Transform.h"
#include "Shader.h"
#include "GameInstance.h"



CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CComponent(pDevice, pDeviceContext)
{
}

CTransform::CTransform(const CTransform & rhs)
	:CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{
}


HRESULT CTransform::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	m_WorldMatrix = XMMatrixIdentity();

	return S_OK; 
}

HRESULT CTransform::Initialize_Clone(void * pArg)
{
	if (FAILED( __super::Initialize_Clone(pArg)))
		return E_FAIL;
	
		if (pArg != nullptr)
			memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));



	return S_OK;
}

_Vector CTransform::Get_MatrixScale(TransformState eState)
{
	return XMVector3Length(XMLoadFloat3((_float3*)&m_WorldMatrix.m[eState][0]));
}

_Matrix CTransform::Get_MatrixScale_All()
{
	_Matrix TransMat = m_WorldMatrix.XMatrix();

	_Matrix matResult;

	matResult.r[0] = XMVector3Length(TransMat.r[0]);
	matResult.r[1] = XMVector3Length(TransMat.r[1]);
	matResult.r[2] = XMVector3Length(TransMat.r[2]);
	matResult.r[3] = _float4(1).XMVector();


	return matResult;
}

void CTransform::Set_MatrixState(TransformState eState, const _fVector & vVec)
{
	XMStoreFloat3((_float3*)(&m_WorldMatrix.m[eState][0]), vVec);
}

void CTransform::Set_MatrixState(TransformState eState, const _float3 & vVec)
{
	memcpy((_float3*)(&m_WorldMatrix.m[eState][0]), &vVec, sizeof(_float3));

}


void CTransform::Set_Matrix(const _Matrix& mat)
{
	XMStoreFloat4x4(&(m_WorldMatrix), mat);
}



void CTransform::Move_Forward(_double fDeltaTime)
{
	_Vector vPos = Get_MatrixState(CTransform::STATE_POS);

	vPos += Get_MatrixState_Normalized(CTransform::STATE_LOOK) * m_TransformDesc.fMovePerSec *(_float)fDeltaTime;

	Set_MatrixState(CTransform::STATE_POS, vPos);
}

void CTransform::Move_Backward(_double fDeltaTime)
{
	Move_Forward(-fDeltaTime);
}

void CTransform::Move_Right(_double fDeltaTime)
{
	_Vector vPos = Get_MatrixState(CTransform::STATE_POS);

	vPos += Get_MatrixState_Normalized(CTransform::STATE_RIGHT) * m_TransformDesc.fMovePerSec *(_float)fDeltaTime;

	Set_MatrixState(CTransform::STATE_POS, vPos);
}

void CTransform::Move_Left(_double fDeltaTime)
{
	Move_Right(-fDeltaTime);
}

void CTransform::Move_Up(_double fDeltaTime)
{
	_Vector vPos = Get_MatrixState(CTransform::STATE_POS);

	vPos += Get_MatrixState_Normalized(CTransform::STATE_UP) * m_TransformDesc.fMovePerSec * (_float)fDeltaTime;

	Set_MatrixState(CTransform::STATE_POS, vPos);
}

void CTransform::Move_Down(_double fDeltaTime)
{
	Move_Up(-fDeltaTime);
}

void CTransform::MovetoDir(_fVector vDir, _double fDeltaTime)
{
	_Vector vPos = Get_MatrixState(CTransform::STATE_POS);
	
	vPos += XMVector3Normalize(vDir)* m_TransformDesc.fMovePerSec *(_float)fDeltaTime;

	Set_MatrixState(CTransform::STATE_POS, vPos);
}

void CTransform::MovetoDir_bySpeed(_fVector vDir, _float fSpeed, _double fDeltaTime)
{
	_Vector vPos = Get_MatrixState(CTransform::STATE_POS);

	vPos += XMVector3Normalize(vDir)* fSpeed *(_float)fDeltaTime;

	Set_MatrixState(CTransform::STATE_POS, vPos);
}

void CTransform::MovetoTarget(_fVector vTarget, _double fDeltaTime)
{

	_Vector vPos = Get_MatrixState(CTransform::STATE_POS);

	vPos += XMVector3Normalize(vTarget - vPos)* m_TransformDesc.fMovePerSec *(_float)fDeltaTime;

	Set_MatrixState(CTransform::STATE_POS, vPos);
}

void CTransform::LookAt(_fVector vTarget)
{
	_Vector vPos = Get_MatrixState(CTransform::STATE_POS);

	_Matrix matScale = Get_MatrixScale_All();

	_Vector vRight;
	_Vector vLook = XMVector3Normalize(vTarget - vPos);

	XMStoreFloat3((_float3*)(m_WorldMatrix.m[STATE_LOOK]), vLook * matScale.r[STATE_LOOK]);

	if (XMVector3Equal(vLook, _float3(0, 1, 0).XMVector()))
	{
		//__debugbreak();
		//MSGBOX("Can't Cross With Same Vector");
		OutputDebugStringW(L"Can't Cross With Same Vector");

		vRight = XMVector3Normalize(XMVector3Cross(_float3(0.0000001f, 1, 0).XMVector(), vLook));
		XMStoreFloat3((_float3*)(m_WorldMatrix.m[STATE_RIGHT]), vRight * matScale.r[STATE_RIGHT]);
	}
	else
	{
		vRight = XMVector3Normalize(XMVector3Cross(_float3(0, 1, 0).XMVector(), vLook));
		XMStoreFloat3((_float3*)(m_WorldMatrix.m[STATE_RIGHT]), vRight * matScale.r[STATE_RIGHT]);
	}

	XMStoreFloat3((_float3*)(m_WorldMatrix.m[STATE_UP]), XMVector3Normalize(XMVector3Cross(vLook, vRight)) * matScale.r[STATE_UP]);

}

void CTransform::LookDir(_fVector vTargetLook)
{
	_Matrix matScale = Get_MatrixScale_All();

	_Vector vRight;
	_Vector vLook = XMVector3Normalize(vTargetLook);

	XMStoreFloat3((_float3*)(m_WorldMatrix.m[STATE_LOOK]), vLook * matScale.r[STATE_LOOK]);

	if (XMVector3Equal(vLook, _float3(0, 1, 0).XMVector()))
	{
		//__debugbreak();
		//MSGBOX("Can't Cross With Same Vector");
		OutputDebugStringW(L"Can't Cross With Same Vector");

		vRight = XMVector3Normalize(XMVector3Cross(_float3(0.0000001f, 1, 0).XMVector(), vLook));
		XMStoreFloat3((_float3*)(m_WorldMatrix.m[STATE_RIGHT]), vRight * matScale.r[STATE_RIGHT]);
	}
	else
	{
		vRight = XMVector3Normalize(XMVector3Cross(_float3(0, 1, 0).XMVector(), vLook));
		XMStoreFloat3((_float3*)(m_WorldMatrix.m[STATE_RIGHT]), vRight * matScale.r[STATE_RIGHT]);
	}
	
	XMStoreFloat3((_float3*)(m_WorldMatrix.m[STATE_UP]), XMVector3Normalize(XMVector3Cross(vLook, vRight)) * matScale.r[STATE_UP]);
}

void CTransform::Turn_CW(_fVector vAxis, _double fDeltaTime)
{
	_Matrix matRUL = m_WorldMatrix.XMatrix();

	_Matrix	RotationMatrix = XMMatrixRotationAxis(vAxis, m_TransformDesc.fRotationPerSec * (_float)fDeltaTime);

	matRUL.r[STATE_RIGHT] = XMVector4Transform(matRUL.r[STATE_RIGHT], RotationMatrix);
	matRUL.r[STATE_UP] = XMVector4Transform(matRUL.r[STATE_UP], RotationMatrix);
	matRUL.r[STATE_LOOK] = XMVector4Transform(matRUL.r[STATE_LOOK], RotationMatrix);

	Set_Matrix(matRUL);
}

void CTransform::Turn_CCW(_fVector vAxis, _double fDeltaTime)
{
	Turn_CW(vAxis, -fDeltaTime);
}

void CTransform::Turn_Direct(_fVector vAxis, _float fRadian)
{
	_Matrix matRUL = m_WorldMatrix.XMatrix();

	_Matrix	RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	matRUL.r[STATE_RIGHT] = XMVector4Transform(matRUL.r[STATE_RIGHT], RotationMatrix);
	matRUL.r[STATE_UP] = XMVector4Transform(matRUL.r[STATE_UP], RotationMatrix);
	matRUL.r[STATE_LOOK] = XMVector4Transform(matRUL.r[STATE_LOOK], RotationMatrix);

	Set_Matrix(matRUL);
}
	

void CTransform::Turn_Dir(_fVector vTargetDir, _float fWeight)
{
	if (0 > fWeight || 1 < fWeight)
	{
		__debugbreak();
		MSGBOX("fWeight is Out of ranged");
		return;
	}

	_Vector vRotDir, vMyLook = Get_MatrixState_Normalized(TransformState::STATE_LOOK);
	
	vRotDir = XMVector3Normalize(vTargetDir);
	_Vector vDot = XMVector3Dot(vRotDir, vMyLook);
	if (-0.8f > XMVectorGetX(vDot))
	{
		vRotDir = (Get_MatrixState_Normalized(TransformState::STATE_RIGHT) * (0.5f)) + vRotDir  * (0.5f);
		XMVector3Normalize(vRotDir);
	}

	vRotDir = (vMyLook * fWeight) + vRotDir  * (1.f - fWeight);
	XMVector3Normalize(vRotDir);
	vRotDir = XMVectorSetW(vRotDir, 0.f);
	LookDir(vRotDir);
}

void CTransform::Turn_Interp(_fVector vTargetDir, _float fWeight)
{

}

void CTransform::Rotation_CW(_fVector vAxis, _float fRadian)
{
	_Matrix matRUL = XMMatrixIdentity();
	_Matrix matSacle = Get_MatrixScale_All();
	_Matrix	RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	matRUL.r[STATE_RIGHT]	= XMVector4Transform(matRUL.r[STATE_RIGHT] * matSacle.r[STATE_RIGHT] , RotationMatrix);
	matRUL.r[STATE_UP]		= XMVector4Transform(matRUL.r[STATE_UP] * matSacle.r[STATE_UP], RotationMatrix);
	matRUL.r[STATE_LOOK]	= XMVector4Transform(matRUL.r[STATE_LOOK] * matSacle.r[STATE_LOOK], RotationMatrix);
	matRUL.r[STATE_POS]		= Get_MatrixState(CTransform::STATE_POS);

	Set_Matrix(matRUL);
}

void CTransform::Rotation_CCW(_fVector vAxis, _float fRadian)
{
	Rotation_CW(vAxis, -fRadian);
}

void CTransform::Scaled(TransformState eState, _float fScale)
{
	Set_MatrixState(eState, (Get_MatrixState_Normalized(eState))*_float4(fScale).XMVector());
}

void CTransform::Scaled_All(_float3 vfScale)
{
	Scaled(CTransform::STATE_RIGHT, vfScale.x);
	Scaled(CTransform::STATE_UP, vfScale.y);
	Scaled(CTransform::STATE_LOOK, vfScale.z);
}


void CTransform::Scaling(TransformState eState, _double fDeltaTime)
{

	Set_MatrixState(eState, (Get_MatrixState_Normalized(eState)) * Get_MatrixScale(eState) * (1 + m_TransformDesc.fScalingPerSec * _float(fDeltaTime)));
}

void CTransform::Scaling_All(_double fDeltaTime)
{

	Set_MatrixState(STATE_RIGHT, (Get_MatrixState_Normalized(STATE_RIGHT)) * Get_MatrixScale(STATE_RIGHT) * (1 + m_TransformDesc.fScalingPerSec * _float(fDeltaTime)));
	Set_MatrixState(STATE_UP, (Get_MatrixState_Normalized(STATE_UP)) * Get_MatrixScale(STATE_UP) * (1 + m_TransformDesc.fScalingPerSec * _float(fDeltaTime)));
	Set_MatrixState(STATE_LOOK, (Get_MatrixState_Normalized(STATE_LOOK)) * Get_MatrixScale(STATE_LOOK) * (1 + m_TransformDesc.fScalingPerSec * _float(fDeltaTime)));
}

HRESULT CTransform::Bind_OnShader(CShader * pShader, const char * pValueName)
{

	_float4x4		ShaderWorldMatrix;

	XMStoreFloat4x4(&ShaderWorldMatrix, m_WorldMatrix.TransposeXMatrix());
	
	return pShader->Set_RawValue(pValueName, &ShaderWorldMatrix, sizeof(_float4x4));

	return S_OK;
}

HRESULT CTransform::Bind_OnShader_ApplyPivot(CShader * pShader, const char * pValueName)
{

	_float4x4		ShaderWorldMatrix = m_WorldMatrix;

	ShaderWorldMatrix._41 += m_TransformDesc.vPivot.x;
	ShaderWorldMatrix._42 += m_TransformDesc.vPivot.y;
	ShaderWorldMatrix._43 += m_TransformDesc.vPivot.z;

	ShaderWorldMatrix = ShaderWorldMatrix.TransposeXMatrix();

	//XMStoreFloat4x4(&ShaderWorldMatrix, m_WorldMatrix.TransposeXMatrix());

	return pShader->Set_RawValue(pValueName, &ShaderWorldMatrix, sizeof(_float4x4));

}

HRESULT CTransform::Bind_OnShader_BillBoard(CShader * pShader, const char * pValueName, _fMatrix& ViewMatrix)
{
	_Matrix vCamWorldMat = XMMatrixInverse(nullptr,ViewMatrix);

	_Matrix TempWorldMat = m_WorldMatrix.XMatrix();

	_Matrix vScaleMat = Get_MatrixScale_All();

	TempWorldMat.r[0] = vCamWorldMat.r[0] * vScaleMat.r[0];
	TempWorldMat.r[1] = vCamWorldMat.r[1] * vScaleMat.r[1];
	TempWorldMat.r[2] = vCamWorldMat.r[2] * vScaleMat.r[2];

	_float4x4 ShaderMat = XMMatrixTranspose(TempWorldMat);

	return pShader->Set_RawValue(pValueName, &ShaderMat, sizeof(_float4x4));
}

HRESULT CTransform::Bind_OnShader_BillBoard_ApplyPivot(CShader * pShader, const char * pValueName, _fMatrix& ViewMatrix)
{
	_Matrix vCamWorldMat = XMMatrixInverse(nullptr, ViewMatrix);

	_Matrix TempWorldMat = m_WorldMatrix.XMatrix();

	_Matrix vScaleMat = Get_MatrixScale_All();

	TempWorldMat.r[0] = vCamWorldMat.r[0] * vScaleMat.r[0];
	TempWorldMat.r[1] = vCamWorldMat.r[1] * vScaleMat.r[1];
	TempWorldMat.r[2] = vCamWorldMat.r[2] * vScaleMat.r[2];
	TempWorldMat.r[3] = TempWorldMat.r[3] + XMVectorSetW(m_TransformDesc.vPivot.XMVector(),0);

	_float4x4 ShaderMat = XMMatrixTranspose(TempWorldMat);

	return pShader->Set_RawValue(pValueName, &ShaderMat, sizeof(_float4x4));
}




CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{

	CTransform* pInstance = new CTransform(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg))) {
		MSGBOX("Failed to Create Transform Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform* pInstance = new CTransform((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Create Transform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}
