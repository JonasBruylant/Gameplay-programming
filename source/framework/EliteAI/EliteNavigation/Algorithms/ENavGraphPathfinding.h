#pragma once
#include <vector>
#include <iostream>
#include "framework/EliteMath/EMath.h"
#include "framework\EliteAI\EliteGraphs\ENavGraph.h"
#include "framework\EliteAI\EliteGraphs\EliteGraphAlgorithms\EAStar.h"

namespace Elite
{
	class NavMeshPathfinding
	{
	public:
		static std::vector<Vector2> FindPath(Vector2 startPos, Vector2 endPos, NavGraph* pNavGraph, std::vector<Vector2>& debugNodePositions, std::vector<Portal>& debugPortals)
		{
			debugNodePositions = std::vector<Vector2>{};
			//Create the path to return
			std::vector<Vector2> finalPath{};
			 
			//Get the start and endTriangle
			auto startTriangle = pNavGraph->GetNavMeshPolygon()->GetTriangleFromPosition(startPos);
			auto endTriangle = pNavGraph->GetNavMeshPolygon()->GetTriangleFromPosition(endPos);

			//We have valid start/end triangles and they are not the same
			if (startTriangle == nullptr || endTriangle == nullptr)
				return finalPath;
			if (startTriangle == endTriangle)
			{
				finalPath.emplace_back(endPos); 
				return finalPath;
			}
			//=> Start looking for a path
			//Copy the graph
			auto graphCopy = std::static_pointer_cast<NavGraph>(pNavGraph->Clone());
		

			//Create extra node for the Start Node (Agent's position)
			NavGraphNode* startNode = new NavGraphNode(graphCopy->GetNextFreeNodeIndex(), -1, startPos);
			graphCopy->AddNode(startNode);
			for (auto& currentEdge : startTriangle->metaData.IndexLines)
			{
				int nodeIdx = graphCopy->GetNodeIdxFromLineIdx(currentEdge);
				if (nodeIdx != invalid_node_index)
				{
					GraphConnection2D* pConnection = new GraphConnection2D(startNode->GetIndex(), 
					nodeIdx, 
					Distance(startNode->GetPosition(), graphCopy->GetNode(nodeIdx)->GetPosition()));
					graphCopy->AddConnection(pConnection);
				}
			}

			//Create extra node for the endNode
			NavGraphNode* endNode = new NavGraphNode(graphCopy->GetNextFreeNodeIndex(), -1,endPos);
			graphCopy->AddNode(endNode);

			for (auto& currentEdge : endTriangle->metaData.IndexLines)
			{
				int nodeIdx = graphCopy->GetNodeIdxFromLineIdx(currentEdge);
				if (nodeIdx != invalid_node_index)
				{
					GraphConnection2D* pConnection = new GraphConnection2D(endNode->GetIndex(),
						nodeIdx,
						Distance(endNode->GetPosition(), graphCopy->GetNode(nodeIdx)->GetPosition()));
					graphCopy->AddConnection(pConnection);
				}
			}
			//Run A star on new graph

			//AStar(IGraph<T_NodeType, T_ConnectionType>* pGraph, Heuristic hFunction);
			auto pathfinder = AStar<NavGraphNode, GraphConnection2D>(graphCopy.get(), Elite::HeuristicFunctions::Chebyshev);
			
			auto nodePath = pathfinder.FindPath(startNode, endNode);
			//OPTIONAL BUT ADVICED: Debug Visualisation
			for (auto& node : nodePath)
			{

				debugNodePositions.push_back(node->GetPosition());
			}
			//Run optimiser on new graph, MAKE SURE the A star path is working properly before starting this section and uncommenting this!!!
			debugPortals = SSFA::FindPortals(nodePath, pNavGraph->GetNavMeshPolygon());
			finalPath = SSFA::OptimizePortals(debugPortals);

			return finalPath;
		}
	};

}
