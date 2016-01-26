// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIConditionVariable.h>
#include <moai-util/MOAIConditionVariable_posix.h>

//================================================================//
// MOAIMutex
//================================================================//

//----------------------------------------------------------------//
void MOAIConditionVariable::Broadcast () {

	this->mConditionImpl->Broadcast ();
}

//----------------------------------------------------------------//
MOAIConditionVariable::MOAIConditionVariable () {
	
	this->mConditionImpl = new MOAIConditionVariableImpl ();
}

//----------------------------------------------------------------//
MOAIConditionVariable::~MOAIConditionVariable () {

	delete this->mConditionImpl;
}

//----------------------------------------------------------------//
void MOAIConditionVariable::Signal () {

	this->mConditionImpl->Signal ();
}

//----------------------------------------------------------------//
void MOAIConditionVariable::Wait () {

	this->mConditionImpl->Wait ( *this->mMutexImpl );
}
