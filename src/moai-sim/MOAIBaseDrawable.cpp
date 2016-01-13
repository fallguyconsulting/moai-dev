// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIBaseDrawable.h>

//================================================================//
// MOAIBaseDrawable
//================================================================//

//----------------------------------------------------------------//
void MOAIBaseDrawable::DrawDebug ( int subPrimID, float lod ) {
	UNUSED ( subPrimID );
	UNUSED ( lod );
}

//----------------------------------------------------------------//
MOAIBaseDrawable::MOAIBaseDrawable () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIBaseDrawable::~MOAIBaseDrawable () {
}
