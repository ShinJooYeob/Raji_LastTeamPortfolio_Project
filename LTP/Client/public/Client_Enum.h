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
#ifdef _DEBUG
		MSGBOX("Wrong Type InstanceType");
#endif // _DEBUG
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
	InstancePass_MaskingNoising_Bright,
	InstancePass_MaskingNoising_Appear,
	InstancePass_MaskingNoising_Appear_Bright,

	InstancePass_AllDistortion,
	InstancePass_AllDistortion_Bright,
	InstancePass_Distortion_DiffuseMix,
	InstancePass_Distortion_DiffuseMix_Bright,
	InstancePass_Distortion_ColorMix,
	InstancePass_Distortion_ColorMix_Bright,

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
	case InstancePass_MaskingNoising_Bright:
		return "InstancePass_MaskingNoising_Bright";
	case InstancePass_MaskingNoising_Appear:
		return "InstancePass_MaskingNoising_Appear";
	case InstancePass_MaskingNoising_Appear_Bright:
		return "InstancePass_MaskingNoising_Appear_Bright";
	case InstancePass_AllDistortion:
		return "InstancePass_AllDistortion";
	case InstancePass_AllDistortion_Bright:
		return "InstancePass_AllDistortion_Bright";
	case InstancePass_Distortion_DiffuseMix:
		return "InstancePass_Distortion_DiffuseMix";
	case InstancePass_Distortion_DiffuseMix_Bright:
		return "InstancePass_Distortion_DiffuseMix_Bright";
	case InstancePass_Distortion_ColorMix:
		return "InstancePass_Distortion_ColorMix";
	case InstancePass_Distortion_ColorMix_Bright:
		return "InstancePass_Distortion_ColorMix_Bright";


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


	//////////////////////////////////////////////////////////////////////////
	Prototype_Trigger_ChangeCameraView,


	Prototype_Trigger_TestLedgeTrigger,
	//////////////////////////////////////////////////////////////////////////

	Prototype_PlayerSkill_ShellingArrow,
	Prototype_PlayerSkill_SpearWave,

	Prototype_SkyBox,

	Prototype_UIImage,

	Prototype_Rect,
	Prototype_Cube,
	Prototype_Terrain,


	Prototype_StaticMapObject,
	Prototype_InstanceStaticMapObject,


	Prototype_TestObject,
	Prototype_EditorCursor,
	Prototype_EditorTerrain,
	Prototype_Camera_Editor,
	Prototype_TestObject_Himeko,
	Prototype_NaviPoint,
	Prototype_NonInstanceMeshEffect,


	//////////////////////////////////////////////////////////////////////////
	Prototype_Object_Monster_Mahinasura_Minion,

	Prototype_Object_Monster_Mahinasura_Leader,
	Prototype_Object_Monster_Vayusura_Minion,
	Prototype_Object_Monster_Vayusura_Leader,
	Prototype_Object_Monster_Tezabsura_Minion,
	Prototype_Object_Monster_Tezabsura_Purple,
	Prototype_Object_Monster_Tezabsura_Bomber,
	Prototype_Object_Monster_Tezabsura_Landmine,
	Prototype_Object_Monster_Jalsura,
	Prototype_Object_Monster_Ninjasura,
	Prototype_Object_Monster_Ninjasura_Minion,
	Prototype_Object_Monster_Gadasura_Black,

	Prototype_Object_Monster_Gadasura_Rage,
	//////////////////////////////////////////////////////////////////////////

	Prototype_Object_Monster_Gadasura_Rage_Hollogram,
	Prototype_Object_Monster_Wasp,
	Prototype_Object_Monster_Wormgrub,
	Prototype_Object_Monster_Spider,
	Prototype_Object_Monster_Wolf,

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
	Prototype_Object_Map_MandalaPuzzle,
	Prototype_Object_Map_MandalaMesh,
	Prototype_Object_Map_FemaleStatue,
	Prototype_Object_UI_HpUI,

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
	case Prototype_Trigger_TestLedgeTrigger:
		return TEXT("Prototype_Trigger_TestLedgeTrigger");
		break;
		
	case Prototype_PlayerSkill_ShellingArrow:
		return TEXT("Prototype_PlayerSkill_ShellingArrow");
		break;
	case Prototype_PlayerSkill_SpearWave:
		return TEXT("Prototype_PlayerSkill_SpearWave");
		break;
		
	case Prototype_InstanceStaticMapObject:
		return TEXT("Prototype_InstanceStaticMapObject");
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
		
	case Prototype_NonInstanceMeshEffect:
		return TEXT("Prototype_NonInstanceMeshEffect");
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

	case Prototype_Object_Monster_Tezabsura_Minion:
		return TEXT("Prototype_Object_Monster_Tezabsura_Minion");

	case Prototype_Object_Monster_Tezabsura_Purple:
		return TEXT("Prototype_Object_Monster_Tezabsura_Purple");

	case Prototype_Object_Monster_Tezabsura_Bomber:
		return TEXT("Prototype_Object_Monster_Tezabsura_Bomber");

	case Prototype_Object_Monster_Tezabsura_Landmine:
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

	case Prototype_Object_Monster_Wasp:
		return TEXT("Prototype_Object_Monster_Wasp");

	case Prototype_Object_Monster_Wormgrub:
		return TEXT("Prototype_Object_Monster_Wormgrub");

	case Prototype_Object_Monster_Spider:
		return TEXT("Prototype_Object_Monster_Spider");

	case Prototype_Object_Monster_Wolf:
		return TEXT("Prototype_Object_Monster_Wolf");

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

	case Prototype_Object_Map_MandalaPuzzle:
		return TEXT("Prototype_Object_Map_MandalaPuzzle");

	case Prototype_Object_Map_MandalaMesh:
		return TEXT("Prototype_Object_Map_MandalaMesh");

	case Prototype_Object_Map_FemaleStatue:
		return TEXT("Prototype_Object_Map_FemaleStatue");

	case Prototype_Object_UI_HpUI:
		return TEXT("Prototype_Object_UI_HpUI");
			
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
	//Prototype_Collider_PhysX_Base,
	Prototype_Collider_PhysX_Static,
	Prototype_Collider_PhysX_Dynamic,
	Prototype_Collider_PhysX_Joint,

	Prototype_Dissolve,
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
	Prototype_Mesh_ConeMesh,
	Prototype_Mesh_JY_Tornado,
	Prototype_Mesh_Spear_UltiEffect,
	Prototype_Mesh_Spear_NormalEffect,
	
	Prototype_Mesh_CIRCLE,
	Prototype_Mesh_CIRCLE_DIST4,
	Prototype_Mesh_CIRCLE_DIST5,
	Prototype_Mesh_IMPECTFX_02,
	Prototype_Mesh_RING,
	Prototype_Mesh_LOVE,
	Prototype_Mesh_WING,
	Prototype_Mesh_BOW1,
	Prototype_Mesh_BOW2,
	

	Prototype_Mesh_ENV_BLD_Palace_02,
	Prototype_Mesh_ENV_BLD_Palace_05,
	Prototype_Mesh_ENV_BLD_Palace_08,
	Prototype_Mesh_ENV_BLD_Palace_11,
	Prototype_Mesh_ENV_BLD_Palace_16,
	Prototype_Mesh_ENV_BLD_Window_Pane_02,
	Prototype_Mesh_ENV_F_DecorativePillar_02,
	Prototype_Mesh_F_Vinestest,
	Prototype_Mesh_PillarPlatforming,
	Prototype_Mesh_PillarPlatforming_DM,
	Prototype_Mesh_SM_Arrow,
	Prototype_Mesh_SM_Arrow1,
	Prototype_Mesh_SM_BG_Mountain1,
	Prototype_Mesh_SM_BG_Mountain2,
	Prototype_Mesh_SM_BG_Mountain3,
	Prototype_Mesh_SM_BLD_Arch_Base,
	Prototype_Mesh_SM_BLD_Arch_Stairs,
	Prototype_Mesh_SM_BLD_ArchStairs_03,
	Prototype_Mesh_SM_BLD_BridgeMiddle01,
	Prototype_Mesh_SM_BLD_BridgeMiddle03,
	Prototype_Mesh_SM_BLD_BrokenArch_01,
	Prototype_Mesh_SM_BLD_BrokenArch_03,
	Prototype_Mesh_SM_BLD_BrokenArch_04,
	Prototype_Mesh_SM_BLD_CC_TemplePainting_01,
	Prototype_Mesh_SM_BLD_CC_TemplePainting_03,
	Prototype_Mesh_SM_BLD_CC_TemplePainting_04,
	Prototype_Mesh_SM_BLD_CombatArena01,
	Prototype_Mesh_SM_BLD_DB_WoodenPlank,
	Prototype_Mesh_SM_BLD_DBPillar01,
	Prototype_Mesh_SM_BLD_DBPillar02,
	Prototype_Mesh_SM_BLD_DBPillar04,
	Prototype_Mesh_SM_BLD_DBSculpture,
	Prototype_Mesh_SM_BLD_DiyaBridge,
	Prototype_Mesh_SM_BLD_Drawbridge,
	Prototype_Mesh_SM_BLD_DS_BrokenArch_01,
	Prototype_Mesh_SM_BLD_DS_BrokenArch_02,
	Prototype_Mesh_SM_BLD_DS_BrokenArch_03,
	Prototype_Mesh_SM_BLD_DS_SpiralPlatform_01,
	Prototype_Mesh_SM_BLD_F_Column_B_02,
	Prototype_Mesh_SM_BLD_F_Palace_Arch_01,
	Prototype_Mesh_SM_BLD_F_Palace_Block_01,
	Prototype_Mesh_SM_BLD_F_Palace_Block_02,
	Prototype_Mesh_SM_BLD_F_Palace_Column_01,
	Prototype_Mesh_SM_BLD_F_Palace_Column_02,
	Prototype_Mesh_SM_BLD_F_Palace_Column_03,
	Prototype_Mesh_SM_BLD_F_Palace_Column_04,
	Prototype_Mesh_SM_BLD_F_Palace_Door_02,
	Prototype_Mesh_SM_BLD_F_Palace_Door_Arch_01,
	Prototype_Mesh_SM_BLD_F_Palace_STAIRS_01,
	Prototype_Mesh_SM_BLD_F_Palace_TILE_01,
	Prototype_Mesh_SM_BLD_F_Palace_WALL_01,
	Prototype_Mesh_SM_BLD_F_Palace_Window_LG_01,
	Prototype_Mesh_SM_BLD_F_Palace_Window_LG_02,
	Prototype_Mesh_SM_BLD_F_Palace_Window_SM_01,
	Prototype_Mesh_SM_BLD_Lion_Statue,
	Prototype_Mesh_SM_BLD_StraightWall02,
	Prototype_Mesh_SM_BLD_StraightWall06,
	Prototype_Mesh_SM_BLD_SV_PalaceBig_02,
	Prototype_Mesh_SM_BLD_Temple_Arch,
	Prototype_Mesh_SM_BLD_TempleEnt,
	Prototype_Mesh_SM_BLD_Tower02,
	Prototype_Mesh_SM_BLD_TrishulGrant03,
	Prototype_Mesh_SM_BouVines_01,
	Prototype_Mesh_SM_boxbush_01,
	Prototype_Mesh_SM_Branch_Face_01,
	Prototype_Mesh_SM_Branch_Face_02,
	Prototype_Mesh_SM_Branch_LG_01,
	Prototype_Mesh_SM_Branch_LG_02,
	Prototype_Mesh_SM_Branch_LG_03,
	Prototype_Mesh_SM_Branch_LG_04,
	Prototype_Mesh_SM_Branch_LG_05,
	Prototype_Mesh_SM_Branch_Piece_02,
	Prototype_Mesh_SM_Branch_SM_01,
	Prototype_Mesh_SM_bri_decal_08,
	Prototype_Mesh_SM_Bridge_Chainhook,
	Prototype_Mesh_SM_Broken_Branch_01,
	Prototype_Mesh_SM_Broken_Branch_02,
	Prototype_Mesh_SM_Broken_Branch_03,
	Prototype_Mesh_SM_Broken_Branch_04,
	Prototype_Mesh_SM_CC_archi_merg_01,
	Prototype_Mesh_SM_CC_archi_merg_02,
	Prototype_Mesh_SM_CC_straightboxwall_01,
	Prototype_Mesh_SM_CC_straightboxwall_02,
	Prototype_Mesh_SM_CH_Gadasura_Black_Mask_01,
	Prototype_Mesh_SM_CH_Gadasura_Rage_Mask_01,
	Prototype_Mesh_SM_CH_Mahinasura_Leader_Mask_01,
	Prototype_Mesh_SM_CH_Mahinasura_Leader_Stinger_01,
	Prototype_Mesh_SM_CH_Mahinasura_Leader_Tail_01,
	Prototype_Mesh_SM_CH_Mahinasura_Minion_Mask_01,
	Prototype_Mesh_SM_CH_Tezabsura_Kamikaze_Mask_01,
	Prototype_Mesh_SM_CH_Tezabsura_Landmine_Mask_01,
	Prototype_Mesh_SM_CH_Vayusura_Mask_01,
	Prototype_Mesh_SM_CH_Vayusura_Mask_02,
	Prototype_Mesh_SM_circular_plane,
	Prototype_Mesh_SM_Crack_Decal_Ground_01,
	Prototype_Mesh_SM_Crack_Decal_Ground_03,
	Prototype_Mesh_SM_Crack_Decal_Ground_05,
	Prototype_Mesh_SM_Crack_Decal_Ground_06,
	Prototype_Mesh_SM_D_BLD_PalaceArch_01,
	Prototype_Mesh_SM_D_BLD_PalaceCOL_01,
	Prototype_Mesh_SM_D_BLD_PalaceCOL_02,
	Prototype_Mesh_SM_D_BLD_PalaceENT_01,
	Prototype_Mesh_SM_D_BLD_PalaceRFCORNER_01,
	Prototype_Mesh_SM_D_BLD_PalaceRFLEDGE_01,
	Prototype_Mesh_SM_D_BLD_PalaceRFSEG_01,
	Prototype_Mesh_SM_D_BLD_PalaceWIN_01,
	Prototype_Mesh_SM_Damage_Decal_01,
	Prototype_Mesh_SM_Damage_Decal_02,
	Prototype_Mesh_SM_Damage_Decal_03,
	Prototype_Mesh_SM_Debris_01,
	Prototype_Mesh_SM_Debris_02,
	Prototype_Mesh_SM_Debris_03,
	Prototype_Mesh_SM_Debris_04,
	Prototype_Mesh_SM_Demon_Tree_Seg_01,
	Prototype_Mesh_SM_Demon_Tree_Seg_02,
	Prototype_Mesh_SM_Demon_Tree_Seg_03,
	Prototype_Mesh_SM_Demon_Tree_Seg_04,
	Prototype_Mesh_SM_Demon_Tree_Seg_05,
	Prototype_Mesh_SM_DemonT_Creeper_01,
	Prototype_Mesh_SM_DemonT_Creeper_02,
	Prototype_Mesh_SM_DemonT_Creeper_03,
	Prototype_Mesh_SM_DiyaSmall_01,
	Prototype_Mesh_SM_Dry_Green_Bush_01,
	Prototype_Mesh_SM_Dry_Green_Bush_02,
	Prototype_Mesh_SM_Dry_Green_Bush_03,
	Prototype_Mesh_SM_Dry_Stack_01,
	Prototype_Mesh_SM_Dry_Stack_02,
	Prototype_Mesh_SM_Dry_Stack_03,
	Prototype_Mesh_SM_DryLeaves_Clutter_01,
	Prototype_Mesh_SM_DryLeaves_Clutter_02,
	Prototype_Mesh_SM_DryLeaves_Clutter_03,
	Prototype_Mesh_SM_DryLeaves_Clutter_04,
	Prototype_Mesh_SM_DryLeaves_Clutter_05,
	Prototype_Mesh_SM_Dune_01,
	Prototype_Mesh_SM_Dune_02,
	Prototype_Mesh_SM_Dune_03,
	Prototype_Mesh_SM_ENV_BLD_TopFloor_Wall_01,
	Prototype_Mesh_SM_ENV_BLD_TopFloor_Wall_02,
	Prototype_Mesh_SM_ENV_CC_Arch_B,
	Prototype_Mesh_SM_ENV_CC_Arch_Base_01,
	Prototype_Mesh_SM_ENV_CC_Arch_C,
	Prototype_Mesh_SM_ENV_CC_Arch_D,
	Prototype_Mesh_SM_ENV_CC_Boat_01,
	Prototype_Mesh_SM_ENV_CC_Border_01,
	Prototype_Mesh_SM_ENV_CC_Border_02,
	Prototype_Mesh_SM_ENV_CC_Border_03,
	Prototype_Mesh_SM_ENV_CC_Border_05,
	Prototype_Mesh_SM_ENV_CC_Bridge_Arch,
	Prototype_Mesh_SM_ENV_CC_Bridge_Pillar_03,
	Prototype_Mesh_SM_ENV_CC_CogLarge_01,
	Prototype_Mesh_SM_ENV_CC_CogLarge_02,
	Prototype_Mesh_SM_ENV_CC_CogLarge_03,
	Prototype_Mesh_SM_ENV_CC_CogLarge_04,
	Prototype_Mesh_SM_ENV_CC_CogMedium_01,
	Prototype_Mesh_SM_ENV_CC_CogMedium_02,
	Prototype_Mesh_SM_ENV_CC_CogMedium_03,
	Prototype_Mesh_SM_ENV_CC_CogMedium_04,
	Prototype_Mesh_SM_ENV_CC_CogMedium_05,
	Prototype_Mesh_SM_ENV_CC_CogSmall_01,
	Prototype_Mesh_SM_ENV_CC_CogTiny_01,
	Prototype_Mesh_SM_ENV_CC_Diya_02,
	Prototype_Mesh_SM_ENV_CC_DockPlatform_01,
	Prototype_Mesh_SM_ENV_CC_Dome_A,
	Prototype_Mesh_SM_ENV_CC_Dome_B,
	Prototype_Mesh_SM_ENV_CC_Dome_C,
	Prototype_Mesh_SM_ENV_CC_Dome_D,
	Prototype_Mesh_SM_ENV_CC_Dome_E,
	Prototype_Mesh_SM_ENV_CC_DoorWay_01,
	Prototype_Mesh_SM_ENV_CC_Elevator_Horizontal_01,
	Prototype_Mesh_SM_ENV_CC_Elevator_Platform_01,
	Prototype_Mesh_SM_ENV_CC_Elevator_Platform_02,
	Prototype_Mesh_SM_ENV_CC_Elevator_Vertical_01,
	Prototype_Mesh_SM_ENV_CC_ElevatorHorizontal_CogTR_01,
	Prototype_Mesh_SM_ENV_CC_ElevatorHousing_01,
	Prototype_Mesh_SM_ENV_CC_ElevatorLever_01,
	Prototype_Mesh_SM_ENV_CC_ElevatorLeverBase_01,
	Prototype_Mesh_SM_ENV_CC_ElevatorRailing_Horizontal_01,
	Prototype_Mesh_SM_ENV_CC_ElevatorRailing_Vertical_01,
	Prototype_Mesh_SM_ENV_CC_ElevatorVertical_Cog_Bottom_01,
	Prototype_Mesh_SM_ENV_CC_ElevatorVertical_Cog_Top_01,
	Prototype_Mesh_SM_ENV_CC_Garuda_Gate,
	Prototype_Mesh_SM_ENV_CC_Garuda_Tower,
	Prototype_Mesh_SM_ENV_CC_Garuda_Tower_Balcony,
	Prototype_Mesh_SM_ENV_CC_MandalaCircle_01,
	Prototype_Mesh_SM_ENV_CC_MandalaCircle_02,
	Prototype_Mesh_SM_ENV_CC_MandalaCircle_03,
	Prototype_Mesh_SM_ENV_CC_MandalaCircle_04,
	Prototype_Mesh_SM_ENV_CC_PaddyField_01,
	Prototype_Mesh_SM_ENV_CC_PaddyField_02,
	Prototype_Mesh_SM_ENV_CC_PaddyField_03,
	Prototype_Mesh_SM_ENV_CC_PaddyField_04,
	Prototype_Mesh_SM_ENV_CC_PaddyField_05,
	Prototype_Mesh_SM_ENV_CC_PaddyField_06,
	Prototype_Mesh_SM_ENV_CC_PaddyField_07,
	Prototype_Mesh_SM_ENV_CC_Panel_01,
	Prototype_Mesh_SM_ENV_CC_Panel_03,
	Prototype_Mesh_SM_ENV_CC_Panel_04,
	Prototype_Mesh_SM_ENV_CC_Panel_05,
	Prototype_Mesh_SM_ENV_CC_Pillar_03,
	Prototype_Mesh_SM_ENV_CC_Pillar_05,
	Prototype_Mesh_SM_ENV_CC_Pillar_043,
	Prototype_Mesh_SM_ENV_CC_Platform_01,
	Prototype_Mesh_SM_ENV_CC_Platform_02,
	Prototype_Mesh_SM_ENV_CC_Platform_03,
	Prototype_Mesh_SM_ENV_CC_Platform_04,
	Prototype_Mesh_SM_ENV_CC_Platform_06,
	Prototype_Mesh_SM_ENV_CC_Platform_054_SM_ENV_CC_Platform_05,
	Prototype_Mesh_SM_ENV_CC_Platform_Circular_01,
	Prototype_Mesh_SM_ENV_CC_Platform_Circular_02,
	Prototype_Mesh_SM_ENV_CC_Platform_Circular_low,
	Prototype_Mesh_SM_ENV_CC_Platform_Circular_low01,
	Prototype_Mesh_SM_ENV_CC_Puzzle_Water,
	Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_01,
	Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_02,
	Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_03,
	Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_04,
	Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_05,
	Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_08,
	Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_10,
	Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_11,
	Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_12,
	Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_15,
	Prototype_Mesh_SM_ENV_CC_Railing_01,
	Prototype_Mesh_SM_ENV_CC_Railing_02,
	Prototype_Mesh_SM_ENV_CC_Railing_03,
	Prototype_Mesh_SM_ENV_CC_RangdaArena_01,
	Prototype_Mesh_SM_ENV_CC_Stairs_01,
	Prototype_Mesh_SM_ENV_CC_Stairs_02,
	Prototype_Mesh_SM_ENV_CC_Stairs_Railing,
	Prototype_Mesh_SM_ENV_CC_Statue,
	Prototype_Mesh_SM_ENV_CC_StoryPanel_02,
	Prototype_Mesh_SM_ENV_CC_StraightWall_01,
	Prototype_Mesh_SM_ENV_CC_StraightWall_02,
	Prototype_Mesh_SM_ENV_CC_StraightWall_02_SM_ENV_CC_StraightWall_02,
	Prototype_Mesh_SM_ENV_CC_StraightWall_03_SM_ENV_CC_StraightWall_03,
	Prototype_Mesh_SM_ENV_CC_StraightWall_04,
	Prototype_Mesh_SM_ENV_CC_StraightWall_Withwindow_SM_ENV_CC_StraightWall_WithWindow,
	Prototype_Mesh_SM_ENV_CC_StraighWall_Withwindow1,
	Prototype_Mesh_SM_ENV_CC_TempleBase_01,
	Prototype_Mesh_SM_ENV_CC_TempleTop_01,
	Prototype_Mesh_SM_ENV_CC_TempleWalls_01,
	Prototype_Mesh_SM_ENV_CC_Vishnu_Statue,
	Prototype_Mesh_SM_ENV_CC_VishnuTemple_01,
	Prototype_Mesh_SM_ENV_CC_VishnuTemple_Base_01,
	Prototype_Mesh_SM_ENV_CC_VishnuTemple_DoorAssembly_Left,
	Prototype_Mesh_SM_ENV_CC_VishnuTemple_DoorAssembly_Right,
	Prototype_Mesh_SM_ENV_CC_Water_Platform_01,
	Prototype_Mesh_SM_ENV_CC_Water_Platform_02a,
	Prototype_Mesh_SM_ENV_CC_Water_Platform_03,
	Prototype_Mesh_SM_ENV_CC_Water_Platform_04,
	Prototype_Mesh_SM_ENV_CC_Water_Platform_06,
	Prototype_Mesh_SM_ENV_CC_Water_Platform_07,
	Prototype_Mesh_SM_ENV_CC_Water_Platform_10,
	Prototype_Mesh_SM_ENV_CC_WaterFountain_01,
	Prototype_Mesh_SM_ENV_CC_WaterFountain_02,
	Prototype_Mesh_SM_ENV_CC_WaterPuzzle_Base,
	Prototype_Mesh_SM_ENV_CC_WaterPuzzle_Boundary_01,
	Prototype_Mesh_SM_ENV_CC_WaterPuzzle_Boundary_02,
	Prototype_Mesh_SM_ENV_CC_WaterWheel_01,
	Prototype_Mesh_SM_ENV_CC_WaterWheel_02,
	Prototype_Mesh_SM_ENV_CC_Window_A,
	Prototype_Mesh_SM_ENV_CC_Window_A1,
	Prototype_Mesh_SM_ENV_CC_Window_Ba,
	Prototype_Mesh_SM_ENV_CC_Window_D,
	Prototype_Mesh_SM_ENV_CC_Window1,
	Prototype_Mesh_SM_ENV_CC_Wooden_Planks_03,
	Prototype_Mesh_SM_ENV_CC_Wooden_Planks_08,
	Prototype_Mesh_SM_ENV_D_BasePlatform_01,
	Prototype_Mesh_SM_ENV_D_BossArenaArch_01,
	Prototype_Mesh_SM_ENV_D_BossArenaArch_02,
	Prototype_Mesh_SM_ENV_D_CenterPlatform_01,
	Prototype_Mesh_SM_ENV_D_ExitDoor_01,
	Prototype_Mesh_SM_ENV_D_ExitPlatformTop_01,
	Prototype_Mesh_SM_ENV_D_FemaleStatue_01,
	Prototype_Mesh_SM_ENV_D_FemaleStatue_02,
	Prototype_Mesh_SM_ENV_D_FrontPlatformBottom_01,
	Prototype_Mesh_SM_ENV_D_FrontPlatformTop_01,
	Prototype_Mesh_SM_ENV_D_GodHand_01,
	Prototype_Mesh_SM_ENV_D_GroundArch_01,
	Prototype_Mesh_SM_ENV_D_GroundColumn_01,
	Prototype_Mesh_SM_ENV_D_GroundFloorPlatform_01,
	Prototype_Mesh_SM_ENV_D_GroundFloorPlatform_02,
	Prototype_Mesh_SM_ENV_D_MandalaCircle_01,
	Prototype_Mesh_SM_ENV_D_MandalaCircle_02,
	Prototype_Mesh_SM_ENV_D_MandalaCircle_03,
	Prototype_Mesh_SM_ENV_D_MandalaCircle_04,
	Prototype_Mesh_SM_ENV_D_MandalaCircle_05,
	Prototype_Mesh_SM_ENV_D_OuterWall_01,
	Prototype_Mesh_SM_ENV_D_OuterWall_02,
	Prototype_Mesh_SM_ENV_D_OuterWall_03,
	Prototype_Mesh_SM_ENV_D_OuterWall_04,
	Prototype_Mesh_SM_ENV_D_OuterWall_05,
	Prototype_Mesh_SM_ENV_D_OuterWall_06,
	Prototype_Mesh_SM_ENV_D_OuterWall_07,
	Prototype_Mesh_SM_ENV_D_OuterWall_08,
	Prototype_Mesh_SM_ENV_D_Roof_01,
	Prototype_Mesh_SM_ENV_D_SidePlatformTop_01,
	Prototype_Mesh_SM_ENV_D_SoldierStatue_01,
	Prototype_Mesh_SM_ENV_D_SoldierStatue_02,
	Prototype_Mesh_SM_ENV_D_StepwellStairs_01,
	Prototype_Mesh_SM_ENV_D_StepwellStairs_Half_01,
	Prototype_Mesh_SM_ENV_D_StepwellWater_01,
	Prototype_Mesh_SM_ENV_D_TombBase_01,
	Prototype_Mesh_SM_ENV_D_Window_01,
	Prototype_Mesh_SM_ENV_DS_Platform_28,
	Prototype_Mesh_SM_ENV_DT_BackdropTemple_01,
	Prototype_Mesh_SM_ENV_DT_BellSpline_01,
	Prototype_Mesh_SM_ENV_DT_Bull,
	Prototype_Mesh_SM_ENV_DT_BullPlatform_01,
	Prototype_Mesh_SM_ENV_DT_BullPlatform_Broken_01,
	Prototype_Mesh_SM_ENV_DT_Diya_02_01,
	Prototype_Mesh_SM_ENV_DT_DurgaBase_01,
	Prototype_Mesh_SM_ENV_DT_DurgaBase_02,
	Prototype_Mesh_SM_ENV_DT_DurgaBaseStep_01,
	Prototype_Mesh_SM_ENV_DT_DurgaBaseStep_02,
	Prototype_Mesh_SM_ENV_DT_LionEntrance_01,
	Prototype_Mesh_SM_ENV_DT_LionEntranceSplit_01,
	Prototype_Mesh_SM_ENV_DT_Mountain_02,
	Prototype_Mesh_SM_ENV_DT_Mountain_06,
	Prototype_Mesh_SM_ENV_DT_Mountain_08,
	Prototype_Mesh_SM_ENV_DT_PillarLarge_01,
	Prototype_Mesh_SM_ENV_DT_PillarLarge_02,
	Prototype_Mesh_SM_ENV_DT_Platform01,
	Prototype_Mesh_SM_ENV_DT_Platform02_2,
	Prototype_Mesh_SM_ENV_DT_Platform03,
	Prototype_Mesh_SM_ENV_DT_Platform04_2,
	Prototype_Mesh_SM_ENV_DT_Platform05,
	Prototype_Mesh_SM_ENV_DT_Platform06_Base,
	Prototype_Mesh_SM_ENV_DT_Platform06_StatueBase,
	Prototype_Mesh_SM_ENV_DT_Stairs_01_2,
	Prototype_Mesh_SM_ENV_DT_StatueBase_02,
	Prototype_Mesh_SM_ENV_DT_StatueBase_03,
	Prototype_Mesh_SM_ENV_DT_StatueFemale_Pose01,
	Prototype_Mesh_SM_ENV_DT_StatueFemale_Pose02,
	Prototype_Mesh_SM_ENV_DT_StatueFemale_Pose03,
	Prototype_Mesh_SM_ENV_DT_StatueFemale_Pose04,
	Prototype_Mesh_SM_ENV_DT_StatuePillar_01,
	Prototype_Mesh_SM_ENV_DT_StatuePillar_02,
	Prototype_Mesh_SM_ENV_DT_TempleBase_01,
	Prototype_Mesh_SM_ENV_DT_TempleBase_02,
	Prototype_Mesh_SM_ENV_DT_TempleBell_03,
	Prototype_Mesh_SM_ENV_F_3_Decorative_Piece_01,
	Prototype_Mesh_SM_ENV_F_3_Window_02,
	Prototype_Mesh_SM_ENV_F_3_Window_04,
	Prototype_Mesh_SM_ENV_F_3_Window_06,
	Prototype_Mesh_SM_ENV_F_3_WindowArch_01,
	Prototype_Mesh_SM_ENV_F_3_WindowArch_02,
	Prototype_Mesh_SM_ENV_F_Arch_01,
	Prototype_Mesh_SM_ENV_F_Arch_02,
	Prototype_Mesh_SM_ENV_F_Balcony_01,
	Prototype_Mesh_SM_ENV_F_Balcony_1B,
	Prototype_Mesh_SM_ENV_F_Balcony_1C,
	Prototype_Mesh_SM_ENV_F_Balcony_1D,
	Prototype_Mesh_SM_ENV_F_Balcony_1E,
	Prototype_Mesh_SM_ENV_F_Balcony_02,
	Prototype_Mesh_SM_ENV_F_Balcony_03,
	Prototype_Mesh_SM_ENV_F_Bow_Platform_03,
	Prototype_Mesh_SM_ENV_F_Bow_Platform_04,
	Prototype_Mesh_SM_ENV_F_Building_01,
	Prototype_Mesh_SM_ENV_F_Chatri_Base,
	Prototype_Mesh_SM_ENV_F_Corner_Wall_01,
	Prototype_Mesh_SM_ENV_F_Corner_Wall_02,
	Prototype_Mesh_SM_ENV_F_Corner_Wall_05,
	Prototype_Mesh_SM_ENV_F_Defences_01,
	Prototype_Mesh_SM_ENV_F_Defences_02,
	Prototype_Mesh_SM_ENV_F_Defences_02B,
	Prototype_Mesh_SM_ENV_F_Dome_05,
	Prototype_Mesh_SM_ENV_F_Dome_06,
	Prototype_Mesh_SM_ENV_F_Dome_08,
	Prototype_Mesh_SM_ENV_F_Dome_09,
	Prototype_Mesh_SM_ENV_F_Dome_014,
	Prototype_Mesh_SM_ENV_F_Door_Damaged_S_01,
	Prototype_Mesh_SM_ENV_F_Door_Damaged_S_02,
	Prototype_Mesh_SM_ENV_F_Door_NS_01,
	Prototype_Mesh_SM_ENV_F_Door_NS_02,
	Prototype_Mesh_SM_ENV_F_Door_S_01,
	Prototype_Mesh_SM_ENV_F_Door_S_02,
	Prototype_Mesh_SM_ENV_F_DoorWay_01,
	Prototype_Mesh_SM_ENV_F_DoorWayAbandoned_02,
	Prototype_Mesh_SM_ENV_F_DurgaAvatar_Temple_01,
	Prototype_Mesh_SM_ENV_F_DurgaAvatarTemple_Floor_01,
	Prototype_Mesh_SM_ENV_F_FirstFloorEntrance_01,
	Prototype_Mesh_SM_ENV_F_FirstFloorTile_01,
	Prototype_Mesh_SM_ENV_F_FirstFloorTile_02,
	Prototype_Mesh_SM_ENV_F_FirstFloorTile_03,
	Prototype_Mesh_SM_ENV_F_FirstFloorTile_04,
	Prototype_Mesh_SM_ENV_F_FirstFloorTile_05,
	Prototype_Mesh_SM_ENV_F_FirstFloorTile_06,
	Prototype_Mesh_SM_ENV_F_FloatingWall_01,
	Prototype_Mesh_SM_ENV_F_FloatingWall_02,
	Prototype_Mesh_SM_ENV_F_FloatingWall_03,
	Prototype_Mesh_SM_ENV_F_FloatingWall_Trim_07,
	Prototype_Mesh_SM_ENV_F_Floor_01_2,
	Prototype_Mesh_SM_ENV_F_Fort_Pathway_01,
	Prototype_Mesh_SM_ENV_F_Fort_Pathway_02,
	Prototype_Mesh_SM_ENV_F_Fort_Pathway_03,
	Prototype_Mesh_SM_ENV_F_Fort_Pathway_04,
	Prototype_Mesh_SM_ENV_F_FortBridge_Broken_01,
	Prototype_Mesh_SM_ENV_F_FortBridge_Broken_02,
	Prototype_Mesh_SM_ENV_F_FortBridge_Broken_03,
	Prototype_Mesh_SM_ENV_F_FortBridge_Broken_04,
	Prototype_Mesh_SM_ENV_F_FortCobbleArena_01,
	Prototype_Mesh_SM_ENV_F_FortWALL_01b,
	Prototype_Mesh_SM_ENV_F_FortWALL_01d,
	Prototype_Mesh_SM_ENV_F_FortWALL_05,
	Prototype_Mesh_SM_ENV_F_Frame_01_a_int,
	Prototype_Mesh_SM_ENV_F_Frame_01_int,
	Prototype_Mesh_SM_ENV_F_FrameClosed_01,
	Prototype_Mesh_SM_ENV_F_HighWall_01,
	Prototype_Mesh_SM_ENV_F_Mystic_Creeper_06,
	Prototype_Mesh_SM_ENV_F_Mystic_Creeper_07,
	Prototype_Mesh_SM_ENV_F_Mystic_Creeper_09,
	Prototype_Mesh_SM_ENV_F_PlatFORM_02,
	Prototype_Mesh_SM_ENV_F_PlatFORM_03,
	Prototype_Mesh_SM_ENV_F_PlatFORM_05,
	Prototype_Mesh_SM_ENV_F_PlatFORM_07,
	Prototype_Mesh_SM_ENV_F_PlatFORM_08,
	Prototype_Mesh_SM_ENV_F_PlatFORM_09A,
	Prototype_Mesh_SM_ENV_F_PlatFORM_09B,
	Prototype_Mesh_SM_ENV_F_PlatFORM_09C,
	Prototype_Mesh_SM_ENV_F_PlatFORM_09D,
	Prototype_Mesh_SM_ENV_F_PlatFORM_09E,
	Prototype_Mesh_SM_ENV_F_Platform_Wall_01,
	Prototype_Mesh_SM_ENV_F_Rani_Platform_05,
	Prototype_Mesh_SM_ENV_F_Roof_01,
	Prototype_Mesh_SM_ENV_F_RoofWall_Assembled_01,
	Prototype_Mesh_SM_ENV_F_RoofWall_Assembled_02a,
	Prototype_Mesh_SM_ENV_F_RoofWall_Single,
	Prototype_Mesh_SM_ENV_F_RoofWall_Single_02,
	Prototype_Mesh_SM_ENV_F_Stairs_02,
	Prototype_Mesh_SM_ENV_F_Stairs_04,
	Prototype_Mesh_SM_ENV_F_Stairs_Railing_01,
	Prototype_Mesh_SM_ENV_F_StatueBase_03,
	Prototype_Mesh_SM_ENV_F_StoryPanel_01,
	Prototype_Mesh_SM_ENV_F_Tez_Balcony,
	Prototype_Mesh_SM_ENV_F_Tower_01,
	Prototype_Mesh_SM_ENV_F_Tower_02,
	Prototype_Mesh_SM_ENV_F_Tower_06,
	Prototype_Mesh_SM_ENV_F_Tower_07,
	Prototype_Mesh_SM_ENV_F_Tower_08,
	Prototype_Mesh_SM_ENV_F_ViewPlatform_01,
	Prototype_Mesh_SM_ENV_F_Wall_01,
	Prototype_Mesh_SM_ENV_F_Wall_Corner_Holes_02,
	Prototype_Mesh_SM_ENV_F_Wall_Holes_01,
	Prototype_Mesh_SM_ENV_F_Wall_Holes_02,
	Prototype_Mesh_SM_ENV_F_Wall_Holes_Half_01,
	Prototype_Mesh_SM_ENV_F_Wall_Holes_Turret_01,
	Prototype_Mesh_SM_ENV_F_Wall_Holes_Turret_02,
	Prototype_Mesh_SM_ENV_F_Wall_Solid_01,
	Prototype_Mesh_SM_ENV_F_Wall_Solid_02,
	Prototype_Mesh_SM_ENV_F_Wall_Step_01,
	Prototype_Mesh_SM_ENV_F_WallTrim_02,
	Prototype_Mesh_SM_ENV_F_Window_Pane_White,
	Prototype_Mesh_SM_ENV_FemaleStatue_Pose_04,
	Prototype_Mesh_SM_ENV_FemaleStatue_Pose_06,
	Prototype_Mesh_SM_ENV_PRP_PillarStatuesBroken_01,
	Prototype_Mesh_SM_ENV_PRP_PillarStatuesBroken_02,
	Prototype_Mesh_SM_ENV_PRP_PillarStatuesBroken_04,
	Prototype_Mesh_SM_ENV_PRP_PillarStatuesBroken_05,
	Prototype_Mesh_SM_ENV_PRP_PillarStatuesBroken_06,
	Prototype_Mesh_SM_ENV_T_Creeper_01,
	Prototype_Mesh_SM_ENV_T_DemonT_Roots_04,
	Prototype_Mesh_SM_ENV_T_DemonT_Roots_05,
	Prototype_Mesh_SM_ENV_T_HangingPlant_01,
	Prototype_Mesh_SM_ENV_T_TetrisBase_01,
	Prototype_Mesh_SM_ENV_T_TetrisBase_Trim_01,
	Prototype_Mesh_SM_ENV_WallBasic_01,
	Prototype_Mesh_SM_ENV_WallBasic_02,
	Prototype_Mesh_SM_ENV_WallBasic_04,
	Prototype_Mesh_SM_ENV_WallBasic_05,
	Prototype_Mesh_SM_ENV_WallBasic_08,
	Prototype_Mesh_SM_ENV_WallBasic_09,
	Prototype_Mesh_SM_ENV_WallBasic_11,
	Prototype_Mesh_SM_ENV_Window_A_RED_BLOCK_01,
	Prototype_Mesh_SM_F_ArchIvy_01,
	Prototype_Mesh_SM_F_ArchIvy_03,
	Prototype_Mesh_SM_F_BalconyPlant01,
	Prototype_Mesh_SM_F_BalconyPlant03,
	Prototype_Mesh_SM_F_Creepers_V01,
	Prototype_Mesh_SM_F_Flower02,
	Prototype_Mesh_SM_F_HangingIvy_01,
	Prototype_Mesh_SM_F_Mystic_Bridge_Creeper_01,
	Prototype_Mesh_SM_F_Mystic_Bridge_Creeper_02,
	Prototype_Mesh_SM_F_Mystic_Bridge_Creeper_03,
	Prototype_Mesh_SM_F_Mystic_Cliff_Creeper_01,
	Prototype_Mesh_SM_F_Mystic_Cliff_Creeper_02,
	Prototype_Mesh_SM_F_Mystic_Creeper_01,
	Prototype_Mesh_SM_F_Mystic_Creeper_02,
	Prototype_Mesh_SM_F_Mystic_Creeper_05,
	Prototype_Mesh_SM_F_Mystic_Creeper_06,
	Prototype_Mesh_SM_F_Mystic_Creeper_07,
	Prototype_Mesh_SM_F_Mystic_Creeper_08,
	Prototype_Mesh_SM_F_Mystic_Creeper_09,
	Prototype_Mesh_SM_F_Mystic_Creeper_10,
	Prototype_Mesh_SM_F_Mystic_Creeper_11,
	Prototype_Mesh_SM_F_Mystic_Creeper_12,
	Prototype_Mesh_SM_F_Mystic_Creeper_13,
	Prototype_Mesh_SM_F_Mystic_Creeper_14,
	Prototype_Mesh_SM_F_Mystic_Creeper_15,
	Prototype_Mesh_SM_F_Mystic_Creeper_16,
	Prototype_Mesh_SM_F_Mystic_Creeper_17,
	Prototype_Mesh_SM_F_Mystic_Creeper_18,
	Prototype_Mesh_SM_F_Mystic_Creeper_19,
	Prototype_Mesh_SM_F_Mystic_Creeper_20,
	Prototype_Mesh_SM_F_Mystic_Creeper_21,
	Prototype_Mesh_SM_F_Mystic_Creeper_22,
	Prototype_Mesh_SM_F_Mystic_Creeper_24,
	Prototype_Mesh_SM_F_Mystic_Creeper_25,
	Prototype_Mesh_SM_F_Mystic_MergedRoots_04,
	Prototype_Mesh_SM_F_Mystic_Roots_02,
	Prototype_Mesh_SM_F_Mystic_S_Creeper_01,
	Prototype_Mesh_SM_F_Mystic_S_Creeper_02,
	Prototype_Mesh_SM_F_Mystic_Shiv_Creeper_01,
	Prototype_Mesh_SM_F_Mystic_Snake_Creeper_0,
	Prototype_Mesh_SM_F_Mystic_Snake_Creeper_01,
	Prototype_Mesh_SM_F_Mystic_Snake_Creeper_03,
	Prototype_Mesh_SM_F_Mystic_Snake_Creeper_05,
	Prototype_Mesh_SM_F_Mystic_Snake_Creeper_06,
	Prototype_Mesh_SM_F_Mystic_Snake_Creeper_07,
	Prototype_Mesh_SM_F_Mystic_Creeper_03,
	Prototype_Mesh_SM_F_Mystics_Creeper_04,
	Prototype_Mesh_SM_F_Mystics_Creeper_05,
	Prototype_Mesh_SM_F_Mystics_MergedRoots_01,
	Prototype_Mesh_SM_F_Mystics_MergedRoots_02,
	Prototype_Mesh_SM_F_Mystics_MergedRoots_03,
	Prototype_Mesh_SM_F_Mystics_MergedRoots_05,
	Prototype_Mesh_SM_F_Mystics_MergedRoots_06,
	Prototype_Mesh_SM_F_Mystics_Roots_01,
	Prototype_Mesh_SM_F_PurpleFlower_Fort_01,
	Prototype_Mesh_SM_F_PurpleFlower_Fort_02,
	Prototype_Mesh_SM_F_PurpleFlower_Opt02,
	Prototype_Mesh_SM_F_PurpleFlowerOpt_01,
	Prototype_Mesh_SM_F_PurpleFlowerOpt_04,
	Prototype_Mesh_SM_F_Roots_01,
	Prototype_Mesh_SM_F_Roots_03,
	Prototype_Mesh_SM_F_Roots_05,
	Prototype_Mesh_SM_F_Roots01,
	Prototype_Mesh_SM_F_Statue_Creeper_01,
	Prototype_Mesh_SM_F_Statue_Creeper_02,
	Prototype_Mesh_SM_F_Statue_Creeper_03,
	Prototype_Mesh_SM_F_Statue_Creeper_04,
	Prototype_Mesh_SM_F_Vine06,
	Prototype_Mesh_SM_F_Vine07,
	Prototype_Mesh_SM_F_Vine08,
	Prototype_Mesh_SM_F_Vine09,
	Prototype_Mesh_SM_F_Vine10,
	Prototype_Mesh_SM_F_Vine12,
	Prototype_Mesh_SM_F_Vine14,
	Prototype_Mesh_SM_F_Vine15,
	Prototype_Mesh_SM_F_Vine16,
	Prototype_Mesh_SM_F_Vine18,
	Prototype_Mesh_SM_F_Vine19,
	Prototype_Mesh_SM_F_Vine22,
	Prototype_Mesh_SM_F_Vine23,
	Prototype_Mesh_SM_F_Vine24,
	Prototype_Mesh_SM_F_Vine25,
	Prototype_Mesh_SM_F_Vine26,
	Prototype_Mesh_SM_F_Vine27,
	Prototype_Mesh_SM_F_Vine28,
	Prototype_Mesh_SM_F_Vine31,
	Prototype_Mesh_SM_F_Vine32,
	Prototype_Mesh_SM_F_Vine33,
	Prototype_Mesh_SM_F_Vine36,
	Prototype_Mesh_SM_F_Vines40,
	Prototype_Mesh_SM_Fern_01,
	Prototype_Mesh_SM_Fern_02,
	Prototype_Mesh_SM_Fern_03,
	Prototype_Mesh_SM_FlowerBush_01,
	Prototype_Mesh_SM_FlowerBush_02,
	Prototype_Mesh_SM_FlowerBush_03,
	Prototype_Mesh_SM_FLowerTest_06,
	Prototype_Mesh_SM_FlowerVine_01,
	Prototype_Mesh_SM_FlowerVine01,
	Prototype_Mesh_SM_FlowerVines_02,
	Prototype_Mesh_SM_FlowerVines_03,
	Prototype_Mesh_SM_FlowerVines_04,
	Prototype_Mesh_SM_FlowerVines_05,
	Prototype_Mesh_SM_FlowerVines_07,
	Prototype_Mesh_SM_FlowerVines_08,
	Prototype_Mesh_SM_FlowerVines_09,
	Prototype_Mesh_SM_Fort_Bow_Creeper_0,
	Prototype_Mesh_SM_Fort_Bow_Creeper_03,
	Prototype_Mesh_SM_Fort_Bow_Creeper_04,
	Prototype_Mesh_SM_Fort_Courtyard_02,
	Prototype_Mesh_SM_Fort_Courtyard_Creeper_03,
	Prototype_Mesh_SM_Fort_Courtyard_Creeper_04,
	Prototype_Mesh_SM_Fort_Creeper_01,
	Prototype_Mesh_SM_Fort_Creeper_02,
	Prototype_Mesh_SM_Fort_Creeper_03,
	Prototype_Mesh_SM_Fort_Creeper_04,
	Prototype_Mesh_SM_Fort_Creeper_05,
	Prototype_Mesh_SM_Fort_Creeper_06,
	Prototype_Mesh_SM_Fort_Creeper_07,
	Prototype_Mesh_SM_Fort_Creeper_08,
	Prototype_Mesh_SM_Fort_Creeper_10,
	Prototype_Mesh_SM_Fort_Creeper_11,
	Prototype_Mesh_SM_Fort_Creeper_12,
	Prototype_Mesh_SM_Fort_Creeper_13,
	Prototype_Mesh_SM_Fort_Creeper_15,
	Prototype_Mesh_SM_Fort_Creeper_16,
	Prototype_Mesh_SM_Fort_Creeper_17,
	Prototype_Mesh_SM_Fort_Creeper_18,
	Prototype_Mesh_SM_Fort_Creeper_19,
	Prototype_Mesh_SM_Fort_Creeper_20,
	Prototype_Mesh_SM_Fort_Creeper_21,
	Prototype_Mesh_SM_Fort_Creeper_22,
	Prototype_Mesh_SM_Fort_Creeper_23,
	Prototype_Mesh_SM_Fort_Creeper_24,
	Prototype_Mesh_SM_Fort_Creeper_25,
	Prototype_Mesh_SM_Fort_Creeper_26,
	Prototype_Mesh_SM_Fort_Creeper_27,
	Prototype_Mesh_SM_Fort_Creeper_28,
	Prototype_Mesh_SM_Fort_Creeper_29,
	Prototype_Mesh_SM_Fort_Creeper_30,
	Prototype_Mesh_SM_Fort_Creeper_32,
	Prototype_Mesh_SM_Fort_Creeper_33,
	Prototype_Mesh_SM_Fort_Creeper_34,
	Prototype_Mesh_SM_Fort_RedCreeper_01,
	Prototype_Mesh_SM_Fort_RedCreeper_02,
	Prototype_Mesh_SM_Fort_RedCreeper_03,
	Prototype_Mesh_SM_Fort_RedCreeper_04,
	Prototype_Mesh_SM_Fort_WhiteFlower_01,
	Prototype_Mesh_SM_Fortwall_blocker_01,
	Prototype_Mesh_SM_Fortwall_blocker_02,
	Prototype_Mesh_SM_Fortwall_blocker_03,
	Prototype_Mesh_SM_Fortwall_blocker_04,
	Prototype_Mesh_SM_Fortwall_blocker_05,
	Prototype_Mesh_SM_Fortwall_blocker_06,
	Prototype_Mesh_SM_Fortwall_Cover_01,
	Prototype_Mesh_SM_FortWall_Market_01,
	Prototype_Mesh_SM_FortWall_Market_02,
	Prototype_Mesh_SM_FortWall_Market_03,
	Prototype_Mesh_SM_FortWall_Market_04,
	Prototype_Mesh_SM_FortWall_Market_05,
	Prototype_Mesh_SM_FortWall_Market_06,
	Prototype_Mesh_SM_FortWall_Market_07,
	Prototype_Mesh_SM_FortWall_Market_08,
	Prototype_Mesh_SM_FortWall_Market_09,
	Prototype_Mesh_SM_FortWall_Market_10,
	Prototype_Mesh_SM_FortWall_Market_11,
	Prototype_Mesh_SM_FortWall_Market_12,
	Prototype_Mesh_SM_Fortwall_Market2_01,
	Prototype_Mesh_SM_FortWall_side_01,
	Prototype_Mesh_SM_FortWall_side_02,
	Prototype_Mesh_SM_Gadasura_Head_03,
	Prototype_Mesh_SM_GEN_Tree_01,
	Prototype_Mesh_SM_GEN_Tree_02,
	Prototype_Mesh_SM_Grass_01A,
	Prototype_Mesh_SM_Grass_01A1,
	Prototype_Mesh_SM_Green_Stack_01,
	Prototype_Mesh_SM_GRND_Bush_01,
	Prototype_Mesh_SM_GRND_Bush_02,
	Prototype_Mesh_SM_GRND_Bush_03,
	Prototype_Mesh_SM_GRND_Bush_05,
	Prototype_Mesh_SM_GRND_PLANT_01,
	Prototype_Mesh_SM_GRND_Plant_CVR_01,
	Prototype_Mesh_SM_GroundClutter_01,
	Prototype_Mesh_SM_GroundScatter_01A,
	Prototype_Mesh_SM_GroundScatter_02A,
	Prototype_Mesh_SM_GroundScatter_03A,
	Prototype_Mesh_SM_GroundScatter_03B,
	Prototype_Mesh_SM_Hedgerow_01,
	Prototype_Mesh_SM_Hedgerow_01_A,
	Prototype_Mesh_SM_Hedgerow_Corner_01,
	Prototype_Mesh_SM_lillypad_01,
	Prototype_Mesh_SM_lillypad_02,
	Prototype_Mesh_SM_Lillypad_02_A,
	Prototype_Mesh_SM_Lillypad_02_B,
	Prototype_Mesh_SM_Lillypad_02_C,
	Prototype_Mesh_SM_Lillypad_02_D,
	Prototype_Mesh_SM_lillypad_03,
	Prototype_Mesh_SM_lillypad_04,
	Prototype_Mesh_SM_lillypad_05,
	Prototype_Mesh_SM_lillypad_06,
	Prototype_Mesh_SM_Lotus_01,
	Prototype_Mesh_SM_Lotus_01_flower,
	Prototype_Mesh_SM_Lotus_01_stem,
	Prototype_Mesh_SM_Lotus_bud_01,
	Prototype_Mesh_SM_Lotus_bud_main_01,
	Prototype_Mesh_SM_Lotus_Pink,
	Prototype_Mesh_SM_MERGED_PRESET_Ivy1_BP_6,
	Prototype_Mesh_SM_MERGED_PRESET_Liana1_BP_11,
	Prototype_Mesh_SM_MERGED_PRESET_Liana1_BP_14,
	Prototype_Mesh_SM_MERGED_PRESET_Liana1_BP2,
	Prototype_Mesh_SM_MFort_Creeper_07,
	Prototype_Mesh_SM_Mossplant,
	Prototype_Mesh_SM_Mystic_Bush_01,
	Prototype_Mesh_SM_Mystic_Bush_01A,
	Prototype_Mesh_SM_Mystic_Bush_02A,
	Prototype_Mesh_SM_Mystic_Roots_Hanging_01,
	Prototype_Mesh_SM_Mystic_Roots_Hanging_02,
	Prototype_Mesh_SM_Mystic_Roots_Hanging_03,
	Prototype_Mesh_SM_Ninja_Run_Pose,
	Prototype_Mesh_SM_palmbush_01,
	Prototype_Mesh_SM_palmbush_02,
	Prototype_Mesh_SM_Pinkflower_vine_01,
	Prototype_Mesh_SM_Pinkvine_wall_01,
	Prototype_Mesh_SM_Pinkvine_wall_01a,
	Prototype_Mesh_SM_PRP_Arrow_01,
	Prototype_Mesh_SM_PRP_Banner_01_Static,
	Prototype_Mesh_SM_PRP_Banner_05_Static,
	Prototype_Mesh_SM_PRP_Bow_01,
	Prototype_Mesh_SM_PRP_BrokenPillars_01,
	Prototype_Mesh_SM_PRP_BrokenPillars_06,
	Prototype_Mesh_SM_PRP_BrokenPillars_07,
	Prototype_Mesh_SM_PRP_BrokenWall03,
	Prototype_Mesh_SM_PRP_CC_Anvil_01,
	Prototype_Mesh_SM_PRP_CC_Barrel_01,
	Prototype_Mesh_SM_PRP_CC_Barrel_02,
	Prototype_Mesh_SM_PRP_CC_BarrelGroup_01,
	Prototype_Mesh_SM_PRP_CC_BarrelGroup_04,
	Prototype_Mesh_SM_PRP_CC_BarrelGroup_06,
	Prototype_Mesh_SM_PRP_CC_BarrelGroup_07,
	Prototype_Mesh_SM_PRP_CC_BarrelGroup_08,
	Prototype_Mesh_SM_PRP_CC_BoatRound_01,
	Prototype_Mesh_SM_PRP_CC_Bucket_01,
	Prototype_Mesh_SM_PRP_CC_Bucket_02,
	Prototype_Mesh_SM_PRP_CC_Cauldron_01,
	Prototype_Mesh_SM_PRP_CC_Cauldron_02,
	Prototype_Mesh_SM_PRP_CC_CauldronLarge_01,
	Prototype_Mesh_SM_PRP_CC_Chair_01,
	Prototype_Mesh_SM_PRP_CC_Chisel_02,
	Prototype_Mesh_SM_PRP_CC_Chisel_05,
	Prototype_Mesh_SM_PRP_CC_ClaypotGroup_01,
	Prototype_Mesh_SM_PRP_CC_ClaypotGroup_03,
	Prototype_Mesh_SM_PRP_CC_ClaypotGroup_04,
	Prototype_Mesh_SM_PRP_CC_ClaypotGroup_05,
	Prototype_Mesh_SM_PRP_CC_Cushion_01,
	Prototype_Mesh_SM_PRP_CC_Cushion_02,
	Prototype_Mesh_SM_PRP_CC_Cushion_03,
	Prototype_Mesh_SM_PRP_CC_DockPlanks_01,
	Prototype_Mesh_SM_PRP_CC_DockPlanks_02,
	Prototype_Mesh_SM_PRP_CC_DockPlanks_03,
	Prototype_Mesh_SM_PRP_CC_DockPlanks_04,
	Prototype_Mesh_SM_PRP_CC_DockPlanks_05,
	Prototype_Mesh_SM_PRP_CC_DockPlanks_06,
	Prototype_Mesh_SM_PRP_CC_DockPlanks_07,
	Prototype_Mesh_SM_PRP_CC_DockStairs_01,
	Prototype_Mesh_SM_PRP_CC_DockStepWall_01,
	Prototype_Mesh_SM_PRP_CC_ElephantHead_01,
	Prototype_Mesh_SM_PRP_CC_FishCrate_01,
	Prototype_Mesh_SM_PRP_CC_FishCrate_02,
	Prototype_Mesh_SM_PRP_CC_FishCrate_03,
	Prototype_Mesh_SM_PRP_CC_FishCrate_04,
	Prototype_Mesh_SM_PRP_CC_FishCrate_05,
	Prototype_Mesh_SM_PRP_CC_FishCrate_06,
	Prototype_Mesh_SM_PRP_CC_GlassPanel_01,
	Prototype_Mesh_SM_PRP_CC_Grinder_01,
	Prototype_Mesh_SM_PRP_CC_Hammer_01,
	Prototype_Mesh_SM_PRP_CC_Jar_02,
	Prototype_Mesh_SM_PRP_CC_Ladle_01,
	Prototype_Mesh_SM_PRP_CC_LampPillar_01,
	Prototype_Mesh_SM_PRP_CC_LanternCotton_01,
	Prototype_Mesh_SM_PRP_CC_LanternCotton_02,
	Prototype_Mesh_SM_PRP_CC_LanternMetal_01,
	Prototype_Mesh_SM_PRP_CC_LanternMetal_02,
	Prototype_Mesh_SM_PRP_CC_LeverBase_02,
	Prototype_Mesh_SM_PRP_CC_Pier_01,
	Prototype_Mesh_SM_PRP_CC_PierCorner_01,
	Prototype_Mesh_SM_PRP_CC_PierCorner_02,
	Prototype_Mesh_SM_PRP_CC_PierFenceEnd_01,
	Prototype_Mesh_SM_PRP_CC_PierFenceEnd_02,
	Prototype_Mesh_SM_PRP_CC_PierFenceStart_01,
	Prototype_Mesh_SM_PRP_CC_PierFenceStart_02,
	Prototype_Mesh_SM_PRP_CC_PierNarrow_01,
	Prototype_Mesh_SM_PRP_CC_PierNarrow_02,
	Prototype_Mesh_SM_PRP_CC_PierNarrow_03,
	Prototype_Mesh_SM_PRP_CC_PierNarrow_04,
	Prototype_Mesh_SM_PRP_CC_PierWide_01,
	Prototype_Mesh_SM_PRP_CC_PierWide_02,
	Prototype_Mesh_SM_PRP_CC_PierWide_03,
	Prototype_Mesh_SM_PRP_CC_PierWide_04,
	Prototype_Mesh_SM_PRP_CC_RedCarpet_01,
	Prototype_Mesh_SM_PRP_CC_RedCarpet_02,
	Prototype_Mesh_SM_PRP_CC_RedCarpet_03,
	Prototype_Mesh_SM_PRP_CC_SackGroup_01,
	Prototype_Mesh_SM_PRP_CC_SackGroup_02,
	Prototype_Mesh_SM_PRP_CC_SackGroup_03,
	Prototype_Mesh_SM_PRP_CC_SackGroup_04,
	Prototype_Mesh_SM_PRP_CC_SackGroup_05,
	Prototype_Mesh_SM_PRP_CC_SackGroup_06,
	Prototype_Mesh_SM_PRP_CC_ShieldStandGroup_01,
	Prototype_Mesh_SM_PRP_CC_ShieldStandGroup_02,
	Prototype_Mesh_SM_PRP_CC_ShieldStandGroup_03,
	Prototype_Mesh_SM_PRP_CC_ShieldStandGroup_04,
	Prototype_Mesh_SM_PRP_CC_Soldier_01,
	Prototype_Mesh_SM_PRP_CC_Soldier_02,
	Prototype_Mesh_SM_PRP_CC_SpearStandGroup_02,
	Prototype_Mesh_SM_PRP_CC_SquareLantern_01,
	Prototype_Mesh_SM_PRP_CC_SquareLantern_02,
	Prototype_Mesh_SM_PRP_CC_SquareLantern_03,
	Prototype_Mesh_SM_PRP_CC_Stool_01,
	Prototype_Mesh_SM_PRP_CC_SwordStandGroup_01,
	Prototype_Mesh_SM_PRP_CC_SwordStandGroup_02,
	Prototype_Mesh_SM_PRP_CC_SwordStandGroup_04,
	Prototype_Mesh_SM_PRP_CC_SwordStandGroup_05,
	Prototype_Mesh_SM_PRP_CC_Table_01,
	Prototype_Mesh_SM_PRP_CC_TableGroup_01,
	Prototype_Mesh_SM_PRP_CC_TableGroup_02,
	Prototype_Mesh_SM_PRP_CC_TableGroup_03,
	Prototype_Mesh_SM_PRP_CC_TableGroup_04,
	Prototype_Mesh_SM_PRP_CC_TableGroup_05,
	Prototype_Mesh_SM_PRP_CC_TableGroup_06,
	Prototype_Mesh_SM_PRP_CC_TableGroup_07,
	Prototype_Mesh_SM_PRP_CC_TableGroup_08,
	Prototype_Mesh_SM_PRP_CC_TableGroup_09,
	Prototype_Mesh_SM_PRP_CC_TableSmall_01,
	Prototype_Mesh_SM_PRP_CC_ToolsGroup_01,
	Prototype_Mesh_SM_PRP_CC_ToolsGroup_02,
	Prototype_Mesh_SM_PRP_CC_ToolsGroup_03,
	Prototype_Mesh_SM_PRP_CC_ToolsGroup_04,
	Prototype_Mesh_SM_PRP_CC_ToolsGroup_05,
	Prototype_Mesh_SM_PRP_CC_ToolsGroup_06,
	Prototype_Mesh_SM_PRP_CC_ToolsGroup_07,
	Prototype_Mesh_SM_PRP_CC_TreeBase_01,
	Prototype_Mesh_SM_PRP_CC_WoodBox_01,
	Prototype_Mesh_SM_PRP_CC_WoodBoxGroup_01,
	Prototype_Mesh_SM_PRP_CC_WoodBoxGroup_02,
	Prototype_Mesh_SM_PRP_CC_WoodBoxGroup_03,
	Prototype_Mesh_SM_PRP_CC_WoodBoxGroup_04,
	Prototype_Mesh_SM_PRP_CC_WoodBoxGroup_05,
	Prototype_Mesh_SM_PRP_CC_WoodBoxGroup_06,
	Prototype_Mesh_SM_PRP_CC_WoodBoxGroup_07,
	Prototype_Mesh_SM_PRP_ChainLink_01,
	Prototype_Mesh_SM_PRP_Curtain_01,
	Prototype_Mesh_SM_PRP_Curtain_02,
	Prototype_Mesh_SM_PRP_Curtain_03,
	Prototype_Mesh_SM_PRP_Curtain_04,
	Prototype_Mesh_SM_PRP_D_Ribbon_Extension,
	Prototype_Mesh_SM_PRP_D_RibbonLarge_01,
	Prototype_Mesh_SM_PRP_D_RibbonLarge_02,
	Prototype_Mesh_SM_PRP_D_RibbonLarge_03,
	Prototype_Mesh_SM_PRP_D_RibbonSmall_01,
	Prototype_Mesh_SM_PRP_D_StoneStep_01,
	Prototype_Mesh_SM_PRP_D_StoneStep_02,
	Prototype_Mesh_SM_PRP_D_TreePedestal_01,
	Prototype_Mesh_SM_PRP_D_WillowTREE_01,
	Prototype_Mesh_SM_PRP_DBPully,
	Prototype_Mesh_SM_PRP_DBPully_DM,
	Prototype_Mesh_SM_PRP_DBPully_Handle_,
	Prototype_Mesh_SM_PRP_DeadSoldier_02,
	Prototype_Mesh_SM_PRP_DeadSoldier_03,
	Prototype_Mesh_SM_PRP_DeadSoldier_04,
	Prototype_Mesh_SM_PRP_DeadSoldier_05,
	Prototype_Mesh_SM_PRP_DeadSoldier_06,
	Prototype_Mesh_SM_PRP_DeadSoldier_07,
	Prototype_Mesh_SM_PRP_DeadSoldier_08,
	Prototype_Mesh_SM_PRP_DeadSoldier_09,
	Prototype_Mesh_SM_PRP_DT_Arm_left_01,
	Prototype_Mesh_SM_PRP_DT_Arm_left_02,
	Prototype_Mesh_SM_PRP_DT_Arm_Right_01,
	Prototype_Mesh_SM_PRP_DT_Arm_Right_02,
	Prototype_Mesh_SM_PRP_DT_Durga_Statue_01,
	Prototype_Mesh_SM_PRP_DT_ElephantHead_01,
	Prototype_Mesh_SM_PRP_DT_WP_Bell_01,
	Prototype_Mesh_SM_PRP_DT_WP_Bow_01,
	Prototype_Mesh_SM_PRP_DT_WP_Chakra_01,
	Prototype_Mesh_SM_PRP_DT_WP_Drum_01,
	Prototype_Mesh_SM_PRP_DT_WP_Gada_01,
	Prototype_Mesh_SM_PRP_DT_WP_Lotus_01,
	Prototype_Mesh_SM_PRP_DT_WP_Shield_01,
	Prototype_Mesh_SM_PRP_DT_WP_Sword_01,
	Prototype_Mesh_SM_PRP_DT_WP_Trishul_01,
	Prototype_Mesh_SM_PRP_F_DoorLedge_01,
	Prototype_Mesh_SM_PRP_F_FemaleStatue_DemonTree_01,
	Prototype_Mesh_SM_PRP_F_GlassPanel_01,
	Prototype_Mesh_SM_PRP_F_GlassPanel_02,
	Prototype_Mesh_SM_PRP_F_PaintingFrame_01,
	Prototype_Mesh_SM_PRP_F_PaintingFrame_02,
	Prototype_Mesh_SM_PRP_Fireplace_01,
	Prototype_Mesh_SM_PRP_Fireplace_02,
	Prototype_Mesh_SM_PRP_FireplaceCooking_01,
	Prototype_Mesh_SM_PRP_MandalaFrame_01,
	Prototype_Mesh_SM_PRP_MS_RootBlocker_01,
	Prototype_Mesh_SM_PRP_Rock01,
	Prototype_Mesh_SM_PRP_Scarecrow_01,
	Prototype_Mesh_SM_PRP_Sculpture01,
	Prototype_Mesh_SM_PRP_Sculpture02,
	Prototype_Mesh_SM_PRP_Shield_01,
	Prototype_Mesh_SM_PRP_Spear_01,
	Prototype_Mesh_SM_PRP_TrishulStatue,
	Prototype_Mesh_SM_PRP_UtensilGroup_01,
	Prototype_Mesh_SM_PRP_UtensilGroup_02,
	Prototype_Mesh_SM_PRP_UtensilGroup_03,
	Prototype_Mesh_SM_PRP_UtensilGroup_04,
	Prototype_Mesh_SM_PRP_UtensilGroup_05,
	Prototype_Mesh_SM_PRP_UtensilGroup_06,
	Prototype_Mesh_SM_PRP_UtensilGroup_07,
	Prototype_Mesh_SM_PRP_UtensilGroup_08,
	Prototype_Mesh_SM_PRP_UtensilGroup_10,
	Prototype_Mesh_SM_PRP_UtensilGroup_11,
	Prototype_Mesh_SM_PRP_Wall02,
	Prototype_Mesh_SM_PRP_WallPillar01,
	Prototype_Mesh_SM_PRP_WeaponGroup_01,
	Prototype_Mesh_SM_PRP_WeaponGroup_02,
	Prototype_Mesh_SM_PRP_WeaponGroup_03,
	Prototype_Mesh_SM_PRP_WeaponGroup_04,
	Prototype_Mesh_SM_PRP_WeaponGroup_05,
	Prototype_Mesh_SM_PRP_WeaponGroup_06,
	Prototype_Mesh_SM_PRP_WeaponGroup_07,
	Prototype_Mesh_SM_Rangda_Finger_Left,
	Prototype_Mesh_SM_rock_river_01,
	Prototype_Mesh_SM_rock_river_02,
	Prototype_Mesh_SM_rock_river_03,
	Prototype_Mesh_SM_rock_river_04,
	Prototype_Mesh_SM_rock_river_05,
	Prototype_Mesh_SM_rock_river_06,
	Prototype_Mesh_SM_rock_river_07,
	Prototype_Mesh_SM_rock_river_08,
	Prototype_Mesh_SM_rock_river_10,
	Prototype_Mesh_SM_Root_01,
	Prototype_Mesh_SM_Root_02,
	Prototype_Mesh_SM_Root_03,
	Prototype_Mesh_SM_Roots_01,
	Prototype_Mesh_SM_Roots_Hanging_01,
	Prototype_Mesh_SM_Roots_Hanging_02,
	Prototype_Mesh_SM_Roots_Hanging_03,
	Prototype_Mesh_SM_Roots_Hanging_04,
	Prototype_Mesh_SM_Roots_Hanging_05,
	Prototype_Mesh_SM_Roots_Hanging_06,
	Prototype_Mesh_SM_Roundbush_01,
	Prototype_Mesh_SM_Roundbush_02,
	Prototype_Mesh_SM_Statue_Female_ReBirth_Body,
	Prototype_Mesh_SM_Statue_Female_ReBirth_Feet,
	Prototype_Mesh_SM_Statue_Female_ReBirth_Leg_02,
	Prototype_Mesh_SM_Statue_Female_ReBirth_Leg_03,
	Prototype_Mesh_SM_Statue_Ganga_01,
	Prototype_Mesh_SM_Statue_Shiva_01,
	Prototype_Mesh_SM_T_Creeper_02,
	Prototype_Mesh_SM_T_Creeper_04,
	Prototype_Mesh_SM_T_Creeper_05,
	Prototype_Mesh_SM_T_Creeper_06,
	Prototype_Mesh_SM_T_Creeper_09,
	Prototype_Mesh_SM_T_Creeper_11,
	Prototype_Mesh_SM_T_Creeper_12,
	Prototype_Mesh_SM_T_Creeper_13,
	Prototype_Mesh_SM_T_Creepers_10,
	Prototype_Mesh_SM_T_Creepers_14,
	Prototype_Mesh_SM_T_Creepers_15,
	Prototype_Mesh_SM_T_Roots_01,
	Prototype_Mesh_SM_T_Roots_02,
	Prototype_Mesh_SM_T_Roots_03,
	Prototype_Mesh_SM_Tree_Big_01,
	Prototype_Mesh_SM_Tree_Big_02,
	Prototype_Mesh_SM_Tree_Trunk_1,
	Prototype_Mesh_SM_tree_trunk2,
	Prototype_Mesh_SM_Tropical_Tree_02,
	Prototype_Mesh_SM_Tropical_Tree_03,
	Prototype_Mesh_SM_TropicalTree_01,
	Prototype_Mesh_SM_Trunk_Ground_01,
	Prototype_Mesh_SM_Vine_Pillar_01,
	Prototype_Mesh_SM_Vine_Pillar_02,
	Prototype_Mesh_SM_Vine_Pillar_04,
	Prototype_Mesh_SM_Vine_Pillar_05,
	Prototype_Mesh_SM_Vine_Pillar_06,
	Prototype_Mesh_SM_Vine_Pillar_07,
	Prototype_Mesh_SM_Vine_Pillar_08,
	Prototype_Mesh_SM_Vines_Ruins_Exterior_ENT_02,
	Prototype_Mesh_SM_Vines_Straight_004,
	Prototype_Mesh_SM_Vines_Straight_005,
	Prototype_Mesh_SM_Vines_Straight_006,
	Prototype_Mesh_SM_Wall_Scatter_01A,
	Prototype_Mesh_SM_Wall_Scatter_01B,
	Prototype_Mesh_SM_Whiteflower_vine_01,
	Prototype_Mesh_SM_Whitevine_wall_01,
	Prototype_Mesh_SM_Whitevine_wall_01a,
	Prototype_Mesh_SM_Whitevine_wall_02,
	Prototype_Mesh_SM_Whitevine_wall_03,
	Prototype_Mesh_SM_Whitevine_wall_04a,
	Prototype_Mesh_SM_ENV_F_Mandala02_01,
	Prototype_Mesh_SM_ENV_F_Mandala02_02,
	Prototype_Mesh_SM_ENV_F_Mandala02_03,
	Prototype_Mesh_SM_ENV_F_Mandala02_04,
	Prototype_Mesh_SM_ENV_F_Mandala02_05,
	Prototype_Mesh_SM_ENV_F_MandalaCircle_01,
	Prototype_Mesh_SM_ENV_F_MandalaCircle_02,
	Prototype_Mesh_SM_ENV_F_MandalaCircle_03,
	Prototype_Mesh_SM_ENV_F_MandalaCircle_04,
	Prototype_Mesh_SM_ENV_F_MandalaCircle_05,
	Prototype_Mesh_VentQStone,

	Prototype_Mesh_DemonTree_Seg01,
	Prototype_Mesh_DemonTree_Seg02,
	Prototype_Mesh_DemonTree_Seg03,
	Prototype_Mesh_DemonTree_Seg04,
	Prototype_Mesh_DemonTree_Seg05,

	Prototype_Mesh_MandalaCircle_0,
	Prototype_Mesh_MandalaCircle_1,
	Prototype_Mesh_MandalaCircle_2,
	Prototype_Mesh_MandalaCircle_3,
	Prototype_Mesh_MandalaCircle_4,

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
	Prototype_Mesh_Monster_Wasp,
	Prototype_Mesh_Monster_WormGrub,
	Prototype_Mesh_Monster_Spider,
	Prototype_Mesh_Monster_Wolf,

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
	//MapObject
	Prototype_Mesh_FemaleStatue,


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
	Prototype_Texture_HPUI,
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
		

	case Prototype_Dissolve:
		return TEXT("Prototype_Component_Dissolve");
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
	case Prototype_Mesh_ConeMesh:
		return TEXT("ConeMesh.fbx");
		break;

	case Prototype_Mesh_JY_Tornado:
		return TEXT("JY_Tornado.fbx");
		break;
	case Prototype_Mesh_Spear_NormalEffect:
		return TEXT("Spear_NormalEffect.fbx");
		break;
	case Prototype_Mesh_Spear_UltiEffect:
		return TEXT("Spear_UltiEffect.fbx");
		break;


		
	case Prototype_Mesh_CIRCLE:
		return TEXT("circle.fbx");
		break;
	case Prototype_Mesh_CIRCLE_DIST4:
		return TEXT("circleDist4.fbx");
		break;
	case Prototype_Mesh_CIRCLE_DIST5:
		return TEXT("circleDist5.fbx");
		break;
	case Prototype_Mesh_IMPECTFX_02:
		return TEXT("SM_4E_ImpactFX_02.fbx");
		break;
	case Prototype_Mesh_RING:
		return TEXT("SM_AOG_Ring.fbx");
		break;
	case Prototype_Mesh_LOVE:
		return TEXT("SM_EM_LoveLetter_02.fbx");
		break;
	case Prototype_Mesh_WING:
		return TEXT("SM_Bow_Em_Wing_T.fbx");
		break;
	case Prototype_Mesh_BOW1:
		return TEXT("SM_Bow_Em_01.fbx");
		break;
	case Prototype_Mesh_BOW2:
		return TEXT("SM_Bow_Em_02.fbx");
		break;

	case Prototype_Mesh_ENV_BLD_Palace_02:
		return TEXT("ENV_BLD_Palace_02.fbx");
		break;
	case Prototype_Mesh_ENV_BLD_Palace_05:
		return TEXT("ENV_BLD_Palace_05.fbx");
		break;
	case Prototype_Mesh_ENV_BLD_Palace_08:
		return TEXT("ENV_BLD_Palace_08.fbx");
		break;
	case Prototype_Mesh_ENV_BLD_Palace_11:
		return TEXT("ENV_BLD_Palace_11.fbx");
		break;
	case Prototype_Mesh_ENV_BLD_Palace_16:
		return TEXT("ENV_BLD_Palace_16.fbx");
		break;
	case Prototype_Mesh_ENV_BLD_Window_Pane_02:
		return TEXT("ENV_BLD_Window_Pane_02.fbx");
		break;
	case Prototype_Mesh_ENV_F_DecorativePillar_02:
		return TEXT("ENV_F_DecorativePillar_02.fbx");
		break;
	case Prototype_Mesh_F_Vinestest:
		return TEXT("F_Vinestest.fbx");
		break;
	case Prototype_Mesh_PillarPlatforming:
		return TEXT("PillarPlatforming.fbx");
		break;
	case Prototype_Mesh_PillarPlatforming_DM:
		return TEXT("PillarPlatforming_DM.fbx");
		break;
	case Prototype_Mesh_SM_Arrow:
		return TEXT("SM_Arrow.fbx");
		break;
	case Prototype_Mesh_SM_Arrow1:
		return TEXT("SM_Arrow1.fbx");
		break;
	case Prototype_Mesh_SM_BG_Mountain1:
		return TEXT("SM_BG_Mountain1.fbx");
		break;
	case Prototype_Mesh_SM_BG_Mountain2:
		return TEXT("SM_BG_Mountain2.fbx");
		break;
	case Prototype_Mesh_SM_BG_Mountain3:
		return TEXT("SM_BG_Mountain3.fbx");
		break;
	case Prototype_Mesh_SM_BLD_Arch_Base:
		return TEXT("SM_BLD_Arch_Base.fbx");
		break;
	case Prototype_Mesh_SM_BLD_Arch_Stairs:
		return TEXT("SM_BLD_Arch_Stairs.fbx");
		break;
	case Prototype_Mesh_SM_BLD_ArchStairs_03:
		return TEXT("SM_BLD_ArchStairs_03.fbx");
		break;
	case Prototype_Mesh_SM_BLD_BridgeMiddle01:
		return TEXT("SM_BLD_BridgeMiddle01.fbx");
		break;
	case Prototype_Mesh_SM_BLD_BridgeMiddle03:
		return TEXT("SM_BLD_BridgeMiddle03.fbx");
		break;
	case Prototype_Mesh_SM_BLD_BrokenArch_01:
		return TEXT("SM_BLD_BrokenArch_01.fbx");
		break;
	case Prototype_Mesh_SM_BLD_BrokenArch_03:
		return TEXT("SM_BLD_BrokenArch_03.fbx");
		break;
	case Prototype_Mesh_SM_BLD_BrokenArch_04:
		return TEXT("SM_BLD_BrokenArch_04.fbx");
		break;
	case Prototype_Mesh_SM_BLD_CC_TemplePainting_01:
		return TEXT("SM_BLD_CC_TemplePainting_01.fbx");
		break;
	case Prototype_Mesh_SM_BLD_CC_TemplePainting_03:
		return TEXT("SM_BLD_CC_TemplePainting_03.fbx");
		break;
	case Prototype_Mesh_SM_BLD_CC_TemplePainting_04:
		return TEXT("SM_BLD_CC_TemplePainting_04.fbx");
		break;
	case Prototype_Mesh_SM_BLD_CombatArena01:
		return TEXT("SM_BLD_CombatArena01.fbx");
		break;
	case Prototype_Mesh_SM_BLD_DB_WoodenPlank:
		return TEXT("SM_BLD_DB_WoodenPlank.fbx");
		break;
	case Prototype_Mesh_SM_BLD_DBPillar01:
		return TEXT("SM_BLD_DBPillar01.fbx");
		break;
	case Prototype_Mesh_SM_BLD_DBPillar02:
		return TEXT("SM_BLD_DBPillar02.fbx");
		break;
	case Prototype_Mesh_SM_BLD_DBPillar04:
		return TEXT("SM_BLD_DBPillar04.fbx");
		break;
	case Prototype_Mesh_SM_BLD_DBSculpture:
		return TEXT("SM_BLD_DBSculpture.fbx");
		break;
	case Prototype_Mesh_SM_BLD_DiyaBridge:
		return TEXT("SM_BLD_DiyaBridge.fbx");
		break;
	case Prototype_Mesh_SM_BLD_Drawbridge:
		return TEXT("SM_BLD_Drawbridge.fbx");
		break;
	case Prototype_Mesh_SM_BLD_DS_BrokenArch_01:
		return TEXT("SM_BLD_DS_BrokenArch_01.fbx");
		break;
	case Prototype_Mesh_SM_BLD_DS_BrokenArch_02:
		return TEXT("SM_BLD_DS_BrokenArch_02.fbx");
		break;
	case Prototype_Mesh_SM_BLD_DS_BrokenArch_03:
		return TEXT("SM_BLD_DS_BrokenArch_03.fbx");
		break;
	case Prototype_Mesh_SM_BLD_DS_SpiralPlatform_01:
		return TEXT("SM_BLD_DS_SpiralPlatform_01.fbx");
		break;
	case Prototype_Mesh_SM_BLD_F_Column_B_02:
		return TEXT("SM_BLD_F_Column_B_02.fbx");
		break;
	case Prototype_Mesh_SM_BLD_F_Palace_Arch_01:
		return TEXT("SM_BLD_F_Palace_Arch_01.fbx");
		break;
	case Prototype_Mesh_SM_BLD_F_Palace_Block_01:
		return TEXT("SM_BLD_F_Palace_Block_01.fbx");
		break;
	case Prototype_Mesh_SM_BLD_F_Palace_Block_02:
		return TEXT("SM_BLD_F_Palace_Block_02.fbx");
		break;
	case Prototype_Mesh_SM_BLD_F_Palace_Column_01:
		return TEXT("SM_BLD_F_Palace_Column_01.fbx");
		break;
	case Prototype_Mesh_SM_BLD_F_Palace_Column_02:
		return TEXT("SM_BLD_F_Palace_Column_02.fbx");
		break;
	case Prototype_Mesh_SM_BLD_F_Palace_Column_03:
		return TEXT("SM_BLD_F_Palace_Column_03.fbx");
		break;
	case Prototype_Mesh_SM_BLD_F_Palace_Column_04:
		return TEXT("SM_BLD_F_Palace_Column_04.fbx");
		break;
	case Prototype_Mesh_SM_BLD_F_Palace_Door_02:
		return TEXT("SM_BLD_F_Palace_Door_02.fbx");
		break;
	case Prototype_Mesh_SM_BLD_F_Palace_Door_Arch_01:
		return TEXT("SM_BLD_F_Palace_Door_Arch_01.fbx");
		break;
	case Prototype_Mesh_SM_BLD_F_Palace_STAIRS_01:
		return TEXT("SM_BLD_F_Palace_STAIRS_01.fbx");
		break;
	case Prototype_Mesh_SM_BLD_F_Palace_TILE_01:
		return TEXT("SM_BLD_F_Palace_TILE_01.fbx");
		break;
	case Prototype_Mesh_SM_BLD_F_Palace_WALL_01:
		return TEXT("SM_BLD_F_Palace_WALL_01.fbx");
		break;
	case Prototype_Mesh_SM_BLD_F_Palace_Window_LG_01:
		return TEXT("SM_BLD_F_Palace_Window_LG_01.fbx");
		break;
	case Prototype_Mesh_SM_BLD_F_Palace_Window_LG_02:
		return TEXT("SM_BLD_F_Palace_Window_LG_02.fbx");
		break;
	case Prototype_Mesh_SM_BLD_F_Palace_Window_SM_01:
		return TEXT("SM_BLD_F_Palace_Window_SM_01.fbx");
		break;
	case Prototype_Mesh_SM_BLD_Lion_Statue:
		return TEXT("SM_BLD_Lion_Statue.fbx");
		break;
	case Prototype_Mesh_SM_BLD_StraightWall02:
		return TEXT("SM_BLD_StraightWall02.fbx");
		break;
	case Prototype_Mesh_SM_BLD_StraightWall06:
		return TEXT("SM_BLD_StraightWall06.fbx");
		break;
	case Prototype_Mesh_SM_BLD_SV_PalaceBig_02:
		return TEXT("SM_BLD_SV_PalaceBig_02.fbx");
		break;
	case Prototype_Mesh_SM_BLD_Temple_Arch:
		return TEXT("SM_BLD_Temple_Arch.fbx");
		break;
	case Prototype_Mesh_SM_BLD_TempleEnt:
		return TEXT("SM_BLD_TempleEnt.fbx");
		break;
	case Prototype_Mesh_SM_BLD_Tower02:
		return TEXT("SM_BLD_Tower02.fbx");
		break;
	case Prototype_Mesh_SM_BLD_TrishulGrant03:
		return TEXT("SM_BLD_TrishulGrant03.fbx");
		break;
	case Prototype_Mesh_SM_BouVines_01:
		return TEXT("SM_BouVines_01.fbx");
		break;
	case Prototype_Mesh_SM_boxbush_01:
		return TEXT("SM_boxbush_01.fbx");
		break;
	case Prototype_Mesh_SM_Branch_Face_01:
		return TEXT("SM_Branch_Face_01.fbx");
		break;
	case Prototype_Mesh_SM_Branch_Face_02:
		return TEXT("SM_Branch_Face_02.fbx");
		break;
	case Prototype_Mesh_SM_Branch_LG_01:
		return TEXT("SM_Branch_LG_01.fbx");
		break;
	case Prototype_Mesh_SM_Branch_LG_02:
		return TEXT("SM_Branch_LG_02.fbx");
		break;
	case Prototype_Mesh_SM_Branch_LG_03:
		return TEXT("SM_Branch_LG_03.fbx");
		break;
	case Prototype_Mesh_SM_Branch_LG_04:
		return TEXT("SM_Branch_LG_04.fbx");
		break;
	case Prototype_Mesh_SM_Branch_LG_05:
		return TEXT("SM_Branch_LG_05.fbx");
		break;
	case Prototype_Mesh_SM_Branch_Piece_02:
		return TEXT("SM_Branch_Piece_02.fbx");
		break;
	case Prototype_Mesh_SM_Branch_SM_01:
		return TEXT("SM_Branch_SM_01.fbx");
		break;
	case Prototype_Mesh_SM_bri_decal_08:
		return TEXT("SM_bri_decal_08.fbx");
		break;
	case Prototype_Mesh_SM_Bridge_Chainhook:
		return TEXT("SM_Bridge_Chainhook.fbx");
		break;
	case Prototype_Mesh_SM_Broken_Branch_01:
		return TEXT("SM_Broken_Branch_01.fbx");
		break;
	case Prototype_Mesh_SM_Broken_Branch_02:
		return TEXT("SM_Broken_Branch_02.fbx");
		break;
	case Prototype_Mesh_SM_Broken_Branch_03:
		return TEXT("SM_Broken_Branch_03.fbx");
		break;
	case Prototype_Mesh_SM_Broken_Branch_04:
		return TEXT("SM_Broken_Branch_04.fbx");
		break;
	case Prototype_Mesh_SM_CC_archi_merg_01:
		return TEXT("SM_CC_archi_merg_01.fbx");
		break;
	case Prototype_Mesh_SM_CC_archi_merg_02:
		return TEXT("SM_CC_archi_merg_02.fbx");
		break;
	case Prototype_Mesh_SM_CC_straightboxwall_01:
		return TEXT("SM_CC_straightboxwall_01.fbx");
		break;
	case Prototype_Mesh_SM_CC_straightboxwall_02:
		return TEXT("SM_CC_straightboxwall_02.fbx");
		break;
	case Prototype_Mesh_SM_CH_Gadasura_Black_Mask_01:
		return TEXT("SM_CH_Gadasura_Black_Mask_01.fbx");
		break;
	case Prototype_Mesh_SM_CH_Gadasura_Rage_Mask_01:
		return TEXT("SM_CH_Gadasura_Rage_Mask_01.fbx");
		break;
	case Prototype_Mesh_SM_CH_Mahinasura_Leader_Mask_01:
		return TEXT("SM_CH_Mahinasura_Leader_Mask_01.fbx");
		break;
	case Prototype_Mesh_SM_CH_Mahinasura_Leader_Stinger_01:
		return TEXT("SM_CH_Mahinasura_Leader_Stinger_01.fbx");
		break;
	case Prototype_Mesh_SM_CH_Mahinasura_Leader_Tail_01:
		return TEXT("SM_CH_Mahinasura_Leader_Tail_01.fbx");
		break;
	case Prototype_Mesh_SM_CH_Mahinasura_Minion_Mask_01:
		return TEXT("SM_CH_Mahinasura_Minion_Mask_01.fbx");
		break;
	case Prototype_Mesh_SM_CH_Tezabsura_Kamikaze_Mask_01:
		return TEXT("SM_CH_Tezabsura_Kamikaze_Mask_01.fbx");
		break;
	case Prototype_Mesh_SM_CH_Tezabsura_Landmine_Mask_01:
		return TEXT("SM_CH_Tezabsura_Landmine_Mask_01.fbx");
		break;
	case Prototype_Mesh_SM_CH_Vayusura_Mask_01:
		return TEXT("SM_CH_Vayusura_Mask_01.fbx");
		break;
	case Prototype_Mesh_SM_CH_Vayusura_Mask_02:
		return TEXT("SM_CH_Vayusura_Mask_02.fbx");
		break;
	case Prototype_Mesh_SM_circular_plane:
		return TEXT("SM_circular_plane.fbx");
		break;
	case Prototype_Mesh_SM_Crack_Decal_Ground_01:
		return TEXT("SM_Crack_Decal_Ground_01.fbx");
		break;
	case Prototype_Mesh_SM_Crack_Decal_Ground_03:
		return TEXT("SM_Crack_Decal_Ground_03.fbx");
		break;
	case Prototype_Mesh_SM_Crack_Decal_Ground_05:
		return TEXT("SM_Crack_Decal_Ground_05.fbx");
		break;
	case Prototype_Mesh_SM_Crack_Decal_Ground_06:
		return TEXT("SM_Crack_Decal_Ground_06.fbx");
		break;
	case Prototype_Mesh_SM_D_BLD_PalaceArch_01:
		return TEXT("SM_D_BLD_PalaceArch_01.fbx");
		break;
	case Prototype_Mesh_SM_D_BLD_PalaceCOL_01:
		return TEXT("SM_D_BLD_PalaceCOL_01.fbx");
		break;
	case Prototype_Mesh_SM_D_BLD_PalaceCOL_02:
		return TEXT("SM_D_BLD_PalaceCOL_02.fbx");
		break;
	case Prototype_Mesh_SM_D_BLD_PalaceENT_01:
		return TEXT("SM_D_BLD_PalaceENT_01.fbx");
		break;
	case Prototype_Mesh_SM_D_BLD_PalaceRFCORNER_01:
		return TEXT("SM_D_BLD_PalaceRFCORNER_01.fbx");
		break;
	case Prototype_Mesh_SM_D_BLD_PalaceRFLEDGE_01:
		return TEXT("SM_D_BLD_PalaceRFLEDGE_01.fbx");
		break;
	case Prototype_Mesh_SM_D_BLD_PalaceRFSEG_01:
		return TEXT("SM_D_BLD_PalaceRFSEG_01.fbx");
		break;
	case Prototype_Mesh_SM_D_BLD_PalaceWIN_01:
		return TEXT("SM_D_BLD_PalaceWIN_01.fbx");
		break;
	case Prototype_Mesh_SM_Damage_Decal_01:
		return TEXT("SM_Damage_Decal_01.fbx");
		break;
	case Prototype_Mesh_SM_Damage_Decal_02:
		return TEXT("SM_Damage_Decal_02.fbx");
		break;
	case Prototype_Mesh_SM_Damage_Decal_03:
		return TEXT("SM_Damage_Decal_03.fbx");
		break;
	case Prototype_Mesh_SM_Debris_01:
		return TEXT("SM_Debris_01.fbx");
		break;
	case Prototype_Mesh_SM_Debris_02:
		return TEXT("SM_Debris_02.fbx");
		break;
	case Prototype_Mesh_SM_Debris_03:
		return TEXT("SM_Debris_03.fbx");
		break;
	case Prototype_Mesh_SM_Debris_04:
		return TEXT("SM_Debris_04.fbx");
		break;
	case Prototype_Mesh_SM_Demon_Tree_Seg_01:
		return TEXT("SM_Demon_Tree_Seg_01.fbx");
		break;
	case Prototype_Mesh_SM_Demon_Tree_Seg_02:
		return TEXT("SM_Demon_Tree_Seg_02.fbx");
		break;
	case Prototype_Mesh_SM_Demon_Tree_Seg_03:
		return TEXT("SM_Demon_Tree_Seg_03.fbx");
		break;
	case Prototype_Mesh_SM_Demon_Tree_Seg_04:
		return TEXT("SM_Demon_Tree_Seg_04.fbx");
		break;
	case Prototype_Mesh_SM_Demon_Tree_Seg_05:
		return TEXT("SM_Demon_Tree_Seg_05.fbx");
		break;
	case Prototype_Mesh_SM_DemonT_Creeper_01:
		return TEXT("SM_DemonT_Creeper_01.fbx");
		break;
	case Prototype_Mesh_SM_DemonT_Creeper_02:
		return TEXT("SM_DemonT_Creeper_02.fbx");
		break;
	case Prototype_Mesh_SM_DemonT_Creeper_03:
		return TEXT("SM_DemonT_Creeper_03.fbx");
		break;
	case Prototype_Mesh_SM_DiyaSmall_01:
		return TEXT("SM_DiyaSmall_01.fbx");
		break;
	case Prototype_Mesh_SM_Dry_Green_Bush_01:
		return TEXT("SM_Dry_Green_Bush_01.fbx");
		break;
	case Prototype_Mesh_SM_Dry_Green_Bush_02:
		return TEXT("SM_Dry_Green_Bush_02.fbx");
		break;
	case Prototype_Mesh_SM_Dry_Green_Bush_03:
		return TEXT("SM_Dry_Green_Bush_03.fbx");
		break;
	case Prototype_Mesh_SM_Dry_Stack_01:
		return TEXT("SM_Dry_Stack_01.fbx");
		break;
	case Prototype_Mesh_SM_Dry_Stack_02:
		return TEXT("SM_Dry_Stack_02.fbx");
		break;
	case Prototype_Mesh_SM_Dry_Stack_03:
		return TEXT("SM_Dry_Stack_03.fbx");
		break;
	case Prototype_Mesh_SM_DryLeaves_Clutter_01:
		return TEXT("SM_DryLeaves_Clutter_01.fbx");
		break;
	case Prototype_Mesh_SM_DryLeaves_Clutter_02:
		return TEXT("SM_DryLeaves_Clutter_02.fbx");
		break;
	case Prototype_Mesh_SM_DryLeaves_Clutter_03:
		return TEXT("SM_DryLeaves_Clutter_03.fbx");
		break;
	case Prototype_Mesh_SM_DryLeaves_Clutter_04:
		return TEXT("SM_DryLeaves_Clutter_04.fbx");
		break;
	case Prototype_Mesh_SM_DryLeaves_Clutter_05:
		return TEXT("SM_DryLeaves_Clutter_05.fbx");
		break;
	case Prototype_Mesh_SM_Dune_01:
		return TEXT("SM_Dune_01.fbx");
		break;
	case Prototype_Mesh_SM_Dune_02:
		return TEXT("SM_Dune_02.fbx");
		break;
	case Prototype_Mesh_SM_Dune_03:
		return TEXT("SM_Dune_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_BLD_TopFloor_Wall_01:
		return TEXT("SM_ENV_BLD_TopFloor_Wall_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_BLD_TopFloor_Wall_02:
		return TEXT("SM_ENV_BLD_TopFloor_Wall_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Arch_B:
		return TEXT("SM_ENV_CC_Arch_B.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Arch_Base_01:
		return TEXT("SM_ENV_CC_Arch_Base_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Arch_C:
		return TEXT("SM_ENV_CC_Arch_C.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Arch_D:
		return TEXT("SM_ENV_CC_Arch_D.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Boat_01:
		return TEXT("SM_ENV_CC_Boat_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Border_01:
		return TEXT("SM_ENV_CC_Border_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Border_02:
		return TEXT("SM_ENV_CC_Border_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Border_03:
		return TEXT("SM_ENV_CC_Border_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Border_05:
		return TEXT("SM_ENV_CC_Border_05.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Bridge_Arch:
		return TEXT("SM_ENV_CC_Bridge_Arch.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Bridge_Pillar_03:
		return TEXT("SM_ENV_CC_Bridge_Pillar_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_CogLarge_01:
		return TEXT("SM_ENV_CC_CogLarge_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_CogLarge_02:
		return TEXT("SM_ENV_CC_CogLarge_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_CogLarge_03:
		return TEXT("SM_ENV_CC_CogLarge_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_CogLarge_04:
		return TEXT("SM_ENV_CC_CogLarge_04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_CogMedium_01:
		return TEXT("SM_ENV_CC_CogMedium_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_CogMedium_02:
		return TEXT("SM_ENV_CC_CogMedium_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_CogMedium_03:
		return TEXT("SM_ENV_CC_CogMedium_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_CogMedium_04:
		return TEXT("SM_ENV_CC_CogMedium_04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_CogMedium_05:
		return TEXT("SM_ENV_CC_CogMedium_05.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_CogSmall_01:
		return TEXT("SM_ENV_CC_CogSmall_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_CogTiny_01:
		return TEXT("SM_ENV_CC_CogTiny_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Diya_02:
		return TEXT("SM_ENV_CC_Diya_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_DockPlatform_01:
		return TEXT("SM_ENV_CC_DockPlatform_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Dome_A:
		return TEXT("SM_ENV_CC_Dome_A.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Dome_B:
		return TEXT("SM_ENV_CC_Dome_B.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Dome_C:
		return TEXT("SM_ENV_CC_Dome_C.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Dome_D:
		return TEXT("SM_ENV_CC_Dome_D.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Dome_E:
		return TEXT("SM_ENV_CC_Dome_E.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_DoorWay_01:
		return TEXT("SM_ENV_CC_DoorWay_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Elevator_Horizontal_01:
		return TEXT("SM_ENV_CC_Elevator_Horizontal_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Elevator_Platform_01:
		return TEXT("SM_ENV_CC_Elevator_Platform_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Elevator_Platform_02:
		return TEXT("SM_ENV_CC_Elevator_Platform_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Elevator_Vertical_01:
		return TEXT("SM_ENV_CC_Elevator_Vertical_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_ElevatorHorizontal_CogTR_01:
		return TEXT("SM_ENV_CC_ElevatorHorizontal_CogTR_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_ElevatorHousing_01:
		return TEXT("SM_ENV_CC_ElevatorHousing_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_ElevatorLever_01:
		return TEXT("SM_ENV_CC_ElevatorLever_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_ElevatorLeverBase_01:
		return TEXT("SM_ENV_CC_ElevatorLeverBase_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_ElevatorRailing_Horizontal_01:
		return TEXT("SM_ENV_CC_ElevatorRailing_Horizontal_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_ElevatorRailing_Vertical_01:
		return TEXT("SM_ENV_CC_ElevatorRailing_Vertical_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_ElevatorVertical_Cog_Bottom_01:
		return TEXT("SM_ENV_CC_ElevatorVertical_Cog_Bottom_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_ElevatorVertical_Cog_Top_01:
		return TEXT("SM_ENV_CC_ElevatorVertical_Cog_Top_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Garuda_Gate:
		return TEXT("SM_ENV_CC_Garuda_Gate.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Garuda_Tower:
		return TEXT("SM_ENV_CC_Garuda_Tower.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Garuda_Tower_Balcony:
		return TEXT("SM_ENV_CC_Garuda_Tower_Balcony.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_MandalaCircle_01:
		return TEXT("SM_ENV_CC_MandalaCircle_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_MandalaCircle_02:
		return TEXT("SM_ENV_CC_MandalaCircle_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_MandalaCircle_03:
		return TEXT("SM_ENV_CC_MandalaCircle_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_MandalaCircle_04:
		return TEXT("SM_ENV_CC_MandalaCircle_04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_PaddyField_01:
		return TEXT("SM_ENV_CC_PaddyField_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_PaddyField_02:
		return TEXT("SM_ENV_CC_PaddyField_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_PaddyField_03:
		return TEXT("SM_ENV_CC_PaddyField_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_PaddyField_04:
		return TEXT("SM_ENV_CC_PaddyField_04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_PaddyField_05:
		return TEXT("SM_ENV_CC_PaddyField_05.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_PaddyField_06:
		return TEXT("SM_ENV_CC_PaddyField_06.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_PaddyField_07:
		return TEXT("SM_ENV_CC_PaddyField_07.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Panel_01:
		return TEXT("SM_ENV_CC_Panel_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Panel_03:
		return TEXT("SM_ENV_CC_Panel_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Panel_04:
		return TEXT("SM_ENV_CC_Panel_04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Panel_05:
		return TEXT("SM_ENV_CC_Panel_05.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Pillar_03:
		return TEXT("SM_ENV_CC_Pillar_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Pillar_05:
		return TEXT("SM_ENV_CC_Pillar_05.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Pillar_043:
		return TEXT("SM_ENV_CC_Pillar_043.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Platform_01:
		return TEXT("SM_ENV_CC_Platform_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Platform_02:
		return TEXT("SM_ENV_CC_Platform_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Platform_03:
		return TEXT("SM_ENV_CC_Platform_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Platform_04:
		return TEXT("SM_ENV_CC_Platform_04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Platform_06:
		return TEXT("SM_ENV_CC_Platform_06.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Platform_054_SM_ENV_CC_Platform_05:
		return TEXT("SM_ENV_CC_Platform_054_SM_ENV_CC_Platform_05.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Platform_Circular_01:
		return TEXT("SM_ENV_CC_Platform_Circular_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Platform_Circular_02:
		return TEXT("SM_ENV_CC_Platform_Circular_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Platform_Circular_low:
		return TEXT("SM_ENV_CC_Platform_Circular_low.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Platform_Circular_low01:
		return TEXT("SM_ENV_CC_Platform_Circular_low01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Puzzle_Water:
		return TEXT("SM_ENV_CC_Puzzle_Water.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_01:
		return TEXT("SM_ENV_CC_PuzzleAssembly_Right_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_02:
		return TEXT("SM_ENV_CC_PuzzleAssembly_Right_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_03:
		return TEXT("SM_ENV_CC_PuzzleAssembly_Right_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_04:
		return TEXT("SM_ENV_CC_PuzzleAssembly_Right_04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_05:
		return TEXT("SM_ENV_CC_PuzzleAssembly_Right_05.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_08:
		return TEXT("SM_ENV_CC_PuzzleAssembly_Right_08.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_10:
		return TEXT("SM_ENV_CC_PuzzleAssembly_Right_10.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_11:
		return TEXT("SM_ENV_CC_PuzzleAssembly_Right_11.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_12:
		return TEXT("SM_ENV_CC_PuzzleAssembly_Right_12.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_15:
		return TEXT("SM_ENV_CC_PuzzleAssembly_Right_15.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Railing_01:
		return TEXT("SM_ENV_CC_Railing_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Railing_02:
		return TEXT("SM_ENV_CC_Railing_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Railing_03:
		return TEXT("SM_ENV_CC_Railing_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_RangdaArena_01:
		return TEXT("SM_ENV_CC_RangdaArena_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Stairs_01:
		return TEXT("SM_ENV_CC_Stairs_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Stairs_02:
		return TEXT("SM_ENV_CC_Stairs_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Stairs_Railing:
		return TEXT("SM_ENV_CC_Stairs_Railing.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Statue:
		return TEXT("SM_ENV_CC_Statue.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_StoryPanel_02:
		return TEXT("SM_ENV_CC_StoryPanel_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_StraightWall_01:
		return TEXT("SM_ENV_CC_StraightWall_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_StraightWall_02:
		return TEXT("SM_ENV_CC_StraightWall_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_StraightWall_02_SM_ENV_CC_StraightWall_02:
		return TEXT("SM_ENV_CC_StraightWall_02_SM_ENV_CC_StraightWall_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_StraightWall_03_SM_ENV_CC_StraightWall_03:
		return TEXT("SM_ENV_CC_StraightWall_03_SM_ENV_CC_StraightWall_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_StraightWall_04:
		return TEXT("SM_ENV_CC_StraightWall_04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_StraightWall_Withwindow_SM_ENV_CC_StraightWall_WithWindow:
		return TEXT("SM_ENV_CC_StraightWall_Withwindow_SM_ENV_CC_StraightWall_WithWindow.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_StraighWall_Withwindow1:
		return TEXT("SM_ENV_CC_StraighWall_Withwindow1.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_TempleBase_01:
		return TEXT("SM_ENV_CC_TempleBase_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_TempleTop_01:
		return TEXT("SM_ENV_CC_TempleTop_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_TempleWalls_01:
		return TEXT("SM_ENV_CC_TempleWalls_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Vishnu_Statue:
		return TEXT("SM_ENV_CC_Vishnu_Statue.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_VishnuTemple_01:
		return TEXT("SM_ENV_CC_VishnuTemple_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_VishnuTemple_Base_01:
		return TEXT("SM_ENV_CC_VishnuTemple_Base_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_VishnuTemple_DoorAssembly_Left:
		return TEXT("SM_ENV_CC_VishnuTemple_DoorAssembly_Left.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_VishnuTemple_DoorAssembly_Right:
		return TEXT("SM_ENV_CC_VishnuTemple_DoorAssembly_Right.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Water_Platform_01:
		return TEXT("SM_ENV_CC_Water_Platform_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Water_Platform_02a:
		return TEXT("SM_ENV_CC_Water_Platform_02a.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Water_Platform_03:
		return TEXT("SM_ENV_CC_Water_Platform_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Water_Platform_04:
		return TEXT("SM_ENV_CC_Water_Platform_04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Water_Platform_06:
		return TEXT("SM_ENV_CC_Water_Platform_06.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Water_Platform_07:
		return TEXT("SM_ENV_CC_Water_Platform_07.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Water_Platform_10:
		return TEXT("SM_ENV_CC_Water_Platform_10.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_WaterFountain_01:
		return TEXT("SM_ENV_CC_WaterFountain_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_WaterFountain_02:
		return TEXT("SM_ENV_CC_WaterFountain_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_WaterPuzzle_Base:
		return TEXT("SM_ENV_CC_WaterPuzzle_Base.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_WaterPuzzle_Boundary_01:
		return TEXT("SM_ENV_CC_WaterPuzzle_Boundary_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_WaterPuzzle_Boundary_02:
		return TEXT("SM_ENV_CC_WaterPuzzle_Boundary_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_WaterWheel_01:
		return TEXT("SM_ENV_CC_WaterWheel_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_WaterWheel_02:
		return TEXT("SM_ENV_CC_WaterWheel_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Window_A:
		return TEXT("SM_ENV_CC_Window_A.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Window_A1:
		return TEXT("SM_ENV_CC_Window_A1.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Window_Ba:
		return TEXT("SM_ENV_CC_Window_Ba.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Window_D:
		return TEXT("SM_ENV_CC_Window_D.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Window1:
		return TEXT("SM_ENV_CC_Window1.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Wooden_Planks_03:
		return TEXT("SM_ENV_CC_Wooden_Planks_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_CC_Wooden_Planks_08:
		return TEXT("SM_ENV_CC_Wooden_Planks_08.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_BasePlatform_01:
		return TEXT("SM_ENV_D_BasePlatform_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_BossArenaArch_01:
		return TEXT("SM_ENV_D_BossArenaArch_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_BossArenaArch_02:
		return TEXT("SM_ENV_D_BossArenaArch_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_CenterPlatform_01:
		return TEXT("SM_ENV_D_CenterPlatform_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_ExitDoor_01:
		return TEXT("SM_ENV_D_ExitDoor_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_ExitPlatformTop_01:
		return TEXT("SM_ENV_D_ExitPlatformTop_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_FemaleStatue_01:
		return TEXT("SM_ENV_D_FemaleStatue_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_FemaleStatue_02:
		return TEXT("SM_ENV_D_FemaleStatue_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_FrontPlatformBottom_01:
		return TEXT("SM_ENV_D_FrontPlatformBottom_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_FrontPlatformTop_01:
		return TEXT("SM_ENV_D_FrontPlatformTop_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_GodHand_01:
		return TEXT("SM_ENV_D_GodHand_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_GroundArch_01:
		return TEXT("SM_ENV_D_GroundArch_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_GroundColumn_01:
		return TEXT("SM_ENV_D_GroundColumn_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_GroundFloorPlatform_01:
		return TEXT("SM_ENV_D_GroundFloorPlatform_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_GroundFloorPlatform_02:
		return TEXT("SM_ENV_D_GroundFloorPlatform_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_MandalaCircle_01:
		return TEXT("SM_ENV_D_MandalaCircle_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_MandalaCircle_02:
		return TEXT("SM_ENV_D_MandalaCircle_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_MandalaCircle_03:
		return TEXT("SM_ENV_D_MandalaCircle_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_MandalaCircle_04:
		return TEXT("SM_ENV_D_MandalaCircle_04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_MandalaCircle_05:
		return TEXT("SM_ENV_D_MandalaCircle_05.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_OuterWall_01:
		return TEXT("SM_ENV_D_OuterWall_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_OuterWall_02:
		return TEXT("SM_ENV_D_OuterWall_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_OuterWall_03:
		return TEXT("SM_ENV_D_OuterWall_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_OuterWall_04:
		return TEXT("SM_ENV_D_OuterWall_04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_OuterWall_05:
		return TEXT("SM_ENV_D_OuterWall_05.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_OuterWall_06:
		return TEXT("SM_ENV_D_OuterWall_06.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_OuterWall_07:
		return TEXT("SM_ENV_D_OuterWall_07.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_OuterWall_08:
		return TEXT("SM_ENV_D_OuterWall_08.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_Roof_01:
		return TEXT("SM_ENV_D_Roof_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_SidePlatformTop_01:
		return TEXT("SM_ENV_D_SidePlatformTop_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_SoldierStatue_01:
		return TEXT("SM_ENV_D_SoldierStatue_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_SoldierStatue_02:
		return TEXT("SM_ENV_D_SoldierStatue_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_StepwellStairs_01:
		return TEXT("SM_ENV_D_StepwellStairs_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_StepwellStairs_Half_01:
		return TEXT("SM_ENV_D_StepwellStairs_Half_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_StepwellWater_01:
		return TEXT("SM_ENV_D_StepwellWater_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_TombBase_01:
		return TEXT("SM_ENV_D_TombBase_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_D_Window_01:
		return TEXT("SM_ENV_D_Window_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DS_Platform_28:
		return TEXT("SM_ENV_DS_Platform_28.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_BackdropTemple_01:
		return TEXT("SM_ENV_DT_BackdropTemple_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_BellSpline_01:
		return TEXT("SM_ENV_DT_BellSpline_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_Bull:
		return TEXT("SM_ENV_DT_Bull.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_BullPlatform_01:
		return TEXT("SM_ENV_DT_BullPlatform_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_BullPlatform_Broken_01:
		return TEXT("SM_ENV_DT_BullPlatform_Broken_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_Diya_02_01:
		return TEXT("SM_ENV_DT_Diya_02_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_DurgaBase_01:
		return TEXT("SM_ENV_DT_DurgaBase_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_DurgaBase_02:
		return TEXT("SM_ENV_DT_DurgaBase_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_DurgaBaseStep_01:
		return TEXT("SM_ENV_DT_DurgaBaseStep_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_DurgaBaseStep_02:
		return TEXT("SM_ENV_DT_DurgaBaseStep_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_LionEntrance_01:
		return TEXT("SM_ENV_DT_LionEntrance_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_LionEntranceSplit_01:
		return TEXT("SM_ENV_DT_LionEntranceSplit_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_Mountain_02:
		return TEXT("SM_ENV_DT_Mountain_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_Mountain_06:
		return TEXT("SM_ENV_DT_Mountain_06.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_Mountain_08:
		return TEXT("SM_ENV_DT_Mountain_08.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_PillarLarge_01:
		return TEXT("SM_ENV_DT_PillarLarge_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_PillarLarge_02:
		return TEXT("SM_ENV_DT_PillarLarge_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_Platform01:
		return TEXT("SM_ENV_DT_Platform01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_Platform02_2:
		return TEXT("SM_ENV_DT_Platform02_2.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_Platform03:
		return TEXT("SM_ENV_DT_Platform03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_Platform04_2:
		return TEXT("SM_ENV_DT_Platform04_2.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_Platform05:
		return TEXT("SM_ENV_DT_Platform05.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_Platform06_Base:
		return TEXT("SM_ENV_DT_Platform06_Base.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_Platform06_StatueBase:
		return TEXT("SM_ENV_DT_Platform06_StatueBase.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_Stairs_01_2:
		return TEXT("SM_ENV_DT_Stairs_01_2.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_StatueBase_02:
		return TEXT("SM_ENV_DT_StatueBase_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_StatueBase_03:
		return TEXT("SM_ENV_DT_StatueBase_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_StatueFemale_Pose01:
		return TEXT("SM_ENV_DT_StatueFemale_Pose01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_StatueFemale_Pose02:
		return TEXT("SM_ENV_DT_StatueFemale_Pose02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_StatueFemale_Pose03:
		return TEXT("SM_ENV_DT_StatueFemale_Pose03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_StatueFemale_Pose04:
		return TEXT("SM_ENV_DT_StatueFemale_Pose04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_StatuePillar_01:
		return TEXT("SM_ENV_DT_StatuePillar_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_StatuePillar_02:
		return TEXT("SM_ENV_DT_StatuePillar_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_TempleBase_01:
		return TEXT("SM_ENV_DT_TempleBase_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_TempleBase_02:
		return TEXT("SM_ENV_DT_TempleBase_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_DT_TempleBell_03:
		return TEXT("SM_ENV_DT_TempleBell_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_3_Decorative_Piece_01:
		return TEXT("SM_ENV_F_3_Decorative_Piece_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_3_Window_02:
		return TEXT("SM_ENV_F_3_Window_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_3_Window_04:
		return TEXT("SM_ENV_F_3_Window_04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_3_Window_06:
		return TEXT("SM_ENV_F_3_Window_06.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_3_WindowArch_01:
		return TEXT("SM_ENV_F_3_WindowArch_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_3_WindowArch_02:
		return TEXT("SM_ENV_F_3_WindowArch_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Arch_01:
		return TEXT("SM_ENV_F_Arch_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Arch_02:
		return TEXT("SM_ENV_F_Arch_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Balcony_01:
		return TEXT("SM_ENV_F_Balcony_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Balcony_1B:
		return TEXT("SM_ENV_F_Balcony_1B.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Balcony_1C:
		return TEXT("SM_ENV_F_Balcony_1C.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Balcony_1D:
		return TEXT("SM_ENV_F_Balcony_1D.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Balcony_1E:
		return TEXT("SM_ENV_F_Balcony_1E.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Balcony_02:
		return TEXT("SM_ENV_F_Balcony_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Balcony_03:
		return TEXT("SM_ENV_F_Balcony_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Bow_Platform_03:
		return TEXT("SM_ENV_F_Bow_Platform_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Bow_Platform_04:
		return TEXT("SM_ENV_F_Bow_Platform_04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Building_01:
		return TEXT("SM_ENV_F_Building_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Chatri_Base:
		return TEXT("SM_ENV_F_Chatri_Base.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Corner_Wall_01:
		return TEXT("SM_ENV_F_Corner_Wall_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Corner_Wall_02:
		return TEXT("SM_ENV_F_Corner_Wall_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Corner_Wall_05:
		return TEXT("SM_ENV_F_Corner_Wall_05.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Defences_01:
		return TEXT("SM_ENV_F_Defences_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Defences_02:
		return TEXT("SM_ENV_F_Defences_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Defences_02B:
		return TEXT("SM_ENV_F_Defences_02B.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Dome_05:
		return TEXT("SM_ENV_F_Dome_05.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Dome_06:
		return TEXT("SM_ENV_F_Dome_06.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Dome_08:
		return TEXT("SM_ENV_F_Dome_08.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Dome_09:
		return TEXT("SM_ENV_F_Dome_09.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Dome_014:
		return TEXT("SM_ENV_F_Dome_014.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Door_Damaged_S_01:
		return TEXT("SM_ENV_F_Door_Damaged_S_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Door_Damaged_S_02:
		return TEXT("SM_ENV_F_Door_Damaged_S_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Door_NS_01:
		return TEXT("SM_ENV_F_Door_NS_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Door_NS_02:
		return TEXT("SM_ENV_F_Door_NS_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Door_S_01:
		return TEXT("SM_ENV_F_Door_S_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Door_S_02:
		return TEXT("SM_ENV_F_Door_S_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_DoorWay_01:
		return TEXT("SM_ENV_F_DoorWay_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_DoorWayAbandoned_02:
		return TEXT("SM_ENV_F_DoorWayAbandoned_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_DurgaAvatar_Temple_01:
		return TEXT("SM_ENV_F_DurgaAvatar_Temple_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_DurgaAvatarTemple_Floor_01:
		return TEXT("SM_ENV_F_DurgaAvatarTemple_Floor_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_FirstFloorEntrance_01:
		return TEXT("SM_ENV_F_FirstFloorEntrance_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_FirstFloorTile_01:
		return TEXT("SM_ENV_F_FirstFloorTile_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_FirstFloorTile_02:
		return TEXT("SM_ENV_F_FirstFloorTile_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_FirstFloorTile_03:
		return TEXT("SM_ENV_F_FirstFloorTile_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_FirstFloorTile_04:
		return TEXT("SM_ENV_F_FirstFloorTile_04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_FirstFloorTile_05:
		return TEXT("SM_ENV_F_FirstFloorTile_05.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_FirstFloorTile_06:
		return TEXT("SM_ENV_F_FirstFloorTile_06.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_FloatingWall_01:
		return TEXT("SM_ENV_F_FloatingWall_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_FloatingWall_02:
		return TEXT("SM_ENV_F_FloatingWall_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_FloatingWall_03:
		return TEXT("SM_ENV_F_FloatingWall_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_FloatingWall_Trim_07:
		return TEXT("SM_ENV_F_FloatingWall_Trim_07.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Floor_01_2:
		return TEXT("SM_ENV_F_Floor_01_2.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Fort_Pathway_01:
		return TEXT("SM_ENV_F_Fort_Pathway_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Fort_Pathway_02:
		return TEXT("SM_ENV_F_Fort_Pathway_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Fort_Pathway_03:
		return TEXT("SM_ENV_F_Fort_Pathway_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Fort_Pathway_04:
		return TEXT("SM_ENV_F_Fort_Pathway_04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_FortBridge_Broken_01:
		return TEXT("SM_ENV_F_FortBridge_Broken_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_FortBridge_Broken_02:
		return TEXT("SM_ENV_F_FortBridge_Broken_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_FortBridge_Broken_03:
		return TEXT("SM_ENV_F_FortBridge_Broken_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_FortBridge_Broken_04:
		return TEXT("SM_ENV_F_FortBridge_Broken_04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_FortCobbleArena_01:
		return TEXT("SM_ENV_F_FortCobbleArena_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_FortWALL_01b:
		return TEXT("SM_ENV_F_FortWALL_01b.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_FortWALL_01d:
		return TEXT("SM_ENV_F_FortWALL_01d.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_FortWALL_05:
		return TEXT("SM_ENV_F_FortWALL_05.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Frame_01_a_int:
		return TEXT("SM_ENV_F_Frame_01_a_int.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Frame_01_int:
		return TEXT("SM_ENV_F_Frame_01_int.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_FrameClosed_01:
		return TEXT("SM_ENV_F_FrameClosed_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_HighWall_01:
		return TEXT("SM_ENV_F_HighWall_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Mystic_Creeper_06:
		return TEXT("SM_ENV_F_Mandala02_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Mystic_Creeper_07:
		return TEXT("SM_ENV_F_Mandala02_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Mystic_Creeper_09:
		return TEXT("SM_ENV_F_Mandala02_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_PlatFORM_02:
		return TEXT("SM_ENV_F_Mandala02_04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_PlatFORM_03:
		return TEXT("SM_ENV_F_Mandala02_05.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_PlatFORM_05:
		return TEXT("SM_ENV_F_MandalaCircle_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_PlatFORM_07:
		return TEXT("SM_ENV_F_MandalaCircle_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_PlatFORM_08:
		return TEXT("SM_ENV_F_MandalaCircle_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_PlatFORM_09A:
		return TEXT("SM_ENV_F_MandalaCircle_04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_PlatFORM_09B:
		return TEXT("SM_ENV_F_MandalaCircle_05.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_PlatFORM_09C:
		return TEXT("SM_ENV_F_Mystic_Creeper_06.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_PlatFORM_09D:
		return TEXT("SM_ENV_F_Mystic_Creeper_07.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_PlatFORM_09E:
		return TEXT("SM_ENV_F_Mystic_Creeper_09.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Platform_Wall_01:
		return TEXT("SM_ENV_F_PlatFORM_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Rani_Platform_05:
		return TEXT("SM_ENV_F_PlatFORM_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Roof_01:
		return TEXT("SM_ENV_F_PlatFORM_05.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_RoofWall_Assembled_01:
		return TEXT("SM_ENV_F_PlatFORM_07.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_RoofWall_Assembled_02a:
		return TEXT("SM_ENV_F_PlatFORM_08.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_RoofWall_Single:
		return TEXT("SM_ENV_F_PlatFORM_09A.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_RoofWall_Single_02:
		return TEXT("SM_ENV_F_PlatFORM_09B.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Stairs_02:
		return TEXT("SM_ENV_F_PlatFORM_09C.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Stairs_04:
		return TEXT("SM_ENV_F_PlatFORM_09D.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Stairs_Railing_01:
		return TEXT("SM_ENV_F_PlatFORM_09E.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_StatueBase_03:
		return TEXT("SM_ENV_F_Platform_Wall_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_StoryPanel_01:
		return TEXT("SM_ENV_F_Rani_Platform_05.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Tez_Balcony:
		return TEXT("SM_ENV_F_Roof_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Tower_01:
		return TEXT("SM_ENV_F_RoofWall_Assembled_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Tower_02:
		return TEXT("SM_ENV_F_RoofWall_Assembled_02a.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Tower_06:
		return TEXT("SM_ENV_F_RoofWall_Single.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Tower_07:
		return TEXT("SM_ENV_F_RoofWall_Single_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Tower_08:
		return TEXT("SM_ENV_F_Stairs_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_ViewPlatform_01:
		return TEXT("SM_ENV_F_Stairs_04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Wall_01:
		return TEXT("SM_ENV_F_Stairs_Railing_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Wall_Corner_Holes_02:
		return TEXT("SM_ENV_F_StatueBase_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Wall_Holes_01:
		return TEXT("SM_ENV_F_StoryPanel_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Wall_Holes_02:
		return TEXT("SM_ENV_F_Tez_Balcony.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Wall_Holes_Half_01:
		return TEXT("SM_ENV_F_Tower_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Wall_Holes_Turret_01:
		return TEXT("SM_ENV_F_Tower_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Wall_Holes_Turret_02:
		return TEXT("SM_ENV_F_Tower_06.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Wall_Solid_01:
		return TEXT("SM_ENV_F_Tower_07.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Wall_Solid_02:
		return TEXT("SM_ENV_F_Tower_08.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Wall_Step_01:
		return TEXT("SM_ENV_F_ViewPlatform_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_WallTrim_02:
		return TEXT("SM_ENV_F_Wall_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Window_Pane_White:
		return TEXT("SM_ENV_F_Wall_Corner_Holes_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_FemaleStatue_Pose_04:
		return TEXT("SM_ENV_F_Wall_Holes_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_FemaleStatue_Pose_06:
		return TEXT("SM_ENV_F_Wall_Holes_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_PRP_PillarStatuesBroken_01:
		return TEXT("SM_ENV_F_Wall_Holes_Half_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_PRP_PillarStatuesBroken_02:
		return TEXT("SM_ENV_F_Wall_Holes_Turret_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_PRP_PillarStatuesBroken_04:
		return TEXT("SM_ENV_F_Wall_Holes_Turret_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_PRP_PillarStatuesBroken_05:
		return TEXT("SM_ENV_F_Wall_Solid_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_PRP_PillarStatuesBroken_06:
		return TEXT("SM_ENV_F_Wall_Solid_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_T_Creeper_01:
		return TEXT("SM_ENV_F_Wall_Step_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_T_DemonT_Roots_04:
		return TEXT("SM_ENV_F_WallTrim_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_T_DemonT_Roots_05:
		return TEXT("SM_ENV_F_Window_Pane_White.fbx");
		break;
	case Prototype_Mesh_SM_ENV_T_HangingPlant_01:
		return TEXT("SM_ENV_FemaleStatue_Pose_04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_T_TetrisBase_01:
		return TEXT("SM_ENV_FemaleStatue_Pose_06.fbx");
		break;
	case Prototype_Mesh_SM_ENV_T_TetrisBase_Trim_01:
		return TEXT("SM_ENV_PRP_PillarStatuesBroken_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_WallBasic_01:
		return TEXT("SM_ENV_PRP_PillarStatuesBroken_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_WallBasic_02:
		return TEXT("SM_ENV_PRP_PillarStatuesBroken_04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_WallBasic_04:
		return TEXT("SM_ENV_PRP_PillarStatuesBroken_05.fbx");
		break;
	case Prototype_Mesh_SM_ENV_WallBasic_05:
		return TEXT("SM_ENV_PRP_PillarStatuesBroken_06.fbx");
		break;
	case Prototype_Mesh_SM_ENV_WallBasic_08:
		return TEXT("SM_ENV_T_Creeper_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_WallBasic_09:
		return TEXT("SM_ENV_T_DemonT_Roots_04.fbx");
		break;
	case Prototype_Mesh_SM_ENV_WallBasic_11:
		return TEXT("SM_ENV_T_DemonT_Roots_05.fbx");
		break;
	case Prototype_Mesh_SM_ENV_Window_A_RED_BLOCK_01:
		return TEXT("SM_ENV_T_HangingPlant_01.fbx");
		break;
	case Prototype_Mesh_SM_F_ArchIvy_01:
		return TEXT("SM_ENV_T_TetrisBase_01.fbx");
		break;
	case Prototype_Mesh_SM_F_ArchIvy_03:
		return TEXT("SM_ENV_T_TetrisBase_Trim_01.fbx");
		break;
	case Prototype_Mesh_SM_F_BalconyPlant01:
		return TEXT("SM_ENV_WallBasic_01.fbx");
		break;
	case Prototype_Mesh_SM_F_BalconyPlant03:
		return TEXT("SM_ENV_WallBasic_02.fbx");
		break;
	case Prototype_Mesh_SM_F_Creepers_V01:
		return TEXT("SM_ENV_WallBasic_04.fbx");
		break;
	case Prototype_Mesh_SM_F_Flower02:
		return TEXT("SM_ENV_WallBasic_05.fbx");
		break;
	case Prototype_Mesh_SM_F_HangingIvy_01:
		return TEXT("SM_ENV_WallBasic_08.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Bridge_Creeper_01:
		return TEXT("SM_ENV_WallBasic_09.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Bridge_Creeper_02:
		return TEXT("SM_ENV_WallBasic_11.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Bridge_Creeper_03:
		return TEXT("SM_ENV_Window_A_RED_BLOCK_01.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Cliff_Creeper_01:
		return TEXT("SM_F_ArchIvy_01.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Cliff_Creeper_02:
		return TEXT("SM_F_ArchIvy_03.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_01:
		return TEXT("SM_F_BalconyPlant01.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_02:
		return TEXT("SM_F_BalconyPlant03.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_05:
		return TEXT("SM_F_Creepers_V01.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_06:
		return TEXT("SM_F_Flower02.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_07:
		return TEXT("SM_F_HangingIvy_01.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_08:
		return TEXT("SM_F_Mystic_Bridge_Creeper_01.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_09:
		return TEXT("SM_F_Mystic_Bridge_Creeper_02.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_10:
		return TEXT("SM_F_Mystic_Bridge_Creeper_03.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_11:
		return TEXT("SM_F_Mystic_Cliff_Creeper_01.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_12:
		return TEXT("SM_F_Mystic_Cliff_Creeper_02.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_13:
		return TEXT("SM_F_Mystic_Creeper_01.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_14:
		return TEXT("SM_F_Mystic_Creeper_02.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_15:
		return TEXT("SM_F_Mystic_Creeper_03.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_16:
		return TEXT("SM_F_Mystic_Creeper_05.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_17:
		return TEXT("SM_F_Mystic_Creeper_06.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_18:
		return TEXT("SM_F_Mystic_Creeper_07.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_19:
		return TEXT("SM_F_Mystic_Creeper_08.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_20:
		return TEXT("SM_F_Mystic_Creeper_09.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_21:
		return TEXT("SM_F_Mystic_Creeper_10.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_22:
		return TEXT("SM_F_Mystic_Creeper_11.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_24:
		return TEXT("SM_F_Mystic_Creeper_12.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_25:
		return TEXT("SM_F_Mystic_Creeper_13.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_MergedRoots_04:
		return TEXT("SM_F_Mystic_Creeper_14.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Roots_02:
		return TEXT("SM_F_Mystic_Creeper_15.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_S_Creeper_01:
		return TEXT("SM_F_Mystic_Creeper_16.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_S_Creeper_02:
		return TEXT("SM_F_Mystic_Creeper_17.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Shiv_Creeper_01:
		return TEXT("SM_F_Mystic_Creeper_18.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Snake_Creeper_0:
		return TEXT("SM_F_Mystic_Creeper_19.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Snake_Creeper_01:
		return TEXT("SM_F_Mystic_Creeper_20.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Snake_Creeper_03:
		return TEXT("SM_F_Mystic_Creeper_21.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Snake_Creeper_05:
		return TEXT("SM_F_Mystic_Creeper_22.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Snake_Creeper_06:
		return TEXT("SM_F_Mystic_Creeper_24.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Snake_Creeper_07:
		return TEXT("SM_F_Mystic_Creeper_25.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystic_Creeper_03:
		return TEXT("SM_F_Mystic_MergedRoots_04.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystics_Creeper_04:
		return TEXT("SM_F_Mystic_Roots_02.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystics_Creeper_05:
		return TEXT("SM_F_Mystic_S_Creeper_01.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystics_MergedRoots_01:
		return TEXT("SM_F_Mystic_S_Creeper_02.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystics_MergedRoots_02:
		return TEXT("SM_F_Mystic_Shiv_Creeper_01.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystics_MergedRoots_03:
		return TEXT("SM_F_Mystic_Snake_Creeper_0.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystics_MergedRoots_05:
		return TEXT("SM_F_Mystic_Snake_Creeper_01.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystics_MergedRoots_06:
		return TEXT("SM_F_Mystic_Snake_Creeper_03.fbx");
		break;
	case Prototype_Mesh_SM_F_Mystics_Roots_01:
		return TEXT("SM_F_Mystic_Snake_Creeper_05.fbx");
		break;
	case Prototype_Mesh_SM_F_PurpleFlower_Fort_01:
		return TEXT("SM_F_Mystic_Snake_Creeper_06.fbx");
		break;
	case Prototype_Mesh_SM_F_PurpleFlower_Fort_02:
		return TEXT("SM_F_Mystic_Snake_Creeper_07.fbx");
		break;
	case Prototype_Mesh_SM_F_PurpleFlower_Opt02:
		return TEXT("SM_F_Mystics_Creeper_04.fbx");
		break;
	case Prototype_Mesh_SM_F_PurpleFlowerOpt_01:
		return TEXT("SM_F_Mystics_Creeper_05.fbx");
		break;
	case Prototype_Mesh_SM_F_PurpleFlowerOpt_04:
		return TEXT("SM_F_Mystics_MergedRoots_01.fbx");
		break;
	case Prototype_Mesh_SM_F_Roots_01:
		return TEXT("SM_F_Mystics_MergedRoots_02.fbx");
		break;
	case Prototype_Mesh_SM_F_Roots_03:
		return TEXT("SM_F_Mystics_MergedRoots_03.fbx");
		break;
	case Prototype_Mesh_SM_F_Roots_05:
		return TEXT("SM_F_Mystics_MergedRoots_05.fbx");
		break;
	case Prototype_Mesh_SM_F_Roots01:
		return TEXT("SM_F_Mystics_MergedRoots_06.fbx");
		break;
	case Prototype_Mesh_SM_F_Statue_Creeper_01:
		return TEXT("SM_F_Mystics_Roots_01.fbx");
		break;
	case Prototype_Mesh_SM_F_Statue_Creeper_02:
		return TEXT("SM_F_PurpleFlower_Fort_01.fbx");
		break;
	case Prototype_Mesh_SM_F_Statue_Creeper_03:
		return TEXT("SM_F_PurpleFlower_Fort_02.fbx");
		break;
	case Prototype_Mesh_SM_F_Statue_Creeper_04:
		return TEXT("SM_F_PurpleFlower_Opt02.fbx");
		break;
	case Prototype_Mesh_SM_F_Vine06:
		return TEXT("SM_F_PurpleFlowerOpt_01.fbx");
		break;
	case Prototype_Mesh_SM_F_Vine07:
		return TEXT("SM_F_PurpleFlowerOpt_04.fbx");
		break;
	case Prototype_Mesh_SM_F_Vine08:
		return TEXT("SM_F_Roots_01.fbx");
		break;
	case Prototype_Mesh_SM_F_Vine09:
		return TEXT("SM_F_Roots_03.fbx");
		break;
	case Prototype_Mesh_SM_F_Vine10:
		return TEXT("SM_F_Roots_05.fbx");
		break;
	case Prototype_Mesh_SM_F_Vine12:
		return TEXT("SM_F_Roots01.fbx");
		break;
	case Prototype_Mesh_SM_F_Vine14:
		return TEXT("SM_F_Statue_Creeper_01.fbx");
		break;
	case Prototype_Mesh_SM_F_Vine15:
		return TEXT("SM_F_Statue_Creeper_02.fbx");
		break;
	case Prototype_Mesh_SM_F_Vine16:
		return TEXT("SM_F_Statue_Creeper_03.fbx");
		break;
	case Prototype_Mesh_SM_F_Vine18:
		return TEXT("SM_F_Statue_Creeper_04.fbx");
		break;
	case Prototype_Mesh_SM_F_Vine19:
		return TEXT("SM_F_Vine06.fbx");
		break;
	case Prototype_Mesh_SM_F_Vine22:
		return TEXT("SM_F_Vine07.fbx");
		break;
	case Prototype_Mesh_SM_F_Vine23:
		return TEXT("SM_F_Vine08.fbx");
		break;
	case Prototype_Mesh_SM_F_Vine24:
		return TEXT("SM_F_Vine09.fbx");
		break;
	case Prototype_Mesh_SM_F_Vine25:
		return TEXT("SM_F_Vine10.fbx");
		break;
	case Prototype_Mesh_SM_F_Vine26:
		return TEXT("SM_F_Vine12.fbx");
		break;
	case Prototype_Mesh_SM_F_Vine27:
		return TEXT("SM_F_Vine14.fbx");
		break;
	case Prototype_Mesh_SM_F_Vine28:
		return TEXT("SM_F_Vine15.fbx");
		break;
	case Prototype_Mesh_SM_F_Vine31:
		return TEXT("SM_F_Vine16.fbx");
		break;
	case Prototype_Mesh_SM_F_Vine32:
		return TEXT("SM_F_Vine18.fbx");
		break;
	case Prototype_Mesh_SM_F_Vine33:
		return TEXT("SM_F_Vine19.fbx");
		break;
	case Prototype_Mesh_SM_F_Vine36:
		return TEXT("SM_F_Vine22.fbx");
		break;
	case Prototype_Mesh_SM_F_Vines40:
		return TEXT("SM_F_Vine23.fbx");
		break;
	case Prototype_Mesh_SM_Fern_01:
		return TEXT("SM_F_Vine24.fbx");
		break;
	case Prototype_Mesh_SM_Fern_02:
		return TEXT("SM_F_Vine25.fbx");
		break;
	case Prototype_Mesh_SM_Fern_03:
		return TEXT("SM_F_Vine26.fbx");
		break;
	case Prototype_Mesh_SM_FlowerBush_01:
		return TEXT("SM_F_Vine27.fbx");
		break;
	case Prototype_Mesh_SM_FlowerBush_02:
		return TEXT("SM_F_Vine28.fbx");
		break;
	case Prototype_Mesh_SM_FlowerBush_03:
		return TEXT("SM_F_Vine31.fbx");
		break;
	case Prototype_Mesh_SM_FLowerTest_06:
		return TEXT("SM_F_Vine32.fbx");
		break;
	case Prototype_Mesh_SM_FlowerVine_01:
		return TEXT("SM_F_Vine33.fbx");
		break;
	case Prototype_Mesh_SM_FlowerVine01:
		return TEXT("SM_F_Vine36.fbx");
		break;
	case Prototype_Mesh_SM_FlowerVines_02:
		return TEXT("SM_F_Vines40.fbx");
		break;
	case Prototype_Mesh_SM_FlowerVines_03:
		return TEXT("SM_Fern_01.fbx");
		break;
	case Prototype_Mesh_SM_FlowerVines_04:
		return TEXT("SM_Fern_02.fbx");
		break;
	case Prototype_Mesh_SM_FlowerVines_05:
		return TEXT("SM_Fern_03.fbx");
		break;
	case Prototype_Mesh_SM_FlowerVines_07:
		return TEXT("SM_FlowerBush_01.fbx");
		break;
	case Prototype_Mesh_SM_FlowerVines_08:
		return TEXT("SM_FlowerBush_02.fbx");
		break;
	case Prototype_Mesh_SM_FlowerVines_09:
		return TEXT("SM_FlowerBush_03.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Bow_Creeper_0:
		return TEXT("SM_FLowerTest_06.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Bow_Creeper_03:
		return TEXT("SM_FlowerVine_01.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Bow_Creeper_04:
		return TEXT("SM_FlowerVine01.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Courtyard_02:
		return TEXT("SM_FlowerVines_02.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Courtyard_Creeper_03:
		return TEXT("SM_FlowerVines_03.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Courtyard_Creeper_04:
		return TEXT("SM_FlowerVines_04.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_01:
		return TEXT("SM_FlowerVines_05.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_02:
		return TEXT("SM_FlowerVines_07.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_03:
		return TEXT("SM_FlowerVines_08.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_04:
		return TEXT("SM_FlowerVines_09.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_05:
		return TEXT("SM_Fort_Bow_Creeper_0.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_06:
		return TEXT("SM_Fort_Bow_Creeper_03.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_07:
		return TEXT("SM_Fort_Bow_Creeper_04.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_08:
		return TEXT("SM_Fort_Courtyard_02.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_10:
		return TEXT("SM_Fort_Courtyard_Creeper_03.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_11:
		return TEXT("SM_Fort_Courtyard_Creeper_04.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_12:
		return TEXT("SM_Fort_Creeper_01.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_13:
		return TEXT("SM_Fort_Creeper_02.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_15:
		return TEXT("SM_Fort_Creeper_03.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_16:
		return TEXT("SM_Fort_Creeper_04.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_17:
		return TEXT("SM_Fort_Creeper_05.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_18:
		return TEXT("SM_Fort_Creeper_06.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_19:
		return TEXT("SM_Fort_Creeper_07.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_20:
		return TEXT("SM_Fort_Creeper_08.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_21:
		return TEXT("SM_Fort_Creeper_10.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_22:
		return TEXT("SM_Fort_Creeper_11.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_23:
		return TEXT("SM_Fort_Creeper_12.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_24:
		return TEXT("SM_Fort_Creeper_13.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_25:
		return TEXT("SM_Fort_Creeper_15.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_26:
		return TEXT("SM_Fort_Creeper_16.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_27:
		return TEXT("SM_Fort_Creeper_17.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_28:
		return TEXT("SM_Fort_Creeper_18.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_29:
		return TEXT("SM_Fort_Creeper_19.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_30:
		return TEXT("SM_Fort_Creeper_20.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_32:
		return TEXT("SM_Fort_Creeper_21.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_33:
		return TEXT("SM_Fort_Creeper_22.fbx");
		break;
	case Prototype_Mesh_SM_Fort_Creeper_34:
		return TEXT("SM_Fort_Creeper_23.fbx");
		break;
	case Prototype_Mesh_SM_Fort_RedCreeper_01:
		return TEXT("SM_Fort_Creeper_24.fbx");
		break;
	case Prototype_Mesh_SM_Fort_RedCreeper_02:
		return TEXT("SM_Fort_Creeper_25.fbx");
		break;
	case Prototype_Mesh_SM_Fort_RedCreeper_03:
		return TEXT("SM_Fort_Creeper_26.fbx");
		break;
	case Prototype_Mesh_SM_Fort_RedCreeper_04:
		return TEXT("SM_Fort_Creeper_27.fbx");
		break;
	case Prototype_Mesh_SM_Fort_WhiteFlower_01:
		return TEXT("SM_Fort_Creeper_28.fbx");
		break;
	case Prototype_Mesh_SM_Fortwall_blocker_01:
		return TEXT("SM_Fort_Creeper_29.fbx");
		break;
	case Prototype_Mesh_SM_Fortwall_blocker_02:
		return TEXT("SM_Fort_Creeper_30.fbx");
		break;
	case Prototype_Mesh_SM_Fortwall_blocker_03:
		return TEXT("SM_Fort_Creeper_32.fbx");
		break;
	case Prototype_Mesh_SM_Fortwall_blocker_04:
		return TEXT("SM_Fort_Creeper_33.fbx");
		break;
	case Prototype_Mesh_SM_Fortwall_blocker_05:
		return TEXT("SM_Fort_Creeper_34.fbx");
		break;
	case Prototype_Mesh_SM_Fortwall_blocker_06:
		return TEXT("SM_Fort_RedCreeper_01.fbx");
		break;
	case Prototype_Mesh_SM_Fortwall_Cover_01:
		return TEXT("SM_Fort_RedCreeper_02.fbx");
		break;
	case Prototype_Mesh_SM_FortWall_Market_01:
		return TEXT("SM_Fort_RedCreeper_03.fbx");
		break;
	case Prototype_Mesh_SM_FortWall_Market_02:
		return TEXT("SM_Fort_RedCreeper_04.fbx");
		break;
	case Prototype_Mesh_SM_FortWall_Market_03:
		return TEXT("SM_Fort_WhiteFlower_01.fbx");
		break;
	case Prototype_Mesh_SM_FortWall_Market_04:
		return TEXT("SM_Fortwall_blocker_01.fbx");
		break;
	case Prototype_Mesh_SM_FortWall_Market_05:
		return TEXT("SM_Fortwall_blocker_02.fbx");
		break;
	case Prototype_Mesh_SM_FortWall_Market_06:
		return TEXT("SM_Fortwall_blocker_03.fbx");
		break;
	case Prototype_Mesh_SM_FortWall_Market_07:
		return TEXT("SM_Fortwall_blocker_04.fbx");
		break;
	case Prototype_Mesh_SM_FortWall_Market_08:
		return TEXT("SM_Fortwall_blocker_05.fbx");
		break;
	case Prototype_Mesh_SM_FortWall_Market_09:
		return TEXT("SM_Fortwall_blocker_06.fbx");
		break;
	case Prototype_Mesh_SM_FortWall_Market_10:
		return TEXT("SM_Fortwall_Cover_01.fbx");
		break;
	case Prototype_Mesh_SM_FortWall_Market_11:
		return TEXT("SM_FortWall_Market_01.fbx");
		break;
	case Prototype_Mesh_SM_FortWall_Market_12:
		return TEXT("SM_FortWall_Market_02.fbx");
		break;
	case Prototype_Mesh_SM_Fortwall_Market2_01:
		return TEXT("SM_FortWall_Market_03.fbx");
		break;
	case Prototype_Mesh_SM_FortWall_side_01:
		return TEXT("SM_FortWall_Market_04.fbx");
		break;
	case Prototype_Mesh_SM_FortWall_side_02:
		return TEXT("SM_FortWall_Market_05.fbx");
		break;
	case Prototype_Mesh_SM_Gadasura_Head_03:
		return TEXT("SM_FortWall_Market_06.fbx");
		break;
	case Prototype_Mesh_SM_GEN_Tree_01:
		return TEXT("SM_FortWall_Market_07.fbx");
		break;
	case Prototype_Mesh_SM_GEN_Tree_02:
		return TEXT("SM_FortWall_Market_08.fbx");
		break;
	case Prototype_Mesh_SM_Grass_01A:
		return TEXT("SM_FortWall_Market_09.fbx");
		break;
	case Prototype_Mesh_SM_Grass_01A1:
		return TEXT("SM_FortWall_Market_10.fbx");
		break;
	case Prototype_Mesh_SM_Green_Stack_01:
		return TEXT("SM_FortWall_Market_11.fbx");
		break;
	case Prototype_Mesh_SM_GRND_Bush_01:
		return TEXT("SM_FortWall_Market_12.fbx");
		break;
	case Prototype_Mesh_SM_GRND_Bush_02:
		return TEXT("SM_Fortwall_Market2_01.fbx");
		break;
	case Prototype_Mesh_SM_GRND_Bush_03:
		return TEXT("SM_FortWall_side_01.fbx");
		break;
	case Prototype_Mesh_SM_GRND_Bush_05:
		return TEXT("SM_FortWall_side_02.fbx");
		break;
	case Prototype_Mesh_SM_GRND_PLANT_01:
		return TEXT("SM_Gadasura_Head_03.fbx");
		break;
	case Prototype_Mesh_SM_GRND_Plant_CVR_01:
		return TEXT("SM_GEN_Tree_01.fbx");
		break;
	case Prototype_Mesh_SM_GroundClutter_01:
		return TEXT("SM_GEN_Tree_02.fbx");
		break;
	case Prototype_Mesh_SM_GroundScatter_01A:
		return TEXT("SM_Grass_01A.fbx");
		break;
	case Prototype_Mesh_SM_GroundScatter_02A:
		return TEXT("SM_Grass_01A1.fbx");
		break;
	case Prototype_Mesh_SM_GroundScatter_03A:
		return TEXT("SM_Green_Stack_01.fbx");
		break;
	case Prototype_Mesh_SM_GroundScatter_03B:
		return TEXT("SM_GRND_Bush_01.fbx");
		break;
	case Prototype_Mesh_SM_Hedgerow_01:
		return TEXT("SM_GRND_Bush_02.fbx");
		break;
	case Prototype_Mesh_SM_Hedgerow_01_A:
		return TEXT("SM_GRND_Bush_03.fbx");
		break;
	case Prototype_Mesh_SM_Hedgerow_Corner_01:
		return TEXT("SM_GRND_Bush_05.fbx");
		break;
	case Prototype_Mesh_SM_lillypad_01:
		return TEXT("SM_GRND_PLANT_01.fbx");
		break;
	case Prototype_Mesh_SM_lillypad_02:
		return TEXT("SM_GRND_Plant_CVR_01.fbx");
		break;
	case Prototype_Mesh_SM_Lillypad_02_A:
		return TEXT("SM_GroundClutter_01.fbx");
		break;
	case Prototype_Mesh_SM_Lillypad_02_B:
		return TEXT("SM_GroundScatter_01A.fbx");
		break;
	case Prototype_Mesh_SM_Lillypad_02_C:
		return TEXT("SM_GroundScatter_02A.fbx");
		break;
	case Prototype_Mesh_SM_Lillypad_02_D:
		return TEXT("SM_GroundScatter_03A.fbx");
		break;
	case Prototype_Mesh_SM_lillypad_03:
		return TEXT("SM_GroundScatter_03B.fbx");
		break;
	case Prototype_Mesh_SM_lillypad_04:
		return TEXT("SM_Hedgerow_01.fbx");
		break;
	case Prototype_Mesh_SM_lillypad_05:
		return TEXT("SM_Hedgerow_01_A.fbx");
		break;
	case Prototype_Mesh_SM_lillypad_06:
		return TEXT("SM_Hedgerow_Corner_01.fbx");
		break;
	case Prototype_Mesh_SM_Lotus_01:
		return TEXT("SM_lillypad_01.fbx");
		break;
	case Prototype_Mesh_SM_Lotus_01_flower:
		return TEXT("SM_lillypad_02.fbx");
		break;
	case Prototype_Mesh_SM_Lotus_01_stem:
		return TEXT("SM_Lillypad_02_A.fbx");
		break;
	case Prototype_Mesh_SM_Lotus_bud_01:
		return TEXT("SM_Lillypad_02_B.fbx");
		break;
	case Prototype_Mesh_SM_Lotus_bud_main_01:
		return TEXT("SM_Lillypad_02_C.fbx");
		break;
	case Prototype_Mesh_SM_Lotus_Pink:
		return TEXT("SM_Lillypad_02_D.fbx");
		break;
	case Prototype_Mesh_SM_MERGED_PRESET_Ivy1_BP_6:
		return TEXT("SM_lillypad_03.fbx");
		break;
	case Prototype_Mesh_SM_MERGED_PRESET_Liana1_BP_11:
		return TEXT("SM_lillypad_04.fbx");
		break;
	case Prototype_Mesh_SM_MERGED_PRESET_Liana1_BP_14:
		return TEXT("SM_lillypad_05.fbx");
		break;
	case Prototype_Mesh_SM_MERGED_PRESET_Liana1_BP2:
		return TEXT("SM_lillypad_06.fbx");
		break;
	case Prototype_Mesh_SM_MFort_Creeper_07:
		return TEXT("SM_Lotus_01.fbx");
		break;
	case Prototype_Mesh_SM_Mossplant:
		return TEXT("SM_Lotus_01_flower.fbx");
		break;
	case Prototype_Mesh_SM_Mystic_Bush_01:
		return TEXT("SM_Lotus_01_stem.fbx");
		break;
	case Prototype_Mesh_SM_Mystic_Bush_01A:
		return TEXT("SM_Lotus_bud_01.fbx");
		break;
	case Prototype_Mesh_SM_Mystic_Bush_02A:
		return TEXT("SM_Lotus_bud_main_01.fbx");
		break;
	case Prototype_Mesh_SM_Mystic_Roots_Hanging_01:
		return TEXT("SM_Lotus_Pink.fbx");
		break;
	case Prototype_Mesh_SM_Mystic_Roots_Hanging_02:
		return TEXT("SM_MERGED_PRESET_Ivy1_BP_6.fbx");
		break;
	case Prototype_Mesh_SM_Mystic_Roots_Hanging_03:
		return TEXT("SM_MERGED_PRESET_Liana1_BP_11.fbx");
		break;
	case Prototype_Mesh_SM_Ninja_Run_Pose:
		return TEXT("SM_MERGED_PRESET_Liana1_BP_14.fbx");
		break;
	case Prototype_Mesh_SM_palmbush_01:
		return TEXT("SM_MERGED_PRESET_Liana1_BP2.fbx");
		break;
	case Prototype_Mesh_SM_palmbush_02:
		return TEXT("SM_MFort_Creeper_07.fbx");
		break;
	case Prototype_Mesh_SM_Pinkflower_vine_01:
		return TEXT("SM_Mossplant.fbx");
		break;
	case Prototype_Mesh_SM_Pinkvine_wall_01:
		return TEXT("SM_Mystic_Bush_01.fbx");
		break;
	case Prototype_Mesh_SM_Pinkvine_wall_01a:
		return TEXT("SM_Mystic_Bush_01A.fbx");
		break;
	case Prototype_Mesh_SM_PRP_Arrow_01:
		return TEXT("SM_Mystic_Bush_02A.fbx");
		break;
	case Prototype_Mesh_SM_PRP_Banner_01_Static:
		return TEXT("SM_Mystic_Roots_Hanging_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_Banner_05_Static:
		return TEXT("SM_Mystic_Roots_Hanging_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_Bow_01:
		return TEXT("SM_Mystic_Roots_Hanging_03.fbx");
		break;
	case Prototype_Mesh_SM_PRP_BrokenPillars_01:
		return TEXT("SM_Ninja_Run_Pose.fbx");
		break;
	case Prototype_Mesh_SM_PRP_BrokenPillars_06:
		return TEXT("SM_palmbush_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_BrokenPillars_07:
		return TEXT("SM_palmbush_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_BrokenWall03:
		return TEXT("SM_Pinkflower_vine_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_Anvil_01:
		return TEXT("SM_Pinkvine_wall_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_Barrel_01:
		return TEXT("SM_Pinkvine_wall_01a.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_Barrel_02:
		return TEXT("SM_PRP_Arrow_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_BarrelGroup_01:
		return TEXT("SM_PRP_Banner_01_Static.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_BarrelGroup_04:
		return TEXT("SM_PRP_Banner_05_Static.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_BarrelGroup_06:
		return TEXT("SM_PRP_Bow_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_BarrelGroup_07:
		return TEXT("SM_PRP_BrokenPillars_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_BarrelGroup_08:
		return TEXT("SM_PRP_BrokenPillars_06.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_BoatRound_01:
		return TEXT("SM_PRP_BrokenPillars_07.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_Bucket_01:
		return TEXT("SM_PRP_BrokenWall03.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_Bucket_02:
		return TEXT("SM_PRP_CC_Anvil_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_Cauldron_01:
		return TEXT("SM_PRP_CC_Barrel_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_Cauldron_02:
		return TEXT("SM_PRP_CC_Barrel_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_CauldronLarge_01:
		return TEXT("SM_PRP_CC_BarrelGroup_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_Chair_01:
		return TEXT("SM_PRP_CC_BarrelGroup_04.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_Chisel_02:
		return TEXT("SM_PRP_CC_BarrelGroup_06.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_Chisel_05:
		return TEXT("SM_PRP_CC_BarrelGroup_07.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_ClaypotGroup_01:
		return TEXT("SM_PRP_CC_BarrelGroup_08.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_ClaypotGroup_03:
		return TEXT("SM_PRP_CC_BoatRound_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_ClaypotGroup_04:
		return TEXT("SM_PRP_CC_Bucket_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_ClaypotGroup_05:
		return TEXT("SM_PRP_CC_Bucket_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_Cushion_01:
		return TEXT("SM_PRP_CC_Cauldron_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_Cushion_02:
		return TEXT("SM_PRP_CC_Cauldron_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_Cushion_03:
		return TEXT("SM_PRP_CC_CauldronLarge_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_DockPlanks_01:
		return TEXT("SM_PRP_CC_Chair_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_DockPlanks_02:
		return TEXT("SM_PRP_CC_Chisel_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_DockPlanks_03:
		return TEXT("SM_PRP_CC_Chisel_05.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_DockPlanks_04:
		return TEXT("SM_PRP_CC_ClaypotGroup_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_DockPlanks_05:
		return TEXT("SM_PRP_CC_ClaypotGroup_03.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_DockPlanks_06:
		return TEXT("SM_PRP_CC_ClaypotGroup_04.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_DockPlanks_07:
		return TEXT("SM_PRP_CC_ClaypotGroup_05.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_DockStairs_01:
		return TEXT("SM_PRP_CC_Cushion_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_DockStepWall_01:
		return TEXT("SM_PRP_CC_Cushion_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_ElephantHead_01:
		return TEXT("SM_PRP_CC_Cushion_03.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_FishCrate_01:
		return TEXT("SM_PRP_CC_DockPlanks_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_FishCrate_02:
		return TEXT("SM_PRP_CC_DockPlanks_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_FishCrate_03:
		return TEXT("SM_PRP_CC_DockPlanks_03.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_FishCrate_04:
		return TEXT("SM_PRP_CC_DockPlanks_04.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_FishCrate_05:
		return TEXT("SM_PRP_CC_DockPlanks_05.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_FishCrate_06:
		return TEXT("SM_PRP_CC_DockPlanks_06.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_GlassPanel_01:
		return TEXT("SM_PRP_CC_DockPlanks_07.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_Grinder_01:
		return TEXT("SM_PRP_CC_DockStairs_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_Hammer_01:
		return TEXT("SM_PRP_CC_DockStepWall_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_Jar_02:
		return TEXT("SM_PRP_CC_ElephantHead_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_Ladle_01:
		return TEXT("SM_PRP_CC_FishCrate_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_LampPillar_01:
		return TEXT("SM_PRP_CC_FishCrate_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_LanternCotton_01:
		return TEXT("SM_PRP_CC_FishCrate_03.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_LanternCotton_02:
		return TEXT("SM_PRP_CC_FishCrate_04.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_LanternMetal_01:
		return TEXT("SM_PRP_CC_FishCrate_05.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_LanternMetal_02:
		return TEXT("SM_PRP_CC_FishCrate_06.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_LeverBase_02:
		return TEXT("SM_PRP_CC_GlassPanel_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_Pier_01:
		return TEXT("SM_PRP_CC_Grinder_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_PierCorner_01:
		return TEXT("SM_PRP_CC_Hammer_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_PierCorner_02:
		return TEXT("SM_PRP_CC_Jar_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_PierFenceEnd_01:
		return TEXT("SM_PRP_CC_Ladle_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_PierFenceEnd_02:
		return TEXT("SM_PRP_CC_LampPillar_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_PierFenceStart_01:
		return TEXT("SM_PRP_CC_LanternCotton_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_PierFenceStart_02:
		return TEXT("SM_PRP_CC_LanternCotton_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_PierNarrow_01:
		return TEXT("SM_PRP_CC_LanternMetal_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_PierNarrow_02:
		return TEXT("SM_PRP_CC_LanternMetal_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_PierNarrow_03:
		return TEXT("SM_PRP_CC_LeverBase_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_PierNarrow_04:
		return TEXT("SM_PRP_CC_Pier_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_PierWide_01:
		return TEXT("SM_PRP_CC_PierCorner_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_PierWide_02:
		return TEXT("SM_PRP_CC_PierCorner_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_PierWide_03:
		return TEXT("SM_PRP_CC_PierFenceEnd_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_PierWide_04:
		return TEXT("SM_PRP_CC_PierFenceEnd_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_RedCarpet_01:
		return TEXT("SM_PRP_CC_PierFenceStart_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_RedCarpet_02:
		return TEXT("SM_PRP_CC_PierFenceStart_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_RedCarpet_03:
		return TEXT("SM_PRP_CC_PierNarrow_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_SackGroup_01:
		return TEXT("SM_PRP_CC_PierNarrow_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_SackGroup_02:
		return TEXT("SM_PRP_CC_PierNarrow_03.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_SackGroup_03:
		return TEXT("SM_PRP_CC_PierNarrow_04.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_SackGroup_04:
		return TEXT("SM_PRP_CC_PierWide_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_SackGroup_05:
		return TEXT("SM_PRP_CC_PierWide_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_SackGroup_06:
		return TEXT("SM_PRP_CC_PierWide_03.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_ShieldStandGroup_01:
		return TEXT("SM_PRP_CC_PierWide_04.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_ShieldStandGroup_02:
		return TEXT("SM_PRP_CC_RedCarpet_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_ShieldStandGroup_03:
		return TEXT("SM_PRP_CC_RedCarpet_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_ShieldStandGroup_04:
		return TEXT("SM_PRP_CC_RedCarpet_03.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_Soldier_01:
		return TEXT("SM_PRP_CC_SackGroup_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_Soldier_02:
		return TEXT("SM_PRP_CC_SackGroup_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_SpearStandGroup_02:
		return TEXT("SM_PRP_CC_SackGroup_03.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_SquareLantern_01:
		return TEXT("SM_PRP_CC_SackGroup_04.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_SquareLantern_02:
		return TEXT("SM_PRP_CC_SackGroup_05.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_SquareLantern_03:
		return TEXT("SM_PRP_CC_SackGroup_06.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_Stool_01:
		return TEXT("SM_PRP_CC_ShieldStandGroup_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_SwordStandGroup_01:
		return TEXT("SM_PRP_CC_ShieldStandGroup_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_SwordStandGroup_02:
		return TEXT("SM_PRP_CC_ShieldStandGroup_03.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_SwordStandGroup_04:
		return TEXT("SM_PRP_CC_ShieldStandGroup_04.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_SwordStandGroup_05:
		return TEXT("SM_PRP_CC_Soldier_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_Table_01:
		return TEXT("SM_PRP_CC_Soldier_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_TableGroup_01:
		return TEXT("SM_PRP_CC_SpearStandGroup_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_TableGroup_02:
		return TEXT("SM_PRP_CC_SquareLantern_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_TableGroup_03:
		return TEXT("SM_PRP_CC_SquareLantern_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_TableGroup_04:
		return TEXT("SM_PRP_CC_SquareLantern_03.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_TableGroup_05:
		return TEXT("SM_PRP_CC_Stool_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_TableGroup_06:
		return TEXT("SM_PRP_CC_SwordStandGroup_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_TableGroup_07:
		return TEXT("SM_PRP_CC_SwordStandGroup_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_TableGroup_08:
		return TEXT("SM_PRP_CC_SwordStandGroup_04.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_TableGroup_09:
		return TEXT("SM_PRP_CC_SwordStandGroup_05.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_TableSmall_01:
		return TEXT("SM_PRP_CC_Table_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_ToolsGroup_01:
		return TEXT("SM_PRP_CC_TableGroup_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_ToolsGroup_02:
		return TEXT("SM_PRP_CC_TableGroup_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_ToolsGroup_03:
		return TEXT("SM_PRP_CC_TableGroup_03.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_ToolsGroup_04:
		return TEXT("SM_PRP_CC_TableGroup_04.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_ToolsGroup_05:
		return TEXT("SM_PRP_CC_TableGroup_05.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_ToolsGroup_06:
		return TEXT("SM_PRP_CC_TableGroup_06.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_ToolsGroup_07:
		return TEXT("SM_PRP_CC_TableGroup_07.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_TreeBase_01:
		return TEXT("SM_PRP_CC_TableGroup_08.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_WoodBox_01:
		return TEXT("SM_PRP_CC_TableGroup_09.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_WoodBoxGroup_01:
		return TEXT("SM_PRP_CC_TableSmall_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_WoodBoxGroup_02:
		return TEXT("SM_PRP_CC_ToolsGroup_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_WoodBoxGroup_03:
		return TEXT("SM_PRP_CC_ToolsGroup_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_WoodBoxGroup_04:
		return TEXT("SM_PRP_CC_ToolsGroup_03.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_WoodBoxGroup_05:
		return TEXT("SM_PRP_CC_ToolsGroup_04.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_WoodBoxGroup_06:
		return TEXT("SM_PRP_CC_ToolsGroup_05.fbx");
		break;
	case Prototype_Mesh_SM_PRP_CC_WoodBoxGroup_07:
		return TEXT("SM_PRP_CC_ToolsGroup_06.fbx");
		break;
	case Prototype_Mesh_SM_PRP_ChainLink_01:
		return TEXT("SM_PRP_CC_ToolsGroup_07.fbx");
		break;
	case Prototype_Mesh_SM_PRP_Curtain_01:
		return TEXT("SM_PRP_CC_TreeBase_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_Curtain_02:
		return TEXT("SM_PRP_CC_WoodBox_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_Curtain_03:
		return TEXT("SM_PRP_CC_WoodBoxGroup_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_Curtain_04:
		return TEXT("SM_PRP_CC_WoodBoxGroup_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_D_Ribbon_Extension:
		return TEXT("SM_PRP_CC_WoodBoxGroup_03.fbx");
		break;
	case Prototype_Mesh_SM_PRP_D_RibbonLarge_01:
		return TEXT("SM_PRP_CC_WoodBoxGroup_04.fbx");
		break;
	case Prototype_Mesh_SM_PRP_D_RibbonLarge_02:
		return TEXT("SM_PRP_CC_WoodBoxGroup_05.fbx");
		break;
	case Prototype_Mesh_SM_PRP_D_RibbonLarge_03:
		return TEXT("SM_PRP_CC_WoodBoxGroup_06.fbx");
		break;
	case Prototype_Mesh_SM_PRP_D_RibbonSmall_01:
		return TEXT("SM_PRP_CC_WoodBoxGroup_07.fbx");
		break;
	case Prototype_Mesh_SM_PRP_D_StoneStep_01:
		return TEXT("SM_PRP_ChainLink_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_D_StoneStep_02:
		return TEXT("SM_PRP_Curtain_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_D_TreePedestal_01:
		return TEXT("SM_PRP_Curtain_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_D_WillowTREE_01:
		return TEXT("SM_PRP_Curtain_03.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DBPully:
		return TEXT("SM_PRP_Curtain_04.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DBPully_DM:
		return TEXT("SM_PRP_D_Ribbon_Extension.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DBPully_Handle_:
		return TEXT("SM_PRP_D_RibbonLarge_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DeadSoldier_02:
		return TEXT("SM_PRP_D_RibbonLarge_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DeadSoldier_03:
		return TEXT("SM_PRP_D_RibbonLarge_03.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DeadSoldier_04:
		return TEXT("SM_PRP_D_RibbonSmall_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DeadSoldier_05:
		return TEXT("SM_PRP_D_StoneStep_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DeadSoldier_06:
		return TEXT("SM_PRP_D_StoneStep_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DeadSoldier_07:
		return TEXT("SM_PRP_D_TreePedestal_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DeadSoldier_08:
		return TEXT("SM_PRP_D_WillowTREE_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DeadSoldier_09:
		return TEXT("SM_PRP_DBPully.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DT_Arm_left_01:
		return TEXT("SM_PRP_DBPully_DM.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DT_Arm_left_02:
		return TEXT("SM_PRP_DBPully_Handle_.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DT_Arm_Right_01:
		return TEXT("SM_PRP_DeadSoldier_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DT_Arm_Right_02:
		return TEXT("SM_PRP_DeadSoldier_03.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DT_Durga_Statue_01:
		return TEXT("SM_PRP_DeadSoldier_04.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DT_ElephantHead_01:
		return TEXT("SM_PRP_DeadSoldier_05.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DT_WP_Bell_01:
		return TEXT("SM_PRP_DeadSoldier_06.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DT_WP_Bow_01:
		return TEXT("SM_PRP_DeadSoldier_07.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DT_WP_Chakra_01:
		return TEXT("SM_PRP_DeadSoldier_08.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DT_WP_Drum_01:
		return TEXT("SM_PRP_DeadSoldier_09.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DT_WP_Gada_01:
		return TEXT("SM_PRP_DT_Arm_left_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DT_WP_Lotus_01:
		return TEXT("SM_PRP_DT_Arm_left_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DT_WP_Shield_01:
		return TEXT("SM_PRP_DT_Arm_Right_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DT_WP_Sword_01:
		return TEXT("SM_PRP_DT_Arm_Right_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_DT_WP_Trishul_01:
		return TEXT("SM_PRP_DT_Durga_Statue_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_F_DoorLedge_01:
		return TEXT("SM_PRP_DT_ElephantHead_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_F_FemaleStatue_DemonTree_01:
		return TEXT("SM_PRP_DT_WP_Bell_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_F_GlassPanel_01:
		return TEXT("SM_PRP_DT_WP_Bow_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_F_GlassPanel_02:
		return TEXT("SM_PRP_DT_WP_Chakra_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_F_PaintingFrame_01:
		return TEXT("SM_PRP_DT_WP_Drum_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_F_PaintingFrame_02:
		return TEXT("SM_PRP_DT_WP_Gada_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_Fireplace_01:
		return TEXT("SM_PRP_DT_WP_Lotus_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_Fireplace_02:
		return TEXT("SM_PRP_DT_WP_Shield_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_FireplaceCooking_01:
		return TEXT("SM_PRP_DT_WP_Sword_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_MandalaFrame_01:
		return TEXT("SM_PRP_DT_WP_Trishul_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_MS_RootBlocker_01:
		return TEXT("SM_PRP_F_DoorLedge_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_Rock01:
		return TEXT("SM_PRP_F_FemaleStatue_DemonTree_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_Scarecrow_01:
		return TEXT("SM_PRP_F_GlassPanel_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_Sculpture01:
		return TEXT("SM_PRP_F_GlassPanel_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_Sculpture02:
		return TEXT("SM_PRP_F_PaintingFrame_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_Shield_01:
		return TEXT("SM_PRP_F_PaintingFrame_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_Spear_01:
		return TEXT("SM_PRP_Fireplace_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_TrishulStatue:
		return TEXT("SM_PRP_Fireplace_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_UtensilGroup_01:
		return TEXT("SM_PRP_FireplaceCooking_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_UtensilGroup_02:
		return TEXT("SM_PRP_MandalaFrame_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_UtensilGroup_03:
		return TEXT("SM_PRP_MS_RootBlocker_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_UtensilGroup_04:
		return TEXT("SM_PRP_Rock01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_UtensilGroup_05:
		return TEXT("SM_PRP_Scarecrow_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_UtensilGroup_06:
		return TEXT("SM_PRP_Sculpture01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_UtensilGroup_07:
		return TEXT("SM_PRP_Sculpture02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_UtensilGroup_08:
		return TEXT("SM_PRP_Shield_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_UtensilGroup_10:
		return TEXT("SM_PRP_Spear_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_UtensilGroup_11:
		return TEXT("SM_PRP_TrishulStatue.fbx");
		break;
	case Prototype_Mesh_SM_PRP_Wall02:
		return TEXT("SM_PRP_UtensilGroup_01.fbx");
		break;
	case Prototype_Mesh_SM_PRP_WallPillar01:
		return TEXT("SM_PRP_UtensilGroup_02.fbx");
		break;
	case Prototype_Mesh_SM_PRP_WeaponGroup_01:
		return TEXT("SM_PRP_UtensilGroup_03.fbx");
		break;
	case Prototype_Mesh_SM_PRP_WeaponGroup_02:
		return TEXT("SM_PRP_UtensilGroup_04.fbx");
		break;
	case Prototype_Mesh_SM_PRP_WeaponGroup_03:
		return TEXT("SM_PRP_UtensilGroup_05.fbx");
		break;
	case Prototype_Mesh_SM_PRP_WeaponGroup_04:
		return TEXT("SM_PRP_UtensilGroup_06.fbx");
		break;
	case Prototype_Mesh_SM_PRP_WeaponGroup_05:
		return TEXT("SM_PRP_UtensilGroup_07.fbx");
		break;
	case Prototype_Mesh_SM_PRP_WeaponGroup_06:
		return TEXT("SM_PRP_UtensilGroup_08.fbx");
		break;
	case Prototype_Mesh_SM_PRP_WeaponGroup_07:
		return TEXT("SM_PRP_UtensilGroup_10.fbx");
		break;
	case Prototype_Mesh_SM_Rangda_Finger_Left:
		return TEXT("SM_PRP_UtensilGroup_11.fbx");
		break;
	case Prototype_Mesh_SM_rock_river_01:
		return TEXT("SM_PRP_Wall02.fbx");
		break;
	case Prototype_Mesh_SM_rock_river_02:
		return TEXT("SM_PRP_WallPillar01.fbx");
		break;
	case Prototype_Mesh_SM_rock_river_03:
		return TEXT("SM_PRP_WeaponGroup_01.fbx");
		break;
	case Prototype_Mesh_SM_rock_river_04:
		return TEXT("SM_PRP_WeaponGroup_02.fbx");
		break;
	case Prototype_Mesh_SM_rock_river_05:
		return TEXT("SM_PRP_WeaponGroup_03.fbx");
		break;
	case Prototype_Mesh_SM_rock_river_06:
		return TEXT("SM_PRP_WeaponGroup_04.fbx");
		break;
	case Prototype_Mesh_SM_rock_river_07:
		return TEXT("SM_PRP_WeaponGroup_05.fbx");
		break;
	case Prototype_Mesh_SM_rock_river_08:
		return TEXT("SM_PRP_WeaponGroup_06.fbx");
		break;
	case Prototype_Mesh_SM_rock_river_10:
		return TEXT("SM_PRP_WeaponGroup_07.fbx");
		break;
	case Prototype_Mesh_SM_Root_01:
		return TEXT("SM_Rangda_Finger_Left.fbx");
		break;
	case Prototype_Mesh_SM_Root_02:
		return TEXT("SM_rock_river_01.fbx");
		break;
	case Prototype_Mesh_SM_Root_03:
		return TEXT("SM_rock_river_02.fbx");
		break;
	case Prototype_Mesh_SM_Roots_01:
		return TEXT("SM_rock_river_03.fbx");
		break;
	case Prototype_Mesh_SM_Roots_Hanging_01:
		return TEXT("SM_rock_river_04.fbx");
		break;
	case Prototype_Mesh_SM_Roots_Hanging_02:
		return TEXT("SM_rock_river_05.fbx");
		break;
	case Prototype_Mesh_SM_Roots_Hanging_03:
		return TEXT("SM_rock_river_06.fbx");
		break;
	case Prototype_Mesh_SM_Roots_Hanging_04:
		return TEXT("SM_rock_river_07.fbx");
		break;
	case Prototype_Mesh_SM_Roots_Hanging_05:
		return TEXT("SM_rock_river_08.fbx");
		break;
	case Prototype_Mesh_SM_Roots_Hanging_06:
		return TEXT("SM_rock_river_10.fbx");
		break;
	case Prototype_Mesh_SM_Roundbush_01:
		return TEXT("SM_Root_01.fbx");
		break;
	case Prototype_Mesh_SM_Roundbush_02:
		return TEXT("SM_Root_02.fbx");
		break;
	case Prototype_Mesh_SM_Statue_Female_ReBirth_Body:
		return TEXT("SM_Root_03.fbx");
		break;
	case Prototype_Mesh_SM_Statue_Female_ReBirth_Feet:
		return TEXT("SM_Roots_01.fbx");
		break;
	case Prototype_Mesh_SM_Statue_Female_ReBirth_Leg_02:
		return TEXT("SM_Roots_Hanging_01.fbx");
		break;
	case Prototype_Mesh_SM_Statue_Female_ReBirth_Leg_03:
		return TEXT("SM_Roots_Hanging_02.fbx");
		break;
	case Prototype_Mesh_SM_Statue_Ganga_01:
		return TEXT("SM_Roots_Hanging_03.fbx");
		break;
	case Prototype_Mesh_SM_Statue_Shiva_01:
		return TEXT("SM_Roots_Hanging_04.fbx");
		break;
	case Prototype_Mesh_SM_T_Creeper_02:
		return TEXT("SM_Roots_Hanging_05.fbx");
		break;
	case Prototype_Mesh_SM_T_Creeper_04:
		return TEXT("SM_Roots_Hanging_06.fbx");
		break;
	case Prototype_Mesh_SM_T_Creeper_05:
		return TEXT("SM_Roundbush_01.fbx");
		break;
	case Prototype_Mesh_SM_T_Creeper_06:
		return TEXT("SM_Roundbush_02.fbx");
		break;
	case Prototype_Mesh_SM_T_Creeper_09:
		return TEXT("SM_Statue_Female_ReBirth_Body.fbx");
		break;
	case Prototype_Mesh_SM_T_Creeper_11:
		return TEXT("SM_Statue_Female_ReBirth_Feet.fbx");
		break;
	case Prototype_Mesh_SM_T_Creeper_12:
		return TEXT("SM_Statue_Female_ReBirth_Leg_02.fbx");
		break;
	case Prototype_Mesh_SM_T_Creeper_13:
		return TEXT("SM_Statue_Female_ReBirth_Leg_03.fbx");
		break;
	case Prototype_Mesh_SM_T_Creepers_10:
		return TEXT("SM_Statue_Ganga_01.fbx");
		break;
	case Prototype_Mesh_SM_T_Creepers_14:
		return TEXT("SM_Statue_Shiva_01.fbx");
		break;
	case Prototype_Mesh_SM_T_Creepers_15:
		return TEXT("SM_T_Creeper_02.fbx");
		break;
	case Prototype_Mesh_SM_T_Roots_01:
		return TEXT("SM_T_Creeper_04.fbx");
		break;
	case Prototype_Mesh_SM_T_Roots_02:
		return TEXT("SM_T_Creeper_05.fbx");
		break;
	case Prototype_Mesh_SM_T_Roots_03:
		return TEXT("SM_T_Creeper_06.fbx");
		break;
	case Prototype_Mesh_SM_Tree_Big_01:
		return TEXT("SM_T_Creeper_09.fbx");
		break;
	case Prototype_Mesh_SM_Tree_Big_02:
		return TEXT("SM_T_Creeper_11.fbx");
		break;
	case Prototype_Mesh_SM_Tree_Trunk_1:
		return TEXT("SM_T_Creeper_12.fbx");
		break;
	case Prototype_Mesh_SM_tree_trunk2:
		return TEXT("SM_T_Creeper_13.fbx");
		break;
	case Prototype_Mesh_SM_Tropical_Tree_02:
		return TEXT("SM_T_Creepers_10.fbx");
		break;
	case Prototype_Mesh_SM_Tropical_Tree_03:
		return TEXT("SM_T_Creepers_14.fbx");
		break;
	case Prototype_Mesh_SM_TropicalTree_01:
		return TEXT("SM_T_Creepers_15.fbx");
		break;
	case Prototype_Mesh_SM_Trunk_Ground_01:
		return TEXT("SM_T_Roots_01.fbx");
		break;
	case Prototype_Mesh_SM_Vine_Pillar_01:
		return TEXT("SM_T_Roots_02.fbx");
		break;
	case Prototype_Mesh_SM_Vine_Pillar_02:
		return TEXT("SM_T_Roots_03.fbx");
		break;
	case Prototype_Mesh_SM_Vine_Pillar_04:
		return TEXT("SM_Tree_Big_01.fbx");
		break;
	case Prototype_Mesh_SM_Vine_Pillar_05:
		return TEXT("SM_Tree_Big_02.fbx");
		break;
	case Prototype_Mesh_SM_Vine_Pillar_06:
		return TEXT("SM_Tree_Trunk_1.fbx");
		break;
	case Prototype_Mesh_SM_Vine_Pillar_07:
		return TEXT("SM_tree_trunk2.fbx");
		break;
	case Prototype_Mesh_SM_Vine_Pillar_08:
		return TEXT("SM_Tropical_Tree_02.fbx");
		break;
	case Prototype_Mesh_SM_Vines_Ruins_Exterior_ENT_02:
		return TEXT("SM_Tropical_Tree_03.fbx");
		break;
	case Prototype_Mesh_SM_Vines_Straight_004:
		return TEXT("SM_TropicalTree_01.fbx");
		break;
	case Prototype_Mesh_SM_Vines_Straight_005:
		return TEXT("SM_Trunk_Ground_01.fbx");
		break;
	case Prototype_Mesh_SM_Vines_Straight_006:
		return TEXT("SM_Vine_Pillar_01.fbx");
		break;
	case Prototype_Mesh_SM_Wall_Scatter_01A:
		return TEXT("SM_Vine_Pillar_02.fbx");
		break;
	case Prototype_Mesh_SM_Wall_Scatter_01B:
		return TEXT("SM_Vine_Pillar_04.fbx");
		break;
	case Prototype_Mesh_SM_Whiteflower_vine_01:
		return TEXT("SM_Vine_Pillar_05.fbx");
		break;
	case Prototype_Mesh_SM_Whitevine_wall_01:
		return TEXT("SM_Vine_Pillar_06.fbx");
		break;
	case Prototype_Mesh_SM_Whitevine_wall_01a:
		return TEXT("SM_Vine_Pillar_07.fbx");
		break;
	case Prototype_Mesh_SM_Whitevine_wall_02:
		return TEXT("SM_Vine_Pillar_08.fbx");
		break;
	case Prototype_Mesh_SM_Whitevine_wall_03:
		return TEXT("SM_Vines_Ruins_Exterior_ENT_02.fbx");
		break;
	case Prototype_Mesh_SM_Whitevine_wall_04a:
		return TEXT("SM_Vines_Straight_004.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Mandala02_01:
		return TEXT("SM_Vines_Straight_005.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Mandala02_02:
		return TEXT("SM_Vines_Straight_006.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Mandala02_03:
		return TEXT("SM_Wall_Scatter_01A.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Mandala02_04:
		return TEXT("SM_Wall_Scatter_01B.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_Mandala02_05:
		return TEXT("SM_Whiteflower_vine_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_MandalaCircle_01:
		return TEXT("SM_Whitevine_wall_01.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_MandalaCircle_02:
		return TEXT("SM_Whitevine_wall_01a.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_MandalaCircle_03:
		return TEXT("SM_Whitevine_wall_02.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_MandalaCircle_04:
		return TEXT("SM_Whitevine_wall_03.fbx");
		break;
	case Prototype_Mesh_SM_ENV_F_MandalaCircle_05:
		return TEXT("SM_Whitevine_wall_04a.fbx");
		break;

	case Prototype_Mesh_VentQStone:
		return TEXT("VentQStone.fbx");

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

	case Prototype_Mesh_MandalaCircle_0:
		return TEXT("MandalaCircle_0.fbx");

	case Prototype_Mesh_MandalaCircle_1:
		return TEXT("MandalaCircle_1.fbx");

	case Prototype_Mesh_MandalaCircle_2:
		return TEXT("MandalaCircle_2.fbx");

	case Prototype_Mesh_MandalaCircle_3:
		return TEXT("MandalaCircle_3.fbx");

	case Prototype_Mesh_MandalaCircle_4:
		return TEXT("MandalaCircle_4.fbx");

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

	case Prototype_Mesh_Monster_Wasp:
		return TEXT("Monster_Wasp.fbx");

	case Prototype_Mesh_Monster_WormGrub:
		return TEXT("Monster_WormGrub.fbx");

	case Prototype_Mesh_Monster_Spider:
		return TEXT("Monster_Spider.fbx");

	case Prototype_Mesh_Monster_Wolf:
		return TEXT("Monster_Wolf.fbx");

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

	case Prototype_Mesh_FemaleStatue:
		return TEXT("Female_statue.fbx");
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

		case Prototype_Texture_HPUI:
			return TEXT("Prototype_Texture_HPUI");
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
	Com_ColliderSub,
	Com_ColliderSubSub,
	Com_Collider_PhysX,
	Com_SwordTrail,
	Com_SubSwordTrail,
	Com_MotionTrail,
	Com_Navaigation,
	Com_ModelInstance,
	Com_SubTransform,
	Com_Dissolve,
	//JJB
	Com_HPUI,
	Com_Collider_1
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
	case Com_Collider_1:
		return TEXT("Com_Collider_1");
		break;

	case Com_ColliderSub:
		return TEXT("Com_ColliderSub");
		break;

	case Com_ColliderSubSub:
		return TEXT("Com_ColliderSubSub");
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

	case Com_Dissolve:
		return TEXT("Com_Dissolve");
		break;

	case Com_HPUI:
		return TEXT("Com_HPUI");
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