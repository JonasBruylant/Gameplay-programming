#include "stdafx.h"
#include "ENavGraph.h"
#include "framework\EliteAI\EliteGraphs\EliteGraphAlgorithms\EAStar.h"

using namespace Elite;

Elite::NavGraph::NavGraph(const Polygon& contourMesh, float playerRadius = 1.0f) :
	Graph2D(false),
	m_pNavMeshPolygon(nullptr)
{
	//Create the navigation mesh (polygon of navigatable area= Contour - Static Shapes)
	m_pNavMeshPolygon = new Polygon(contourMesh); // Create copy on heap

	//Get all shapes from all static rigidbodies with NavigationCollider flag
	auto vShapes = PHYSICSWORLD->GetAllStaticShapesInWorld(PhysicsFlags::NavigationCollider);

	//Store all children
	for (auto shape : vShapes)
	{
		shape.ExpandShape(playerRadius);
		m_pNavMeshPolygon->AddChild(shape);
	}

	//Triangulate
	m_pNavMeshPolygon->Triangulate();

	//Create the actual graph (nodes & connections) from the navigation mesh
	CreateNavigationGraph();
}

Elite::NavGraph::~NavGraph()
{
	delete m_pNavMeshPolygon; 
	m_pNavMeshPolygon = nullptr;
}

int Elite::NavGraph::GetNodeIdxFromLineIdx(int lineIdx) const
{
	auto nodeIt = std::find_if(m_Nodes.begin(), m_Nodes.end(), [lineIdx](const NavGraphNode* n) { return n->GetLineIndex() == lineIdx; });
	if (nodeIt != m_Nodes.end())
	{
		return (*nodeIt)->GetIndex();
	}

	return invalid_node_index;
}

Elite::Polygon* Elite::NavGraph::GetNavMeshPolygon() const
{
	return m_pNavMeshPolygon;
}

void Elite::NavGraph::CreateNavigationGraph()
{
	//1. Go over all the edges of the navigationmesh and create nodes
	auto lines = m_pNavMeshPolygon->GetLines();

	for (auto& currentLine : lines)
	{
		//Gets the 3 lines that form the triangle, returns vector of Triangle*
		auto triangle = m_pNavMeshPolygon->GetTrianglesFromLineIndex(currentLine->index);

		if (triangle.size() > 1) //Check if the amount of triangles connected to the line is more than one
		{
			Vector2 center{ (currentLine->p1.x + currentLine->p2.x) * 0.5f, (currentLine->p1.y + currentLine->p2.y) * 0.5f };
			NavGraphNode* node{ new NavGraphNode(GetNextFreeNodeIndex(), currentLine->index ,center) };
			AddNode(node);
		}
	}


	for (auto& triangle : m_pNavMeshPolygon->GetTriangles())
	{
		std::vector<int> validNodes;
		for (auto& lineIdx : triangle->metaData.IndexLines)
		{
			if (GetNodeIdxFromLineIdx(lineIdx) != invalid_node_index)
			{
				validNodes.push_back(GetNodeIdxFromLineIdx(lineIdx)); //Pushback the number of the valid node connected to the triangle
				//2. Create connections now that every node is created
				//std::cout << GetNodeIdxFromLineIdx(lineIdx) << '\n';
			}

		}

		if (validNodes.size() == 2)
		{
			GraphConnection2D* pConnection = new GraphConnection2D(validNodes[0], validNodes[1]);
			AddConnection(pConnection);
		}
		if (validNodes.size() == 3)
		{
			GraphConnection2D* pConnection1 = new GraphConnection2D(validNodes[0], validNodes[1]);
			GraphConnection2D* pConnection2 = new GraphConnection2D(validNodes[1], validNodes[2]);
			GraphConnection2D* pConnection3 = new GraphConnection2D(validNodes[2], validNodes[0]);
			AddConnection(pConnection1);
			AddConnection(pConnection2);
			AddConnection(pConnection3);
		}

	}
		//3. Set the connections cost to the actual distance
		SetConnectionCostsToDistance(); 

	
}

