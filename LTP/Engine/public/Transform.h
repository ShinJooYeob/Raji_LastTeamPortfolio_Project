#pragma once

#include "Component.h"


BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
public:
	enum TransformState { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POS, STATE_END };
	typedef struct tagTransformDesc
	{
		_float		fMovePerSec;
		_float		fRotationPerSec;
		_float		fScalingPerSec = 0.f;
		//바인드 할 때 월드 좌표를 밀기는 별로일때 좌표를 살짝 보정해주는 용도
		_float3		vPivot = _float3(0, 0, 0);
	}TRANSFORMDESC;

	

private:
	explicit CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;
private:
	HRESULT Initialize_Prototype(void * pArg);
	HRESULT Initialize_Clone(void * pArg);

public:
	//SetDesc
	void Set_TransformDesc(const TRANSFORMDESC& TransformDesc) { m_TransformDesc = TransformDesc; };

	void Set_MoveSpeed(_float fMoveSpeed) { m_TransformDesc.fMovePerSec = fMoveSpeed; };
	void Set_TurnSpeed(_float TurnSpeed) { m_TransformDesc.fRotationPerSec = TurnSpeed; };
	void Set_ScalingSpeed(_float ScalingSpeed) { m_TransformDesc.fScalingPerSec = ScalingSpeed; };	
	void Set_Pivot(_float3 vPivot) { memcpy(&(m_TransformDesc.vPivot), &vPivot, sizeof(_float3)); };


	_float Get_TurnSpeed() { return m_TransformDesc.fRotationPerSec; };
	_float Get_MoveSpeed() { return m_TransformDesc.fMovePerSec; };
	_Vector Get_Pivot() {return m_TransformDesc.vPivot.XMVector(); };

public:
	/*Get Set Matrix*/
	_float3 Get_MatrixState_Float3(TransformState eState) { return *((_float3*)(m_WorldMatrix.m[eState])); };
	_Vector Get_MatrixState(TransformState eState) { return ((_float4*)(m_WorldMatrix.m[eState]))->XMVector(); };
	_Vector Get_MatrixState_Normalized(TransformState eState) { return ((_float4*)(m_WorldMatrix.m[eState]))->Get_Nomalize(); };
	_Vector Get_MatrixScale(TransformState eState);
	_Matrix Get_MatrixScale_All();
	_Vector Get_Scale();

	void Set_MatrixState(TransformState eState, const _fVector& vVec);
	void Set_MatrixState(TransformState eState, const _float3& vVec);
	void Set_Matrix(const _float4x4& mat) { memcpy((m_WorldMatrix.m), mat.m, sizeof(_float) * 16); };
	void Set_Matrix(const _Matrix& mat);

	_float4x4 Get_WorldFloat4x4() { return m_WorldMatrix; };
	_Matrix Get_WorldMatrix() { return m_WorldMatrix.XMatrix(); };
	_Matrix Get_InverseWorldMatrix() { return m_WorldMatrix.InverseXMatrix(); };
	_Matrix Get_TransposeXMatrix() { return m_WorldMatrix.TransposeXMatrix(); };


public:
	//Move////////////////////////////////////////////////////////////////////////
	void Move_Forward(_double fDeltaTime);
	void Move_Backward(_double fDeltaTime);
	void Move_Right(_double fDeltaTime);
	void Move_Left(_double fDeltaTime);
	void Move_Up(_double fDeltaTime);
	void Move_Down(_double fDeltaTime);

	void MovetoDir(_fVector vDir, _double fDeltaTime);
	void MovetoDir_bySpeed(_fVector vDir,_float fSpeed ,_double fDeltaTime);
	void MovetoTarget(_fVector vTarget, _double fDeltaTime);

	_bool MovetoBezierCurve(_float fTimeAcc, _fVector vStartPos, _fVector vControlPos, _fVector vEndPos);
	_bool MovetoBezierCurve(_float Total_Time, _float fTimeAcc, _fVector vStartPos, _fVector vControlPos, _fVector vEndPos);

//Turn////////////////////////////////////////////////////////////////////////
	void LookAt(_fVector vTarget);
	void LookAtExceptY(_fVector vTargetPos, _double TimeDelta);
	void LookDir(_fVector vTargetLook);
	
	//월드 좌표 기준으로 현제 회전 상태에서 이어서 회전을 하고싶을 떄
	void Turn_CW(_fVector vAxis, _double fDeltaTime);
	void Turn_CCW(_fVector vAxis, _double fDeltaTime);
	void Turn_Direct(_fVector vAxis, _float fRadian);											// Made_By Jino
	void Turn_Dir(_fVector vTargetDir, _float fWeight);											// Made_By Jino
	void Turn_Revolution_CW(_fVector vCenterPos, _float fDistance, _double fDeltaTime);			// Made_By Jino
	void Turn_Revolution_CCW(_fVector vCenterPos, _float fDistance, _double fDeltaTime);		// Made_By Jino

	//로컬 좌표 기준으로 회전을 시켜주고 싶을 떄
	void Rotation_CW(_fVector vAxis, _float fRadian);
	void Rotation_CCW(_fVector vAxis, _float fRadian);
	void Rotation_Multi(_float3 fRadians);					// Made_By Jino

//Scale////////////////////////////////////////////////////////////////////////
	//로컬 기준으로 특정 사이즈로 세팅하고 싶을 떄
	void Scaled(TransformState eState, _float fScale);
	void Scaled_All(_float3 vScale);
	//월드 기준으로 이어서 사이즈를 조정하고 싶을 떄
	void Scaling(TransformState eState, _double fDeltaTime);
	void Scaling_All(_double fDeltaTime);

public:
	//쉐이더에 월드 행렬을 연결시키는 함수
	HRESULT Bind_OnShader(class CShader* pShader, const char* pValueName);
	HRESULT Bind_OnShader_BillBoard(class CShader* pShader, const char* pValueName, _fMatrix& ViewMatrix);
	//단순히 바인드 되는 좌표를 Pivot 값 만큼 이동시켜서 바인드 해주는 함수
	HRESULT Bind_OnShader_ApplyPivot(class CShader* pShader, const char* pValueName);
	HRESULT Bind_OnShader_BillBoard_ApplyPivot(class CShader* pShader, const char* pValueName, _fMatrix& ViewMatrix);
	
public:
	//시작 포인트, 마지막 포인트, 중앙 포인트, 시간을 담을 변수, 지속시간, 델타타임
	_Vector BezierCurve(_Vector vStartPoint, _Vector vLastPoint, _Vector vCenterPoint, _double& dTempTime, _double dDurationTime, _double dDeltaTime);

private:
	TRANSFORMDESC		m_TransformDesc;
	_float4x4			m_WorldMatrix;

public:
	static CTransform* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free()override;
};

END