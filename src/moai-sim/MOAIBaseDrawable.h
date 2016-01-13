// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBASEDRAWABLE_H
#define	MOAIBASEDRAWABLE_H

//================================================================//
// MOAIBaseDrawable
//================================================================//
class MOAIBaseDrawable :
	public virtual MOAILuaObject {
public:

	static const int NO_SUBPRIM_ID = 0xffffffff;

	//----------------------------------------------------------------//
	virtual void		Draw						( int subPrimID = NO_SUBPRIM_ID, float lod = 0.0f ) = 0;
	virtual void		DrawDebug					( int subPrimID = NO_SUBPRIM_ID, float lod = 0.0f );
						MOAIBaseDrawable			();
	virtual				~MOAIBaseDrawable			();
};

#endif
