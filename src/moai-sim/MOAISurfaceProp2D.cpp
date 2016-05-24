// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDeckRemapper.h>
#include <moai-sim/MOAIDraw.h>
#include <moai-sim/MOAISurfaceDeck2D.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIMaterialBatch.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAISurfaceProp2D.h>
#include <moai-sim/MOAISurfaceSampler2D.h>

//================================================================//
// MOAIEdgeDebugList
//================================================================//

//----------------------------------------------------------------//
void MOAIEdgeDebugList::Draw () {

	ZLVec2D defaultVec2D ( 0.0f, 0.0f );

	MOAIDraw& draw = MOAIDraw::Get ();
	MOAIGfxDevice& device = MOAIGfxDevice::Get ();

	size_t top = this->mEdgeStream.GetCursor ();

	this->mEdgeStream.Seek ( 0 );
	while ( this->mEdgeStream.GetCursor () < top ) {
	
		u32 color = this->mEdgeStream.Read < u32 >( 0x00000000 );
		float width = this->mEdgeStream.Read < float >( 1.0f );
		ZLVec2D v0 = this->mEdgeStream.Read < ZLVec2D >( defaultVec2D );
		ZLVec2D v1 = this->mEdgeStream.Read < ZLVec2D >( defaultVec2D );
		
		device.SetPenColor ( color );
		device.SetPenWidth ( width );
		
		draw.DrawLine ( v0, v1 );
	}
}

//----------------------------------------------------------------//
MOAIEdgeDebugList::MOAIEdgeDebugList () {
}

//----------------------------------------------------------------//
MOAIEdgeDebugList::~MOAIEdgeDebugList () {
}

//----------------------------------------------------------------//
void MOAIEdgeDebugList::Reset () {

	this->mEdgeStream.Seek ( 0 );
}

//----------------------------------------------------------------//
void MOAIEdgeDebugList::SetPenColor ( u32 color ) {

	this->mPenColor = color;
}

//----------------------------------------------------------------//
void MOAIEdgeDebugList::SetPenColor ( const ZLColorVec& colorVec ) {

	this->SetPenColor ( colorVec.PackRGBA ());
}

//----------------------------------------------------------------//
void MOAIEdgeDebugList::SetPenColor ( float r, float g, float b, float a ) {

	this->SetPenColor ( ZLColor::PackRGBA ( r, g, b, a ));
}

//----------------------------------------------------------------//
void MOAIEdgeDebugList::SetPenWidth ( float width ) {

	this->mPenWidth = width;
}

//----------------------------------------------------------------//
void MOAIEdgeDebugList::WriteEdge ( ZLVec2D v0, ZLVec2D v1 ) {

	this->mEdgeStream.Write < u32 >( this->mPenColor );
	this->mEdgeStream.Write < float >( this->mPenWidth );
	this->mEdgeStream.Write < ZLVec2D >( v0 );
	this->mEdgeStream.Write < ZLVec2D >( v1 );
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
//int MOAISurfaceProp2D::_getStatus ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAISurfaceProp2D, "U" );
//
//	state.Push ( self->mSteps );
//	state.Push ( self->mCompleted );
//	return 2;
//}

//================================================================//
// MOAISurfaceProp2D
//================================================================//

//----------------------------------------------------------------//
u32 MOAISurfaceProp2D::AffirmInterfaceMask ( MOAIPartition& partition ) {

	return partition.AffirmInterfaceMask < MOAIBaseDrawable >() | partition.AffirmInterfaceMask < MOAISurfaceProp2D >();
}

//----------------------------------------------------------------//
void MOAISurfaceProp2D::Draw ( int subPrimID, float lod ) {
	UNUSED ( subPrimID );
	UNUSED ( lod );
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM, this->GetLocalToWorldMtx ());
	gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
	
	MOAIMaterialBatch materials; // TODO: get rid of this
	this->mDeck->Draw ( MOAIDeckRemapper::Remap ( this->mRemapper, this->mIndex ), materials );
}

//----------------------------------------------------------------//
void MOAISurfaceProp2D::GatherSurfaces ( MOAISurfaceSampler2D& sampler ) {
	UNUSED ( sampler );
}

//----------------------------------------------------------------//
MOAISurfaceProp2D::MOAISurfaceProp2D () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIProp )
		RTTI_EXTEND ( MOAIBaseDrawable )
	RTTI_END
}

//----------------------------------------------------------------//
MOAISurfaceProp2D::~MOAISurfaceProp2D () {
}

//----------------------------------------------------------------//
u32 MOAISurfaceProp2D::OnGetModelBounds ( ZLBox& bounds ) {

	return BOUNDS_GLOBAL;
}

//----------------------------------------------------------------//
void MOAISurfaceProp2D::OnSetDeck ( MOAIDeck* deck ) {

	this->mSurfaceDeck = deck->AsType < MOAISurfaceDeck2D >();
}

//----------------------------------------------------------------//
void MOAISurfaceProp2D::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIProp::RegisterLuaClass ( state );
	MOAIBaseDrawable::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAISurfaceProp2D::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIProp::RegisterLuaFuncs ( state );
	MOAIBaseDrawable::RegisterLuaFuncs ( state );
}

//----------------------------------------------------------------//
void MOAISurfaceProp2D::SampleSurfaces ( MOAISurfaceSampler2D& sampler ) {

	if ( this->mSurfaceDeck ) {
	
		this->mSurfaceDeck->SampleSurfaces ( this->mIndex, sampler );
	}
}
