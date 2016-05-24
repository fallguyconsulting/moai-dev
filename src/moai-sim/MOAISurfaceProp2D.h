// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISURFACEPROP2D_H
#define	MOAISURFACEPROP2D_H

#include <moai-sim/MOAIProp.h>

class MOAISurfaceDeck2D;
class MOAISurfaceSampler2D;

// TODO: MOAIDebugEdgeList is a freaking hack and it sucks, but I'm doing it as an
// interim to more robust display list support. It's tempting to make a formal shape
// list object, but that would overlap with the purpose of MOAIDraw. We could make
// an ad hoc retained mode for MOAIDraw, but that would either duplicate or not
// support the full set of drawing commands (shaders, textures, blend modes, etc.)
// So for now I'll add this hacky edge list just for implementing/debugging collision,
// but this should be removed as soon as the zlgfx refactor is finished.

//================================================================//
// MOAIEdgeDebugList
//================================================================//
class MOAIEdgeDebugList {
private:

	u32				mPenColor;
	float			mPenWidth;

	ZLMemStream		mEdgeStream;

public:

	//----------------------------------------------------------------//
	void	Draw					();
			MOAIEdgeDebugList		();
			~MOAIEdgeDebugList		();
	void	Reset					();
	void	SetPenColor				( u32 color );
	void	SetPenColor				( const ZLColorVec& colorVec );
	void	SetPenColor				( float r, float g, float b, float a );
	void	SetPenWidth				( float width );
	void	WriteEdge				( ZLVec2D v0, ZLVec2D v1 );
};

//================================================================//
// MOAISurfaceProp2D
//================================================================//
// TODO: doxygen
class MOAISurfaceProp2D :
	public MOAIProp,
	public MOAIBaseDrawable {
private:

	MOAISurfaceDeck2D*		mSurfaceDeck;
	
	//----------------------------------------------------------------//
	void			OnSetDeck				( MOAIDeck* deck );

public:
	
	DECL_LUA_FACTORY ( MOAISurfaceProp2D )
	
	//----------------------------------------------------------------//
	u32				AffirmInterfaceMask		( MOAIPartition& partition );
	void			Draw					( int subPrimID, float lod );
	void			GatherSurfaces			( MOAISurfaceSampler2D& sampler );
					MOAISurfaceProp2D		();
					~MOAISurfaceProp2D		();
	u32				OnGetModelBounds		( ZLBox& bounds );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SampleSurfaces			( MOAISurfaceSampler2D& sampler );
};

#endif
