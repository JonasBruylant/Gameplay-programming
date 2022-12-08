/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
/*=============================================================================*/
// StatesAndTransitions.h: Implementation of the state/transition classes
/*=============================================================================*/
#ifndef ELITE_APPLICATION_FSM_STATES_TRANSITIONS
#define ELITE_APPLICATION_FSM_STATES_TRANSITIONS

#include "projects/Shared/Agario/AgarioAgent.h"
#include "projects/Shared/Agario/AgarioFood.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
#include "framework/EliteAI/EliteData/EBlackboard.h"

//------------
//---STATES---
//------------
namespace FSMStates
{
	class WanderState : public Elite::FSMState
	{
	public:
		WanderState() : FSMState() {};
		virtual void OnEnter(Elite::Blackboard* pBlackBoard) override;

	};

	class SeekFoodstate : public Elite::FSMState
	{
	public:
		SeekFoodstate() : FSMState() {};
		virtual void OnEnter(Elite::Blackboard* pBlackBoard) override;

	};

	class PersueState : public Elite::FSMState
	{
	public:
		PersueState() : FSMState() {};
		virtual void OnEnter(Elite::Blackboard* pBlackBoard) override;

	};

	class FleeState : public Elite::FSMState
	{
	public:
		FleeState() : FSMState() {};
		virtual void OnEnter(Elite::Blackboard* pBlackBoard) override;

	};
}

//-----------------
//---TRANSITIONS---
//-----------------
namespace FSMConditions
{
	class FoodNearbyCondition : public Elite::FSMCondition
	{
	public:
		FoodNearbyCondition() : FSMCondition() {};
		virtual bool Evaluate(Elite::Blackboard* pBlackboard) const override;
	};

	class AreaClear : public Elite::FSMCondition
	{
	public:
		AreaClear() : FSMCondition() {};
		virtual bool Evaluate(Elite::Blackboard* pBlackboard) const override;
	};

	class SmallerEnemyNearby : public Elite::FSMCondition
	{
	public: 
		SmallerEnemyNearby() : FSMCondition() {  };
		virtual bool Evaluate(Elite::Blackboard* pBlackboard) const override;

	private:
		float m_ChaseDistance{15.f};
	};
}



#endif