// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultMgr.h>
#include <moai-sim/MOAISimpleSurfaceFeeler2D.h>
#include <moai-sim/MOAISimpleSurfaceFeelerState2D.h>
#include <moai-sim/MOAISurfaceProp2D.h>
#include <moai-sim/MOAISurfaceSampler2D.h>

//================================================================//
// MOAISimpleSurfaceFeelerState2D
//================================================================//

//----------------------------------------------------------------//
ZLVec2D MOAISimpleSurfaceFeelerState2D::ClipMoveToBounds ( const ZLVec2D& loc, ZLVec2D move ) {
	
//	float t;
//	if (( ZLSect::RayToRect ( loc, move, this->mBounds, t ) == ZLSect::SECT_HIT ) && ( t < 1.0f )) {
//		move.Scale ( t );
//	}
	return move;
}

//----------------------------------------------------------------//
bool MOAISimpleSurfaceFeelerState2D::DoMove () {

	this->mMove = this->ClipMoveToBounds ( this->mLoc, this->mMove );

	if ( this->mMove.LengthSquared () < 0.00001f ) return false; // done

	SurfaceHit2D hit;
	hit.mTime = 1.0f;
	
	const ZLSurface2D* surface = 0;

	for ( u32 i = 0; i < this->mSurfaceBuffer.mTop; ++i ) {
	
		const MOAISurface2D& testSurface = this->mSurfaceBuffer.mSurfaces [ i ];
		
		if ( !testSurface.GetHit ( this->mLoc, this->mMove, hit )) continue;
		surface = &testSurface;
	}

	if ( surface ) {

		this->mLoc.Add ( this->mMove, hit.mTime );
		this->mMove.Scale ( 1.0f - hit.mTime );
		this->mMove.PerpProject ( hit.mNorm );
		
		return true; // more
	}
	
	this->mLoc.Add ( this->mMove );
	this->mMove.Init ( 0.0f, 0.0f );
	
	return false; // done
}

//----------------------------------------------------------------//
bool MOAISimpleSurfaceFeelerState2D::DoShove () {

	// shove
	u32 count = 0;
	ZLVec2D shove ( 0.0f, 0.0f );
	
	for ( u32 i = 0; i < this->mSurfaceBuffer.mTop; ++i ) {
	
		const MOAISurface2D& testSurface = this->mSurfaceBuffer.mSurfaces [ i ];
		
		ZLVec2D contact;
		ZLVec2D norm;
		float dist;
		
		if ( testSurface.GetContact ( this->mLoc, contact, norm, dist )) {
		
			if ( dist < 1.0f ) {
				norm.Scale ( 1.0f - dist );
				shove.Add ( norm );
				count++;
			}
		}
	}
	
	if ( count ) {
		float div = 1.0f / ( float )count;
		shove.Scale ( div );
		this->mLoc.Add ( shove );
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
ZLBox MOAISimpleSurfaceFeelerState2D::GetWorldBoundsForMove ( MOAISimpleSurfaceFeeler2D& body ) {

	ZLBox worldBounds = body.GetBounds ();
	worldBounds.Bless ();
	//worldBounds.Inflate ( 1.0f );

	ZLBox offsetBounds = worldBounds;
	ZLVec3D offset ( body.mMove.mX * 2.0f, body.mMove.mY * 2.0f, 0.0f );
	offsetBounds.Offset ( offset );

	worldBounds.Grow ( offsetBounds );

//	worldBounds.mMin.mX -= body.mHRad;
//	worldBounds.mMax.mX += body.mHRad;
//
//	worldBounds.mMin.mY -= body.mVRad;
//	worldBounds.mMax.mY += body.mVRad;
	
	return worldBounds;
}

//----------------------------------------------------------------//
void MOAISimpleSurfaceFeelerState2D::Move ( MOAISimpleSurfaceFeeler2D& body ) {

	this->mLoc.Init ( 0.0f, 0.0f );
	
	this->mMove = body.mMove;
	this->mMove.Scale ( 1.0f / body.mHRad, 1.0f / body.mVRad );
	
	ZLBox worldBounds = this->GetWorldBoundsForMove ( body );
	body.GatherSurfacesForBounds ( this->mSurfaceBuffer, worldBounds );
	this->mBounds = body.GetUnitRectForWorldBounds ( worldBounds );
	body.mDebugBounds = this->mBounds;

	this->DoShove ();
	
	for ( u32 steps = 0; this->DoMove () && ( steps < MAX_STEPS ); ++steps );
	
	body.mLoc.mX += this->mLoc.mX * body.mHRad;
	body.mLoc.mY += this->mLoc.mY * body.mVRad;
	body.mMove.Init ( 0.0f, 0.0f );
}
