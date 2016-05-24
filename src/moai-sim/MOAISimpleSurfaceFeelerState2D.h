// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISIMPLESURFACEFEELERSTATE2D_H
#define	MOAISIMPLESURFACEFEELERSTATE2D_H

#include <moai-sim/MOAISurfaceSampler2D.h>

class MOAISimpleSurfaceFeeler2D;

//================================================================//
// MOAISimpleSurfaceFeelerState2D
//================================================================//
class MOAISimpleSurfaceFeelerState2D {
private:
	
	friend class MOAISimpleSurfaceFeeler2D;
	
	static const u32		MAX_STEPS = 8;
	
	MOAISurfaceSampler2D	mSurfaceBuffer;
	
	ZLVec2D					mMove;					// current move (original vector)
	
	ZLVec2D					mLoc;
	
	ZLRect					mBounds;
	
	//----------------------------------------------------------------//
	ZLVec2D		ClipMoveToBounds				( const ZLVec2D& loc, ZLVec2D move );
	bool		DoMove							();
	bool		DoShove							();
	ZLBox		GetWorldBoundsForMove			( MOAISimpleSurfaceFeeler2D& body );
	void		Move							( MOAISimpleSurfaceFeeler2D& body );
	void		SetCeiling						( const MOAISurface2D& ceiling );
	void		SetFloor						( const MOAISurface2D& floor );
};

#endif
