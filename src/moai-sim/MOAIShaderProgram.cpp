// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIEaseDriver.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGfxResourceMgr.h>
#include <moai-sim/MOAIShaderProgram.h>
#include <moai-sim/MOAITransformBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	clearUniform
	@text	Clears a uniform mapping.

	@in		MOAIShaderProgram self
	@in		number idx
	@out	nil
*/
int MOAIShaderProgram::_clearUniform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "UN" )

	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;

	self->ClearUniform ( idx );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	declareUniform
	@text	Declares a uniform mapping.

	@in		MOAIShaderProgram self
	@in		number idx
	@in		string name
	@opt	number type		One of MOAIShaderProgram.UNIFORM_FLOAT, MOAIShaderProgram.UNIFORM_INDEX, MOAIShaderProgram.UNIFORM_INT,
							MOAIShaderProgram.UNIFORM_MATRIX_F3, MOAIShaderProgram.UNIFORM_MATRIX_F4, MOAIShaderProgram.UNIFORM_VECTOR_F4
	@out	nil
*/
int MOAIShaderProgram::_declareUniform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "UNS" )

	u32 idx				= state.GetValue < u32 >( 2, 1 ) - 1;
	STLString name		= state.GetValue < cc8* >( 3, "" );
	u32  type			= state.GetValue < u32 >( 4, MOAIShaderUniform::UNIFORM_NONE );

	self->DeclareUniform ( idx, name, type );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	declareUniformFloat
	@text	Declares an float uniform.

	@in		MOAIShaderProgram self
	@in		number idx
	@in		string name
	@opt	number value		Default value is 0.
	@out	nil
*/
int MOAIShaderProgram::_declareUniformFloat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "UNSN" )

	u32 idx				= state.GetValue < u32 >( 2, 1 ) - 1;
	STLString name		= state.GetValue < cc8* >( 3, "" );
	float value			= state.GetValue < float >( 4, 0.0f );

	self->DeclareUniform ( idx, name, MOAIShaderUniform::UNIFORM_FLOAT, value );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	declareUniformInt
	@text	Declares an integer uniform.

	@in		MOAIShaderProgram self
	@in		number idx
	@in		string name
	@opt	number value		Default value is 0.
	@out	nil
*/
int MOAIShaderProgram::_declareUniformInt ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "UNSN" )

	u32 idx				= state.GetValue < u32 >( 2, 1 ) - 1;
	STLString name		= state.GetValue < cc8* >( 3, "" );
	int value			= state.GetValue < int >( 4, 0 );

	self->DeclareUniform ( idx, name, MOAIShaderUniform::UNIFORM_INT, value );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	declareUniformSampler
	@text	Declares an uniform to be used as a texture unit index. This uniform is
			internally an int, but when loaded into the shader the number one is subtracted
			from its value. This allows the user to maintain consistency with Lua's
			convention of indexing from one.

	@in		MOAIShaderProgram self
	@in		number idx
	@in		string name
	@opt	number textureUnit		Default value is 1.
	@out	nil
*/
int MOAIShaderProgram::_declareUniformSampler ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "UNSN" )

	u32 idx				= state.GetValue < u32 >( 2, 1 ) - 1;
	STLString name		= state.GetValue < cc8* >( 3, "" );
	int textureUnit		= state.GetValue < int >( 4, 1 );

	self->DeclareUniform ( idx, name, MOAIShaderUniform::UNIFORM_INDEX, textureUnit );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	load
	@text	Load a shader program.

	@in		MOAIShaderProgram self
	@in		string vertexShaderSource
	@in		string fragmentShaderSource
	@out	nil
*/
int MOAIShaderProgram::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "USS" )

	cc8* vtxSource	= state.GetValue < cc8* >( 2, 0 );
	cc8* frgSource	= state.GetValue < cc8* >( 3, 0 );

	self->SetSource ( vtxSource, frgSource );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShaderProgram::_reserveGlobals ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "U" )

	u32 nGlobals = state.GetValue < u32 >( 2, 0 );
	self->ReserveGlobals ( nGlobals );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserveUniforms
	@text	Reserve shader uniforms.

	@in		MOAIShaderProgram self
	@opt	number nUniforms	Default value is 0.
	@out	nil
*/
int MOAIShaderProgram::_reserveUniforms ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "U" )

	u32 nUniforms = state.GetValue < u32 >( 2, 0 );
	self->ReserveUniforms ( nUniforms );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShaderProgram::_setGlobal ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "U" )

	u32 idx			= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 uniformID	= state.GetValue < u32 >( 3, 1 ) - 1;
	u32 globalID	= state.GetValue < u32 >( 4, GLOBAL_NONE );
	
	self->SetGlobal ( idx, uniformID, globalID );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setVertexAttribute
	@text	Names a shader vertex attribute.

	@in		MOAIShaderProgram self
	@in		number index	Default value is 1.
	@in		string name		Name of attribute.
	@out	nil
*/
int MOAIShaderProgram::_setVertexAttribute ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "UNS" )

	u32 idx					= state.GetValue < u32 >( 2, 1 ) - 1;
	STLString attribute		= state.GetValue < cc8* >( 3, "" );

	self->SetVertexAttribute ( idx, attribute );

	return 0;
}

//================================================================//
// MOAIShaderProgram
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderProgram::Clear () {

	this->mVertexShaderSource.clear ();
	this->mFragmentShaderSource.clear ();

	this->mAttributeMap.clear ();
	this->mUniforms.Clear ();
	this->mGlobals.Clear ();
	
	this->Destroy ();
}

//----------------------------------------------------------------//
void MOAIShaderProgram::ClearUniform ( u32 idx ) {

	if ( idx < this->mUniforms.Size ()) {
		this->mUniforms [ idx ].Clear ();
	}
}

//----------------------------------------------------------------//
ZLGfxHandle* MOAIShaderProgram::CompileShader ( u32 type, cc8* source ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	ZLGfx& gfx = gfxDevice.GetDrawingAPI ();

	ZLGfxHandle* shader = gfx.CreateShader ( type );

	STLString buffer;

	buffer.append ( gfxDevice.IsOpenGLES () ? OPENGL_ES_PREPROC : OPENGL_PREPROC );
	if (( type == ZGL_SHADER_TYPE_FRAGMENT ) && gfxDevice.IsOpenGLES ()) {
		buffer.append ( WEBGL_PREPROC );
	}

	buffer.append ( source );

	gfx.PushSection ();

	gfx.ShaderSource ( shader, buffer.c_str (), buffer.size ());
	gfx.CompileShader ( shader, true );

	if ( gfx.PushErrorHandler ()) {
		gfx.DeleteHandle ( shader );
	}

	gfx.PopSection ();

	return shader;
}

//----------------------------------------------------------------//
void MOAIShaderProgram::DeclareUniform ( u32 idx, cc8* name, u32 type ) {

	if ( idx < this->mUniforms.Size ()) {

		this->ClearUniform ( idx );

		MOAIShaderUniform& uniform = this->mUniforms [ idx ];
		uniform.mName = name;
		uniform.SetType ( type );
		
		MOAIShaderUniformBuffer& uniformDefault = this->mUniformDefaults [ idx ];
		uniformDefault.SetType ( type );
		uniformDefault.Default ();
	}
}

//----------------------------------------------------------------//
void MOAIShaderProgram::DeclareUniform ( u32 idx, cc8* name, u32 type, float value ) {

	if ( idx < this->mUniforms.Size ()) {
		this->DeclareUniform ( idx, name, type );
		this->mUniformDefaults [ idx ].SetValue ( value );
	}
}

//----------------------------------------------------------------//
void MOAIShaderProgram::DeclareUniform ( u32 idx, cc8* name, u32 type, int value ) {

	if ( idx < this->mUniforms.Size ()) {
		this->DeclareUniform ( idx, name, type );
		this->mUniformDefaults [ idx ].SetValue ( value );
	}
}

//----------------------------------------------------------------//
MOAIShaderProgram::MOAIShaderProgram () :
	mProgram ( 0 ),
	mVertexShader ( 0 ),
	mFragmentShader ( 0 ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGfxResource )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIShaderProgram::~MOAIShaderProgram () {

	this->Clear ();
}

//----------------------------------------------------------------//
bool MOAIShaderProgram::OnCPUCreate () {

	return true;
}

//----------------------------------------------------------------//
void MOAIShaderProgram::OnCPUDestroy () {
}

//----------------------------------------------------------------//
void MOAIShaderProgram::OnGPUBind () {

	// use shader program.
	MOAIGfxDevice::GetDrawingAPI ().UseProgram ( this->mProgram );

	// reload the uniform values
	//for ( u32 i = 0; i < this->mUniforms.Size (); ++i ) {
	//	this->mUniforms [ i ].Bind ();
	//}
}

//----------------------------------------------------------------//
bool MOAIShaderProgram::OnGPUCreate () {

	ZLGfx& gfx = MOAIGfxDevice::GetDrawingAPI ();

	this->mVertexShader = this->CompileShader ( ZGL_SHADER_TYPE_VERTEX, this->mVertexShaderSource );
	this->mFragmentShader = this->CompileShader ( ZGL_SHADER_TYPE_FRAGMENT, this->mFragmentShaderSource );
	this->mProgram = gfx.CreateProgram ();

	if ( !( this->mVertexShader && this->mFragmentShader && this->mProgram )) {
		this->Clear ();
		return false;
	}

	gfx.AttachShader ( this->mProgram, this->mVertexShader );
	gfx.AttachShader ( this->mProgram, this->mFragmentShader );

	// bind attribute locations.
	// this needs to be done prior to linking.
	AttributeMapIt attrMapIt = this->mAttributeMap.begin ();
	for ( ; attrMapIt != this->mAttributeMap.end (); ++attrMapIt ) {
		gfx.BindAttribLocation ( this->mProgram, attrMapIt->first, attrMapIt->second.str ());
	}

	gfx.LinkProgram ( this->mProgram, true );

	// get the uniform locations and clear out the names (no longer needed)
	for ( u32 i = 0; i < this->mUniforms.Size (); ++i ) {
		MOAIShaderUniform& uniform = this->mUniforms [ i ];

		if ( uniform.mType != MOAIShaderUniform::UNIFORM_NONE ) {
			gfx.GetUniformLocation ( this->mProgram, uniform.mName, this, ( void* )i );
			uniform.ClearValue ();
		}
	}

	gfx.DeleteHandle ( this->mVertexShader );
	this->mVertexShader = 0;

	gfx.DeleteHandle ( this->mFragmentShader );
	this->mFragmentShader = 0;

	//AJV TODO - does the attribute map ever need to be cleared?
	//this->mAttributeMap.clear ();
	
	return true;
}

//----------------------------------------------------------------//
void MOAIShaderProgram::OnGPUDestroy () {

	MOAIGfxResourceMgr::Get ().PushDeleter ( this->mVertexShader );
	this->mVertexShader = 0;

	MOAIGfxResourceMgr::Get ().PushDeleter ( this->mFragmentShader );
	this->mFragmentShader = 0;

	MOAIGfxResourceMgr::Get ().PushDeleter ( this->mProgram );
	this->mProgram = 0;
}

//----------------------------------------------------------------//
void MOAIShaderProgram::OnGPULost () {

	this->mVertexShader = 0;
	this->mFragmentShader = 0;
	this->mProgram = 0;
}

//----------------------------------------------------------------//
void MOAIShaderProgram::OnGPUUnbind () {

	MOAIGfxDevice::GetDrawingAPI ().UseProgram ( 0 );
}

//----------------------------------------------------------------//
bool MOAIShaderProgram::OnGPUUpdate () {

	return true;
}

//----------------------------------------------------------------//
void MOAIShaderProgram::OnUniformLocation ( u32 addr, void* userdata ) {

	size_t i = ( size_t )userdata;
	
	if ( i < this->mUniforms.Size ()) {
		this->mUniforms [ i ].mAddr = addr;
	}
}

//----------------------------------------------------------------//
void MOAIShaderProgram::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIGfxResource::RegisterLuaClass ( state );

	state.SetField ( -1, "UNIFORM_FLOAT",						( u32 )MOAIShaderUniform::UNIFORM_FLOAT );
	state.SetField ( -1, "UNIFORM_INDEX",						( u32 )MOAIShaderUniform::UNIFORM_INDEX );
	state.SetField ( -1, "UNIFORM_INT",							( u32 )MOAIShaderUniform::UNIFORM_INT );
	state.SetField ( -1, "UNIFORM_MATRIX_F3",					( u32 )MOAIShaderUniform::UNIFORM_MATRIX_F3 );
	state.SetField ( -1, "UNIFORM_MATRIX_F4",					( u32 )MOAIShaderUniform::UNIFORM_MATRIX_F4 );
	state.SetField ( -1, "UNIFORM_VECTOR_F4",					( u32 )MOAIShaderUniform::UNIFORM_VECTOR_F4 );
	
	state.SetField ( -1, "GLOBAL_NONE",							( u32 )GLOBAL_NONE );
	state.SetField ( -1, "GLOBAL_PEN_COLOR",					( u32 )GLOBAL_PEN_COLOR );
	state.SetField ( -1, "GLOBAL_VIEW_PROJ",					( u32 )GLOBAL_VIEW_PROJ );
	state.SetField ( -1, "GLOBAL_VIEW_WIDTH",					( u32 )GLOBAL_VIEW_WIDTH );
	state.SetField ( -1, "GLOBAL_VIEW_HEIGHT",					( u32 )GLOBAL_VIEW_HEIGHT );
	state.SetField ( -1, "GLOBAL_WORLD",						( u32 )GLOBAL_WORLD );
	state.SetField ( -1, "GLOBAL_WORLD_INVERSE",				( u32 )GLOBAL_WORLD_INVERSE );
	state.SetField ( -1, "GLOBAL_WORLD_VIEW",					( u32 )GLOBAL_WORLD_VIEW );
	state.SetField ( -1, "GLOBAL_WORLD_VIEW_INVERSE",			( u32 )GLOBAL_WORLD_VIEW_INVERSE );
	state.SetField ( -1, "GLOBAL_WORLD_VIEW_PROJ",				( u32 )GLOBAL_WORLD_VIEW_PROJ );
}

//----------------------------------------------------------------//
void MOAIShaderProgram::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIGfxResource::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "clearUniform",				_clearUniform },
		{ "declareUniform",				_declareUniform },
		{ "declareUniformFloat",		_declareUniformFloat },
		{ "declareUniformInt",			_declareUniformInt },
		{ "declareUniformSampler",		_declareUniformSampler },
		{ "load",						_load },
		{ "reserveGlobals",				_reserveGlobals },
		{ "reserveUniforms",			_reserveUniforms },
		{ "setGlobal",					_setGlobal },
		{ "setVertexAttribute",			_setVertexAttribute },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIShaderProgram::ReserveGlobals ( u32 nGlobals ) {

	this->mGlobals.Init ( nGlobals );
}

//----------------------------------------------------------------//
void MOAIShaderProgram::ReserveUniforms ( u32 nUniforms ) {

	this->mUniforms.Init ( nUniforms );
	this->mUniformDefaults.Init ( nUniforms );
}

//----------------------------------------------------------------//
void MOAIShaderProgram::SetGlobal ( u32 idx, u32 uniformID, u32 globalID ) {

	this->mGlobals [ idx ].mUniformID = uniformID;
	this->mGlobals [ idx ].mGlobalID = globalID;
}

//----------------------------------------------------------------//
void MOAIShaderProgram::SetSource ( cc8* vshSource, cc8* fshSource ) {

	if ( vshSource && fshSource ) {
		this->mVertexShaderSource = vshSource;
		this->mFragmentShaderSource = fshSource;
		this->FinishInit ();
	}
}

//----------------------------------------------------------------//
void MOAIShaderProgram::SetVertexAttribute ( u32 idx, cc8* attribute ) {

	if ( attribute ) {
		this->mAttributeMap [ idx ] = attribute;
	}
}

//----------------------------------------------------------------//
void MOAIShaderProgram::UpdateGlobals () {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	const ZLMatrix4x4& world	= gfxDevice.mVertexTransforms [ MOAIGfxDevice::VTX_WORLD_TRANSFORM ];
	const ZLMatrix4x4& view		= gfxDevice.mVertexTransforms [ MOAIGfxDevice::VTX_VIEW_TRANSFORM ];
	const ZLMatrix4x4& proj		= gfxDevice.mVertexTransforms [ MOAIGfxDevice::VTX_PROJ_TRANSFORM ];

	// TODO: all these need to be cached in the gfx state instead of recomputed every single time
	// TODO: should check against the cached values in the shader to see if the uniforms need to be re-uploaded

	// NOTE: matrices are submitted transposed; it is up to the shader to transform vertices correctly
	// vert * matrix implicitely transposes the matrix; martix * vert uses the matrix as submitted

	for ( u32 i = 0; i < this->mGlobals.Size (); ++i ) {
	
		const MOAIShaderProgramGlobal& global = this->mGlobals [ i ];
		MOAIShaderUniform& uniform = this->mUniforms [ global.mUniformID ];
	
		switch ( global.mGlobalID ) {
		
			case GLOBAL_PEN_COLOR: {
				if ( uniform.SetValue ( gfxDevice.mFinalColor, true )) {
					uniform.Bind ();
				}
				break;
			}
			case GLOBAL_VIEW_PROJ: {
			
				ZLMatrix4x4 mtx = view;
				mtx.Append ( proj );
				
				if ( uniform.SetValue ( mtx, true )) {
					uniform.Bind ();
				}
				break;
			}
			case GLOBAL_VIEW_WIDTH: {
				if ( uniform.SetValue ( gfxDevice.mViewRect.Width ())) {
					uniform.Bind ();
				}
				break;
			}
			case GLOBAL_VIEW_HEIGHT: {
				if ( uniform.SetValue ( gfxDevice.mViewRect.Height ())) {
					uniform.Bind ();
				}
				break;
			}
			case GLOBAL_VIEW_HALF_WIDTH: {
				if ( uniform.SetValue ( gfxDevice.mViewRect.Width () * 0.5f )) {
					uniform.Bind ();
				}
				break;
			}
			case GLOBAL_VIEW_HALF_HEIGHT: {
				if ( uniform.SetValue ( gfxDevice.mViewRect.Height () * 0.5f )) {
					uniform.Bind ();
				}
				break;
			}
			case GLOBAL_WORLD: {
				if ( uniform.SetValue ( world, true )) {
					uniform.Bind ();
				}
				break;
			}
			case GLOBAL_WORLD_INVERSE: {
			
				ZLMatrix4x4 mtx = world;
				mtx.Inverse ();
				
				if ( uniform.SetValue ( mtx, true )) {
					uniform.Bind ();
				}
				break;
			}
			case GLOBAL_WORLD_VIEW: {
			
				ZLMatrix4x4 mtx = world;
				mtx.Append ( view );
				
				if ( uniform.SetValue ( mtx, true )) {
					uniform.Bind ();
				}
				break;
			}
			case GLOBAL_WORLD_VIEW_INVERSE: {
			
				ZLMatrix4x4 mtx = world;
				mtx.Append ( view );
				mtx.Inverse ();
				
				if ( uniform.SetValue ( mtx, true )) {
					uniform.Bind ();
				}
				break;
			}
			case GLOBAL_WORLD_VIEW_PROJ: {
			
				ZLMatrix4x4 mtx = world;
				mtx.Append ( view );
				mtx.Append ( proj );
				
				if ( uniform.SetValue ( mtx, true )) {
					uniform.Bind ();
				}
				break;
			}
		}
	}
}
