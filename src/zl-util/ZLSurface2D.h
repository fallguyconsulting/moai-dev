// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLSURFACE2D_H
#define ZLSURFACE2D_H

#include <zl-util/ZLMatrix3x3.h>
#include <zl-util/ZLPlane.h>

class USCollPrim2D;
class SurfaceHit2D;
class SurfaceSnap2D;

//================================================================//
// ZLEdge2D
//================================================================//
class ZLEdge2D {
public:
	ZLVec2D		mV0;
	ZLVec2D		mV1;
};

//================================================================//
// ZLSurfaceTouch2D
//================================================================//
class ZLSurfaceTouch2D {
public:

	// input
	ZLVec2D		mFinger; // touch vector (unit vector)

	// internal
	float		mDist;
	bool		mTouch;
	bool		mPrevTouch;
	bool		mHit;

	// output
	ZLVec2D		mPoint; // touch point
	ZLVec2D		mNorm;
	
	//----------------------------------------------------------------//
	void		Init			( ZLVec2D finger );
	void		Reset			( float dist = 1.25f );
	void		Scale			( float xSc, float ySc );
};

//================================================================//
// ZLSurface2D
//================================================================//
class ZLSurface2D :
	public ZLPlane2D {
public:

	ZLVec2D			mE0;
	ZLVec2D			mE1;

	ZLVec2D			mTangent; // perp to norm
	
	float			mP0;
	float			mP1;
	
	float			mXMin;
	float			mXMax;
	
	//----------------------------------------------------------------//
	void			ClampPoint			( ZLVec2D& p ) const;
	bool			GetContact			( ZLVec2D& sphereLoc, ZLVec2D& contact, ZLVec2D& norm, float& dist ) const;
	float			GetDepthAlongRay	( ZLVec2D& sphereLoc, ZLVec2D& ray ) const;
	bool			GetHit				( ZLVec2D& sphereLoc, ZLVec2D& move, SurfaceHit2D& hit ) const; // unit sphere
	static ZLVec2D	GetNorm				( const ZLVec2D& e0, const ZLVec2D& e1 );
	bool			GetRayHit			( ZLVec2D& loc, ZLVec2D& ray, float& time ) const; // ray, no edges
	bool			GetRayHit			( ZLVec2D& loc, ZLVec2D& ray, float pad, float& time ) const; // ray
	void			GetSnapUp			( ZLVec2D& loc, float maxSnap, ZLVec2D& move, SurfaceSnap2D& snap ) const;
	bool			GetTouch			( ZLVec2D& sphereLoc, ZLSurfaceTouch2D& touch ) const; // unit sphere
	void			GetVertices			( ZLVec2D& v0, ZLVec2D& v1 ) const;
	void			Init				( const ZLVec2D& e0, const ZLVec2D& e1 );
	bool			IsBridge			( ZLVec2D& loc, ZLVec2D& move ) const;
	bool			IsLeaving			( ZLVec2D& loc, ZLVec2D& move ) const;
	bool			IsOn				( ZLVec2D& loc ) const;
	bool			IsOver				( ZLVec2D& loc ) const;
};

//================================================================//
// SurfaceHit2D
//================================================================//
class SurfaceHit2D {
public:

	float		mTime;
	ZLVec2D		mPoint;
	ZLVec2D		mNorm;
};

//================================================================//
// SurfaceSnap2D
//================================================================//
class SurfaceSnap2D {
public:

	bool		mSnap;
	float		mSnapDist;
	ZLSurface2D	mSurface;
};

#endif
