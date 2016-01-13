// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISURFACEFEELERSTATE2D_H
#define	MOAISURFACEFEELERSTATE2D_H

#include <moai-sim/MOAISurfaceSampler2D.h>

//================================================================//
// MOAISurfaceFeelerState2D
//================================================================//
class MOAISurfaceFeelerState2D {
private:
	
	friend class MOAISurfaceFeeler2D;
	
	static const u32	MAX_STEPS = 8;
	
	MOAISurfaceSampler2D	mSurfaceBuffer;
	
	float					mFloorCos;
	float					mCeilCos;
	
	u32						mState;
	
	ZLVec2D					mMove;					// current move (original vector)
	
	bool					mWallToLeft;
	bool					mWallToRight;
	
	float					mLeftWallDepth;
	float					mRightWallDepth;
	
	float					mMoveDistOnFloor;		// distance to move along floor (scalar on floor tangent)
	float					mShoveDistOnFloor;		// distance to shove along floor (scalar on floor tangent)
	float					mShoveDistInAir;
	
	ZLVec2D					mLoc;
	ZLVec2D					mUp;
	
	const MOAISurface2D*	mFloor;				// may be nil even if state is ON_FLOOR ('ghost' platform from snap)
	ZLVec2D					mFloorNorm;
	ZLVec2D					mFloorTangent;
	
	ZLRect					mBounds;
	
	enum {
		STATE_DONE,
		STATE_IN_AIR,
		STATE_ON_FLOOR,
	};
	
	//----------------------------------------------------------------//
	void		CalculateWallDepthInAir			();
	void		CalculateWallDepthOnFloor		();
	void		CalculateWallShoveInAir			();
	void		CalculateWallShoveOnFloor		();
	ZLVec2D		ClipMoveToBounds				( const ZLVec2D& loc, ZLVec2D move );
	void		DoMoveInAir						();
	void		DoMoveOnFloor					();
	void		DoWallSnapInAir					();
	void		DoVerticalSnap					();
	ZLBox		GetWorldBoundsForMove			( MOAISurfaceFeeler2D& body );
	void		Move							( MOAISurfaceFeeler2D& body );
	void		SetCeiling						( const MOAISurface2D& ceiling );
	void		SetFloor						( const MOAISurface2D& floor );
};

#endif
