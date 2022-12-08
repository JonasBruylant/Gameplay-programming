/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
/*=============================================================================*/
// Behaviors.h: Implementation of certain reusable behaviors for the BT version of the Agario Game
/*=============================================================================*/
#ifndef ELITE_APPLICATION_BEHAVIOR_TREE_BEHAVIORS
#define ELITE_APPLICATION_BEHAVIOR_TREE_BEHAVIORS
//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "framework/EliteMath/EMath.h"
#include "framework/EliteAI/EliteDecisionMaking/EliteBehaviorTree/EBehaviorTree.h"
#include "projects/Shared/Agario/AgarioAgent.h"
#include "projects/Shared/Agario/AgarioFood.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"

//-----------------------------------------------------------------
// Behaviors
//-----------------------------------------------------------------

namespace BT_Actions
{
	Elite::BehaviorState ChangeToWander(Elite::Blackboard* pBlackBoard)
	{
		AgarioAgent* pAgent;

		if (!pBlackBoard->GetData("Agent", pAgent) || !pAgent)
			return Elite::BehaviorState::Failure;

		pAgent->SetToWander();
		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState ChangeToSeekFood(Elite::Blackboard* pBlackBoard)
	{
		AgarioAgent* pAgent;
		Elite::Vector2 targetPos;

		if (!pBlackBoard->GetData("Agent", pAgent) || !pAgent)
			return Elite::BehaviorState::Failure;

		if (!pBlackBoard->GetData("Target", targetPos))
			return Elite::BehaviorState::Failure;

		pAgent->SetToSeek(targetPos);
		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState ChangeToSeekEnemy(Elite::Blackboard* pBlackBoard)
	{
		AgarioAgent* pAgent;
		Elite::Vector2 targetPos;

		if (!pBlackBoard->GetData("Agent", pAgent) || !pAgent)
			return Elite::BehaviorState::Failure;

		if (!pBlackBoard->GetData("Target", targetPos))
			return Elite::BehaviorState::Failure;

		pAgent->SetToSeek(targetPos);
		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState ChangeToFleeEnemy(Elite::Blackboard* pBlackBoard)
	{
		AgarioAgent* pAgent;
		Elite::Vector2 targetPos;

		if (!pBlackBoard->GetData("Agent", pAgent) || !pAgent)
			return Elite::BehaviorState::Failure;

		if (!pBlackBoard->GetData("Target", targetPos))
			return Elite::BehaviorState::Failure;

		pAgent->SetToFlee(targetPos);
		return Elite::BehaviorState::Success;
	}
}


//-----------------------------------------------------------------
// Conditions
//-----------------------------------------------------------------
namespace BT_Conditions
{
	bool isFoodNearby(Elite::Blackboard* pBlackBoard)
	{
		AgarioAgent* pAgent;
		if (!pBlackBoard->GetData("Agent", pAgent) || !pAgent)
			return false;


		std::vector<AgarioFood*>* pFoodVec;
		if (!pBlackBoard->GetData("FoodVec", pFoodVec) || !pFoodVec)
			return false;

		if (pFoodVec->empty())
			return false;

		AgarioFood* pClosestFood{ nullptr };
		const float searchRadius{pAgent->GetRadius() + 5.f};
		DEBUGRENDERER2D->DrawCircle(pAgent->GetPosition(), searchRadius, {1.0f, 0.f, 0.f}, 0.f);
		float closestDistSq{searchRadius * searchRadius };
		Elite::Vector2 agentPos{pAgent->GetPosition()};

		for (auto& pFood : *pFoodVec)
		{
			float distSq = pFood->GetPosition().DistanceSquared(agentPos);

			if (distSq < closestDistSq)
			{
				 closestDistSq = distSq;
				 pClosestFood = pFood;
			}
		}

		if (!pClosestFood) 
			return false;

		pBlackBoard->ChangeData("Target", pClosestFood->GetPosition());
		return true;
	}

	bool IsSmallerEnemyNearby(Elite::Blackboard* pBlackBoard)
	{
		AgarioAgent* pAgent;
		if (!pBlackBoard->GetData("Agent", pAgent) || !pAgent)
			return false;


		std::vector<AgarioAgent*>* m_pEnemies;
		if (!pBlackBoard->GetData("Enemies", m_pEnemies) || !m_pEnemies)
			return false;

		if (m_pEnemies->empty())
			return false;

		AgarioAgent* pClosestEnemy{ nullptr };
		const float searchRadius{ pAgent->GetRadius() + 20.f };
		DEBUGRENDERER2D->DrawCircle(pAgent->GetPosition(), searchRadius, { 0.f, 1.f, 0.f }, 0.f);
		float closestDistSq{ searchRadius * searchRadius };
		Elite::Vector2 agentPos{ pAgent->GetPosition() };

		for (auto& pEnemy : *m_pEnemies)
		{
			float distSq = pEnemy->GetPosition().DistanceSquared(agentPos);

			if (distSq < closestDistSq)
			{
				if ((pEnemy->GetRadius() - 1.f) < pAgent->GetRadius())
				{
					closestDistSq = distSq;
					pClosestEnemy = pEnemy;
					//std::cout << "Smaller enemy nearby \n";
				}
			}
		}

		if (!pClosestEnemy)
			return false;

		pBlackBoard->ChangeData("Target", pClosestEnemy->GetPosition());
		return true;
	}

	bool IsBiggerEnemyNearby(Elite::Blackboard* pBlackBoard)
	{
		AgarioAgent* pAgent;
		if (!pBlackBoard->GetData("Agent", pAgent) || !pAgent)
			return false;


		std::vector<AgarioAgent*>* m_pEnemies;
		if (!pBlackBoard->GetData("Enemies", m_pEnemies) || !m_pEnemies)
			return false;

		if (m_pEnemies->empty())
			return false;

		AgarioAgent* pClosestEnemy{ nullptr };
		const float searchRadius{ pAgent->GetRadius() + 20.f };
		//DEBUGRENDERER2D->DrawCircle(pAgent->GetPosition(), searchRadius, { 0.f, 1.f, 0.f }, 0.f);
		float closestDistSq{ searchRadius * searchRadius };
		Elite::Vector2 agentPos{ pAgent->GetPosition() };

		for (auto& pEnemy : *m_pEnemies)
		{
			float distSq = pEnemy->GetPosition().DistanceSquared(agentPos);

			if (distSq < closestDistSq)
			{
				if (pEnemy->GetRadius() > pAgent->GetRadius())
				{
					closestDistSq = distSq;
					pClosestEnemy = pEnemy;
					//std::cout << "Smaller enemy nearby \n";
				}
			}
		}

		if (!pClosestEnemy)
			return false;

		pBlackBoard->ChangeData("Target", pClosestEnemy->GetPosition());
		return true;
	}
}













#endif