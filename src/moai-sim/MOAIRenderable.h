// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIRENDERABLE_H
#define	MOAIRENDERABLE_H

//================================================================//
// MOAIBaseDrawable
//================================================================//
class MOAIBaseDrawable {
public:

	static const int NO_SUBPRIM_ID = 0xffffffff;

	//----------------------------------------------------------------//
	virtual void		Draw						();
	virtual void		Draw						( int subPrimID, float lod ) = 0;
	virtual void		DrawDebug					( int subPrimID, float lod );
						MOAIBaseDrawable			();
	virtual				~MOAIBaseDrawable			();
};

//================================================================//
// MOAIRenderable
//================================================================//
/**	@lua	MOAIRenderable
	@text	Abstract base class for objects that can be rendered by MOAIRenderMgr.
*/
class MOAIRenderable :
	public virtual MOAILuaObject {
public:

	//----------------------------------------------------------------//
						MOAIRenderable			();
	virtual				~MOAIRenderable			();
	virtual void		Render					() = 0;
};

#endif
