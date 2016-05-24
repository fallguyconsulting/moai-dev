// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISIMPLESURFACEFEELER2D_H
#define	MOAISIMPLESURFACEFEELER2D_H

#include <moai-sim/MOAIProp.h>
#include <moai-sim/MOAISurfaceProp2D.h>

class MOAISurfaceBuffer2D;
class MOAIPartition;
class MOAIGrid;

//================================================================//
// MOAISimpleSurfaceFeeler2D
//================================================================//
// TODO: doxygen
class MOAISimpleSurfaceFeeler2D :
	public MOAIProp,
	public MOAIAction,
	public MOAIBaseDrawable {
private:
	
	friend class MOAISimpleSurfaceFeelerState2D;
	
	float		mFloorAngle;
	float		mFloorCos;
	
	float		mCeilAngle;
	float		mCeilCos;
	
	ZLVec2D		mMove;
	float		mHRad;
	float		mVRad;
	
	float		mSkirt;
	
	u32			mSteps;
	bool		mCompleted;
	
	u32			mDetachMode;
	
	bool		mIsStanding;
	
	ZLRect				mDebugBounds; // used for debug drawing only
	MOAIEdgeDebugList	mDebugLines; // TODO: make optional
	
	//----------------------------------------------------------------//
	static int			_getStatus					( lua_State* L );
	static int			_setCeilingAngle			( lua_State* L );
	static int			_setEllipse					( lua_State* L );
	static int			_setFloorAngle				( lua_State* L );
	static int			_setMove					( lua_State* L );
	
	//----------------------------------------------------------------//
	u32					AffirmInterfaceMask			( MOAIPartition& partition );
	void				GatherSurfacesForBounds		( MOAISurfaceSampler2D& sampler, const ZLBox& bounds );
	void				GetTouching					( ZLVec2D& loc, MOAISurfaceSampler2D& sampler );
	ZLRect				GetUnitRectForWorldBounds	( const ZLBox& bounds );
	ZLAffine3D			GetUnitToWorldMtx			();
	ZLAffine3D			GetWorldToUnitMtx			();
	void				SetCeilingAngle				( float angle );
	void				SetFloorAngle				( float angle );

public:
	
	enum {
		DETACH_MODE_NONE,
		DETACH_ON_UP,
		DETACH_ON_ANY,
	};
	
	DECL_LUA_FACTORY ( MOAISimpleSurfaceFeeler2D )
	
	IS ( Standing, mIsStanding, true )
	
	//----------------------------------------------------------------//
	void			Draw							( int subPrimID, float lod );
	bool			IsDone							();
					MOAISimpleSurfaceFeeler2D		();
					~MOAISimpleSurfaceFeeler2D		();
	void			OnDepNodeUpdate					();
	u32				OnGetModelBounds				( ZLBox& bounds );
	void			OnUpdate						( double step );
	void			RegisterLuaClass				( MOAILuaState& state );
	void			RegisterLuaFuncs				( MOAILuaState& state );
	void			SetMove							( float x, float y );
};

#endif
