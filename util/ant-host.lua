--==============================================================
-- setup
--==============================================================

MOAI_SDK_HOME	= MOAIFileSystem.getAbsoluteDirectoryPath ( '../' ) -- default path to Moai SDK relative to script dir
SCRIPT_DIR		= MOAIFileSystem.getAbsoluteDirectoryPath ( arg [ 1 ])
INVOKE_DIR		= MOAIFileSystem.getAbsoluteDirectoryPath ( arg [ 2 ])

MOAIFileSystem.setWorkingDirectory ( SCRIPT_DIR )

require ( 'util' )

--==============================================================
-- args
--==============================================================

OUTPUT_DIR				= INVOKE_DIR .. 'host/'
MOAI_PROJECT_PATH		= OUTPUT_DIR .. 'project/'
MOAI_JAVA_NAMESPACE		= 'com.ziplinegames.moai'

----------------------------------------------------------------
for i, escape, param, iter in util.iterateCommandLine ( arg or {}) do
end

MODULES = {}

MODULE_APP_DECLARATIONS			= ''
MODULE_MANIFEST_PERMISSIONS		= ''
MODULE_PROJECT_INCLUDES			= ''

----------------------------------------------------------------
local config = {}

config.PROJECT_NAME					= 'MoaiSample'
config.PACKAGE_NAME					= 'com.ziplinegames.moaisample'
config.PLATFORM_NAME				= 'android'

config.LUA_WORKING_DIR				= 'bundle/assets/lua'
config.LUA_MAIN						= 'main.lua'

config.MANIFEST_DEBUGGABLE			= 'false'

config.KEYSTORE_PATH				= nil
config.KEYSTORE_NAME				= nil
config.KEYSTORE_PASSWORD			= nil
config.KEYSTORE_ALIAS				= nil
config.KEYSTORE_ALIAS_PASSWORD		= nil

config.LUA_WORKING_DIR				= 'bundle/assets/lua'
config.LUA_MAIN						= 'main.lua'

--==============================================================
-- util
--==============================================================

local importJava
local importLib
local processConfigFile

----------------------------------------------------------------
local importJava = function ( path, namespace )

	if not path then return end

	local projectSrcFolder	= string.format ( '%ssrc/%s/', MOAI_PROJECT_PATH, string.gsub ( namespace, '%.', '/' ))

	local files = util.listFiles ( path, 'java' )
	for i, filename in ipairs ( files ) do
		MOAIFileSystem.copy (  path .. filename, projectSrcFolder .. filename )
	end
end

----------------------------------------------------------------
local importLib = function ( path )
	
	if not path then return end
	
	local srcPath = path .. 'src/'
	if MOAIFileSystem.checkPathExists ( srcPath ) then
		MOAIFileSystem.copy (  srcPath, MOAI_PROJECT_PATH .. 'src/' )
	end

	local libPath = path .. 'lib/'
	if MOAIFileSystem.checkPathExists ( libPath ) then
		for i, filename in ipairs ( util.listFiles ( libPath, 'jar' )) do
			MOAIFileSystem.copy (  libPath .. filename, MOAI_PROJECT_PATH .. 'libs/' .. filename )
		end
	end
	
	local projectPath = path .. 'project/'
	if MOAIFileSystem.checkPathExists ( projectPath ) then
		for i, pathname in ipairs ( util.listDirectories ( projectPath )) do
			MOAIFileSystem.copy (  projectPath .. pathname, OUTPUT_DIR .. pathname )
			MODULE_PROJECT_INCLUDES = MODULE_PROJECT_INCLUDES .. string.format ( 'android.library.reference.1=../%s/\n', pathname )
		end
	end
	
	local appDeclarationsPath = path .. 'manifest_declarations.xml'
	
	if MOAIFileSystem.checkFileExists ( appDeclarationsPath ) then
		local fp = io.open ( appDeclarationsPath, "r" )
		MODULE_APP_DECLARATIONS = MODULE_APP_DECLARATIONS .. '\n' .. fp:read ( "*all" )
		fp:close ()
	end
	
	local manifestPermissionsPath = path .. 'manifest_permissions.xml'
	if MOAIFileSystem.checkFileExists ( manifestPermissionsPath ) then
		local fp = io.open ( manifestPermissionsPath, "r" )
		MODULE_MANIFEST_PERMISSIONS = MODULE_MANIFEST_PERMISSIONS .. '\n' .. fp:read ( "*all" )
		fp:close ()
	end
end

----------------------------------------------------------------
processConfigFile = function ( filename )

	filename = MOAIFileSystem.getAbsoluteFilePath ( filename )
	if not MOAIFileSystem.checkFileExists ( filename ) then return end
	local configPath = util.getFolderFromPath ( filename )

	local configFile = { MOAI_SDK_HOME = MOAI_SDK_HOME }
	util.dofileWithEnvironment ( filename, configFile )

	for k, v in pairs ( configFile ) do
		config [ k ] = config [ k ] and v
	end

	if configFile.MODULES then
		for name, mod in pairs ( configFile.MODULES ) do

			local src = mod.src
			if src and string.find ( src, '^%.' ) then src = configPath .. src end

			local lib = mod.lib
			if lib and string.find ( lib, '^%.' ) then lib = configPath .. lib end

			MODULES [ name ] = {
				namespace = mod.namespace,
				src = MOAIFileSystem.getAbsoluteDirectoryPath ( src ),
				lib = MOAIFileSystem.getAbsoluteDirectoryPath ( lib ),
			}
		end
	end
end

--==============================================================
-- main
--==============================================================

--[[
PACKAGE_NAME				= string.lower ( string.format ( 'com.%s.%s', PUBLISHER_NAME, PROJECT_NAME ))
PACKAGE_PATH				= string.gsub ( PACKAGE_NAME, '.', '/' )

APP_SRC_PATH				= MOAI_PROJECT_PATH .. 'src/' .. PACKAGE_PATH .. '/'

COPY = {
	{ dst = MOAI_PROJECT_PATH .. 'assets/lua',		src = LUA_SRC_PATH },
	{ dst = MOAI_PROJECT_PATH .. 'res/',			src = string.format ( '%sres-%s', TEMPLATE_PATH, TARGET )},
}
]]--

processConfigFile ( MOAI_SDK_HOME .. 'ant/host/config.lua' )

MOAIFileSystem.deleteDirectory ( OUTPUT_DIR, true )
MOAIFileSystem.affirmPath ( OUTPUT_DIR )

MOAIFileSystem.copy ( 'ant-host/project', MOAI_PROJECT_PATH )

if config.KEYSTORE_PATH and config.KEYSTORE_NAME then
	MOAIFileSystem.copy ( config.KEYSTORE_PATH .. config.KEYSTORE_NAME, MOAI_PROJECT_PATH .. config.KEYSTORE_NAME )
end

for name, mod in pairs ( MODULES ) do
	importJava ( mod.src, mod.namespace or MOAI_JAVA_NAMESPACE )
	importLib ( mod.lib )
end

util.replaceInFiles ({
	
	[ MOAI_PROJECT_PATH .. 'AndroidManifest.xml' ] = {
		[ '@EXTERNAL_APPLICATION_ENTRIES@' ]	= MODULE_APP_DECLARATIONS,
		[ '@EXTERNAL_MANIFEST_PERMISSIONS@' ] 	= MODULE_MANIFEST_PERMISSIONS,
	},
	
	[ MOAI_PROJECT_PATH .. 'project.properties' ] = {
		[ '@EXTERNAL_PROJECT_INCLUDES@' ] 		= MODULE_PROJECT_INCLUDES,
	},
})

util.replaceInFiles ({
	
	[ MOAI_PROJECT_PATH .. '.project' ] = {
		[ '@NAME@' ]							= config.PROJECT_NAME,
	},
	
	[ MOAI_PROJECT_PATH .. 'build.xml' ] = {
		[ '@NAME@' ]							= config.PROJECT_NAME,
	},
	
	[ MOAI_PROJECT_PATH .. 'AndroidManifest.xml' ] = {
		[ '@NAME@' ]							= config.PROJECT_NAME,
		[ '@PACKAGE@' ]							= config.PACKAGE_NAME,
		[ '@VERSION_CODE@' ]					= '1',
		[ '@VERSION_NAME@' ]					= '1.0',
		[ '@DEBUGGABLE@' ]						= config.MANIFEST_DEBUGGABLE,
		[ '@EXTERNAL_APPLICATION_ENTRIES@' ]	= MODULE_APP_DECLARATIONS,
		[ '@EXTERNAL_MANIFEST_PERMISSIONS@' ] 	= MODULE_MANIFEST_PERMISSIONS,
	},
	
	[ MOAI_PROJECT_PATH .. 'ant.properties' ] = {
		[ '@KEY_STORE@' ]						= config.KEYSTORE_NAME,
		[ '@KEY_ALIAS@' ]						= config.KEYSTORE_ALIAS,
		[ '@KEY_STORE_PASSWORD@' ]				= config.KEYSTORE_PASSWORD,
		[ '@KEY_ALIAS_PASSWORD@' ]				= config.KEYSTORE_ALIAS_PASSWORD,
	},
	
	[ util.wrap ( util.iterateFiles, MOAI_PROJECT_PATH, '^local.properties$' )] = {
		[ '@SDK_ROOT@' ]						= '$ANDROID_HOME',
	},
	
	[ MOAI_PROJECT_PATH .. 'project.properties' ] = {
		[ '@EXTERNAL_PROJECT_INCLUDES@' ] 		= MODULE_PROJECT_INCLUDES,
	},

	[ util.wrap ( util.iterateFiles, MOAI_PROJECT_PATH .. 'src', '.java$' )] = {
		[ '@PACKAGE@' ]							= config.PACKAGE_NAME,
		[ '@PLATFORM_NAME@' ]					= string.upper ( config.PLATFORM_NAME ),
		[ '@RUN_SCRIPTS@' ]						= config.LUA_MAIN,
		[ '@WORKING_DIR@' ]						= config.LUA_WORKING_DIR,
	},
})
