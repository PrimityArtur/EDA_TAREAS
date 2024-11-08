#pragma once
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

#include <algorithm>
#include <functional>
#include <vector>
#include <limits>
#include <iostream>

#include "json.hpp"
#include <fstream>

#define MAXNODES 2
#define MINNODES 1 

using namespace std;
using json = nlohmann::json;

#define ASSERT assert // RTree uses ASSERT( condition )
#define Min min
#define Max max


struct Rect
{
	Rect() {}

	Rect(int a_minX, int a_minY, int a_maxX, int a_maxY)
	{
		m_min[0] = a_minX;
		m_min[1] = a_minY;

		m_max[0] = a_maxX;
		m_max[1] = a_maxY;
	}

	int m_min[2];                      ///< Min dimensions of bounding box 
	int m_max[2];                      ///< Max dimensions of bounding box 
};

struct  Node;

struct Branch
{
	Rect m_rect;                                  ///< Bounds
	Node* m_child;                                ///< Child node
	vector<pair<int, int>> m_data;                              ///< Data Id
};

struct Node
{
	bool IsInternalNode() { return (m_level > 0); } // Not a leaf, but a internal node
	bool IsLeaf() { return (m_level == 0); } // A leaf, contains data

	int m_count;                                  ///< Count
	int m_level;                                  ///< Leaf is zero, others positive
	Branch m_branch[MAXNODES + 1];                    ///< Branch
};

/// A link list of nodes for reinsertion after a delete operation
struct ListNode
{
	ListNode* m_next;                             ///< Next in list
	Node* m_node;                                 ///< Node
};

/// Variables for finding a split partition
struct PartitionVars
{
	enum { NOT_TAKEN = -1 }; // indicates that position

	int m_partition[MAXNODES + 1];
	int m_total;
	int m_minFill;
	int m_count[2];
	Rect m_cover[2];
	float m_area[2];

	Branch m_branchBuf[MAXNODES + 1];
	int m_branchCount;
	Rect m_coverSplit;
	float m_coverSplitArea;
};


class RTree
{
public:

	RTree();
	RTree(const RTree& other);
	virtual ~RTree();
	vector<vector<pair<int, int>>> mObjs;

	void Insert(const int a_min[2], const int a_max[2], vector<pair<int, int>>& a_dataId);
	void Remove(const int a_min[2], const int a_max[2], const vector<pair<int, int>>& a_dataId);
	void RemoveAll();

	/// Count the data elements in this container.  This is slow as no internal counter is maintained.
	int Count();

	/// get the MBRs;
	bool getMBRs(vector<vector<vector<pair<int, int>>>>& mbrs_n);
	Rect MBR(vector<pair<int, int>> pol);

	//Init - SEARCH 
	void SearchRectanglesAtRectangle(const int a_min[2], const int a_max[2], vector<pair<vector<pair<int, int>>, Rect>>& result, vector<Rect>& outsideRects);
	void printSearchRectanglesAtRectangle(const int a_min[2], const int a_max[2], vector<pair<vector<pair<int, int>>, Rect>>& result);

	void SearchPointsInRect(const int a_min[2], const int a_max[2], vector<pair<int, int>>& result);
	
	void SearchRectanglesAtPoint(const int point[2], vector<vector<pair<int, int>>>& result);
	
	void SearchRectanglesInsideRectangle(const int a_min[2], const int a_max[2], vector<vector<pair<int, int>>>& result, vector<Rect>& rects);

	//End - SEARCH

	//Init - EXPORTJSON
	void ExportToJson(const std::string& filename);
	void ExportSearchRectanglesAtRectangleToJson(const std::string& filename, vector<Rect>& outsideRects);
	//End - EXPORT
protected:

	//Init - SEARCH 
	void SearchRectanglesAtRectangleRec(Node* a_node, Rect* a_rect, vector<pair<vector<pair<int, int>>, Rect>>& result, vector<Rect>& outsideRects);

	void SearchPointsInRectRec(Node* a_node, Rect* a_rect, vector<pair<int, int>>& result);
	
	void SearchRectanglesAtPointRec(Node* a_node, const int point[2], vector<vector<pair<int, int>>>& result);
	
	void SearchRectanglesInsideRectangleRec(Node* a_node, Rect* a_rect, vector<vector<pair<int, int>>>& result, vector<Rect>& rects);
	// verificar si un rectángulo coliciona con el punto
	bool ContainsPoint(const Rect* rect, const int point[2]) const;

	//End - SEARCH

	//Init - EXPORTJSON
	void ExportNodeToJson(Node* node, json& jnode);
	//End - EXPORT

	Node* AllocNode();
	void FreeNode(Node* a_node);
	void InitNode(Node* a_node);
	void InitRect(Rect* a_rect);
	bool InsertRectRec(const Branch& a_branch, Node* a_node, Node** a_newNode, int a_level);
	bool InsertRect(const Branch& a_branch, Node** a_root, int a_level);
	Rect NodeCover(Node* a_node);
	bool AddBranch(const Branch* a_branch, Node* a_node, Node** a_newNode);
	void DisconnectBranch(Node* a_node, int a_index);
	int PickBranch(const Rect* a_rect, Node* a_node);
	Rect CombineRect(const Rect* a_rectA, const Rect* a_rectB);
	void SplitNode(Node* a_node, const Branch* a_branch, Node** a_newNode);
	float CalcRectArea(Rect* a_rect);
	void GetBranches(Node* a_node, const Branch* a_branch, PartitionVars* a_parVars);
	void ChoosePartition(PartitionVars* a_parVars, int a_minFill);
	void LoadNodes(Node* a_nodeA, Node* a_nodeB, PartitionVars* a_parVars);
	void InitParVars(PartitionVars* a_parVars, int a_maxRects, int a_minFill);
	void PickSeeds(PartitionVars* a_parVars);
	void Classify(int a_index, int a_group, PartitionVars* a_parVars);
	bool RemoveRect(Rect* a_rect, const vector<pair<int, int>>& a_id, Node** a_root);
	bool RemoveRectRec(Rect* a_rect, const vector<pair<int, int>>& a_id, Node* a_node, ListNode** a_listNode);
	ListNode* AllocListNode();
	void FreeListNode(ListNode* a_listNode);
	bool Overlap(Rect* a_rectA, Rect* a_rectB) const; // si A y B colocionan
	bool Overlap2(Rect* a_rectA, Rect* a_rectB) const; // B dentro de A
	void ReInsert(Node* a_node, ListNode** a_listNode);
	void RemoveAllRec(Node* a_node);
	void Reset();
	void CountRec(Node* a_node, int& a_count);

	void CopyRec(Node* current, Node* other);


	Node* m_root;                                    ///< Root of tree
	float m_unitSphereVolume;                 ///< Unit sphere constant for required number of dimensions
};