#include "stdafx.h"
#include "StatesAndTransitions.h"

using namespace Elite;
using namespace FSMStates;

void WanderState::OnEnter(Blackboard* pBlackBoard)
{
	AgarioAgent* pAgent;
	bool isValid = pBlackBoard->GetData("Agent", pAgent);

	if (!isValid || !pAgent) //If it's not valid or agent is nullptr, return
		return;

	pAgent->SetToWander();
}

void FSMStates::SeekFoodstate::OnEnter(Elite::Blackboard* pBlackBoard)
{
	AgarioAgent* pAgent;
	bool isValid = pBlackBoard->GetData("Agent", pAgent);

	if (!isValid || !pAgent) //If it's not valid or agent is nullptr, return
		return;
	
	AgarioFood* nearestFood;
	if (!pBlackBoard->GetData("NearestFood", nearestFood) || !nearestFood)
		return;

	pAgent->SetToSeek(nearestFood->GetPosition());
}


void FSMStates::PersueState::OnEnter(Elite::Blackboard* pBlackBoard)
{
	AgarioAgent* pAgent;
	std::vector<AgarioAgent*>* pEnemyAgents;
	

	if (!pBlackBoard->GetData("Agent", pAgent) || !pBlackBoard->GetData("Enemies", pEnemyAgents)) //If it's not valid or agent is nullptr, return
		return;

	//pAgent->SetToSeek(pEnemyAgents);
}

void FSMStates::FleeState::OnEnter(Elite::Blackboard* pBlackBoard)
{
	AgarioAgent* pAgent;
	bool isValid = pBlackBoard->GetData("Agent", pAgent);

	if (!isValid || !pAgent) //If it's not valid or agent is nullptr, return
		return;


}


bool FSMConditions::FoodNearbyCondition::Evaluate(Blackboard* pBlackBoard) const
{
	AgarioAgent* pAgent;
	bool isValid = pBlackBoard->GetData("Agent", pAgent);

	if (!isValid || !pAgent) //If it's not valid or agent is nullptr, return
		return false;
	

	std::vector<AgarioFood*>* pFoodVec;
	bool foodNearby = pBlackBoard->GetData("FoodVec", pFoodVec);

	if (!foodNearby || !pFoodVec)
		return false;
	
	const float radius{10.f};
	Vector2 agentPos = pAgent->GetPosition();

	DEBUGRENDERER2D->DrawCircle(agentPos, radius, Elite::Color{ 0.f, 0.5f, 0.5f }, DEBUGRENDERER2D->NextDepthSlice());

	auto isCloser = [agentPos](AgarioFood* pFood1, AgarioFood* pFood2)
	{
		float dist1 = pFood1->GetPosition().DistanceSquared(agentPos);

		float dist2 = pFood2->GetPosition().DistanceSquared(agentPos);

		return dist1 < dist2;
	};

	auto closestElement_It = std::min_element(pFoodVec->begin(), pFoodVec->end(), isCloser);

	if (closestElement_It == pFoodVec->end())
		return false;

	AgarioFood* closestFood = *closestElement_It;

	if (closestFood->GetPosition().Distance(agentPos) < (radius * radius))
	{
		pBlackBoard->ChangeData("NearestFood", closestFood);
		return true;
	}

	return false;
}

bool FSMConditions::AreaClear::Evaluate(Elite::Blackboard* pBlackBoard) const
{
	AgarioAgent* pAgent;
	bool isValid = pBlackBoard->GetData("Agent", pAgent);

	if (!isValid || !pAgent) //If it's not valid or agent is nullptr, return
		return false;

	std::vector<AgarioFood*>* pFoodVec;
	bool foodNearby = pBlackBoard->GetData("FoodVec", pFoodVec);
	float radius{ 10.f };
	for (auto& currentFood : *pFoodVec)
	{
		if (currentFood->GetPosition().DistanceSquared(pAgent->GetPosition()) < (radius * radius))
			return false;
	}

	return true;
}

bool FSMConditions::SmallerEnemyNearby::Evaluate(Elite::Blackboard* pBlackboard) const
{
	AgarioAgent* pAgent;
	bool isAgentValid = pBlackboard->GetData("Agent", pAgent);

	std::vector<AgarioAgent*>* pEnemyAgents;
	bool isenemyValid = pBlackboard->GetData("Enemies", pEnemyAgents);

	Vector2 agentPos = pAgent->GetPosition();
	auto isCloser = [agentPos](AgarioAgent* pAgent1, AgarioAgent* pAgent2)
	{
		float dist1 = pAgent1->GetPosition().DistanceSquared(agentPos);

		float dist2 = pAgent2->GetPosition().DistanceSquared(agentPos);

		return dist1 > dist2;
	};

	auto closestElement_It = std::min_element(pEnemyAgents->begin(), pEnemyAgents->end(), isCloser);
	for (auto& currentEnemy : *pEnemyAgents)
	{
		float distance = pAgent->GetPosition().DistanceSquared(currentEnemy->GetPosition());

		if (pAgent->GetRadius() > currentEnemy->GetRadius())
			if (m_ChaseDistance > distance)
				return true;
	}

	if (closestElement_It == pEnemyAgents->end())
		return false;

	return false;
}
