// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXRESOURCEMGR_H
#define	MOAIGFXRESOURCEMGR_H

class MOAIGfxResource;

//================================================================//
// MOAIGfxResourceMgr
//================================================================//
class MOAIGfxResourceMgr :
	public MOAIGlobalClass < MOAIGfxResourceMgr, MOAILuaObject > {
private:
	
	typedef ZLLeanList < MOAIGfxResource* >::Iterator ResourceIt;
	ZLLeanList < MOAIGfxResource* >		mResources;
	
	ZLLeanList < MOAIGfxResource* >		mPendingForLoadList;
	ZLLeanList < MOAIGfxResource* >		mPendingForDrawList;
	
	ZLLeanStack < ZLGfxHandle*, 32 >	mDeleterStack;

	//----------------------------------------------------------------//
	static int		_purgeResources				( lua_State* L );
	static int		_renewResources				( lua_State* L );

	//----------------------------------------------------------------//
	void			InsertGfxResource		( MOAIGfxResource& resource );
	void			ProcessDeleters			();
	void			ProcessPending			( ZLLeanList < MOAIGfxResource* > &list );
	void			RemoveGfxResource		( MOAIGfxResource& resource );
	void			RenewResources			();
	void			ScheduleGPUAffirm		( MOAIGfxResource& resource, u32 listID );
	
public:
	
	friend class MOAIGfxDevice;
	friend class MOAIGfxResource;
	friend class MOAIRenderMgr;
	
	DECL_LUA_SINGLETON ( MOAIGfxResourceMgr )
	
	//----------------------------------------------------------------//
					MOAIGfxResourceMgr		();
					~MOAIGfxResourceMgr		();
	void			PurgeResources			( u32 age = 0 );
	void			PushDeleter				( ZLGfxHandle* handle );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			Update					();
};

#endif
