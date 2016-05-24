// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXDEVICESTATECACHE_H
#define	MOAIGFXDEVICESTATECACHE_H

#include <moai-sim/MOAIGfxDeviceBase.h>
#include <moai-sim/MOAIBlendMode.h>


class MOAICamera;
class MOAIFrameBuffer;
class MOAIGfxResource;
class MOAIIndexBuffer;
class MOAIMultiTexture;
class MOAIShader;
class MOAIShaderProgram;
class MOAITexture;
class MOAISingleTexture;
class MOAITextureBase;
class MOAIVertexArray;
class MOAIVertexBuffer;
class MOAIVertexFormat;
class MOAIViewport;

// TODO: All this crap needs to get pushed down into zlgfx. The implementation of ZLGfxImmediate, in particular. Way
// too low level to be here, and just makes the render code more confusing. Also, pushing down will make it much
// easier to expose display lists as a framework-wide concept, and to add support for drawing higher level primitives
// inside the display list. And can more easily layer in a "nice" line-drawing mode. So... bunch of reasons to move
// this down.

//================================================================//
// MOAIGfxDeviceStateCache
//================================================================//
class MOAIGfxDeviceStateCache :
	public MOAIGfxDeviceBase {
protected:

	int										mCullFunc;
	
	int										mDepthFunc;
	bool									mDepthMask;

	MOAIBlendMode							mBlendMode;
	bool									mBlendEnabled;

	float									mPenWidth;

	ZLRect									mScissorRect;
	MOAIShaderProgram*						mShaderProgram;
	
	ZLLeanArray < MOAISingleTexture* >		mTextureUnits;
	u32										mActiveTextures;

	MOAIFrameBuffer*						mCurrentFrameBuffer;
	MOAIIndexBuffer*						mCurrentIdxBuffer;
	MOAITextureBase*						mCurrentTexture;
	MOAIVertexArray*						mCurrentVtxArray;
	MOAIVertexBuffer*						mCurrentVtxBuffer;
	MOAIVertexFormat*						mCurrentVtxFormat;

	//----------------------------------------------------------------//
	virtual	MOAIFrameBuffer*	GetDefaultFrameBuffer		() = 0;
	virtual	MOAITexture*		GetDefaultTexture			() = 0;
	bool						BindTexture					( u32 textureUnit, MOAISingleTexture* texture );

public:
	
	//----------------------------------------------------------------//
	bool			BindFrameBuffer				( MOAIFrameBuffer* frameBuffer = 0 );
	bool			BindIndexBuffer				( MOAIIndexBuffer* buffer = 0 );
	bool			BindShader					( MOAIShader* shader = 0 );
	bool			BindShader					( u32 preset );
	bool			BindShader					( MOAIShaderProgram* program );
	bool			BindTexture					( MOAITextureBase* textureSet = 0 );
	bool			BindVertexArray				( MOAIVertexArray* vtxArray = 0 );
	bool			BindVertexBuffer			( MOAIVertexBuffer* buffer = 0 );
	bool			BindVertexFormat			( MOAIVertexFormat* format = 0, bool copyBuffer = false );
	
					MOAIGfxDeviceStateCache		();
	virtual			~MOAIGfxDeviceStateCache	();

	virtual void	OnGfxStateWillChange		() = 0;

	void			SetBlendMode				();
	void			SetBlendMode				( const MOAIBlendMode& blendMode );
	void			SetBlendMode				( int srcFactor, int dstFactor, int equation = ZGL_BLEND_MODE_ADD );

	void			SetCullFunc					();
	void			SetCullFunc					( int cullFunc );
	
	void			SetDepthFunc				();
	void			SetDepthFunc				( int depthFunc );
	void			SetDepthMask				( bool depthMask );
	
	void			SetPenWidth					( float penWidth );
	
	void			SetScissorRect				();
	void			SetScissorRect				( ZLRect rect );
	
	void			UnbindAll					();
};

#endif
