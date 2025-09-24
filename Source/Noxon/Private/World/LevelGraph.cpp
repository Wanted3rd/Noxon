#include "World/LevelGraph.h"

bool ULevelGraph::FindNodeByID(int32 nodeID, FLevelNode& outNode)
{
	FLevelNode* foundNode = levelNodes.FindByPredicate([nodeID](const FLevelNode& node)
	{
		return node.nodeID == nodeID;
	});

	if (foundNode)
	{
		outNode = *foundNode;
		return true;
	}
	return false;
}

bool ULevelGraph::FindNodeByName(const FName& levelName, FLevelNode& outNode)
{
	FLevelNode* foundNode = levelNodes.FindByPredicate([levelName](const FLevelNode& node)
	{
		return node.levelName == levelName;
	});

	if (foundNode)
	{
		outNode = *foundNode;
		return true;
	}
	return false;
}

bool ULevelGraph::FindNodeByID(int32 nodeID, FLevelNode& outNode) const
{
	const FLevelNode* foundNode = levelNodes.FindByPredicate([nodeID](const FLevelNode& node)
	{
		return node.nodeID == nodeID;
	});

	if (foundNode)
	{
		outNode = *foundNode;
		return true;
	}
	return false;
}

bool ULevelGraph::FindNodeByName(const FName& levelName, FLevelNode& outNode) const
{
	const FLevelNode* foundNode = levelNodes.FindByPredicate([levelName](const FLevelNode& node)
	{
		return node.levelName == levelName;
	});

	if (foundNode)
	{
		outNode = *foundNode;
		return true;
	}
	return false;
}

TArray<int32> ULevelGraph::GetConnectedNodes(int32 nodeID) const
{
	TArray<int32> connectedNodes;

	for (const FLevelEdge& edge : levelEdges)
	{
		if (edge.fromNodeID == nodeID)
		{
			connectedNodes.AddUnique(edge.toNodeID);
		}
		else if (edge.toNodeID == nodeID)
		{
			connectedNodes.AddUnique(edge.fromNodeID);
		}
	}

	return connectedNodes;
}

TArray<int32> ULevelGraph::GetAccessibleNodes(int32 fromNodeID) const
{
	TArray<int32> accessibleNodes;

	for (const FLevelEdge& edge : levelEdges)
	{
		if (edge.fromNodeID == fromNodeID)
		{
			if (!edge.bRequiresClearance)
			{
				accessibleNodes.AddUnique(edge.toNodeID);
			}
			else
			{
				FLevelNode fromNode;
				if (FindNodeByID(fromNodeID, fromNode) && fromNode.bIsCleared)
				{
					accessibleNodes.AddUnique(edge.toNodeID);
				}
			}
		}
	}

	return accessibleNodes;
}

bool ULevelGraph::AreNodesConnected(int32 fromNodeID, int32 toNodeID) const
{
	return levelEdges.ContainsByPredicate([fromNodeID, toNodeID](const FLevelEdge& edge)
	{
		return (edge.fromNodeID == fromNodeID && edge.toNodeID == toNodeID) ||
			   (edge.fromNodeID == toNodeID && edge.toNodeID == fromNodeID);
	});
}

float ULevelGraph::GetConnectionDistance(int32 fromNodeID, int32 toNodeID) const
{
	const FLevelEdge* edge = levelEdges.FindByPredicate([fromNodeID, toNodeID](const FLevelEdge& edge)
	{
		return (edge.fromNodeID == fromNodeID && edge.toNodeID == toNodeID) ||
			   (edge.fromNodeID == toNodeID && edge.toNodeID == fromNodeID);
	});

	return edge ? edge->transitionDistance : -1.f;
}

bool ULevelGraph::IsAllLevelsCleared() const
{
	for (const FLevelNode& node : levelNodes)
	{
		if (node.bHasQuestConditions && !node.bIsCleared)
		{
			return false;
		}
	}
	return true;
}

TArray<FName> ULevelGraph::GetLevelsWithQuests() const
{
	TArray<FName> questLevels;
	for (const FLevelNode& node : levelNodes)
	{
		if (node.bHasQuestConditions)
		{
			questLevels.Add(node.levelName);
		}
	}
	return questLevels;
}