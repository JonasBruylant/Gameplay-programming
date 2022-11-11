#include "stdafx.h"
#include "Flock.h"

#include "../SteeringAgent.h"
#include "../Steering/SteeringBehaviors.h"
#include "../CombinedSteering/CombinedSteeringBehaviors.h"

using namespace Elite;

//Constructor & Destructor
Flock::Flock(
	int flockSize /*= 50*/, 
	float worldSize /*= 100.f*/, 
	SteeringAgent* pAgentToEvade /*= nullptr*/, 
	bool trimWorld /*= false*/)

	: m_WorldSize{ worldSize }
	, m_FlockSize{ flockSize }
	, m_TrimWorld { trimWorld }
	, m_pAgentToEvade{pAgentToEvade}
	, m_NeighborhoodRadius{ 15 }
	, m_NrOfNeighbors{0}
{
	m_Agents.resize(m_FlockSize);

	// TODO: initialize the flock and the memory pool

	for (size_t i = 0; i < m_Agents.size(); ++i)
	{
		m_Agents[i] = new SteeringAgent();
	}
}

Flock::~Flock()
{
	// TODO: clean up any additional data

	SAFE_DELETE(m_pBlendedSteering);
	SAFE_DELETE(m_pPrioritySteering);

	for(auto pAgent: m_Agents)
	{
		SAFE_DELETE(pAgent);
	}
	m_Agents.clear();
}

void Flock::Update(float deltaT)
{
	// TODO: update the flock
	m_Neighbors.resize(m_FlockSize - 1);

	for (auto agents : m_Agents)
	{
		RegisterNeighbors(agents);
		agents->Update(deltaT);
		m_NrOfNeighbors = 0;
	}

	m_Neighbors.clear();
	// loop over all the agents
		// register its neighbors	(-> memory pool is filled with neighbors of the currently evaluated agent)
		// update it				(-> the behaviors can use the neighbors stored in the pool, next iteration they will be the next agent's neighbors)
		// trim it to the world
}

void Flock::Render(float deltaT)
{
	// TODO: render the flock
}

void Flock::UpdateAndRenderUI()
{
	//Setup
	int menuWidth = 235;
	int const width = DEBUGRENDERER2D->GetActiveCamera()->GetWidth();
	int const height = DEBUGRENDERER2D->GetActiveCamera()->GetHeight();
	bool windowActive = true;
	ImGui::SetNextWindowPos(ImVec2((float)width - menuWidth - 10, 10));
	ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)height - 20));
	ImGui::Begin("Gameplay Programming", &windowActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::PushAllowKeyboardFocus(false);

	//Elements
	ImGui::Text("CONTROLS");
	ImGui::Indent();
	ImGui::Text("LMB: place target");
	ImGui::Text("RMB: move cam.");
	ImGui::Text("Scrollwheel: zoom cam.");
	ImGui::Unindent();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("STATS");
	ImGui::Indent();
	ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	ImGui::Unindent();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Text("Flocking");
	ImGui::Spacing();

	// TODO: Implement checkboxes for debug rendering and weight sliders here

	//End
	ImGui::PopAllowKeyboardFocus();
	ImGui::End();
	
}

void Flock::RegisterNeighbors(SteeringAgent* pAgent)
{
	// TODO: Implement
	for (auto agent : m_Agents)
	{
		if (agent != pAgent)
		{		
			float dis = (agent->GetPosition() - pAgent->GetPosition()).Magnitude();
			if (dis <= m_NeighborhoodRadius)
			{
				m_Neighbors[m_NrOfNeighbors] = agent;
				++m_NrOfNeighbors;
			}
		}
	}
}

Elite::Vector2 Flock::GetAverageNeighborPos() const
{
	// TODO: Implement
	Vector2 averageNeighbourPosition{ };
	Vector2 lastPosition{};

	for (size_t i = 0; i < m_FlockSize; ++i)
	{
		if (i == 0)
			averageNeighbourPosition = m_Agents[i]->GetPosition();
		else
		{
			lastPosition = averageNeighbourPosition;
			averageNeighbourPosition -= m_Neighbors[i]->GetPosition();
			averageNeighbourPosition += lastPosition;
		}
	}
	return averageNeighbourPosition;
}

Elite::Vector2 Flock::GetAverageNeighborVelocity() const
{
	// TODO: Implement
	Vector2 averageNeighbourVelocity{  };
	for (size_t i = 0; i < m_FlockSize; ++i)
	{
		if (i == 0)
			averageNeighbourVelocity = m_Agents[i]->GetLinearVelocity();
		else
		{
			averageNeighbourVelocity += m_Neighbors[i]->GetLinearVelocity();
		}
	}

	averageNeighbourVelocity /= m_NrOfNeighbors;

	return averageNeighbourVelocity;
}

//WIP
Elite::Vector2 Flock::SeperateFromNeighbours()
{
	Vector2 averageNeighborPos{GetAverageNeighborPos()};
	Vector2 currentDirection{};

	for (auto agent : m_Agents)
	{
		currentDirection =	agent->GetPosition() - averageNeighborPos;
		currentDirection = -currentDirection;
	}
	return currentDirection;
}

void Flock::SetTarget_Seek(TargetData target)
{
	// TODO: Set target for seek behavior
}


float* Flock::GetWeight(ISteeringBehavior* pBehavior) 
{
	if (m_pBlendedSteering)
	{
		auto& weightedBehaviors = m_pBlendedSteering->GetWeightedBehaviorsRef();
		auto it = find_if(weightedBehaviors.begin(),
			weightedBehaviors.end(),
			[pBehavior](BlendedSteering::WeightedBehavior el)
			{
				return el.pBehavior == pBehavior;
			}
		);

		if(it!= weightedBehaviors.end())
			return &it->weight;
	}

	return nullptr;
}
