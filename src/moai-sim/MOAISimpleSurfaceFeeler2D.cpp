// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDraw.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultMgr.h>
#include <moai-sim/MOAISimpleSurfaceFeeler2D.h>
#include <moai-sim/MOAISimpleSurfaceFeelerState2D.h>
#include <moai-sim/MOAISurfaceProp2D.h>
#include <moai-sim/MOAISurfaceSampler2D.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAISimpleSurfaceFeeler2D::_getStatus ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISimpleSurfaceFeeler2D, "U" );

	state.Push ( self->mSteps );
	state.Push ( self->mCompleted );
	return 2;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAISimpleSurfaceFeeler2D::_setCeilingAngle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISimpleSurfaceFeeler2D, "U" );

	float angle = state.GetValue < float >( 2, 0.0f );
	self->SetCeilingAngle ( angle );
	self->ScheduleUpdate ();
	return 0;
}
//----------------------------------------------------------------//
// TODO: doxygen
int MOAISimpleSurfaceFeeler2D::_setEllipse ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISimpleSurfaceFeeler2D, "U" );

	self->mHRad = state.GetValue < float >( 2, 32.0f );
	self->mVRad = state.GetValue < float >( 3, self->mHRad );
	self->ScheduleUpdate ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAISimpleSurfaceFeeler2D::_setFloorAngle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISimpleSurfaceFeeler2D, "U" );

	float angle = state.GetValue < float >( 2, 0.0f );
	self->SetFloorAngle ( angle );
	self->ScheduleUpdate ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAISimpleSurfaceFeeler2D::_setMove ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISimpleSurfaceFeeler2D, "U" );
	
	self->mMove.mX = state.GetValue < float >( 2, self->mMove.mX );
	self->mMove.mY = state.GetValue < float >( 3, self->mMove.mY );
	
	return 0;
}

//================================================================//
// MOAISimpleSurfaceFeeler2D
//================================================================//

//----------------------------------------------------------------//
u32 MOAISimpleSurfaceFeeler2D::AffirmInterfaceMask ( MOAIPartition& partition ) {

	return partition.AffirmInterfaceMask < MOAIBaseDrawable >();
}

//----------------------------------------------------------------//
void MOAISimpleSurfaceFeeler2D::Draw ( int subPrimID, float lod ) {
	UNUSED ( subPrimID );
	UNUSED ( lod );

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008
	
	draw.Bind ();
	
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM, this->GetUnitToWorldMtx ());
	gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
	
	gfxDevice.SetPenColor ( 0xffffffff );
	gfxDevice.SetPenWidth ( 1.0f );
	
	draw.DrawRectOutline ( this->mDebugBounds );
	
//	gfxDevice.SetPenColor ( 0x7f7f7f7f );
//	gfxDevice.SetPenWidth ( 1.0f );
//	ZLAffine3D worldToUnit = this->GetWorldToUnitMtx ();
//	ZLRect debugBoundsRect = this->mDebugBounds.GetRect ( ZLBox::PLANE_XY );
//	worldToUnit.Transform ( debugBoundsRect );
//	draw.DrawRectOutline ( debugBoundsRect );
	
	gfxDevice.SetPenColor ( 0x7f7f7f7f );
	gfxDevice.SetPenWidth ( 1.0f );
	draw.DrawEllipseOutline ( 0.0f, 0.0f, 1.0f, 1.0f, 32 );
//	draw.DrawLine ( 0.0f, 0.0f, 0.0f, -( 1.0f + ( this->mSkirt / this->mVRad )));
	
//	gfxDevice.SetPenColor ( 0xffffffff );
//	gfxDevice.SetPenWidth ( 2.0f );
//	draw.DrawEllipseArcOutline ( 0.0f, 0.0f, 1.0f, 1.0f, this->mCeilAngle, 180.0f - this->mFloorAngle, 8 );
//	draw.DrawEllipseArcOutline ( 0.0f, 0.0f, 1.0f, 1.0f, 180.0f + this->mFloorAngle, 360.0f - this->mCeilAngle, 8 );

	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM );

	this->mDebugLines.Draw ();
	this->mDebugLines.Reset ();
}

//----------------------------------------------------------------//
void MOAISimpleSurfaceFeeler2D::GatherSurfacesForBounds ( MOAISurfaceSampler2D& sampler, const ZLBox& bounds ) {
	UNUSED ( bounds );

	MOAIPartition* partition = this->GetPartition ();
	if ( !partition ) return;
	
	u32 interfaceMask = partition->GetInterfaceMask < MOAISurfaceProp2D >();
	if ( !interfaceMask ) return;
	
	MOAIPartitionResultBuffer& props = MOAIPartitionResultMgr::Get ().GetBuffer ();
	
	u32 totalResults = partition->GatherProps ( props, this, interfaceMask );
//	u32 totalResults = partition->GatherProps ( props, 0, bounds, 0 );
	
	if ( totalResults ) {

		sampler.Init ( this->GetWorldToUnitMtx (), bounds.GetRect ( ZLBox::PLANE_XY ));
		
		for ( u32 i = 0; i < totalResults; ++i ) {
			MOAIPartitionResult* result = props.GetResultUnsafe ( i );
			MOAISurfaceProp2D* prop = result->mProp->AsType < MOAISurfaceProp2D >();
			sampler.SetSourcePrim ( prop );
			prop->SampleSurfaces ( sampler );
		}
		
		if ( sampler.mTop ) {
		
			ZLAffine3D unitToWorld = this->GetUnitToWorldMtx ();
			
			this->mDebugLines.SetPenColor ( 0xff0000ff );
			this->mDebugLines.SetPenWidth ( 2.0f );
			
			for ( size_t i = 0; i < sampler.mTop; ++i ) {
				MOAISurface2D& surface = sampler.mSurfaces [ i ];
				
				ZLVec2D v0;
				ZLVec2D v1;
				
				surface.GetVertices ( v0, v1 );
				
				unitToWorld.Transform ( v0 );
				unitToWorld.Transform ( v1 );
				
				this->mDebugLines.WriteEdge ( v0, v1 );
			}
		}
	}
}

//----------------------------------------------------------------//
//u32 MOAISimpleSurfaceFeeler2D::GetLocalFrame ( ZLRect& frame ) {
//	
//	frame.mXMin = -this->mHRad;
//	frame.mYMin = -( this->mVRad + this->mSkirt );
//	frame.mXMax = this->mHRad;
//	frame.mYMax = this->mVRad;
//	
//	return FRAME_OK;
//}

//----------------------------------------------------------------//
ZLRect MOAISimpleSurfaceFeeler2D::GetUnitRectForWorldBounds ( const ZLBox& bounds ) {

	ZLVec3D loc = this->GetWorldLoc ();
	ZLRect rect = bounds.GetRect ( ZLBox::PLANE_XY );
	rect.Offset ( -loc.mX, -loc.mY );
	rect.Scale ( 1.0f / this->mHRad, 1.0f / this->mVRad );
	return rect;
}

//----------------------------------------------------------------//
ZLAffine3D MOAISimpleSurfaceFeeler2D::GetUnitToWorldMtx () {
	
	ZLAffine3D transform;
	
	transform.Scale (
		this->mHRad,
		this->mVRad,
		1.0f
	);
	
	transform.Append ( this->GetLocalToWorldMtx ());
	return transform;
}

//----------------------------------------------------------------//
ZLAffine3D MOAISimpleSurfaceFeeler2D::GetWorldToUnitMtx () {
	
	ZLAffine3D transform;
	
	transform.Scale (
		1.0f / this->mHRad,
		1.0f / this->mVRad,
		1.0f
	);
	
	transform.Prepend ( this->GetWorldToLocalMtx ());
	return transform;
}

//----------------------------------------------------------------//
bool MOAISimpleSurfaceFeeler2D::IsDone () {

	return false;
}

//----------------------------------------------------------------//
MOAISimpleSurfaceFeeler2D::MOAISimpleSurfaceFeeler2D () :
	mMove ( 0.0f, 0.0f ),
	mHRad ( 32.0f ),
	mVRad ( 32.0f ),
	mSkirt ( 0.0f ),
	mSteps ( 0 ),
	mCompleted ( true ),
	mDetachMode ( DETACH_ON_UP ),
	mIsStanding ( false ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIProp )
		RTTI_EXTEND ( MOAIAction )
		RTTI_EXTEND ( MOAIBaseDrawable )
	RTTI_END
	
	//this->SetQueryMask ( MOAIContentLibrary2D::CAN_DRAW_DEBUG );
	
	this->SetFloorAngle ( 60.0f );
	this->SetCeilingAngle ( 120.0f );
}

//----------------------------------------------------------------//
MOAISimpleSurfaceFeeler2D::~MOAISimpleSurfaceFeeler2D () {
}

//----------------------------------------------------------------//
void MOAISimpleSurfaceFeeler2D::OnDepNodeUpdate () {
	
//	MOAISurfaceFeelerState2D fsm;
//	this->BuildTransforms (); // not sure about this here
//	fsm.Move ( *this );
	MOAIProp::OnDepNodeUpdate ();
}

//----------------------------------------------------------------//
u32 MOAISimpleSurfaceFeeler2D::OnGetModelBounds ( ZLBox& bounds ) {

	ZLRect rect;

	rect.mXMin = -this->mHRad;
	rect.mYMin = -( this->mVRad + this->mSkirt );
	rect.mXMax = this->mHRad;
	rect.mYMax = this->mVRad;
	
	bounds.Init ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMin, 0.0f, 0.0f );
	return BOUNDS_OK;
}

//----------------------------------------------------------------//
void MOAISimpleSurfaceFeeler2D::OnUpdate ( double step ) {
	
	if (( this->mMove.mX != 0.0f ) || ( this->mMove.mY != 0.0f )) {
	
		MOAISimpleSurfaceFeelerState2D state;
		state.Move ( *this );
		
		this->mMove.Init ( 0.0f, 0.0f );
	
		this->ScheduleUpdate ();
	}
}

//----------------------------------------------------------------//
void MOAISimpleSurfaceFeeler2D::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIProp::RegisterLuaClass ( state );
	MOAIAction::RegisterLuaClass ( state );
	MOAIBaseDrawable::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAISimpleSurfaceFeeler2D::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIProp::RegisterLuaFuncs ( state );
	MOAIAction::RegisterLuaFuncs ( state );
	MOAIBaseDrawable::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "getStatus",				_getStatus },
		{ "setCeilingAngle",		_setCeilingAngle },
		{ "setEllipse",				_setEllipse },
		{ "setFloorAngle",			_setFloorAngle },
		{ "setMove",				_setMove },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAISimpleSurfaceFeeler2D::SetCeilingAngle ( float angle ) {

	this->mCeilAngle = angle;
	this->mCeilCos = -Cos ( angle * ( float )D2R );
}

//----------------------------------------------------------------//
void MOAISimpleSurfaceFeeler2D::SetFloorAngle ( float angle ) {

	this->mFloorAngle = angle;
	this->mFloorCos = Cos ( angle * ( float )D2R );
}

//----------------------------------------------------------------//
void MOAISimpleSurfaceFeeler2D::SetMove ( float x, float y ) {

	this->mMove.Init ( x, y );
	this->ScheduleUpdate ();
}



