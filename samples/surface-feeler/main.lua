----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

SPEED = 1
DECAY = 0.85

MOAISim.openWindow ( "test", 480, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 480, 480 )
viewport:setScale ( 480, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

surfaceDeck = MOAISurfaceDeck2D.new ()
surfaceDeck:reserveSurfaceLists ( 1 )
surfaceDeck:reserveSurfaces ( 1, 4 )

--surfaceDeck:setSurface ( 1, 1,	-100, 100, 100, 100 )
--surfaceDeck:setSurface ( 1, 2,	100, 100, 100, -100 )
--surfaceDeck:setSurface ( 1, 3,	100, -100, -100, -100 )
--surfaceDeck:setSurface ( 1, 4,	-100, -100, -100, 100 )

surfaceDeck:setSurface ( 1, 1,	100, 100, -100, 100 )
surfaceDeck:setSurface ( 1, 2,	100, -100, 100, 100 )
surfaceDeck:setSurface ( 1, 3,	-100, -100, 100, -100 )
surfaceDeck:setSurface ( 1, 4,	-100, 100, -100, -100 )

surfaceProp = MOAISurfaceProp2D.new ()
surfaceProp:setDeck ( surfaceDeck )
layer:insertProp ( surfaceProp )

feeler = MOAISimpleSurfaceFeeler2D.new ()
feeler:setEllipse ( 64 )
feeler:start ()
layer:insertProp ( feeler )

main = function ()

	local x = 0
	local y = 0

	while true do

		x = x * DECAY
		y = y * DECAY

		if MOAIInputMgr.device.keyboard:keyIsDown ( MOAIKeyCode.RIGHT ) then
			x = x + SPEED
		end

		if MOAIInputMgr.device.keyboard:keyIsDown ( MOAIKeyCode.LEFT ) then
			x = x - SPEED
		end

		if MOAIInputMgr.device.keyboard:keyIsDown ( MOAIKeyCode.UP ) then
			y = y + SPEED
		end

		if MOAIInputMgr.device.keyboard:keyIsDown ( MOAIKeyCode.DOWN ) then
			y = y - SPEED
		end

		surfaceProp:moveRot ( 0, 0, 0.5 )

		feeler:setMove ( x, y )
		coroutine.yield ()
	end
end

thread = MOAICoroutine.new ()
thread:run ( main )
