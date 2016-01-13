// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIRenderable.h>

//================================================================//
// MOAIBaseDrawable
//================================================================//

//----------------------------------------------------------------//
void MOAIBaseDrawable::Draw () {

	this->Draw ( NO_SUBPRIM_ID, 0.0f );
}

//----------------------------------------------------------------//
void MOAIBaseDrawable::DrawDebug ( int subPrimID, float lod ) {
	UNUSED ( subPrimID );
	UNUSED ( lod );
}

//----------------------------------------------------------------//
MOAIBaseDrawable::MOAIBaseDrawable () {
}

//----------------------------------------------------------------//
MOAIBaseDrawable::~MOAIBaseDrawable () {
}

//================================================================//
// MOAIRenderable
//================================================================//

//----------------------------------------------------------------//
MOAIRenderable::MOAIRenderable () {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIRenderable::~MOAIRenderable () {
}
