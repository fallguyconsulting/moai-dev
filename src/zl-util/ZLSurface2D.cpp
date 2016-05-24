// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLDistance.h>
#include <zl-util/ZLIntersect.h>
#include <zl-util/ZLSurface2D.h>

//================================================================//
// ZLSurfaceTouch2D
//================================================================//

//----------------------------------------------------------------//
void ZLSurfaceTouch2D::Init ( ZLVec2D finger ) {

	this->mTouch = false;
	this->mPrevTouch = false;
	this->mHit = false;
	this->mFinger = finger;
	this->mFinger.Norm ();
}

//----------------------------------------------------------------//
void ZLSurfaceTouch2D::Reset ( float dist ) {

	this->mDist = dist;
	
	this->mPrevTouch = this->mTouch;
	this->mTouch = false;
	this->mHit = false;
	this->mNorm.Init ( 0.0f, 0.0f );
}

//----------------------------------------------------------------//
void ZLSurfaceTouch2D::Scale ( float xSc, float ySc ) {

	this->mPoint.Scale ( xSc, ySc );
}

//================================================================//
// ZLSurface2D
//================================================================//

//----------------------------------------------------------------//
void ZLSurface2D::ClampPoint ( ZLVec2D& p ) const {

	ZLDist::SnapToPlane2D ( p, *this );
	
	float edgeDist = this->mTangent.Dot ( p );
	
	if ( edgeDist < this->mP0 ) {
	
		ZLVec2D offset = this->mTangent;
		offset.Scale ( this->mP0 - edgeDist );
		p.Add ( offset );
	}
	else if ( edgeDist > this->mP1 ) {
	
		ZLVec2D offset = this->mTangent;
		offset.Scale ( this->mP1 - edgeDist );
		p.Add ( offset );
	}
}

//----------------------------------------------------------------//
bool ZLSurface2D::GetContact ( ZLVec2D& sphereLoc, ZLVec2D& contact, ZLVec2D& norm, float& dist ) const {

	// The usual stuff...
	dist = ZLDist::PointToPlane2D ( sphereLoc, *this );
	if ( dist <= 0.0f ) return false;
	if ( dist > 1.0f ) return false;
	
	// Get the point of first contact on the polygon...
	contact = this->mNorm;
	contact.Reverse ();
	contact.Add ( sphereLoc );
	this->ClampPoint ( contact );

	norm = sphereLoc;
	norm.Sub ( contact );
	dist = norm.NormSafe ();
	
	return dist <= 1.0f;
}

//----------------------------------------------------------------//
float ZLSurface2D::GetDepthAlongRay ( ZLVec2D& sphereLoc, ZLVec2D& ray ) const {
	
	// Get the point of first contact on the polygon...
	ZLVec2D pofcop = this->mNorm;
	pofcop.Reverse ();
	pofcop.Add ( sphereLoc );
	this->ClampPoint ( pofcop );

	float t0, t1;
	u32 sectType;
	sectType = ZLSect::VecToCircle ( t0, t1, pofcop, ray, sphereLoc, 1.0f );

	// Bail if the point will not intersect the sphere.
	if ( sectType == ZLSect::SECT_PARALLEL ) return 0.0f;
	if ( sectType == ZLSect::SECT_TANGENT ) return 0.0f;

	return t0;
}

//----------------------------------------------------------------//
bool ZLSurface2D::GetHit ( ZLVec2D& sphereLoc, ZLVec2D& move, SurfaceHit2D& hit ) const {

	// The usual stuff...
	ZLVec2D unitMove = move;
	unitMove.Norm ();
	
	if ( unitMove.Dot ( this->mNorm ) >= -0.001f ) return false;
	if ( ZLDist::PointToPlane2D ( sphereLoc, *this ) <= 0.0f ) return false;
	
	// Get the point of first contact on the polygon...
	ZLVec2D pofcop = this->mNorm;
	pofcop.Reverse ();
	pofcop.Add ( sphereLoc );
	this->ClampPoint ( pofcop );

	// Send a ray from the point on the surface to intersect the circle.
	// The ray is the inverse of the move vec.
	ZLVec2D inverseMove = move;
	inverseMove.Reverse ();

	float t0, t1;
	u32 sectType;
	sectType = ZLSect::VecToCircle ( t0, t1, pofcop, inverseMove, sphereLoc, 1.0f );

	// Bail if the point will not intersect the sphere.
	if ( sectType == ZLSect::SECT_PARALLEL ) return false;

	if ( t0 >= hit.mTime ) return false;

	// Bail if the point will graze the sphere.
	if ( sectType == ZLSect::SECT_TANGENT ) return false;

	// Bail if the point will stay outside of the sphere.
	if (( t0 > 1.0f ) || ( t1 < 0.0f )) return false;

	// OMG!  We hit something!

	float time = t0;
	
	hit.mTime = time;

	inverseMove.Scale ( t0 );
	hit.mPoint = pofcop;
	hit.mPoint.Add ( inverseMove );
	
	hit.mNorm = sphereLoc;
	hit.mNorm.Sub ( hit.mPoint );
	hit.mNorm.Norm ();
	
	if ( unitMove.Dot ( hit.mNorm ) >= -0.001f ) return false;
	
	return true;
}

//----------------------------------------------------------------//
ZLVec2D ZLSurface2D::GetNorm ( const ZLVec2D& e0, const ZLVec2D& e1 ) {

	ZLVec2D norm;

	norm = e0;
	norm.Sub ( e1 );
	norm.Rotate90Anticlockwise ();
	norm.Norm ();
	
	return norm;
}

//----------------------------------------------------------------//
bool ZLSurface2D::GetRayHit ( ZLVec2D& loc, ZLVec2D& ray, float& time ) const {

	float d;
	d = ray.Dot ( this->mNorm );
	if ( d == 0.0f ) return false; // ray is parallel
	
	time = ( loc.Dot ( this->mNorm ) + this->mDist ) / -d;
	
	return true;
}

//----------------------------------------------------------------//
bool ZLSurface2D::GetRayHit ( ZLVec2D& loc, ZLVec2D& ray, float pad, float& time ) const {

	float d;
	d = ray.Dot ( this->mNorm );
	if ( d == 0.0f ) return false;
	
	time = ( loc.Dot ( this->mNorm ) + this->mDist ) / -d;
	
	// now get the point of impact
	ZLVec2D sect = ray;
	sect.Scale ( time );
	sect.Add ( loc );
	
	float dot = this->mTangent.Dot ( sect );
	
	if ( dot < ( this->mP0 - pad )) return false;
	if ( dot > ( this->mP1 + pad )) return false;
	
	return true;
}

//----------------------------------------------------------------//
void ZLSurface2D::GetSnapUp ( ZLVec2D& loc, float maxSnap, ZLVec2D& move, SurfaceSnap2D& snap ) const {

	if ( this->mNorm.mY < 0.0f ) return;
	//if ( !this->IsOver ( loc )) return;

	//if ( this->IsLeaving ( loc, move )) return;

	ZLVec2D up ( 0.0f, 1.0f );
	float snapDist = 0.0f; // dist will be returned here
	if ( this->GetRayHit ( loc, up, snapDist ))  {
		
		if ( snapDist > maxSnap ) return; // return if snap is above us
		if ( snapDist < snap.mSnapDist ) return; // return if snap is lower than last best snap
		
		// 'snap' is true if we already have a valid snap
		if ( snap.mSnap ) {
			
			// looks like we have multiple floors to choose from...
			if ( snapDist > snap.mSnapDist ) {
			
				// we have a clear winner
				snap.mSurface = *this;
				snap.mSnapDist = snapDist;
			}
			else {
				
				// snap is the same as the last snap...
				
				// break the tie
				// if there's a move, choose the surface with the steepest angle
				// (against the move)
				if ( move.mX > 0.0f ) {
					if ( this->mNorm.mX < snap.mSurface.mNorm.mX ) {
						snap.mSurface = *this;
					}
				}
				else if ( move.mX < 0.0f ) {
					if ( this->mNorm.mX > snap.mSurface.mNorm.mX ) {
						snap.mSurface = *this;
					}
				}
				//else {
				//	if ( this->mNorm.mY < snap.mSurface.mNorm.mY ) {
				//		snap.mSurface = *this;
				//	}
				//}
			}
		}
		else {
		
			// first floor, so go with it
			snap.mSurface = *this;
			snap.mSnapDist = snapDist;
			snap.mSnap = true;
		}
		
		if ( snapDist > snap.mSnapDist ) {
			snap.mSnapDist = snapDist;
		}
	}
}

//----------------------------------------------------------------//
bool ZLSurface2D::GetTouch ( ZLVec2D& sphereLoc, ZLSurfaceTouch2D& touch ) const {

	// The usual stuff...
	float dist = ZLDist::PointToPlane2D ( sphereLoc, *this );
	if ( dist <= 0.0f ) return false;
	if ( dist > 1.001f ) return false;
	
	// Get the point of first contact on the polygon...
	ZLVec2D pofcop = this->mNorm;
	pofcop.Reverse ();
	pofcop.Add ( sphereLoc );
	this->ClampPoint ( pofcop );

	ZLVec2D ray = pofcop;
	ray.Sub ( sphereLoc );
	dist = ray.NormSafe ();
	
	if ( dist >= touch.mDist ) return false;

	float dot = ray.Dot ( touch.mFinger );
	if ( dot < 0.85f ) return false;

	// we have a touch...
	touch.mDist = dist;
	touch.mPoint = pofcop;
	touch.mTouch = true;
	touch.mHit = !touch.mPrevTouch;
	//touch.mNorm = this->GetNorm ();
	
	return true;
}

//----------------------------------------------------------------//
void ZLSurface2D::GetVertices ( ZLVec2D& v0, ZLVec2D& v1 ) const {

	v0 = this->mE0;
	v1 = this->mE1;
}

//----------------------------------------------------------------//
void ZLSurface2D::Init ( const ZLVec2D& e0, const ZLVec2D& e1 ) {

	this->mE0 = e0;
	this->mE1 = e1;

	ZLVec2D worldNorm = this->GetNorm ( e0, e1 );

	this->ZLPlane2D::Init ( e0, e1 );
	
	this->mTangent = this->mNorm;
	this->mTangent.Rotate90Anticlockwise ();
	
	this->mP0 = this->mTangent.Dot ( e0 );
	this->mP1 = this->mTangent.Dot ( e1 );
	
	if ( this->mP1 < this->mP0 ) {
		float p = this->mP0;
		this->mP0 = this->mP1;
		this->mP1 = p;
	}
	
	if ( e0.mX < e1.mX ) {
		this->mXMin = e0.mX;
		this->mXMax = e1.mX;
	}
	else {
		this->mXMin = e1.mX;
		this->mXMax = e0.mX;
	}
}

//----------------------------------------------------------------//
bool ZLSurface2D::IsBridge ( ZLVec2D& loc, ZLVec2D& move ) const {

	ZLVec2D destLoc = loc;
	destLoc.Add ( move );
	
	return ( this->IsOn ( destLoc ));
}

//----------------------------------------------------------------//
bool ZLSurface2D::IsLeaving ( ZLVec2D& loc, ZLVec2D& move ) const {

	if ( move.mX > 0.0f ) {
		if ( loc.mX >= ( this->mXMax - 0.001f )) {
			return true;
		}
	}
	
	if ( move.mX < 0.0f ) {
		if ( loc.mX <= ( this->mXMin + 0.001f )) {
			return true;
		}
	}
	
	return false;
}

//----------------------------------------------------------------//
bool ZLSurface2D::IsOn ( ZLVec2D& loc ) const {

	if ( this->IsOver ( loc )) {

		float dist = ZLDist::PointToPlane2D ( loc, *this );
		return (( dist > -0.0001f ) && ( dist < 0.0001f ));
	}
	return false;
}

//----------------------------------------------------------------//
bool ZLSurface2D::IsOver ( ZLVec2D& loc ) const {

	float epsilon = 0.01f;

	if ( loc.mX < ( this->mXMin - epsilon )) return false;
	if ( loc.mX > ( this->mXMax + epsilon )) return false;

	return true;
}
