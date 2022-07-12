#pragma once

enum eInstanceEffectID
{
	InstanceEffect_Ball,
	InstanceEffect_Straight,
	InstanceEffect_Cone,
	InstanceEffect_Spread,
	InstanceEffect_Fountain,
	InstanceEffect_Suck,


	InstanceEffect_End
};
static const char* Tag_TextureInstanceEffect(eInstanceEffectID eTag)
{

	switch (eTag)
	{
	case InstanceEffect_Ball:
		return "InstanceEffect_Ball";
	case InstanceEffect_Straight:
		return "InstanceEffect_Straight";
	case InstanceEffect_Cone:
		return "InstanceEffect_Cone";
	case InstanceEffect_Spread:
		return "InstanceEffect_Spread";
	case InstanceEffect_Fountain:
		return "InstanceEffect_Fountain";
	case InstanceEffect_Suck:
		return "InstanceEffect_Suck";
	default:
		MSGBOX("Wrong Type InstanceType");
		return nullptr;
		break;
	}
}

#define  TAG_TEXINSTEFFECT Tag_TextureInstanceEffect



enum eInstancePassID
{
	InstancePass_OriginColor,
	InstancePass_BrightColor,
	InstancePass_MaskingNoising,
	InstancePass_AlphaDistortion,
	InstancePass_AllDistortion,

	InstancePass_End
};

enum eMeshInstancePassID
{
	MeshPass_OriginColor,
	MeshPass_BrightColor,

	MeshPass_MaskingNoising,
	MeshPass_MaskingNoising_Bright,
	MeshPass_MaskingNoising_Appear,
	MeshPass_MaskingNoising_Appear_Bright,

	MeshPass_AllDistortion,
	MeshPass_AllDistortion_Bright,
	MeshPass_Distortion_DiffuseMix,
	MeshPass_Distortion_DiffuseMix_Bright,
	MeshPass_Distortion_ColorMix,
	MeshPass_Distortion_ColorMix_Bright,

	MeshPass_End
};

enum eFollowingDirID
{
	FollowingDir_Right,
	FollowingDir_Up,
	FollowingDir_Look,

	FollowingDir_End
};
static const char* Tag_InstancePass(eFollowingDirID eTag)
{
	switch (eTag)
	{
	case FollowingDir_Right:
		return "Right";
	case FollowingDir_Up:
		return "Up";
	case FollowingDir_Look:
		return "Look";
		
	default:
		MSGBOX("Wrong Type Flw Dir");
		return nullptr;
		break;
	}

}

static const char* Tag_InstancePass(eInstancePassID eTag)
{
	switch (eTag)
	{
	case InstancePass_OriginColor:
		return "InstancePass_OriginColor";
	case InstancePass_BrightColor:
		return "InstancePass_BrightColor";

	case InstancePass_MaskingNoising:
		return "InstancePass_MaskingNoising";
	case InstancePass_AlphaDistortion:
		return "InstancePass_AlphaDistortion";
	case InstancePass_AllDistortion:
		return "InstancePass_AllDistortion";




	default:
		MSGBOX("Wrong Type Pass");
		return nullptr;
		break;
	}

}

#define  TAG_INSTPASS Tag_InstancePass


static const char* Tag_MeshPass(eMeshInstancePassID eTag)
{

	switch (eTag)
	{
	case MeshPass_OriginColor:
		return "MeshPass_OriginColor";
	case MeshPass_BrightColor:
		return "MeshPass_BrightColor";
	case MeshPass_MaskingNoising:
		return "MeshPass_MaskingNoising";
	case MeshPass_MaskingNoising_Bright:
		return "MeshPass_MaskingNoising_Bright";
	case MeshPass_MaskingNoising_Appear:
		return "MeshPass_MaskingNoising_Appear";
	case MeshPass_MaskingNoising_Appear_Bright:
		return "MeshPass_MaskingNoising_Appear_Bright";


	case MeshPass_AllDistortion:
		return "MeshPass_AllDistortion";
	case MeshPass_AllDistortion_Bright:
		return "MeshPass_AllDistortion_Bright";
	case MeshPass_Distortion_DiffuseMix:
		return "MeshPass_Distortion_DiffuseMix";
	case MeshPass_Distortion_DiffuseMix_Bright:
		return "MeshPass_Distortion_DiffuseMix_Bright";
	case MeshPass_Distortion_ColorMix:
		return "MeshPass_Distortion_ColorMix";
	case MeshPass_Distortion_ColorMix_Bright:
		return "MeshPass_Distortion_ColorMix_Bright";


	default:
		MSGBOX("Wrong Type Pass");
		return nullptr;
		break;
	}

}

#define  TAG_MESHINSTPASS Tag_MeshPass




enum ECameraMode
{
	CAM_MODE_FREE, CAM_MODE_NOMAL, CAM_MODE_TARGETING
};

enum UPGRADEID
{
	Upgrade_Vopal, Upgrade_PaperGrinder, Upgrade_Horse, Upgrade_Teapot, Upgrade_End
};

enum SCENEID
{
	SCENE_STATIC,
	SCENE_LOBY,
	SCENE_LOADING,
	SCENE_STAGESELECT,
	SCENE_STAGE1,
	SCENE_STAGE2,
	SCENE_STAGE3,
	SCENE_STAGE4,
	SCENE_STAGE5,
	SCENE_STAGE6,
	SCENE_STAGE7,
	SCENE_BOSS,
	SCENE_ENDING,

	SCENE_EDIT,
	SCENE_END
};
static const char* Tag_ScenenName(SCENEID eTag)
{
	switch (eTag)
	{
	case SCENE_STATIC: 
		return "SCENE_STATIC";
	case SCENE_LOBY:
		return "SCENE_LOBY";
	case SCENE_LOADING:
		return "SCENE_LOADING";
	case SCENE_STAGESELECT:
		return "SCENE_STAGESELECT";
	case SCENE_STAGE1:
		return "SCENE_STAGE1";
	case SCENE_STAGE2:
		return "SCENE_STAGE2";
	case SCENE_STAGE3:
		return "SCENE_STAGE3";
	case SCENE_STAGE4:
		return "SCENE_STAGE4";
	case SCENE_STAGE5:
		return "SCENE_STAGE5";
	case SCENE_STAGE6:
		return "SCENE_STAGE6";
	case SCENE_STAGE7:
		return "SCENE_STAGE7";
	case SCENE_BOSS:
		return "SCENE_BOSS";
	case SCENE_ENDING:
		return "SCENE_ENDING";
	case SCENE_EDIT:
		return "SCENE_EDIT";
	default:
		MSGBOX("Wrong Type SceneNumber");
		return nullptr;
		break;
	}

}

#define  TAG_SCENE Tag_ScenenName





enum OBJECTPROTOTYPEID
{
	Prototype_Camera_Main,
	Prototype_Player,
	Prototype_PlayerWeapon_Spear,
	Prototype_PlayerWeapon_Bow,
	Prototype_PlayerWeapon_Sword,
	Prototype_PlayerWeapon_Shield,
	Prototype_PlayerWeapon_Chakra,
	Prototype_PlayerWeapon_Arrow,
	Prototype_PlayerEffect_ShellingRange,
	Prototype_PlayerEffect_ShellingPoint,

	Prototype_Trigger_ChangeCameraView,

	Prototype_PlayerSkill_ShellingArrow,

	Prototype_SkyBox,

	Prototype_UIImage,

	Prototype_Rect,
	Prototype_Cube,
	Prototype_Terrain,


	Prototype_StaticMapObject,


	Prototype_TestObject,
	Prototype_EditorCursor,
	Prototype_EditorTerrain,
	Prototype_Camera_Editor,
	Prototype_TestObject_Himeko,
	Prototype_NaviPoint,


	Prototype_Object_Monster_Mahinasura_Minion,
	Prototype_Object_Monster_Mahinasura_Leader,
	Prototype_Object_Monster_Vayusura_Minion,
	Prototype_Object_Monster_Vayusura_Leader,
	Prototype_Obejct_Monster_Tezabsura_Minion,
	Prototype_Obejct_Monster_Tezabsura_Purple,
	Prototype_Obejct_Monster_Tezabsura_Bomber,
	Prototype_Obejct_Monster_Tezabsura_Landmine,
	Prototype_Object_Monster_Jalsura,
	Prototype_Object_Monster_Ninjasura,
	Prototype_Object_Monster_Ninjasura_Minion,
	Prototype_Object_Monster_Gadasura_Black,
	Prototype_Object_Monster_Gadasura_Rage,
	Prototype_Object_Monster_Gadasura_Rage_Hollogram,

	// PhyTest
	Prototype_Object_Static_PhysX,
	Prototype_Object_Dynamic_PhysX,
	Prototype_Object_Joint_PhysX,
	Prototype_Object_BaseCollision,


	//JJB
	Prototype_Object_Boss_Rangda,
	Prototype_Object_Boss_Rangda_Finger,
	Prototype_Object_Boss_Chiedtian,
	Prototype_Object_Boss_ChiedtianWeapon,
	Prototype_Object_Boss_Snake,
	Prototype_Object_Boss_Mahabalasura,
	Prototype_Object_Boss_MahabalasuraArms,
	Prototype_Object_Boss_MahabalasuraAttackArms,
	Prototype_Object_Boss_MahabalasuraWeapon,
	Prototype_Object_Boss_MahabalasuraCopy,
	Prototype_Object_Effect_MagicCircle,
	Prototype_Object_Map_Demon_Tree,
	Prototype_Object_Map_TreeMesh,

	//Monster_Bullet
	Prototype_Object_Monster_Bullet_Universal,
	Prototype_Object_Monster_Texture_Bullet,
	Prototype_Object_Monster_Weapon_Universal,

	Object_Prototype_End
};



static const _tchar* Tag_Object_Prototype(OBJECTPROTOTYPEID eTag)
{
	switch (eTag)
	{


	case Prototype_Camera_Main:
		return TEXT("Prototype_Camera_Main");
		break;
	case Prototype_SkyBox:
		return TEXT("Prototype_SkyBox");
		break;
	case Prototype_UIImage:
		return TEXT("Prototype_UI_Image");
		break;


	case Prototype_Rect:
		return TEXT("Prototype_Rect");
		break;
	case Prototype_Cube:
		return TEXT("Prototype_Cube");
		break;
	case Prototype_Terrain:
		return TEXT("Prototype_Terrain");
		break;
	case Prototype_Player:
		return TEXT("Prototype_Player");
		break;
	case Prototype_PlayerWeapon_Spear:
		return TEXT("Prototype_PlayerWeapon_Spear");
		break;
	case Prototype_PlayerWeapon_Bow:
		return TEXT("Prototype_PlayerWeapon_Bow");
		break;
	case Prototype_PlayerWeapon_Sword:
		return TEXT("Prototype_PlayerWeapon_Sword");
		break;
	case Prototype_PlayerWeapon_Shield:
		return TEXT("Prototype_PlayerWeapon_Shield");
		break;
	case Prototype_PlayerWeapon_Chakra:
		return TEXT("Prototype_PlayerWeapon_Chakra");
		break;
	case Prototype_PlayerWeapon_Arrow:
		return TEXT("Prototype_PlayerWeapon_Arrow");
		break;
	case Prototype_PlayerEffect_ShellingRange:
		return TEXT("Prototype_PlayerEffect_ShellingRange");
		break;
	case Prototype_PlayerEffect_ShellingPoint:
		return TEXT("Prototype_PlayerEffect_ShellingPoint");
		break;
		
	case Prototype_Trigger_ChangeCameraView:
		return TEXT("Prototype_Trigger_ChangeCameraView");
		break;

	case Prototype_PlayerSkill_ShellingArrow:
		return TEXT("Prototype_PlayerSkill_ShellingArrow");
		break;
	
	case Prototype_StaticMapObject:
		return TEXT("Prototype_StaticMapObject");
		break;
		//////////////////////////////////////////////////////////////////////////

	case Prototype_TestObject:
		return TEXT("Prototype_TestObject");
		break;
	case Prototype_EditorCursor:
		return TEXT("Prototype_EditorCursor");
		break;
	case Prototype_EditorTerrain:
		return TEXT("Prototype_EditorTerrain");
		break;
	case Prototype_Camera_Editor:
		return TEXT("Prototype_Camera_Editor");
		break;

	case Prototype_TestObject_Himeko: 
		return TEXT("Prototype_TestObject_Himeko");
		break;

	case Prototype_NaviPoint:
		return TEXT("Prototype_NaviPoint");
		break;

	case Prototype_Object_Monster_Mahinasura_Minion:
		return TEXT("Prototype_Object_Monster_Mahinasura_Minion");

	case Prototype_Object_Monster_Mahinasura_Leader:
		return TEXT("Prototype_Object_Monster_Mahinasura_Leader");

	case Prototype_Object_Monster_Vayusura_Minion:
		return TEXT("Prototype_Object_Monster_Vayusura_Minion");

	case Prototype_Object_Monster_Vayusura_Leader:
		return TEXT("Prototype_Object_Monster_Vayusura_Leader");

	case Prototype_Obejct_Monster_Tezabsura_Minion:
		return TEXT("Prototype_Object_Monster_Tezabsura_Minion");

	case Prototype_Obejct_Monster_Tezabsura_Purple:
		return TEXT("Prototype_Object_Monster_Tezabsura_Purple");

	case Prototype_Obejct_Monster_Tezabsura_Bomber:
		return TEXT("Prototype_Object_Monster_Tezabsura_Bomber");

	case Prototype_Obejct_Monster_Tezabsura_Landmine:
		return TEXT("Prototype_Object_Monster_Tezabsura_Landmine");

	case Prototype_Object_Monster_Jalsura:
		return TEXT("Prototype_Object_Monster_Jalsura");

	case Prototype_Object_Monster_Ninjasura:
		return TEXT("Prototype_Object_Monster_Ninjasura");

	case Prototype_Object_Monster_Ninjasura_Minion:
		return TEXT("Prototype_Object_Monster_Ninjasura_Minion");

	case Prototype_Object_Monster_Gadasura_Black:
		return TEXT("Prototype_Object_Monster_Gadasura_Black");

	case Prototype_Object_Monster_Gadasura_Rage:
		return TEXT("Prototype_Object_Monster_Gadasura_Rage");

	case Prototype_Object_Monster_Gadasura_Rage_Hollogram:
		return TEXT("Prototype_Object_Monster_Gadasura_Rage_Hollogram");

	case Prototype_Object_Boss_Rangda:
		return TEXT("Prototype_Object_Boss_Rangda");
		break;

	case Prototype_Object_Boss_Rangda_Finger:
		return TEXT("Prototype_Object_Boss_Rangda_Finger");
		break;

	case Prototype_Object_Boss_Chiedtian:
		return TEXT("Prototype_Object_Boss_Chiedtian");
		break;

	case Prototype_Object_Boss_ChiedtianWeapon:
		return TEXT("Prototype_Object_Boss_ChiedtianWeapon");
		break;

	case Prototype_Object_Boss_Snake:
		return TEXT("Prototype_Object_Boss_Snake");
		break;

	case Prototype_Object_Boss_Mahabalasura:
		return TEXT("Prototype_Object_Boss_Mahabalasura");
		break;

	case Prototype_Object_Boss_MahabalasuraArms:
		return TEXT("Prototype_Object_Boss_MahabalasuraArms");
		break;
		
	case Prototype_Object_Boss_MahabalasuraAttackArms:
		return TEXT("Prototype_Object_Boss_MahabalasuraAttackArms");
		break;

	case Prototype_Object_Boss_MahabalasuraWeapon:
		return TEXT("Prototype_Object_Boss_MahabalasuraWeapon");
		break;

	case Prototype_Object_Boss_MahabalasuraCopy:
		return TEXT("Prototype_Object_Boss_MahabalasuraCopy");
		break;

	case Prototype_Object_Effect_MagicCircle:
		return TEXT("Prototype_Object_Effect_MagicCircle");

	case Prototype_Object_Map_Demon_Tree:
		return TEXT("Prototype_Object_Map_Demon_Tree");

	case Prototype_Object_Map_TreeMesh:
		return TEXT("Prototype_Object_Map_TreeMesh");
		
	case Prototype_Object_Monster_Bullet_Universal:
		return TEXT("Prototype_Object_Monster_Bullet_Universal");

	case Prototype_Object_Monster_Texture_Bullet:
		return TEXT("Prototype_Object_Monster_Texture_Bullet");

	case Prototype_Object_Monster_Weapon_Universal:
		return TEXT("Prototype_Object_Monster_Weapon_Universal");

		///////////////////////////////////////////////////////////////
	case Prototype_Object_Static_PhysX:
		return TEXT("Prototype_Object_Static_PhysX");

	case Prototype_Object_Dynamic_PhysX:
		return TEXT("Prototype_Object_Dynamic_PhysX");

	case Prototype_Object_Joint_PhysX:
		return TEXT("Prototype_Object_Joint_PhysX");

	case Prototype_Object_BaseCollision:
		return TEXT("Prototype_Object_BaseCollision");

	default:
		MSGBOX("Wrong Type Object Prototype");
		return nullptr;
		break;
	}


}
#define  TAG_OP Tag_Object_Prototype

//
//enum MESHTYPEID
//{
//	Mesh_None,
//	Mesh_Player,
//	Mesh_Monster,
//
//
//
//	Mesh_AlgaeRock_Ledge,
//	Mesh_AlgaeRock_Pillar,
//	Mesh_AlgaeRock_Wall,
//
//	MeshID_End
//};
//
//
//
//static const _tchar* Tag_MeshID(MESHTYPEID eTag)
//{
//	switch (eTag)
//	{
//	case Mesh_None:
//		return TEXT("Mesh_None");
//		break;
//
//	case Mesh_Monster:
//		return TEXT("Monster");
//		break;
//	case Mesh_Player:
//		return TEXT("Player");
//		break;
//
//
//
//
//
//
//
//
//		//////////////////////////////////////////////////////////////////////////
//	case Mesh_AlgaeRock_Ledge:
//		return TEXT("Mesh_AlgaeRock_Ledge");
//		break;
//	case Mesh_TestMesh2:
//		return TEXT("TestMesh2");
//		break;
//	case Mesh_TestMesh3:
//		return TEXT("TestMesh3");
//		break;
//	case Mesh_TestMesh4:
//		return TEXT("TestMesh4");
//		break;
//
//	default:
//		MSGBOX("Wrong Type Mesh");
//		return nullptr;
//		break;
//	}
//
//
//}
//#define  TAG_MESH Tag_MeshID

enum LAYERID
{
	Layer_Camera_Main,
	Layer_Camera_Editor,
	Layer_ScreenEffect,
	Layer_SkyBox,
	Layer_Player,
	Layer_PlayerWeapon,
	Layer_PlayerEffect,
	Layer_PlayerSkill,
	Layer_Bullet,
	Layer_ClockBomb,

	Layer_Monster,
	Layer_Unique_Monster,
	Layer_Monster_Weapon,
	Layer_MonsterBullet,
	Layer_MonsterBullet1,
	Layer_MonsterBullet2,
	Layer_Particle,

	Layer_Terrain,
	Layer_WireTerrain,


	Layer_JumpPad,
	Layer_SteamPad,
	Layer_HiddenPad,
	Layer_TeethObj,
	Layer_RoseObj,
	Layer_Breakable,
	Layer_BreakablePiece,
	Layer_SlideObj,

	Layer_EscalatorPad,
	Layer_ButtonPad,
	Layer_ShrinkFlower,
	Layer_Vent,
	Layer_MazeDoor,
	Layer_ShapeMemoryPad,
	Layer_Presser,



	Layer_StaticMapObj,
	Layer_ColBase,
	Layer_ColStatic,
	Layer_ColTrigger,
	Layer_ColDynamic,


	Layer_UI_GamePlay,
	Layer_NPC,



	Layer_UI_IMG,
	Layer_UI_BTN,
	Layer_UI_SLIDE,
	Layer_UI_INPUT,

	Layer_TestObject,
	//JJB
	Layer_Boss,
	Layer_MapObject,
	Layer_TestEffect,

};

static const _tchar* Tag_Layer(LAYERID eTag)
{
	switch (eTag)
	{
		
	case Layer_NPC:
		return TEXT("Layer_NPC");
		break;

	case Layer_Camera_Main:
		return TEXT("Layer_Camera_Main");
		break;
	case Layer_Camera_Editor:
		return TEXT("Layer_Camera_Editor");
		break;
	case Layer_ScreenEffect:
		return TEXT("Layer_ScreenEffect");
		break;

	case Layer_ColBase:
		return TEXT("Layer_ColBase");
		break;

	case Layer_ColStatic:
		return TEXT("Layer_ColStatic");
		break;

	case Layer_ColTrigger:
		return TEXT("Layer_ColTrigger");
		break;

	case Layer_ColDynamic:
		return TEXT("Layer_ColDynamic");
		
	case Layer_Bullet:
		return TEXT("Layer_Bullet");
		break;
	case Layer_SkyBox:
		return TEXT("Layer_SkyBox");
		break;
	case Layer_Terrain:
		return TEXT("Layer_Terrain");
		break;
	case Layer_WireTerrain:
		return TEXT("Layer_WireTerrain");
		break;
	case Layer_Particle:
		return TEXT("Layer_Particle");
		break;
	case Layer_ClockBomb:
		return TEXT("Layer_ClockBomb");
		break;
		

	case Layer_Monster:
		return TEXT("Layer_Monster");
		break;
	case Layer_Unique_Monster:
		return TEXT("Layer_Unique_Monster");
		break;
	case Layer_Monster_Weapon:
		return TEXT("Layer_Monster_Weapon");
		break;
	case Layer_MonsterBullet:
		return TEXT("Layer_MonsterBullet");
		break;
	case Layer_MonsterBullet1:
		return TEXT("Layer_MonsterBullet1");
		break;
	case Layer_MonsterBullet2:
		return TEXT("Layer_MonsterBullet2");
		break;


	case Layer_Player:
		return TEXT("Layer_Player");
		break;
	case Layer_PlayerWeapon:
		return TEXT("Layer_PlayerWeapon");
		break;
	case Layer_PlayerEffect:
		return TEXT("Layer_PlayerEffect");
		break;
	case Layer_PlayerSkill:
		return TEXT("Layer_PlayerSkill");
		break;
	case Layer_JumpPad:
		return TEXT("Layer_JumpPad");
		break;
	case Layer_SteamPad:
		return TEXT("Layer_SteamPad");
		break;
	case Layer_HiddenPad:
		return TEXT("Layer_HiddenPad");
		break;
	case Layer_TeethObj:
		return TEXT("Layer_TeethObj");
		break;
	case Layer_RoseObj:
		return TEXT("Layer_RoseObj");
		break;
	case Layer_Breakable:
		return TEXT("Layer_Breakable");
		break;
	case Layer_BreakablePiece:
		return TEXT("Layer_BreakablePiece");
		break;
	case Layer_SlideObj:
		return TEXT("Layer_SlideObj");
		break;
	case Layer_EscalatorPad:
		return TEXT("Layer_EscalatorPad");
		break;
	case Layer_ButtonPad:
		return TEXT("Layer_ButtonPad");
		break;
	case Layer_ShrinkFlower:
		return TEXT("Layer_ShrinkFlower");
		break;
	case Layer_Vent:
		return TEXT("Layer_Vent");
		break;
	case Layer_MazeDoor:
		return TEXT("Layer_MazeDoor");
		break;
	case Layer_ShapeMemoryPad:
		return TEXT("Layer_ShapeMemoryPad");
		break;
	case Layer_Presser:
		return TEXT("Layer_Presser");
		break;




	case Layer_StaticMapObj:
		return TEXT("Layer_StaticMapObj");
		break;

	case Layer_UI_GamePlay:
		return TEXT("Layer_UI_GamePlay");
		break;

	case Layer_UI_IMG:
		return TEXT("Layer_UI_IMG");
		break;
	case Layer_UI_BTN:
		return TEXT("Layer_UI_BTN");
		break;
	case Layer_UI_SLIDE:
		return TEXT("Layer_UI_SLIDE");
		break;
	case Layer_UI_INPUT:
		return TEXT("Layer_UI_INPUT");
		break;

		//////////////////////////////////////////////////////////////////////////

	case Layer_TestObject:
		return TEXT("Layer_TestObject");
		break;

	case Layer_Boss:
		return TEXT("Layer_Boss");
		break;

	case Layer_MapObject:
		return TEXT("Layer_MapObject");
		break;

	case Layer_TestEffect:
		return TEXT("Layer_TestEffect");
		break;
		
	default:
		MSGBOX("Wrong Type Layer");
		return nullptr;
		break;
	}


}
#define  TAG_LAY Tag_Layer

enum COMPONENTPROTOTYPEID
{
	/////////////////기본 컴포넌트
	Prototype_Renderer,
	Prototype_Transform,
	Prototype_Collider,
	Prototype_Collider_PhysX_Static,
	Prototype_Collider_PhysX_Dynamic,
	Prototype_Collider_PhysX_Joint,

	Prototype_Navigation,
	Prototype_SwordTrail,
	Prototype_MotionTrail,
	Prototype_Shader_VT,
	Prototype_Shader_VCT,
	Prototype_Shader_VNT,
	Prototype_Shader_VNCT,
	Prototype_Shader_VAM,
	Prototype_Shader_VNAM,
	Prototype_Shader_VTXPOINTINST,
	Prototype_Shader_VTXANIMINST,
	Prototype_Shader_VTXNONANIMINST,


	Prototype_ModelInstance_1,
	Prototype_ModelInstance_2,
	Prototype_ModelInstance_4,
	Prototype_ModelInstance_8,
	Prototype_ModelInstance_16,
	Prototype_ModelInstance_32,
	Prototype_ModelInstance_64,
	Prototype_ModelInstance_128,
	Prototype_ModelInstance_256,
	Prototype_ModelInstance_512,


	//////////////버퍼////////////////////////////////////////////////////////////
	Prototype_VIBuffer_Rect,
	Prototype_VIBuffer_Cube,
	Prototype_VIBuffer_Terrain,
	Prototype_VIBuffer_Terrain_Edit,
	Prototype_VIBuffer_Point_Instance_1,
	Prototype_VIBuffer_Point_Instance_2,
	Prototype_VIBuffer_Point_Instance_4,
	Prototype_VIBuffer_Point_Instance_8,
	Prototype_VIBuffer_Point_Instance_16,
	Prototype_VIBuffer_Point_Instance_32,
	Prototype_VIBuffer_Point_Instance_64,
	Prototype_VIBuffer_Point_Instance_128,
	Prototype_VIBuffer_Point_Instance_256,
	Prototype_VIBuffer_Point_Instance_512,

	//////StaticMesh//////////None하고 알게락 레지 사이에다가만 넣을 것//////////////////////////////////////////////////////////
	Prototype_Mesh_None,
	Prototype_Mesh_DemonTree_Seg01,
	Prototype_Mesh_DemonTree_Seg02,
	Prototype_Mesh_DemonTree_Seg03,
	Prototype_Mesh_DemonTree_Seg04,
	Prototype_Mesh_DemonTree_Seg05,
	Prototype_Mesh_TEST_STATIC,
	Prototype_Mesh_PlayerSkill_SpearWave,

	Prototype_Mesh_AlgaeRock_Ledge,
	//////다이나믹Mesh///////////////플레이어하고 테스트 오브젝트 사이에다가만 넣을 것/////////////////////////////////////////////////////
	Prototype_Mesh_Player,
	Prototype_Mesh_PlayerWeapon_Spear,
	Prototype_Mesh_PlayerWeapon_Bow,
	Prototype_Mesh_PlayerWeapon_Sword,
	Prototype_Mesh_PlayerWeapon_Shield,
	Prototype_Mesh_PlayerWeapon_Chakra,
	Prototype_Mesh_PlayerWeapon_Arrow,
	Prototype_Mesh_PlayerEffect_ShellingParabola,

	Prototype_Mesh_Monster_Mahinasura_Minion,
	Prototype_Mesh_Monster_Mahinasura_Leader,
	Prototype_Mesh_Monster_Vayusura_Minion,
	Prototype_Mesh_Monster_Vayusura_Leader,
	Prototype_Mesh_Monster_Tezabsura_Minion,
	Prototype_Mesh_Monster_Tezabsura_Purple,
	Prototype_Mesh_Monster_Tezabsura_Bomber,
	Prototype_Mesh_Monster_Tezabsura_Landmine,
	Prototype_Mesh_Monster_Jalsura,
	Prototype_Mesh_Monster_Ninjasura_Minion,
	Prototype_Mesh_Monster_Ninjasura,
	Prototype_Mesh_Monster_Gadasura_Black,
	Prototype_Mesh_Monster_Gadasura_Rage,

	Prototype_Mesh_Monster_Gadasura_Rage_Hollogram,
	Prototype_Mesh_Monster_Weapon_Gadasura_Black,
	Prototype_Mesh_Monster_Weapon_Gadasura_Rage,




	Prototype_Mesh_Boss_Rangda,
	Prototype_Mesh_Boss_Rangda_Finger,
	Prototype_Mesh_Boss_Chieftian,
	Prototype_Mesh_Boss_ChieftianWeapon,
	Prototype_Mesh_Boss_ChieftianWeapon2,
	Prototype_Mesh_Boss_ChieftianWeapon3,
	Prototype_Mesh_Boss_ChieftianWeapon4,
	Prototype_Mesh_Boss_Snake,
	Prototype_Mesh_Boss_Mahabalasura,
	Prototype_Mesh_Boss_MahabalasuraArms,
	Prototype_Mesh_Boss_MahabalasuraAttackArms,
	Prototype_Mesh_Boss_MahabalasurWeapon,
	Prototype_Mesh_Boss_MahabalasurCopy,


	Prototype_Mesh_SkyBox,
	Prototype_Mesh_TestObject_Himeko,
	Prototype_Mesh_TestObject,
	/////////텍스쳐/////////////////////////////////////////////////////////////////

	Prototype_Texture_Player,
	Prototype_Texture_SkyBox,
	Prototype_Texture_EditScene,
	Prototype_Texture_Terrain,
	Prototype_Texture_Edit_Terrain,
	Prototype_Texture_DefaultUI,
	Prototype_Texture_InstanceParticleTex,
	Prototype_Texture_ScreenEffectUI,
	Prototype_Texture_PlayerEffect,
	Prototype_Texture_DissolveEffect,
	Prototype_Texture_GamePlayScene,
	Prototype_Texture_LoadingScene,
	Prototype_Texture_LobyScene,
	Prototype_Texture_SettingScene,
	Prototype_Texture_MskTex,
	Prototype_Texture_PauseUI,
	Prototype_Texture_TestEffect,
	Prototype_Texture_Util,
	Prototype_Texture_Monster_Bullet,
	Prototype_Texture_ShellingRange,
	Prototype_Texture_ShellingPoint,
};
static const _tchar* Tag_Component_Prototype(COMPONENTPROTOTYPEID eTag)
{
	switch (eTag)
	{

	case Prototype_Renderer:
		return TEXT("Prototype_Component_Renderer");
		break;
	case Prototype_Transform:
		return TEXT("Prototype_Component_Transform");
		break;
	case Prototype_Collider:
		return TEXT("Prototype_Component_Collider");
		break;
	case Prototype_Collider_PhysX_Static:
		return TEXT("Prototype_Collider_PhysX_Static");
		break;
	case Prototype_Collider_PhysX_Dynamic:
		return TEXT("Prototype_Collider_PhysX_Dynamic");
		break;
	case Prototype_Collider_PhysX_Joint:
		return TEXT("Prototype_Collider_PhysX_Joint");
		break;
	case Prototype_MotionTrail:
		return TEXT("Prototype_Component_MotionTrail");
		break;
		
	case Prototype_SwordTrail:
		return TEXT("Prototype_Component_SwordTrail");
		break;

	case Prototype_Navigation:
		return TEXT("Prototype_Component_Navigation");
		break;

		
	case Prototype_ModelInstance_1:
		return TEXT("Prototype_ModelInstance_1");
		break;
	case Prototype_ModelInstance_2:
		return TEXT("Prototype_ModelInstance_2");
		break;
	case Prototype_ModelInstance_4:
		return TEXT("Prototype_ModelInstance_4");
		break;
	case Prototype_ModelInstance_8:
		return TEXT("Prototype_ModelInstance_8");
		break;
	case Prototype_ModelInstance_16:
		return TEXT("Prototype_ModelInstance_16");
		break;
	case Prototype_ModelInstance_32:
		return TEXT("Prototype_ModelInstance_32");
		break;
	case Prototype_ModelInstance_64:
		return TEXT("Prototype_ModelInstance_64");
		break;
	case Prototype_ModelInstance_128:
		return TEXT("Prototype_ModelInstance_128");
		break;
	case Prototype_ModelInstance_256:
		return TEXT("Prototype_ModelInstance_256");
		break;
	case Prototype_ModelInstance_512:
		return TEXT("Prototype_ModelInstance_512");
		break;




	case Prototype_Shader_VT:
		return TEXT("Prototype_Component_Shader_VTXTEX");
		break;
	case Prototype_Shader_VCT:
		return TEXT("Prototype_Component_Shader_VTXCUBETEX");
		break;

	case Prototype_Shader_VNT:
		return TEXT("Prototype_Component_Shader_VTXNORMTEX");
		break;

	case Prototype_Shader_VNCT:
		return TEXT("Prototype_Component_Shader_VTXNORMCUBETEX");
		break;
	case Prototype_Shader_VAM:
		return TEXT("Prototype_Component_Shader_VTXANIMMODEL");
		break;
	case Prototype_Shader_VNAM:
		return TEXT("Prototype_Component_Shader_VTXNONANIMMODEL");
		break;
	case Prototype_Shader_VTXPOINTINST:
		return TEXT("Prototype_Component_Shader_VTXPOINTINST");
		break;

	case Prototype_Shader_VTXANIMINST:
		return TEXT("Prototype_Component_Shader_VTXANIMINST");
		break;
	case Prototype_Shader_VTXNONANIMINST:
		return TEXT("Prototype_Component_Shader_VTXNONANIMINST");
		break;


	

	case Prototype_VIBuffer_Rect:
		return TEXT("Prototype_Component_VIBuffer_Rect");
		break;

	case Prototype_VIBuffer_Cube:
		return TEXT("Prototype_Component_VIBuffer_Cube");
		break;

	case Prototype_VIBuffer_Point_Instance_1:
		return TEXT("Prototype_VIBuffer_Point_Instance_1");
		break;
	case Prototype_VIBuffer_Point_Instance_2:
		return TEXT("Prototype_VIBuffer_Point_Instance_2");
		break;
	case Prototype_VIBuffer_Point_Instance_4:
		return TEXT("Prototype_VIBuffer_Point_Instance_4");
		break;
	case Prototype_VIBuffer_Point_Instance_8:
		return TEXT("Prototype_VIBuffer_Point_Instance_8");
		break;
	case Prototype_VIBuffer_Point_Instance_16:
		return TEXT("Prototype_VIBuffer_Point_Instance_16");
		break;
	case Prototype_VIBuffer_Point_Instance_32:
		return TEXT("Prototype_VIBuffer_Point_Instance_32");
		break;
	case Prototype_VIBuffer_Point_Instance_64:
		return TEXT("Prototype_VIBuffer_Point_Instance_64");
		break;
	case Prototype_VIBuffer_Point_Instance_128:
		return TEXT("Prototype_VIBuffer_Point_Instance_128");
		break;
	case Prototype_VIBuffer_Point_Instance_256:
		return TEXT("Prototype_VIBuffer_Point_Instance_256");
		break;
	case Prototype_VIBuffer_Point_Instance_512:
		return TEXT("Prototype_VIBuffer_Point_Instance_512");
		break;

		//메쉬////////////////////////////////////////////////////////////////////////

	case Prototype_Mesh_None:
		return TEXT("Prototype_Mesh_None");
		break;
		
	case Prototype_Mesh_TEST_STATIC:
		return TEXT("Prototype_Mesh_TEST_STATIC");

	case Prototype_Mesh_PlayerSkill_SpearWave:
		return TEXT("SpearWave.fbx");
		break;

	case Prototype_Mesh_DemonTree_Seg01:
		return TEXT("DemonTree_Seg01.fbx");

	case Prototype_Mesh_DemonTree_Seg02:
		return TEXT("DemonTree_Seg02.fbx");

	case Prototype_Mesh_DemonTree_Seg03:
		return TEXT("DemonTree_Seg03.fbx");

	case Prototype_Mesh_DemonTree_Seg04:
		return TEXT("DemonTree_Seg04.fbx");

	case Prototype_Mesh_DemonTree_Seg05:
		return TEXT("DemonTree_Seg05.fbx");

	case Prototype_Mesh_AlgaeRock_Ledge:
		return TEXT("Prototype_Mesh_AlgaeRock_Ledge");
		break;
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		

	case Prototype_Mesh_Player:
		return TEXT("Player.fbx");
		break;

	case Prototype_Mesh_PlayerWeapon_Spear:
		return TEXT("PlayerSpear.fbx");
		break;
	case Prototype_Mesh_PlayerWeapon_Bow:
		return TEXT("PlayerBow.fbx");
		break;
	case Prototype_Mesh_PlayerWeapon_Sword:
		return TEXT("PlayerSword.fbx");
		break;
	case Prototype_Mesh_PlayerWeapon_Shield:
		return TEXT("PlayerShield.fbx");
		break;
	case Prototype_Mesh_PlayerWeapon_Chakra:
		return TEXT("PlayerChakra.fbx");
		break;
	case Prototype_Mesh_PlayerWeapon_Arrow:
		return TEXT("BowArrow.fbx");
		break;
	case Prototype_Mesh_PlayerEffect_ShellingParabola:
		return TEXT("ShellingParabola.fbx");
		break;

	case Prototype_Mesh_Monster_Mahinasura_Minion:
		return TEXT("Monster_Mahinasura_Minion.fbx");
		break;

	case Prototype_Mesh_Monster_Mahinasura_Leader:
		return TEXT("Monster_Mahinasura_Leader.fbx");
		break;

	case Prototype_Mesh_Monster_Vayusura_Minion:
		return TEXT("Monster_Vayusura_Minion.fbx");
		break;

	case Prototype_Mesh_Monster_Vayusura_Leader:
		return TEXT("Monster_Vayusura_Leader.fbx");
		break;

	case Prototype_Mesh_Monster_Tezabsura_Minion:
		return TEXT("Monster_Tezabsura_Minion.fbx");
		break;

	case Prototype_Mesh_Monster_Tezabsura_Purple:
		return TEXT("Monster_Tezabsura_Purple.fbx");
		break;

	case Prototype_Mesh_Monster_Tezabsura_Bomber:
		return TEXT("Monster_Tezabsura_Bomber.fbx");
		break;

	case Prototype_Mesh_Monster_Tezabsura_Landmine:
		return TEXT("Monster_Tezabsura_Landmine.fbx");
		break;

	case Prototype_Mesh_Monster_Jalsura:
		return TEXT("Monster_Jalsura.fbx");
		break;

	case Prototype_Mesh_Monster_Ninjasura_Minion:
		return TEXT("Monster_Ninjasura_Minion.fbx");

	case Prototype_Mesh_Monster_Ninjasura:
		return TEXT("Monster_Ninjasura.fbx");
		break;

	case Prototype_Mesh_Monster_Gadasura_Black:
		return TEXT("Monster_Gadasura_Black.fbx");
		break;
	case Prototype_Mesh_Monster_Gadasura_Rage:
		return TEXT("Monster_Gadasura_Rage.fbx");
		break;
	case Prototype_Mesh_Monster_Gadasura_Rage_Hollogram:
		return TEXT("Monster_Gadasura_Rage_Hollogram.fbx");
		break;

	case Prototype_Mesh_Monster_Weapon_Gadasura_Black:
		return TEXT("Gadesura_Black_Waepon.fbx");

	case Prototype_Mesh_Monster_Weapon_Gadasura_Rage:
		return TEXT("Gadesura_Rage_Waepon.fbx");
		
	case Prototype_Mesh_Boss_Rangda:
		return TEXT("Boss_Randa.fbx");
		break;

	case Prototype_Mesh_Boss_Rangda_Finger:
		return TEXT("Finger.fbx");
		break;

	case Prototype_Mesh_Boss_Chieftian:
		return TEXT("Boss_Chiedtian.fbx");
		break;

	case Prototype_Mesh_Boss_ChieftianWeapon:
		return TEXT("Chieft_Weapon.fbx");
		break;

	case Prototype_Mesh_Boss_ChieftianWeapon2:
		return TEXT("Katana_M_19.fbx");
		break;

	case Prototype_Mesh_Boss_ChieftianWeapon3:
		return TEXT("Katana_M_26.fbx");
		break;

	case Prototype_Mesh_Boss_ChieftianWeapon4:
		return TEXT("Katana_M_28.fbx");
		break;

	case Prototype_Mesh_Boss_Snake:
		return TEXT("Boss_Snake.fbx");
		break;

	case Prototype_Mesh_Boss_Mahabalasura:
		return TEXT("Boss_Mahabalasura.fbx");
		break;

	case Prototype_Mesh_Boss_MahabalasuraArms:
		return TEXT("Arms.fbx");
		break;

	case Prototype_Mesh_Boss_MahabalasuraAttackArms:
		return TEXT("AttackArms.fbx");
		break;

	case Prototype_Mesh_Boss_MahabalasurWeapon:
		return TEXT("NoAni_MahabalasuraWeapone.fbx");
		break;

	case Prototype_Mesh_Boss_MahabalasurCopy:
		return TEXT("Copy_Boss.fbx");
		break;

	case Prototype_Mesh_TestObject:
		return TEXT("Prototype_Mesh_TestObject");
		break;

	case Prototype_Mesh_TestObject_Himeko:
		return TEXT("Prototype_Mesh_TestObject_Himeko");
		break;

	case Prototype_Mesh_SkyBox:
		return TEXT("Prototype_Mesh_SkyBox");
		break;


		/////////////////텍스처////////////////////////////////////////////////////////


	case Prototype_Texture_Player:
		return TEXT("Prototype_Component_Texture_Player");
		break;
	case Prototype_Texture_PlayerEffect:
		return TEXT("Prototype_Texture_PlayerEffect");
		break;
	case Prototype_Texture_DissolveEffect:
		return TEXT("Prototype_Texture_DissolveEffect");
		break;
	case Prototype_Texture_ScreenEffectUI:
		return TEXT("Prototype_Texture_ScreenEffectUI");
		break;
		
	case Prototype_Texture_SkyBox:
		return TEXT("Prototype_Component_Texture_SkyBox");
		break;

	case Prototype_Texture_EditScene:
		return TEXT("Prototype_Component_Texture_EditScene");

		break;
	case 	Prototype_Texture_Terrain:
		return TEXT("Prototype_Component_Texture_Terrain");

		break;
	case 	Prototype_Texture_Edit_Terrain:
		return TEXT("Prototype_Texture_Edit_Terrain");

	case Prototype_Texture_Monster_Bullet:
		return TEXT("Prototype_Texture_Monster_Bullet");
		break;

	case Prototype_Texture_ShellingRange:
		return TEXT("Prototype_Texture_ShellingRange");
		break;
	case Prototype_Texture_ShellingPoint:
		return TEXT("Prototype_Texture_ShellingPoint");
		break;
		
	case 	Prototype_Texture_DefaultUI:
		return TEXT("Prototype_Texture_DefaultUI");
		break;

	case 	Prototype_Texture_Util:
		return TEXT("Prototype_Texture_Util");
		break;

		

	case 	Prototype_Texture_GamePlayScene:
		return TEXT("Prototype_Texture_GamePlayScene");
		break;

	case 	Prototype_Texture_LoadingScene:
		return TEXT("Prototype_Texture_LoadingScene");
		break;

	case 	Prototype_Texture_LobyScene:
		return TEXT("Prototype_Texture_LobyScene");
		break;

		case
			Prototype_Texture_SettingScene:
				return TEXT("Prototype_Texture_SettingScene");
				break;

		case Prototype_Texture_MskTex:
			return TEXT("Prototype_Texture_MskTex");
			break;
		case Prototype_Texture_InstanceParticleTex:
			return TEXT("Prototype_Texture_InstanceParticleTex");
			break;
		case Prototype_Texture_PauseUI:
			return TEXT("Prototype_Texture_PauseUI");
			break;

		case Prototype_Texture_TestEffect:
			return TEXT("Prototype_Texture_TestEffect");
			break;
			
				//메쉬////////////////////////////////////////////////////////////////////////







				break;
		case 	Prototype_VIBuffer_Terrain:
			return TEXT("Prototype_Component_VIBuffer_Terrain");
			break;
		case 	Prototype_VIBuffer_Terrain_Edit:
			return TEXT("Prototype_Component_VIBuffer_Terrain_Edit");
			break;



			//////////////////////////////////////////////////////////////////////////
		default:
			MSGBOX("Wrong Type Layer");
			return nullptr;
			break;
	}


}

#define  TAG_CP Tag_Component_Prototype

enum COMPONENTID
{
	Com_Renderer,
	Com_VIBuffer,
	Com_Transform,
	Com_Texture,
	Com_Inventory,
	Com_Shader,
	Com_ShaderSub,
	Com_Model,
	Com_SubModel,
	Com_Collider,
	Com_Collider_PhysX,
	Com_SwordTrail,
	Com_SubSwordTrail,
	Com_MotionTrail,
	Com_Navaigation,
	Com_ModelInstance,
	Com_SubTransform,
};
static const _tchar* Tag_Component(COMPONENTID eTag)
{
	switch (eTag)
	{
	case Com_Renderer:
		return TEXT("Com_Renderer");
		break;
	case Com_VIBuffer:
		return TEXT("Com_VIBuffer");
		break;
	case Com_Transform:
		return TEXT("Com_Transform");
		break;
	case Com_SubTransform:
		return TEXT("Com_SubTransform");
		break;
		
	case Com_Texture:
		return TEXT("Com_Texture");
		break;
	case Com_Inventory:
		return TEXT("Com_Inventory");
		break;
	case Com_Shader:
		return TEXT("Com_Shader");
		break;
	case Com_ShaderSub:
		return TEXT("Com_ShaderSub");
		break;
	case Com_Model:
		return TEXT("Com_Model");
		break;
	case Com_SubModel:
		return TEXT("Com_SubModel");
		break;

	case Com_Collider:
		return TEXT("Com_Collider");
		break;

	case Com_Collider_PhysX:
		return TEXT("Com_Collider_PhysX");
		break;
		

	case Com_SwordTrail:
		return TEXT("Com_SwordTrail");
		break;
	case Com_SubSwordTrail:
		return TEXT("Com_SubSwordTrail");
		break;
	case Com_MotionTrail:
		return TEXT("Com_MotionTrail");
		break;

	case Com_Navaigation:
		return TEXT("Com_Navigation");
		break;

	case Com_ModelInstance:
		return TEXT("Com_ModelInstance");
		break;

		

		//////////////////////////////////////////////////////////////////////////
	default:
		MSGBOX("Wrong Type Component");
		return nullptr;
		break;
	}


}
#define  TAG_COM Tag_Component



static const char* Tag_ModelTextureType(_uint eTag)
{
	switch (eTag)
	{
	case aiTextureType_NONE:		return "None";
		break;
	case aiTextureType_DIFFUSE:	return "g_DiffuseTexture";
		break;
	case aiTextureType_SPECULAR:return "g_SpecularTexture";
		break;
	case aiTextureType_AMBIENT:return "g_AmbientTexture";
		break;
	case aiTextureType_EMISSIVE:return "g_EmissiveTexture";
		break;
	case aiTextureType_HEIGHT:return "g_HeightTexture";
		break;
	case aiTextureType_NORMALS:return "g_NormalTexture";
		break;
	case aiTextureType_SHININESS:return "g_ShininessTexture";
		break;
	case aiTextureType_OPACITY:return "g_OpacityTexture";
		break;
	case aiTextureType_DISPLACEMENT:return "g_DisplaceTexture";
		break;
	case aiTextureType_LIGHTMAP:return "g_LightMapTexture";
		break;
	case aiTextureType_REFLECTION:return "g_ReflectTexture";
		break;
	case aiTextureType_BASE_COLOR:return "g_BaseColorTexture";
		break;
	case aiTextureType_NORMAL_CAMERA:return "g_NormalCamTexture";
		break;
	case aiTextureType_EMISSION_COLOR:return "g_EmissionColorTexture";
		break;
	case aiTextureType_METALNESS:return "g_MetalTexture";
		break;
	case aiTextureType_DIFFUSE_ROUGHNESS:return "g_DiffuseRoughTexture";
		break;
	case aiTextureType_AMBIENT_OCCLUSION:return "g_AmbientOcculusionTexture";
		break;
	case aiTextureType_UNKNOWN:return "";
		break;
	case _aiTextureType_Force32Bit:return "";
		break;
	default:
		OutputDebugStringW(L"Wrong Type Texture");
		__debugbreak();
		return nullptr;
		break;
	}
}

#define  MODLETEXTYPE Tag_ModelTextureType


enum BULLETMESHID
{
	Prototype_Mesh_Monster_Bullet_Vayusura_Leader,
	Prototype_Mesh_Monster_Bullet_Tezabsura_Minion,
	Prototype_Mesh_Monster_Bullet_Tezabsura_Purple,
	Prototype_Mesh_Monster_Bullet_Tezabsura_Bomber,
	Prototype_Mesh_Monster_Bullet_Tezabsura_Landmine,
	Prototype_Mesh_Monster_Bullet_Ninjasura_Knife,
	Prototype_Mesh_Monster_Bullet_Gadasura_Sinkhole,
	Prototype_Mesh_Monster_Bullet_Gadasura_Terrain,
	BULLET_END
};

static const _tchar* Tag_ModelMonsterBulletType(_uint eTag)
{
	switch (eTag)
	{
	case Prototype_Mesh_Monster_Bullet_Vayusura_Leader:
		return TEXT("Monster_Bullet_Vayusura_Leader.fbx");
	case Prototype_Mesh_Monster_Bullet_Tezabsura_Minion:
		return TEXT("Monster_Bullet_Tezabsura_Minion.fbx");
	case Prototype_Mesh_Monster_Bullet_Tezabsura_Purple:
		return TEXT("Monster_Bullet_Tezabsura_Purple.fbx");
	case Prototype_Mesh_Monster_Bullet_Tezabsura_Bomber:
		return TEXT("Monster_Bullet_Tezabsura_Bomber.fbx");
	case Prototype_Mesh_Monster_Bullet_Tezabsura_Landmine:
		return TEXT("Monster_Bullet_Tezabsura_Landmine.fbx");
	case Prototype_Mesh_Monster_Bullet_Ninjasura_Knife:
		return TEXT("Ninjasura_Knife.fbx");
	case Prototype_Mesh_Monster_Bullet_Gadasura_Sinkhole:
		return TEXT("sinkhole.fbx");
	case Prototype_Mesh_Monster_Bullet_Gadasura_Terrain:
		return TEXT("Gadasura_Terrain_Bullet.fbx");
	default:
		MSGBOX("Not Prototype_Component_Model_Bullet");
		return TEXT("ha! ha!");
	}
}

#define TAG_MONSTER_BULLET Tag_ModelMonsterBulletType