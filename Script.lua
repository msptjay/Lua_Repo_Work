--[[
Lua Assignment Game
Author:		Philip Alassad
Filename:	Script.lua
Date:		March 2025
Last Updated:	16/03/2025

Description: The lua script file for the entire assignment game. 
**DO NOT DELETE, MOVE OR CHANGE ANY OF THIS FILES COMMENTS OR COMPLETED CODE EXAMPLES. 
DOING SO MAY RESULT IN A SIGNIFICANT MARKS PENALTY***********************************
]]
math.randomseed(os.time())
--[[***********************************PART 1 OF ASSIGNMENT TASK Instructions*************************************************
I have done a few of the variables to get you started.

DO NOT MOVE OR RENAME THESE, though you are welcome to change the values.

When filling out filenames for assets use the filepath (i.e. "assets/....").

Each variable is worth 1 point, except for the 2 tables in the creature class 
and the table in the level class which are worth 5 points each.

Total number of constants and value sets to expose = 47 (NOT including the one's I've done for you)  worth a total of 59%.

Each class comment states how many elements to be exposed 
(this number includes any already completed by me beforehand).

The Game MUST run without crashing 
otherwise marks capped at 40% for assignment PART 1

To complete this first task there is Absolutely NO NEED to change ANY of the C++ code.
Doing this could result in the above penalty if the game stops working...
*****************************************************************************************************************************]]

--Window init constants (In main) x 4 *************************************************************************************
--[[1]] width = 1600
--[[2]] height = 900
--[[3]] screenX = 400
--[[4]] screenY = 540

--Game class constants x 1 *************************************************************************************************
--[[5]] FPS = 60

--SceneManager constants x 8 ***********************************************************************************************
--[[6]]HiScorePosition = {x = 10, y = 0} --filled out for you
--[[7]]HiScoreTextColour = {r = 150, g = 79, b = 255} --filled out for you
--[[8]]LastScorePosition = {x = 10, y = 50} 
--[[9]]LastScoreTextColour = {r = 150, g = 79, b = 255}

--[[10]]ButtonHoverTextColour = {r = 250, g = 120, b = 200} 
--[[11]]ButtonTextColour = {r = 250, g = 250, b = 210}

--[[12]]MenuClearColour = {r = 0, g = 0, b = 0} 
--[[13]]GameOverClearColour = {r = 0, g = 0, b = 0}

--Menu class constants x 4 *************************************************************************************************
--[[14]]button1Ypos = 400
--[[15]]button2Ypos = 640

--[[16]]buttonName = "assets/buttonLong_grey.png"

--[[17]]buttonTint = {r = 255, g = 255, b = 0} 

--PlayGame class constants x 20 ********************************************************************************************
--[[18]]invincibilityTime = 5.0 --filled out for you
--[[19]]MaximumLevels = 4 --filled out for you
--[[20 make at least 50 enemies (or count the max number in all the levels)]] maximumPooledEnemies = 50 
--[[21]]timeBetweenShots = 0.5
--[[22]]MaximumPooledPlayerShots = 10
--[[23]]MaximumPooledEnemyShots = 20
--[[24 gets the game scrolling]]SceneScrollAmmount = 100 
--[[25]]DistanceFromEndLevelExit = 300

--[[26 bullet variant 1]]PlayerBulletSprite = "assets/laser player.png" 
--[[27 bullet variant 2]]EnemyBulletSprite = "assets/laser enemy.png"
--[[28]]ScrollingBGImage = "assets/blue.png" --filled out for you

--[[29]]LivesTextColour = {r = 250, g = 120, b = 200} --filled out for you
--[[30]]livesTextPosition = {x = 0, y = 0} --filled out for you
--[[31]]LivesIconImagePosition = {x = 0, y = 10} --filled out for you

--[[32]]LevelTextColour = {r = 150, g = 79, b = 255} 
--[[33]]LevelTextPosition = {x = 100, y = 0} --filled out for you

--[[34]]ScoreTextColour = {r = 250, g = 120, b = 200} 
--[[35]]ScoreTextPosition = {x = 750, y = 0} --filled out for you

--[[36]]FramerateColour = {r = 250, g = 250, b = 210} --filled out for you
--[[37]]FrameratePosition = {x = 400, y = 830} --filled out for you

--Game over class constants x 4 *******************************************************************************************
--[[38]]GameOverTimeLimit = 5.0 --filled out for you
--[[39]]GameOverTextColour = {r = 150, g = 79, b = 255} --filled out for you
--[[40]]GameOverMessagePosition = {x = 400, y = 500} 
--[[41]]GameOverMessage = "Game over womp womp"

--Hud class constants x 7 *************************************************************************************************
--[[42]]smallTextSize = 24
--[[43]]mediumTextSize = 48
--[[44]]largeTextSize = 72

--[[45 font 1]]fontName1 = "assets/DejaVuSans.ttf" 
--[[46 font 2]]fontName2 = "assets/DejaVuSans.ttf"
--[[47 font 3]]fontName3 = "assets/gomarice_g_type.ttf"--"assets/DejaVuSans.ttf" --filled out for you

--[[48 the lives image]]LivesIcon = "assets/playerLife3_blue.png"

--Creature class constants x 3 and 2 x tables ***********************************
--[[49 size of death animation]]maxDeathAnimationFrames = 9
--[[50 how long to play frames?]]timeToPlayFrames = 2.0

--make a table here for all 6 of the enemy sprites i.e. the filenames
--The table keys should use letters NOT numbers (i.e. a = "assets/anasset.png")
--look in the place they are set in the creature class to see

--[[51 the different enemy ships]]enemySprites = {a = "assets/enemyBlue5.png", b = "assets/enemyBlack1.png", c = "assets/enemyGreen2.png", d = "assets/enemyOrange3.png", e = "assets/enemyRed3.png", f = "assets/enemyLightBlue4.png"}

--make a table here for all 9 of the explosion frames
--The table keys should use letters NOT numbers (i.e. a = "assets/anasset.png")
--look in the place they are set in the creature class to see
--[[52 all the explosion frames]]explosionFrames = {a = "assets/regularExplosion00.png", b = "assets/regularExplosion01.png", c = "assets/regularExplosion02.png"
, d = "assets/regularExplosion03.png", e = "assets/regularExplosion04.png", f = "assets/regularExplosion05.png", g = "assets/regularExplosion06.png", h = "assets/regularExplosion07.png", i = "assets/regularExplosion08.png"}

--[[53 the player ship]]PlayerSprite = "assets/player ship.png" 

--Player class constants x 4 ************************************************************************************************
--[[54]]MaxLives = 3
--[[55 starting lives]]InitialLives = MaxLives 
--[[56]]ScoreMultiplier = 10
--[[57]]ScoreMultipleToGetBonus = 1000 --filled out for you

--Enemy class constants x 1 ************************************************************************************************* 
--[[58 how much are enemies worth]]enemyPointValue = 100

--Projectile class constants x 1 ********************************************************************************************
--[[59 how fast should bullets be?]]bulletSpeed = 2

--Level class constants x 5 and 1 x table ****************************************

--make a table here for all 6 of the coloured wall images
--The table keys should use letters NOT numbers (i.e. a = "assets/anasset.png")
--look in the place they are set in the Level class to see
--[[60 the different coloured wall blocks]]LevelBasedWallTextures = {a = "assets/spaceBuilding_001.png", b = "assets/spaceBuilding_007.png", c = "assets/spaceBuilding_008.png", d = "assets/spaceBuilding_009.png"
, e = "assets/spaceBuilding_010.png", f = "assets/spaceBuilding_011.png"}


--[[61]]nonCollidableBackgroundWall = "assets/spaceBuilding_001.png" --filled out for you
--[[62 we want slightly darker bg walls]]nonCollidableBackgroundWallTint = {r = 100, g = 100, b = 100} 

--[[63]]foregroundWindowImage = "assets/spaceStation_003.png" --filled out for you
--[[64]]WindowTint = {r = 255, g = 255, b = 255, a = 128} 
--[[65 what level are we starting on]]startingLevel = 1

--Audio class constants x 5 *************************************************************************************************
--[[66]]maximumSFX = 4 --filled out for you
--[[67]]maxMusicTracks = 3 --filled out for you
--[[68]]musicGain = 0.25 --filled out for you. a value between 0 and 1

--The table keys should use letters NOT numbers (i.e. a = "assets/anasset.png")
--[[69]]musicTracks = {a = "assets/Sad Descent.ogg", b = "assets/Space Cadet.ogg", c = "assets/Time Driving.ogg"} --filled out for you
--[[70]]SFX = {a = "assets/error3.ogg", b = "assets/explosionCrunch_003.ogg", 
c = "assets/laserLarge_001.ogg", d = "assets/laserRetro_001.ogg"} --filled out for you

--Overall total constants = 70


--[[**************************************PART 2 OF ASSIGNMENT TASK***********************************************************
This is where you must make lua functions that can be used in the C++ code.

The comments below specify where the possible function locations are.

Look in the class files to see where the possible lua function call is,
comment out the old code block and uncomment the new block containing the lua function call to activate it.

If the game doesn't work for any reason I will copy the function attempts into my own version of this game
to test it/they work(s) correctly and will give the points for it/them ONLY if it/they work(s) in my version

The total number of functions to make in this part of the assignment is 4 worth 4 points each
The total points value for PART 1 and PART 2 combined is 75%

The only changes you will need to make to the C++ code is the commenting/uncommenting of the specified part 2 code blocks.

Making changes to the code which break the game will result in the marks cap specified for PART 1 (PART 1 capped at 40%)
*******************************************************************************************************************************]]
--****************************************************
--I have done the first function for you as an example 
--which is called in PlayGame.cpp line 268 onwards
--and LuaHelper.cpp line 92 - 105
function randomNumber()
	return math.random(1,10000)
end
--****************************************************


--function 1 located in creature.cpp and LuaHelper.cpp
function CallLuaMoveUp(speedVal, PositionY)
	if (PositionY > 0) then
		PositionY = PositionY - speedVal
	end
	return PositionY
end
--function 2 located in creature.cpp and LuaHelper.cpp
 function CallLuaMoveDown(height, speedVal, PositionY)
 	if (PositionY + height + speedVal <= 900) then
		PositionY = PositionY + speedVal
	end
	return PositionY
end
--function 3 located in PlayerCharacter.cpp
function CallLuaIncreaseScoreAndLives(m_Lives, m_Score)

--function 4 located in PlayerCharacter.cpp
function CallLuaResetScore(m_LastScore, m_Score)




--[[*****************************************PART 3 OF ASSIGNMENT TASK************************************************************
This is where you must call C++ functions using your lua script

This and PART 4 are the most challenging parts of the assignment

There are 2 functions worth 5 points each
The total points value for PARTS 1, 2 and 3 is 85%

Look in the class files to see where the possible lua function call is,
comment out the old code block and uncomment the new block containing the lua function call to activate it.
ALL the C++ functions called below should be made private in the 
header file where they are created.
make sure to thoroughly check the relevant .cpp and .h file for any changes you need to make (making functions private and commenting/uncommenting code)
missing some of this won't necessarily break the code, but will result in a marks penalty and a mention of what you missed in the feedback

Making changes to the code which break the game will result in the marks cap specified for PART 1 (PART 1 capped at 40%)
If your attempt breaks the game, comment out the attempt and get the game back to a working state 
to avoid the capping penalty. I'll take a look and give an appropriate mark based on how far off I thought you were
********************************************************************************************************************************]]

--**********************************************************************************************
--I have done the first function for you as an example which is called in PlayGame.cpp line 84**
--**********************************************************************************************
--C++ function call in Game.cpp line 82
function RunGame()
	CDispatcher("Run")--Run is the name of the function in C++
end
--**********************************************************************************************

--C++ function call 1 in Level.cpp line 181 and PlayGame.cpp line 262

--C++ function call 2 in PlayGame.cpp line 261. check the .h file for the function name


--[[*****************************************PART 4 OF ASSIGNMENT TASK***********************************************************
Create up to 2 more C++ function calls yourself.

There's no help for this one, it's up to you to do, INCLUDING the C++ code.

Each C++ function you adapt into this script will get you 12.5%

If your attempt breaks the game, comment out the attempt and get the game back to a working state 
to avoid the capping penalty. I'll take a look and give an appropriate mark based on how far off I thought you were
**********************************************************************************************************************************]]
