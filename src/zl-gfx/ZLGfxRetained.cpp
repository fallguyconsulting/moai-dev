// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/zl_gfx.h>
#include <zl-gfx/ZLGfxLogger.h>
#include <zl-gfx/ZLGfxRetained.h>

//================================================================//
// ZLClonedGfxBuffer
//================================================================//
class ZLClonedGfxBuffer :
	public ZLSharedConstBuffer {
private:

	friend class ZLGfxRetained;

	void*				mBuffer;
	size_t				mSize;
	bool				mLocal;

public:

	GET_CONST ( void*, ConstData, mBuffer )
	GET ( size_t, Size, mSize )
	
	//----------------------------------------------------------------//
	void Delete () {
	
		if ( !this->mLocal ) {
			if ( this->mBuffer ) {
				free ( this->mBuffer );
			}
			delete this;
		}
		else {
			this->~ZLClonedGfxBuffer (); // placement delete
		}
	}
	
	//----------------------------------------------------------------//
	ZLClonedGfxBuffer () :
		mBuffer ( 0 ),
		mSize ( 0 ),
		mLocal ( false ) {
	}
	
	//----------------------------------------------------------------//
	~ZLClonedGfxBuffer () {
	}
};

//================================================================//
// ZLGfxRetainedListenerRecord
//================================================================//

//----------------------------------------------------------------//
ZLGfxListenerRecord::ZLGfxListenerRecord () :
	mListenerHandle ( 0 ),
	mUserdata ( 0 ),
	mCallbackID ( UNKNOWN ),
	mEvent ( 0 ),
	mUniformAddr ( false ) {
}

//----------------------------------------------------------------//
ZLGfxListenerRecord::~ZLGfxListenerRecord () {
}


//================================================================//
// ZLGfxRetained
//================================================================//

//----------------------------------------------------------------//
void ZLGfxRetained::ActiveTexture ( u32 textureUnit ) {

	assert ( this->mStream );
	
	this->mStream->Write < u32 >( ACTIVE_TEXTURE );
	this->mStream->Write < u32 >( textureUnit );
}

//----------------------------------------------------------------//
void ZLGfxRetained::AttachShader ( ZLGfxHandle* program, ZLGfxHandle* shader ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ATTACH_SHADER );
	this->mStream->Write < ZLGfxHandle* >( program );
	this->mStream->Write < ZLGfxHandle* >( shader );
}

//----------------------------------------------------------------//
void ZLGfxRetained::BindAttribLocation ( ZLGfxHandle* program, u32 index, cc8* name ) {
	
	assert ( this->mStream );

	this->mStream->Write < u32 >( BIND_ATTRIB_LOCATION );
	this->mStream->Write < ZLGfxHandle* >( program );
	this->mStream->Write < u32 >( index );
	
	size_t size = strlen ( name );
	this->mStream->Write < size_t >( size );
	this->mStream->WriteBytes ( name, size );
}

//----------------------------------------------------------------//
void ZLGfxRetained::BindBuffer ( u32 target, ZLGfxHandle* handle ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( BIND_BUFFER );
	this->mStream->Write < u32 >( target );
	this->mStream->Write < ZLGfxHandle* >( handle );
}

//----------------------------------------------------------------//
void ZLGfxRetained::BindFramebuffer ( u32 target, ZLGfxHandle* handle ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( BIND_FRAMEBUFFER );
	this->mStream->Write < u32 >( target );
	this->mStream->Write < ZLGfxHandle* >( handle );
}

//----------------------------------------------------------------//
void ZLGfxRetained::BindRenderbuffer ( ZLGfxHandle* handle ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( BIND_RENDERBUFFER );
	this->mStream->Write < ZLGfxHandle* >( handle );
}

//----------------------------------------------------------------//
void ZLGfxRetained::BindTexture ( ZLGfxHandle* handle ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( BIND_TEXTURE );
	this->mStream->Write < ZLGfxHandle* >( handle );
}

//----------------------------------------------------------------//
void ZLGfxRetained::BindVertexArray ( ZLGfxHandle* handle ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( BIND_VERTEX_ARRAY );
	this->mStream->Write < ZLGfxHandle* >( handle );
}

//----------------------------------------------------------------//
void ZLGfxRetained::BlendFunc ( u32 sourceFactor, u32 destFactor ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( BLEND_FUNC );
	this->mStream->Write < u32 >( sourceFactor );
	this->mStream->Write < u32 >( destFactor );
}

//----------------------------------------------------------------//
void ZLGfxRetained::BlendMode ( u32 mode ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( BLEND_MODE );
	this->mStream->Write < u32 >( mode );
}

//----------------------------------------------------------------//
void ZLGfxRetained::BufferData ( u32 target, u32 size, ZLSharedConstBuffer* buffer, size_t offset, u32 usage ) {

	this->Retain ( buffer );

	assert ( this->mStream );

	this->mStream->Write < u32 >( BUFFER_DATA );
	this->mStream->Write < u32 >( target );
	this->mStream->Write < u32 >( size );
	this->mStream->Write < ZLSharedConstBuffer* >( buffer );
	this->mStream->Write < size_t >( offset );
	this->mStream->Write < u32 >( usage );
}

//----------------------------------------------------------------//
void ZLGfxRetained::BufferSubData ( u32 target, u32 offset, u32 size, ZLSharedConstBuffer* buffer, size_t srcOffset ) {

	this->Retain ( buffer );

	assert ( this->mStream );

	this->mStream->Write < u32 >( BUFFER_SUB_DATA );
	this->mStream->Write < u32 >( target );
	this->mStream->Write < u32 >( offset );
	this->mStream->Write < u32 >( size );
	this->mStream->Write < ZLSharedConstBuffer* >( buffer );
	this->mStream->Write < size_t >( srcOffset );
}

//----------------------------------------------------------------//
void ZLGfxRetained::CheckFramebufferStatus ( u32 target ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( CHECK_FRAMEBUFFER_STATUS );
	this->mStream->Write < u32 >( target );
}

//----------------------------------------------------------------//
void ZLGfxRetained::Clear ( u32 mask ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( CLEAR );
	this->mStream->Write < u32 >( mask );
}

//----------------------------------------------------------------//
void ZLGfxRetained::ClearColor ( float r, float g, float b, float a ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( CLEAR_COLOR );
	this->mStream->Write < float >( r );
	this->mStream->Write < float >( g );
	this->mStream->Write < float >( b );
	this->mStream->Write < float >( a );
}

//----------------------------------------------------------------//
void ZLGfxRetained::Color ( float r, float g, float b, float a ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( COLOR );
	this->mStream->Write < float >( r );
	this->mStream->Write < float >( g );
	this->mStream->Write < float >( b );
	this->mStream->Write < float >( a );
}

//----------------------------------------------------------------//
void ZLGfxRetained::Comment ( cc8* comment ) {
	
	assert ( this->mStream );

	this->mStream->Write < u32 >( COMMENT );
	
	size_t size = comment ? strlen ( comment ) : 0;
	this->mStream->Write < size_t >( size );
	this->mStream->WriteBytes ( comment, size );
}

//----------------------------------------------------------------//
void ZLGfxRetained::CompileShader ( ZLGfxHandle* shader, bool log ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( COMPILE_SHADER );
	this->mStream->Write < ZLGfxHandle* >( shader );
	this->mStream->Write < bool >( log );
}

//----------------------------------------------------------------//
void ZLGfxRetained::CompressedTexImage2D ( u32 level, u32 internalFormat, u32 width, u32 height, u32 imageSize, ZLSharedConstBuffer* buffer ) {

	this->Retain ( buffer );

	assert ( this->mStream );

	this->mStream->Write < u32 >( COMPRESSED_TEX_IMAGE_2D );
	this->mStream->Write < u32 >( level );
	this->mStream->Write < u32 >( internalFormat );
	this->mStream->Write < u32 >( width );
	this->mStream->Write < u32 >( height );
	this->mStream->Write < u32 >( imageSize );
	this->mStream->Write < ZLSharedConstBuffer* >( buffer );
}

//----------------------------------------------------------------//
ZLSharedConstBuffer* ZLGfxRetained::CopyBuffer ( ZLSharedConstBuffer* buffer ) {

	if ( buffer ) {
	
		ZLClonedGfxBuffer* copy = 0;
	
		size_t bufferSize = buffer->GetSize ();
		size_t containerSize = sizeof ( ZLClonedGfxBuffer );
		size_t totalSize = bufferSize + containerSize;
		
		if (( this->mBufferTop + totalSize ) <= this->mBufferSize ) {
		
			copy = ( ZLClonedGfxBuffer* )(( size_t )this->mBuffer + this->mBufferTop );
			new ( copy ) ZLClonedGfxBuffer (); // placement new
			this->mBufferTop += containerSize;
			
			void* bufferCopy = ( void* )(( size_t )this->mBuffer + this->mBufferTop );
			this->mBufferTop += bufferSize;
			
			copy->mSize = bufferSize;
			copy->mBuffer = bufferCopy;
			copy->mLocal = true;
		}
		else {
	
			copy = new ZLClonedGfxBuffer ();
			copy->mSize = buffer->GetSize ();
			copy->mBuffer = malloc ( copy->mSize );
			copy->mLocal = false;
		}
		
		memcpy ( copy->mBuffer, buffer->GetConstData (), bufferSize );
		buffer = copy;
		
		this->mAllocated += totalSize;
		this->mMaxAllocated = this->mMaxAllocated < this->mAllocated ? this->mAllocated : this->mMaxAllocated;
	}
	return buffer;
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxRetained::Create ( ZLGfxHandle* handle, u32 param ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( CREATE );
	this->mStream->Write < ZLGfxHandle* >( handle );
	this->mStream->Write < u32 >( param );
	
	return handle;
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxRetained::CreateBuffer () {

	assert ( this->mStream );

	ZLGfxHandle* handle = new ZLGfxHandle ( ZLGfxHandle::BUFFER, 0, false );
	
	this->mStream->Write < u32 >( CREATE );
	this->mStream->Write < ZLGfxHandle* >( handle );
	this->mStream->Write < u32 >( 0 );
	
	return handle;
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxRetained::CreateFramebuffer () {

	assert ( this->mStream );

	ZLGfxHandle* handle = new ZLGfxHandle ( ZLGfxHandle::FRAMEBUFFER, 0, false );
	
	this->mStream->Write < u32 >( CREATE );
	this->mStream->Write < ZLGfxHandle* >( handle );
	this->mStream->Write < u32 >( 0 );
	
	return handle;
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxRetained::CreateProgram () {

	assert ( this->mStream );

	ZLGfxHandle* handle = new ZLGfxHandle ( ZLGfxHandle::PROGRAM, 0, false );
	
	this->mStream->Write < u32 >( CREATE );
	this->mStream->Write < ZLGfxHandle* >( handle );
	this->mStream->Write < u32 >( 0 );
	
	return handle;
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxRetained::CreateRenderbuffer () {

	assert ( this->mStream );

	ZLGfxHandle* handle = new ZLGfxHandle ( ZLGfxHandle::RENDERBUFFER, 0, false );
	
	this->mStream->Write < u32 >( CREATE );
	this->mStream->Write < ZLGfxHandle* >( handle );
	this->mStream->Write < u32 >( 0 );
	
	return handle;
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxRetained::CreateShader ( u32 shaderType ) {

	assert ( this->mStream );

	ZLGfxHandle* handle = new ZLGfxHandle ( ZLGfxHandle::SHADER, 0, false );
	
	this->mStream->Write < u32 >( CREATE );
	this->mStream->Write < ZLGfxHandle* >( handle );
	this->mStream->Write < u32 >( shaderType );
	
	return handle;
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxRetained::CreateTexture () {

	assert ( this->mStream );

	ZLGfxHandle* handle = new ZLGfxHandle ( ZLGfxHandle::TEXTURE, 0, false );
	
	this->mStream->Write < u32 >( CREATE );
	this->mStream->Write < ZLGfxHandle* >( handle );
	this->mStream->Write < u32 >( 0 );
	
	return handle;
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxRetained::CreateVertexArray () {

	assert ( this->mStream );

	ZLGfxHandle* handle = new ZLGfxHandle ( ZLGfxHandle::VERTEXARRAY, 0, false );
	
	this->mStream->Write < u32 >( CREATE );
	this->mStream->Write < ZLGfxHandle* >( handle );
	this->mStream->Write < u32 >( 0 );
	
	return handle;
}

//----------------------------------------------------------------//
void ZLGfxRetained::CullFace ( u32 mode ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( CULL_FACE );
	this->mStream->Write < u32 >( mode );
}

//----------------------------------------------------------------//
void ZLGfxRetained::DeleteHandle ( ZLGfxHandle* handle ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( DELETE );
	this->mStream->Write < ZLGfxHandle* >( handle );
}

//----------------------------------------------------------------//
void ZLGfxRetained::DepthFunc ( u32 depthFunc ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( DEPTH_FUNC );
	this->mStream->Write < u32 >( depthFunc );
}

//----------------------------------------------------------------//
void ZLGfxRetained::DepthMask ( bool flag ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( DEPTH_MASK );
	this->mStream->Write < u32 >( flag );
}

//----------------------------------------------------------------//
void ZLGfxRetained::Disable ( u32 cap ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( DISABLE );
	this->mStream->Write < u32 >( cap );
}

//----------------------------------------------------------------//
void ZLGfxRetained::DisableClientState ( u32 cap ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( DISABLE_CLIENT_STATE );
	this->mStream->Write < u32 >( cap );
}

//----------------------------------------------------------------//
void ZLGfxRetained::DisableVertexAttribArray ( u32 index ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( DISABLE_VERTEX_ATTRIB_ARRAY );
	this->mStream->Write < u32 >( index );
}

//----------------------------------------------------------------//
void ZLGfxRetained::Draw ( ZLGfx& draw ) {

	//zglBegin ();

	assert ( this->mStream );

	size_t top = this->mStream->GetCursor ();
	if ( top == 0 ) return;
	
	this->mStream->Seek ( 0, SEEK_SET );
	
	while ( this->mStream->GetCursor () < top ) {
	
		u32 command = this->mStream->Read < u32 >( UNKNOWN );
		
		if ( command == UNKNOWN ) {
			printf ( "UNKOWN\n" );
			break;
		}
	
		switch ( command ) {
	
			case ACTIVE_TEXTURE: {
			
				draw.ActiveTexture (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case ATTACH_SHADER: {
			
				draw.AttachShader (
					this->mStream->Read < ZLGfxHandle* >( 0 ),
					this->mStream->Read < ZLGfxHandle* >( 0 )
				);
				break;
			}
			case BIND_ATTRIB_LOCATION: {
			
				ZLGfxHandle* handle		= this->mStream->Read < ZLGfxHandle* >( 0 );
				u32 index				= this->mStream->Read < u32 >( 0 );
	
				size_t size				= this->mStream->Read < size_t >( 0 );
				
				char* name = ( char* )alloca ( size + 1 );
				this->mStream->ReadBytes ( name, size );
				name [ size ] = 0;
				
				draw.BindAttribLocation ( handle, index, name );
				
				break;
			}
			case BIND_BUFFER: {
			
				draw.BindBuffer (
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < ZLGfxHandle* >( 0 )
				);
				break;
			}
			case BIND_FRAMEBUFFER: {
			
				draw.BindFramebuffer (
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < ZLGfxHandle* >( 0 )
				);
				break;
			}
			case BIND_RENDERBUFFER: {
			
				draw.BindRenderbuffer (
					this->mStream->Read < ZLGfxHandle* >( 0 )
				);
				break;
			}
			case BIND_TEXTURE: {
			
				draw.BindTexture (
					this->mStream->Read < ZLGfxHandle* >( 0 )
				);
				break;
			}
			case BIND_VERTEX_ARRAY: {
			
				draw.BindVertexArray (
					this->mStream->Read < ZLGfxHandle* >( 0 )
				);
				break;
			}
			case BLEND_FUNC: {
			
				draw.BlendFunc (
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case BLEND_MODE: {
			
				draw.BlendMode (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case BUFFER_DATA: {
			
				u32 target						= this->mStream->Read < u32 >( 0 );
				u32 size						= this->mStream->Read < u32 >( 0 );
				ZLSharedConstBuffer* buffer		= this->mStream->Read < ZLSharedConstBuffer* >( 0 );
				size_t offset					= this->mStream->Read < size_t >( 0 );
				u32 usage						= this->mStream->Read < u32 >( 0 );
			
				draw.BufferData ( target, size, buffer, offset,usage );
				
				break;
			}
			case BUFFER_SUB_DATA: {
			
				u32 target						= this->mStream->Read < u32 >( 0 );
				u32 offset						= this->mStream->Read < u32 >( 0 );
				u32 size						= this->mStream->Read < u32 >( 0 );
				ZLSharedConstBuffer* buffer		= this->mStream->Read < ZLSharedConstBuffer* >( 0 );
				size_t srcOffset				= this->mStream->Read < size_t >( 0 );
				
				draw.BufferSubData ( target, offset, size, buffer, srcOffset );
				
				break;
			}
			case CHECK_FRAMEBUFFER_STATUS: {
			
				// TODO: GFX
				u32 target						= this->mStream->Read < u32 >( 0 );
				
				break;
			}
			case CLEAR: {
			
				draw.Clear (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case CLEAR_COLOR: {
			
				draw.ClearColor (
					this->mStream->Read < float >( 0 ),
					this->mStream->Read < float >( 0 ),
					this->mStream->Read < float >( 0 ),
					this->mStream->Read < float >( 0 )
				);
				break;
			}
			case COLOR: {
			
				draw.Color (
					this->mStream->Read < float >( 0 ),
					this->mStream->Read < float >( 0 ),
					this->mStream->Read < float >( 0 ),
					this->mStream->Read < float >( 0 )
				);
				break;
			}
			case COMMENT: {
	
				size_t size				= this->mStream->Read < size_t >( 0 );
				
				if ( size ) {
					char* comment = ( char* )alloca ( size + 1 );
					this->mStream->ReadBytes ( comment, size );
					comment [ size ] = 0;
					draw.Comment ( comment );
				}
				else {
					draw.Comment ();
				}
				break;
			}
			case COMPILE_SHADER: {
			
				draw.CompileShader (
					this->mStream->Read < ZLGfxHandle* >( 0 ),
					this->mStream->Read < bool >( true )
				);
				break;
			}
			case COMPRESSED_TEX_IMAGE_2D: {
			
				u32 level						= this->mStream->Read < u32 >( 0 );
				u32 internalFormat				= this->mStream->Read < u32 >( 0 );
				u32 width						= this->mStream->Read < u32 >( 0 );
				u32 height						= this->mStream->Read < u32 >( 0 );
				u32 imageSize					= this->mStream->Read < u32 >( 0 );
				ZLSharedConstBuffer* buffer		= this->mStream->Read < ZLSharedConstBuffer* >( 0 );
				
				draw.CompressedTexImage2D ( level, internalFormat, width, height, imageSize, buffer );
			
				break;
			}
			case CREATE: {
			
				ZLGfxHandle* handle = this->mStream->Read < ZLGfxHandle* >( 0 );
				u32 param = this->mStream->Read < u32 >( 0 );
				draw.Create ( handle, param );
				break;
			}
			case CULL_FACE: {
			
				draw.CullFace (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case DELETE: {
			
				ZLGfxHandle* handle = this->mStream->Read < ZLGfxHandle* >( 0 );
				draw.DeleteHandle ( handle );
				break;
			}
			case DEPTH_FUNC: {
			
				draw.DepthFunc (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case DEPTH_MASK: {
			
				draw.DepthMask (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case DISABLE: {
			
				draw.Disable (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case DISABLE_CLIENT_STATE: {
			
				draw.DisableClientState (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case DISABLE_VERTEX_ATTRIB_ARRAY: {
			
				draw.DisableVertexAttribArray (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case DRAW_ARRAYS: {
			
				draw.DrawArrays (
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case DRAW_ELEMENTS: {
			
				u32 primType					= this->mStream->Read < u32 >( 0 );
				u32 count						= this->mStream->Read < u32 >( 0 );
				u32 indexType					= this->mStream->Read < u32 >( 0 );
				ZLSharedConstBuffer* buffer		= this->mStream->Read < ZLSharedConstBuffer* >( 0 );
				size_t offset					= this->mStream->Read < size_t >( 0 );
			
				draw.DrawElements ( primType, count, indexType, buffer, offset );
				
				break;
			}
			case ENABLE: {
			
				draw.Enable (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case ENABLE_CLIENT_STATE: {
			
				draw.EnableClientState (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case ENABLE_VERTEX_ATTRIB_ARRAY: {
			
				draw.EnableVertexAttribArray (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case EVENT: {

				u32 event				= this->mStream->Read < u32 >( 0 );
				u32 listenerRecordIdx	= this->mStream->Read < u32 >( 0 );
				
				draw.Event ( this, event, ( void* )listenerRecordIdx );
				
				break;
			}
			case FLUSH: {
			
				bool finish				= this->mStream->Read < bool >( 0 );
			
				draw.Flush ( finish );
				break;
			}
			case FRAMEBUFFER_RENDERBUFFER: {
			
				u32 target						= this->mStream->Read < u32 >( 0 );
				u32 attachment					= this->mStream->Read < u32 >( 0 );
				ZLGfxHandle* renderbuffer		= this->mStream->Read < ZLGfxHandle* >( 0 );
				
				draw.FramebufferRenderbuffer ( target, attachment, renderbuffer );
				
				break;
			}
			case FRAMEBUFFER_TEXTURE_2D: {
			
				u32 target						= this->mStream->Read < u32 >( 0 );
				u32 attachment					= this->mStream->Read < u32 >( 0 );
				ZLGfxHandle* texture			= this->mStream->Read < ZLGfxHandle* >( 0 );
				s32 level						= this->mStream->Read < s32 >( 0 );
				
				draw.FramebufferTexture2D ( target, attachment, texture, level );
				
				break;
			}
			case GET_CURRENT_FRAMEBUFFER: {
			
				// TODO: GFX
				ZLGfxHandle* handle				= this->mStream->Read < ZLGfxHandle* >( 0 );
				
				break;
			}
			case GET_UNIFORM_LOCATION: {
			
				ZLGfxHandle* handle = this->mStream->Read < ZLGfxHandle* >( 0 );
				size_t size = this->mStream->Read < size_t >( 0 );
				
				char* name = ( char* )alloca ( size + 1 );
				this->mStream->ReadBytes ( name, size );
				name [ size ] = 0;
				
				u32 listenerRecordIdx = this->mStream->Read < u32 >( 0 );
				
				draw.GetUniformLocation ( handle, name, this, ( void* )listenerRecordIdx );
				
				break;
			}
			case LINE_WIDTH: {
			
				draw.LineWidth (
					this->mStream->Read < float >( 0 )
				);
				break;
			}
			case LINK_PROGRAM: {
			
				draw.LinkProgram (
					this->mStream->Read < ZLGfxHandle* >( 0 ),
					this->mStream->Read < bool >( true )
				);
				break;
			}
			case RENDER_BUFFER_STORAGE: {
				
				u32 internalFormat		= this->mStream->Read < u32 >( 0 );
				u32 width				= this->mStream->Read < u32 >( 0 );
				u32 height				= this->mStream->Read < u32 >( 0 );
				
				draw.RenderbufferStorage ( internalFormat, width, height );
				
				break;
			}
			case SCISSOR: {
			
				draw.Scissor (
					this->mStream->Read < s32 >( 0 ),
					this->mStream->Read < s32 >( 0 ),
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case SHADER_SOURCE: {
			
				ZLGfxHandle* handle		= this->mStream->Read < ZLGfxHandle* >( 0 );
				size_t length			= this->mStream->Read < size_t >( 0 );
				
				char* source = ( char* )alloca ( length + 1 );
				this->mStream->ReadBytes ( source, length );
				source [ length ] = 0;
			
				draw.ShaderSource (
					handle,
					source,
					length
				);
				break;
			}
			case TEX_ENVI: {
			
				draw.TexEnvi (
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < s32 >( 0 )
				);
				break;
			}
			case TEX_IMAGE_2D: {
			
				u32 level						= this->mStream->Read < u32 >( 0 );
				u32 internalFormat				= this->mStream->Read < u32 >( 0 );
				u32 width						= this->mStream->Read < u32 >( 0 );
				u32 height						= this->mStream->Read < u32 >( 0 );
				u32 format						= this->mStream->Read < u32 >( 0 );
				u32 type						= this->mStream->Read < u32 >( 0 );
				ZLSharedConstBuffer* buffer		= this->mStream->Read < ZLSharedConstBuffer* >( 0 );
				
				draw.TexImage2D ( level, internalFormat, width, height, format, type, buffer );
				
				break;
			}
			case TEX_PARAMETERI: {
			
				draw.TexParameteri (
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < s32 >( 0 )
				);
				break;
			}
			case TEX_SUB_IMAGE_2D: {
				
				u32 level						= this->mStream->Read < u32 >( 0 );
				u32 xOffset						= this->mStream->Read < u32 >( 0 );
				u32 yOffset						= this->mStream->Read < u32 >( 0 );
				u32 width						= this->mStream->Read < u32 >( 0 );
				u32 height						= this->mStream->Read < u32 >( 0 );
				u32 format						= this->mStream->Read < u32 >( 0 );
				u32 type						= this->mStream->Read < u32 >( 0 );
				ZLSharedConstBuffer* buffer		= this->mStream->Read < ZLSharedConstBuffer* >( 0 );
				
				draw.TexSubImage2D ( level, xOffset, yOffset, width, height, format, type, buffer );
				
				break;
			}
			case UNIFORM_1F: {
			
				draw.Uniform1f (
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < float >( 0 )
				);
				break;
			}
			case UNIFORM_1I: {
			
				draw.Uniform1i (
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < s32 >( 0 )
				);
				break;
			}
			case UNIFORM_4FV: {
			
				u32 location = this->mStream->Read < u32 >( location );
				u32 count = this->mStream->Read < u32 >( count );
				
				float vec [ 4 ];
				this->mStream->ReadBytes ( vec, sizeof ( float ) * 4 );
			
				draw.Uniform4fv ( location, count, vec );
				break;
			}
			case UNIFORM_MATRIX_3FV: {
				
				u32 location = this->mStream->Read < u32 >( 0 );
				u32 count = this->mStream->Read < u32 >( 0 );
				bool transpose = this->mStream->Read < bool >( false );
				
				float mtx [ 9 ];
				this->mStream->ReadBytes ( &mtx, sizeof ( float ) * 9 );
				
				draw.UniformMatrix3fv ( location, count, transpose, mtx );
				
				break;
			}
			case UNIFORM_MATRIX_4FV: {
				
				u32 location = this->mStream->Read < u32 >( 0 );
				u32 count = this->mStream->Read < u32 >( 0 );
				bool transpose = this->mStream->Read < bool >( false );
				
				float mtx [ 16 ];
				this->mStream->ReadBytes ( &mtx, sizeof ( float ) * 16 );
				
				draw.UniformMatrix4fv ( location, count, transpose, mtx );
			
				break;
			}
			case USE_PROGRAM: {
			
				draw.UseProgram (
					this->mStream->Read < ZLGfxHandle* >( 0 )
				);
				break;
			}
			case VERTEX_ATTRIB_POINTER: {
			
				u32 index						= this->mStream->Read < u32 >( 0 );
				u32 size						= this->mStream->Read < u32 >( 0 );
				u32 type						= this->mStream->Read < u32 >( 0 );
				bool normalized					= this->mStream->Read < bool >( false );
				u32 stride						= this->mStream->Read < u32 >( 0 );
				ZLSharedConstBuffer* buffer		= this->mStream->Read < ZLSharedConstBuffer* >( 0 );
				size_t offset					= this->mStream->Read < size_t >( 0 );
				
				draw.VertexAttribPointer (
					index,
					size,
					type,
					normalized,
					stride,
					buffer,
					offset
				);
				
				break;
			}
			case VIEWPORT: {
			
				draw.Viewport (
					this->mStream->Read < s32 >( 0 ),
					this->mStream->Read < s32 >( 0 ),
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
		}
	}
	
	this->mStream->Seek ( top, SEEK_SET );
	
	//zglEnd ();
}

//----------------------------------------------------------------//
void ZLGfxRetained::DrawArrays ( u32 primType, u32 first, u32 count ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( DRAW_ARRAYS );
	this->mStream->Write < u32 >( primType );
	this->mStream->Write < u32 >( first );
	this->mStream->Write < u32 >( count );
}

//----------------------------------------------------------------//
void ZLGfxRetained::DrawElements ( u32 primType, u32 count, u32 indexType, ZLSharedConstBuffer* buffer, size_t offset ) {

	this->Retain ( buffer );

	assert ( this->mStream );

	this->mStream->Write < u32 >( DRAW_ELEMENTS );
	this->mStream->Write < u32 >( primType );
	this->mStream->Write < u32 >( count );
	this->mStream->Write < u32 >( indexType );
	this->mStream->Write < ZLSharedConstBuffer* >( buffer );
	this->mStream->Write < size_t >( offset );
}

//----------------------------------------------------------------//
void ZLGfxRetained::Enable ( u32 cap ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ENABLE );
	this->mStream->Write < u32 >( cap );
}

//----------------------------------------------------------------//
void ZLGfxRetained::EnableClientState ( u32 cap ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ENABLE_CLIENT_STATE );
	this->mStream->Write < u32 >( cap );
}

//----------------------------------------------------------------//
void ZLGfxRetained::EnableVertexAttribArray ( u32 index ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ENABLE_VERTEX_ATTRIB_ARRAY );
	this->mStream->Write < u32 >( index );
}

//----------------------------------------------------------------//
void ZLGfxRetained::Event ( ZLGfxListener* listener, u32 event, void* userdata ) {

	assert ( this->mStream );

	if ( listener ) {
	
		this->mStream->Write < u32 >( EVENT );
		this->mStream->Write < u32 >( event );
		this->WriteListenerRecord ( listener, userdata );
	}
}

//----------------------------------------------------------------//
void ZLGfxRetained::Flush ( bool finish ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( FLUSH );
	this->mStream->Write < bool >( finish );
}

//----------------------------------------------------------------//
void ZLGfxRetained::FramebufferRenderbuffer ( u32 target, u32 attachment, ZLGfxHandle* renderbuffer ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( FRAMEBUFFER_RENDERBUFFER );
	this->mStream->Write < u32 >( target );
	this->mStream->Write < u32 >( attachment );
	this->mStream->Write < ZLGfxHandle* >( renderbuffer );
}

//----------------------------------------------------------------//
void ZLGfxRetained::FramebufferTexture2D ( u32 target, u32 attachment, ZLGfxHandle* texture, s32 level ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( FRAMEBUFFER_TEXTURE_2D );
	this->mStream->Write < u32 >( target );
	this->mStream->Write < u32 >( attachment );
	this->mStream->Write < ZLGfxHandle* >( texture );
	this->mStream->Write < s32 >( level );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxRetained::GetCurrentFramebuffer () {

	assert ( this->mStream );

	ZLGfxHandle* handle = new ZLGfxHandle ( ZLGfxHandle::FRAMEBUFFER, 0, false );
	
	this->mStream->Write < u32 >( GET_CURRENT_FRAMEBUFFER );
	this->mStream->Write < ZLGfxHandle* >( handle );
	
	return handle;
}

//----------------------------------------------------------------//
void ZLGfxRetained::GetUniformLocation ( ZLGfxHandle* program, cc8* uniformName, ZLGfxListener* listener, void* userdata ) {

	assert ( this->mStream );

	if ( listener ) {

		this->mStream->Write < u32 >( GET_UNIFORM_LOCATION );
		this->mStream->Write < ZLGfxHandle* >( program );
		
		size_t size = strlen ( uniformName );
		this->mStream->Write < size_t >( size );
		this->mStream->WriteBytes ( uniformName, size );
		
		this->WriteListenerRecord ( listener, userdata );
	}
}

//----------------------------------------------------------------//
bool ZLGfxRetained::HasContent () {

	return ( this->mStream->GetCursor () > 0 );
}

//----------------------------------------------------------------//
void ZLGfxRetained::LineWidth ( float width ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( LINE_WIDTH );
	this->mStream->Write < float >( width );
}

//----------------------------------------------------------------//
void ZLGfxRetained::LinkProgram ( ZLGfxHandle* program, bool log ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( LINK_PROGRAM );
	this->mStream->Write < ZLGfxHandle* >( program );
	this->mStream->Write < bool >( log );
}

//----------------------------------------------------------------//
void ZLGfxRetained::OnGfxEvent ( u32 event, void* userdata ) {

	size_t idx = ( size_t )userdata;
	if ( idx < this->mListenerRecords.GetTop ()) {
		ZLGfxListenerRecord& listenerRecord = this->mListenerRecords [ idx ];
		
		listenerRecord.mCallbackID = ZLGfxListenerRecord::ON_EVENT;
		listenerRecord.mEvent = event;
	}
}

//----------------------------------------------------------------//
void ZLGfxRetained::OnUniformLocation ( u32 addr, void* userdata ) {
	
	size_t idx = ( size_t )userdata;
	if ( idx < this->mListenerRecords.GetTop ()) {
		ZLGfxListenerRecord& listenerRecord = this->mListenerRecords [ idx ];
		
		listenerRecord.mCallbackID = ZLGfxListenerRecord::ON_UNIFORM_LOCATION;
		listenerRecord.mUniformAddr = addr;
	}
}

//----------------------------------------------------------------//
void ZLGfxRetained::PopSection () {
}

//----------------------------------------------------------------//
void ZLGfxRetained::PublishEvents () {

	size_t top = this->mListenerRecords.GetTop ();
	for ( size_t i = 0; i < top; ++i ) {
	
		ZLGfxListenerRecord& record = this->mListenerRecords [ i ];
		
		ZLGfxListener* listener = record.mListenerHandle->GetTarget ();
		if ( listener ) {
		
			switch ( record.mCallbackID ) {
			
				case ZLGfxListenerRecord::ON_EVENT:
					listener->OnGfxEvent ( record.mEvent, record.mUserdata );
					break;
			
				case ZLGfxListenerRecord::ON_UNIFORM_LOCATION:
					listener->OnUniformLocation ( record.mUniformAddr, record.mUserdata );
					break;
			}
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxRetained::PublishEventsAndReset () {

	this->PublishEvents ();
	this->Reset ();
}

//----------------------------------------------------------------//
bool ZLGfxRetained::PushErrorHandler () {

	return false;
}

//----------------------------------------------------------------//
void ZLGfxRetained::PushSection () {
}

//----------------------------------------------------------------//
bool ZLGfxRetained::PushSuccessHandler () {

	// TODO: gfx

	return true;
}

//----------------------------------------------------------------//
void ZLGfxRetained::ReadPixels ( s32 x, s32 y, u32 width, u32 height, u32 format, u32 type, u32 pixelSize, ZLGfxListener* listener ) {
	UNUSED ( x );
	UNUSED ( y );
	UNUSED ( width );
	UNUSED ( height );
	UNUSED ( format );
	UNUSED ( type );
	UNUSED ( pixelSize );
	UNUSED ( listener );
}

//----------------------------------------------------------------//
void ZLGfxRetained::RenderbufferStorage ( u32 internalFormat, u32 width, u32 height ) {
	
	assert ( this->mStream );
	
	this->mStream->Write < u32 >( RENDER_BUFFER_STORAGE );
	this->mStream->Write < u32 >( internalFormat );
	this->mStream->Write < u32 >( width );
	this->mStream->Write < u32 >( height );
}

//----------------------------------------------------------------//
void ZLGfxRetained::Reset () {

	assert ( this->mStream );

	if ( this->mListenerRecords.GetTop () > 0 ) {
		this->Abandon ();
		this->mListenerRecords.Reset ();
	}

	this->mStream->Seek ( 0, SEEK_SET );
	
	this->mListenerRecords.Reset ();
	
	while ( this->mReleaseStack.GetTop ()) {
		ZLRefCountedObject* object = this->mReleaseStack.Pop ();
		object->Release ();
	}
	
	if ( this->mBufferSize < this->mMaxAllocated ) {
	
		if ( this->mBuffer ) {
			free ( this->mBuffer );
		}
		
		size_t paddedSize = 1;
		while ( paddedSize < this->mMaxAllocated ) paddedSize = paddedSize << 0x01;
		
		this->mBufferSize = paddedSize;
		this->mBuffer = malloc ( paddedSize );
	}
	
	this->mAllocated = 0;
	this->mBufferTop = 0;
}

//----------------------------------------------------------------//
void ZLGfxRetained::Retain ( ZLRefCountedObject* object ) {

	if ( object ) {
		object->Retain ();
		this->mReleaseStack.Push ( object );
	}
}

//----------------------------------------------------------------//
void ZLGfxRetained::Scissor ( s32 x, s32 y, u32 w, u32 h ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( SCISSOR );
	this->mStream->Write < s32 >( x );
	this->mStream->Write < s32 >( y );
	this->mStream->Write < u32 >( w );
	this->mStream->Write < u32 >( h );
}

//----------------------------------------------------------------//
void ZLGfxRetained::ShaderSource ( ZLGfxHandle* shader, cc8* source, size_t length ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( SHADER_SOURCE );
	this->mStream->Write < ZLGfxHandle* >( shader );
	this->mStream->Write < size_t >( length );
	this->mStream->WriteBytes ( source, length );
}

//----------------------------------------------------------------//
void ZLGfxRetained::TexEnvi ( u32 pname, s32 param ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( TEX_ENVI );
	this->mStream->Write < u32 >( pname );
	this->mStream->Write < s32 >( param );
}

//----------------------------------------------------------------//
void ZLGfxRetained::TexImage2D ( u32 level, u32 internalFormat, u32 width, u32 height, u32 format, u32 type, ZLSharedConstBuffer* buffer ) {
	
	this->Retain ( buffer );
	
	assert ( this->mStream );

	this->mStream->Write < u32 >( TEX_IMAGE_2D );
	this->mStream->Write < u32 >( level );
	this->mStream->Write < u32 >( internalFormat );
	this->mStream->Write < u32 >( width );
	this->mStream->Write < u32 >( height );
	this->mStream->Write < u32 >( format );
	this->mStream->Write < u32 >( type );
	this->mStream->Write < ZLSharedConstBuffer* >( buffer );
}

//----------------------------------------------------------------//
void ZLGfxRetained::TexParameteri ( u32 pname, s32 param ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( TEX_PARAMETERI );
	this->mStream->Write < u32 >( pname );
	this->mStream->Write < s32 >( param );
}

//----------------------------------------------------------------//
void ZLGfxRetained::TexSubImage2D ( u32 level, s32 xOffset, s32 yOffset, u32 width, u32 height, u32 format, u32 type, ZLSharedConstBuffer* buffer ) {

	this->Retain ( buffer );

	assert ( this->mStream );

	this->mStream->Write < u32 >( TEX_SUB_IMAGE_2D );
	this->mStream->Write < u32 >( level );
	this->mStream->Write < s32 >( xOffset );
	this->mStream->Write < s32 >( yOffset );
	this->mStream->Write < u32 >( width );
	this->mStream->Write < u32 >( height );
	this->mStream->Write < u32 >( format );
	this->mStream->Write < u32 >( type );
	this->mStream->Write < ZLSharedConstBuffer* >( buffer );
}

//----------------------------------------------------------------//
void ZLGfxRetained::Uniform1f ( u32 location, float v0 ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( UNIFORM_1F );
	this->mStream->Write < u32 >( location );
	this->mStream->Write < float >( v0 );
}

//----------------------------------------------------------------//
void ZLGfxRetained::Uniform1i ( u32 location, s32 v0 ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( UNIFORM_1I );
	this->mStream->Write < u32 >( location );
	this->mStream->Write < s32 >( v0 );
}

//----------------------------------------------------------------//
void ZLGfxRetained::Uniform4fv ( u32 location, u32 count, const float* value ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( UNIFORM_4FV );
	this->mStream->Write < u32 >( location );
	this->mStream->Write < u32 >( count );
	this->mStream->WriteBytes ( value, sizeof ( float ) * 4 );
}

//----------------------------------------------------------------//
void ZLGfxRetained::UniformMatrix3fv ( u32 location, u32 count, bool transpose, const float* mtx ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( UNIFORM_MATRIX_3FV );
	this->mStream->Write < u32 >( location );
	this->mStream->Write < u32 >( count );
	this->mStream->Write < bool >( transpose );
	this->mStream->WriteBytes ( mtx, sizeof ( float ) * 9 );
}

//----------------------------------------------------------------//
void ZLGfxRetained::UniformMatrix4fv ( u32 location, u32 count, bool transpose, const float* mtx ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( UNIFORM_MATRIX_4FV );
	this->mStream->Write < u32 >( location );
	this->mStream->Write < u32 >( count );
	this->mStream->Write < bool >( transpose );
	this->mStream->WriteBytes ( mtx, sizeof ( float ) * 16 );
}

//----------------------------------------------------------------//
void ZLGfxRetained::UseProgram ( ZLGfxHandle* program ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( USE_PROGRAM );
	this->mStream->Write < ZLGfxHandle* >( program );
}

//----------------------------------------------------------------//
void ZLGfxRetained::VertexAttribPointer ( u32 index, u32 size, u32 type, bool normalized, u32 stride, ZLSharedConstBuffer* buffer, size_t offset ) {

	this->Retain ( buffer );

	assert ( this->mStream );

	this->mStream->Write < u32 >( VERTEX_ATTRIB_POINTER );
	this->mStream->Write < u32 >( index );
	this->mStream->Write < u32 >( size );
	this->mStream->Write < u32 >( type );
	this->mStream->Write < bool >( normalized );
	this->mStream->Write < u32 >( stride );
	this->mStream->Write < ZLSharedConstBuffer* >( buffer );
	this->mStream->Write < size_t >( offset );
}

//----------------------------------------------------------------//
void ZLGfxRetained::Viewport ( s32 x, s32 y, u32 w, u32 h ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( VIEWPORT );
	this->mStream->Write < s32 >( x );
	this->mStream->Write < s32 >( y );
	this->mStream->Write < u32 >( w );
	this->mStream->Write < u32 >( h );
}

//----------------------------------------------------------------//
ZLGfxListenerRecord& ZLGfxRetained::WriteListenerRecord ( ZLGfxListener* listener, void* userdata ) {

	ZLGfxListenerHandle* listenerHandle = listener->GetRetainedHandle ();
	this->mReleaseStack.Push ( listenerHandle );
	
	u32 idx = this->mListenerRecords.GetTop ();
	ZLGfxListenerRecord& record = this->mListenerRecords.Push ();

	record.mListenerHandle		= listenerHandle;
	record.mUserdata			= userdata;
	record.mCallbackID			= ZLGfxListenerRecord::UNKNOWN;
	record.mEvent				= 0;
	record.mUniformAddr			= 0;

	this->mStream->Write < u32 >( idx );
	
	return record;
}

//----------------------------------------------------------------//
ZLGfxRetained::ZLGfxRetained () :
	mStream ( &this->mMemStream ),
	mBuffer ( 0 ),
	mBufferSize ( 0 ),
	mBufferTop ( 0 ),
	mAllocated ( 0 ),
	mMaxAllocated ( 0 ) {
}

//----------------------------------------------------------------//
ZLGfxRetained::~ZLGfxRetained () {

	if ( this->mBuffer ) {
		free ( this->mBuffer );
	}
}
