#pragma once


#include "Base.h"

BEGIN(Engine)
class CModel;

class CMotionTrailBuffer final : public CBase
{
private:
	explicit CMotionTrailBuffer();
	virtual ~CMotionTrailBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype(CModel* pOriginalModel);
	virtual HRESULT Initialize_Clone(void* pArg);


public:
	_bool	IsTrailing() { return m_PassedTime < m_TargetTime; };
	_double	Get_PassedTime() { return m_PassedTime / m_TargetTime; }

	HRESULT Start_MotionTrail(_float4x4 WorldMatrix, _float4 vColor ,_double TargetTime);
	_int	Update_MotionBuffer(_double fDeltaTime);
	HRESULT Render(class CShader* pShader, _uint iPassIndex, const char* szBoneValueName = nullptr);
	



private:/*뼈들의 상속관계 순서대로 정렬해 보관*/

	_double									m_PassedTime = 0;
	_double									m_TargetTime = 0;

	_uint									m_iNumMaterials = 0;
	vector<_float4x4>						m_vecUpdatedMatrix;
	typedef vector<_float4x4>				UPDATEDMATRIXS;

	_uint									m_iNumMeshContainers = 0;
	vector<class CMeshContainer*>*			m_vecMeshContainerArr;


private:
	_float4x4								m_WorldMatrix;
	_float4x4								m_DefaultPivotMatrix;
	CModel*									m_pOriginalModel = nullptr;
	_float									m_fEmisive = 0;
	_float4									m_vColor = _float4(1, 0, 0, 1);


private:
	HRESULT Ready_Copy_OriginModel();


public:
	static CMotionTrailBuffer* Create(CModel* pOriginalModel);
	virtual void Free();
};

END