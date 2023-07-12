//===========================================================================//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

/*--------------------------------------------------------------------------------**
**																				  **
**																				  **
** Order functions                                                                **
**																				  **
**																				  **
**--------------------------------------------------------------------------------*/

function Attack(integer TargetID, real AttackRadius) : integer;
function Withdraw : integer;
function Patrol(path thisPath, integer Steps, integer PatrolType, integer Cycles) : integer;
function MovePath(path thisPath, integer Steps) : integer;
function Sentry(real X, real Y) : integer;
function Guard(integer GuardObjectID) : integer;
function Escort(integer GuardObjectID) : integer;
function MoveTo(real X, real Y) : integer;

/*--------------------------------------------------------------------------------**
**																				  **
**																				  **
** MISC HIGH-LEVEL functions                                                      **
**																				  **
**																				  **
**--------------------------------------------------------------------------------*/

function ResetPath;
function ClearHits : integer;
function GetHits : integer;
function BeenHit : boolean;
function CurrentX : real;
function CurrentY : real;
function CurrentPosition : position;
function MyX : real;
function MyY : real;
function MyPosition : position;
function MyTarget : integer;

/*--------------------------------------------------------------------------------**
**																				  **
**																				  **
** Behavior Modifier functions                                                    **
**																				  **
**																				  **
**--------------------------------------------------------------------------------*/

function SetMinimumActionRating(real Rating);
function StartFighting;
function StopFighting;
function AttackInActiveOFF;
function AttackInActiveON;
function IgnoreNonTargetsOFF;
function IgnoreNonTargetsON;
function StickToTargetOFF;
function StickToTargetON;
function SetGuardMod(real Modifier);
function SetConcentrateFireMod(real Modifier);
function SetCurrentTargetMod(real Modifier);
function SetFightPrecedence(integer Precedence);
function MoveToEngageOFF;
function MoveToEngageON;
function ChallengeOFF;
function ChallengeON;
function IdentifySensorContactsOFF;
function IdentifySensorContactsON;
function FreeEngagePoint;
function FixEngagePoint(real X, real Y);
function SetDisengageRadius(real Radius);
function SetEngageRadius(real Radius);
function SetMinimumDistance(real Distance);
function SetSpeed(boolean Speed);
function setTarget(integer TargetID, real RatingMod);

/*--------------------------------------------------------------------------------**
**																				  **
**																				  **
** Core functions                                                                 **
**																				  **
**																				  **
**--------------------------------------------------------------------------------*/

function Move(position MovePoint) : integer;
function Fight : boolean;
function selectTarget (IntList enemyList, integer numEnemies, boolean LimitToVisual) : integer;
function contactwithinzone (integer contactHandle) : boolean;

/*--------------------------------------------------------------------------------**
**																				  **
**																				  **
** Heuristic functions                                                            **
**																				  **
**																				  **
**--------------------------------------------------------------------------------*/

function contactActionRating (integer contactHandle) : real;

/*--------------------------------------------------------------------------------**
**																				  **
**																				  **
** Event Handler Functions                                                        **
**																				  **
**																				  **
**--------------------------------------------------------------------------------*/

function handleNoMovePath;
function handleTargetOfWeaponFire;

/*--------------------------------------------------------------------------------**
**																				  **
**																				  **
** Support functions                                                              **
**																				  **
**																				  **
**--------------------------------------------------------------------------------*/

function NewTarget(integer TargetID, real TargetRating) : boolean;
function getTargetModifier(integer contactId,@boolean OnTargetList) : real;
function breakChallenge (integer targetId);
function challenge (integer targetId);

/*-------------------------------------------------------------------------------**
**																				 **
**																				 **
** The following functions are used to transfer data between warrior and library **
**																				 **
**																				 **
**-------------------------------------------------------------------------------*/

function defaultBrain;
function initOrders;
function BuildTargetList;