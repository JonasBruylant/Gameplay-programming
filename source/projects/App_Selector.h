#pragma once
// Here we determine which application is currently active
// Create the define here using the "ActiveApp_..." convention and set up the correct include and typedef in the #ifdef below.

//#define ActiveApp_Sandbox
//#define ActiveApp_SteeringBehavior
//#define ActiveApp_CombinedSteering
//#define ActiveApp_FlockingBehavior
//#define ActiveApp_GraphTheory
//#define ActiveApp_Astar
#define ActiveApp_NavMesh

//---------- Registered Applications -----------
#ifdef ActiveApp_Sandbox
#include "projects/Movement/Sandbox/App_Sandbox.h"
typedef App_Sandbox CurrentApp;
#endif

#ifdef ActiveApp_SteeringBehavior
#include "projects/Movement/SteeringBehaviors/Steering/App_SteeringBehaviors.h"
typedef App_SteeringBehaviors CurrentApp;
#endif

#ifdef ActiveApp_CombinedSteering
#include "projects/Movement//SteeringBehaviors//CombinedSteering/App_CombinedSteering.h"
typedef App_CombinedSteering CurrentApp;
#endif

#ifdef ActiveApp_FlockingBehavior
#include "projects/Movement/SteeringBehaviors/Flocking/App_Flocking.h"
typedef App_Flocking CurrentApp;
#endif

#ifdef ActiveApp_GraphTheory
#include "projects/Movement/Pathfinding/GraphTheory/App_GraphTheory.h"
typedef App_GraphTheory CurrentApp;
#endif

#ifdef ActiveApp_Astar
#include "projects/Movement/Pathfinding/AStar/App_PathfindingAStar/App_PathfindingAStar.h"
typedef App_PathfindingAStar CurrentApp;
#endif

#ifdef ActiveApp_NavMesh
#include "projects/Movement/Pathfinding/NavMeshGraph/App_NavMeshGraph.h"
typedef App_NavMeshGraph CurrentApp;
#endif

class App_Selector {
public: 
	static IApp* CreateApp() {
		IApp* myApp = new CurrentApp();
		return myApp;
	}
};