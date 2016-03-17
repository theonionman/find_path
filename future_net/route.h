#ifndef __ROUTE_H__
#define __ROUTE_H__
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "math.h"
#include <stdlib.h>
typedef unsigned int uint;
class Edge;
class Node{
	public:
		Node() {
		}
		unsigned int GetNodeID() {
			return NodeID;
		}
		vector<Edge *> *GetEdgeSet() {
			return &_edges;
		}
		unsigned int SetNodeID(unsigned int id) {
			NodeID = id;
			return 0;
		}
		unsigned int AddToEdge(Edge *edge) {
			_edges.push_back(edge);
			return 0;
		}
	private:
		unsigned int NodeID;
		vector<Edge *> _edges;
};

class NodeSet{
	public:
		NodeSet() {
		}
		void AddNode(unsigned int id, Node* node) {
			NodeIdSet.push_back(id);
			_node_set.push_back(node);
		}
		Node *GetNode(unsigned int id) {
			for (vector<unsigned int>::iterator iter = NodeIdSet.begin(); iter != NodeIdSet.end(); ++iter) {
				if( id == *iter) {
					vector<unsigned int>::iterator node_id_iter_begin = NodeIdSet.begin();
					vector<Node *>::iterator node_iter_begin = _node_set.begin();
					return *( node_iter_begin + (iter - node_id_iter_begin) );
				}
			}
			return NULL;
		}
		Node *GetSquNode(unsigned int id) {
			if(id < _node_set.size()) return *(_node_set.begin()+id);
			else return NULL;
		}
		vector<unsigned int> GetNodeIdSet() {
			return NodeIdSet;
		}
		bool IsExist(unsigned int id) {
			for (vector<unsigned int>::iterator iter = NodeIdSet.begin(); iter != NodeIdSet.end(); ++iter) {
				if( id == *iter ){
					return true;
				}
			}
			return false;
		}
		unsigned int GetNodeSetLength() {
			return NodeIdSet.size();			
		}
		void FreeNode() {
			while(!_node_set.empty()) {
				free(*(_node_set.begin()));
				_node_set.erase(_node_set.begin());
			}
		}
		void SetNodeSqu() {
			Node *temp_node;
			for (unsigned int i=0; i < _node_set.size()-1; i++) {
				for (vector<Node *>::iterator iter = _node_set.begin(); iter != _node_set.end()-1; ++iter) {
					if( (*iter)->GetNodeID() > (*(iter+1))->GetNodeID() ) {
						temp_node=*(iter+1);
						*(iter+1)=*iter;
						*iter=temp_node;
					}
				}
			}
		}
		void PrintNode(){
			for (vector<Node *>::iterator iter = _node_set.begin(); iter != _node_set.end(); ++iter) {
					cout << (*iter)->GetNodeID() << endl;
				}
		}
	private:
		vector<unsigned int> NodeIdSet;
		vector<Node *> _node_set;
};

class Edge{
	public:
		Edge() {
		}
		unsigned int EdgeInit( unsigned int link_id, Node *from_node, Node *to_node, unsigned int cost ) {
			EdgeID = link_id;
			_cost = cost; 
			FromNode = from_node;
			ToNode = to_node;
			return 0;
		}
		unsigned int GetEdgeID() {
			return EdgeID;
		}
		unsigned int GetEdgeCost() {
			return _cost;
		}
		Node *GetFromNode() {
			return FromNode;
		}
		Node *GetToNode() {
			return ToNode;
		}
		void SetEdgeID(unsigned int id) {
			EdgeID=id;
		}
		void SetEdgeCost(unsigned int cost) {
			_cost=cost;
		}
	private:
		unsigned int EdgeID;
		unsigned int _cost;
		Node *FromNode;
		Node *ToNode;
};

class EdgeSet{
	public:
		EdgeSet() {
		}
		void AddEdge(unsigned int id, Edge* edge) {
			EdgeIdSet.push_back(id);
			_edge_set.push_back(edge);
		}
		Edge *GetSquEdge(unsigned int id) {
			if(id < _edge_set.size()) return *(_edge_set.begin()+id);
			else return NULL;
		}
		Edge *GetEdge(unsigned int id) {
			for (vector<unsigned int>::iterator iter = EdgeIdSet.begin(); iter != EdgeIdSet.end(); ++iter) {
				if( id == *iter){
					vector<unsigned int>::iterator edge_id_iter_begin = EdgeIdSet.begin();
					vector<Edge *>::iterator edge_iter_begin = _edge_set.begin();
					return *( edge_iter_begin + (iter - edge_id_iter_begin) );
				}
			}
			return NULL;
		}
		bool IsExist(unsigned int id) {
			for (vector<unsigned int>::iterator iter = EdgeIdSet.begin(); iter != EdgeIdSet.end(); ++iter) {
				if( id == *iter ){
					return true;
				}
			}
			return false;
		}
		unsigned int GetEdgeSetLength() {
			return EdgeIdSet.size();			
		}
		void FreeEdge() {
			while(!_edge_set.empty()) {
				free(*(_edge_set.begin()));
				_edge_set.erase(_edge_set.begin());
			}
		}
		void SetEdgeSqu() {
			Edge *temp_node;
			for (unsigned int i=0; i < _edge_set.size()-1; i++) {
				for (vector<Edge *>::iterator iter = _edge_set.begin(); iter != _edge_set.end()-1; ++iter) {
					if( (*iter)->GetEdgeID() > (*(iter+1))->GetEdgeID() ) {
						temp_node=*(iter+1);
						*(iter+1)=*iter;
						*iter=temp_node;
					}
				}
			}
		}
		void PrintEdge(){
			for (vector<Edge *>::iterator iter = _edge_set.begin(); iter != _edge_set.end(); ++iter) {
					cout << (*iter)->GetEdgeID() << endl;
				}
		}
	private:
		vector<unsigned int> EdgeIdSet;
		vector<Edge *> _edge_set;
};

class path {
	public:
		path(): _inc_num(0),  _path_cost(0) {}
		void NewNode(uint node) {
			_cur_node_set.push_back(node);
		}
		void NewUseNode() {
			_inc_num++;
		}
		uint GetLastNode() {
			return *(_cur_node_set.end()-1);
		}
		uint GetNum() {
			return _inc_num;
		}
		uint GetPathCost(NodeSet* node_set) {
			Node *pNode;
			vector<Edge *> *edges;
			for(vector<uint>::iterator node_iter=_cur_node_set.begin(); node_iter!=_cur_node_set.end()-1; node_iter++) {
				pNode = node_set->GetNode(*node_iter);
				edges = pNode->GetEdgeSet();
				for(vector<Edge *>::iterator iter = edges->begin(); iter != edges->end(); iter++) {
					if((*iter)->GetToNode()->GetNodeID() == *(node_iter+1))
						_path_cost += (*iter)->GetEdgeCost();
				}
			}
			return _path_cost;
		}
		void EdgeInfo(NodeSet* node_set, vector<uint> *ppath) {
			Node *pNode;
			vector<Edge *> *edges;
			for(vector<uint>::iterator node_iter=_cur_node_set.begin(); node_iter!=_cur_node_set.end()-1; node_iter++) {
				pNode = node_set->GetNode(*node_iter);
				edges = pNode->GetEdgeSet();
				for(vector<Edge *>::iterator iter = edges->begin(); iter != edges->end(); iter++) {
					if((*iter)->GetToNode()->GetNodeID() == *(node_iter+1))
						ppath->push_back((*iter)->GetEdgeID());
				}
			}
		}
		//judge if the node exists in the current path.
		bool NodeExist(uint node) {
			for(vector<uint>::iterator iter=_cur_node_set.begin(); iter!=_cur_node_set.end(); iter++) {
				if(*iter == node) 
					return true;
			}
			return false;
		}
		bool UsePath(vector<uint> *pass_node_set) {
			if(_inc_num == pass_node_set->size()) return true;
			else return false; 
		}
	private:
		vector<uint> _cur_node_set;
		//vector<uint> _edge_info;
		uint _inc_num;//the variable indicates the num of designated nodes incluede in the cur node set; when a new node joins the path, the num increases by one. 
		uint _path_cost;
};

class pathtree {
	public:
		pathtree(uint, uint, NodeSet*, vector<uint> *);
		void FindPath();
		bool NodeExist();
		void TreeInfo();
		void FreePath();
		bool UseNode(uint node);
		void GetOptPath(vector<unsigned int>*);
	private:
		uint _source_node;
		uint _destination_node;
		vector<uint>   *pass_node_set;     //the nodes must be included in the path.
		NodeSet*       _node_set;          //all nodes in the graph, including nodes ID vector and nodes vector. 
		
		vector<path *>   _path_set; //the set is used to indicate the paths which are from the source node.
		vector<vector<uint> *>   _next_node_set; //the set contains the node set used to update path set.
		vector<path *>   _use_path_set;
		vector<uint>   _path_cost_set;      //the set contains the cost of each path which meets the requirements.
		
		vector<uint>   _opt_path;     //the set represents the cost from source node to current selected node.

};

void search_route(char *graph[5000], int edge_num, char *condition);
void CsvParser(char* [], int, vector<string> *);
void NewDem(char *, vector<unsigned int> *);
void NewGraph(NodeSet *, EdgeSet *, vector<string> *);
class LineParser{
	public:
		LineParser() {
		}
		unsigned int StrParser(string str) {
			unsigned int length[3];
			string s;
			unsigned int i = 0;
			//string::iterator iter_begin = str.begin();
			for (string::iterator iter=str.begin(); iter!=str.end(); iter++) {              //mark the location of each ','
				if( *iter == ',') {
					length[i] =(iter - str.begin());
					i++;
				}
			}
			
			for (unsigned int j=0; j<length[0]; j++) {  //find the string which represents the vaule of link id and calculate link id;
				s.push_back(str[j]);
			}
			link_id = StrToInt(s);
			s = "";
			
			for (unsigned int j=length[0]+1; j<length[1]; j++) {  //find the string which represents the vaule of source node id and calculate source node id;
				s.push_back(str[j]);
			}
			source_node_id = StrToInt(s);
			s = "";
			for (unsigned int j=length[1]+1; j<length[2]; j++) {  //find the string which represents the vaule of destination node id and calculate destination node id;
				s.push_back(str[j]);
			}
			destination_node_id = StrToInt(s);
			s = "";
			for (unsigned int j=length[2]+1; j<str.size(); j++) {  //find the string which represents the vaule of link cost and calculate link cost;
				s.push_back(str[j]);
			}
			link_cost = StrToInt(s);
			s = "";
			return 0;
		}
		//the following function parse the demand, and get source node, destination node and the including node set.
		void GetDem(string str, vector<unsigned int> *inc_node) {
			vector<unsigned int> length;
			string s="";
			for (string::iterator iter=str.begin(); iter!=str.end(); iter++) {              //mark the location of each ',' or '|'
				if( *iter == ',' || *iter == '|') {
					length.push_back((iter - str.begin()));
				}
			}
			for (vector<unsigned int>::iterator iter=length.begin(); iter!=length.end(); iter++) {
				if(iter == length.begin()) {
					for (string::iterator str_iter=str.begin(); str_iter!=str.begin()+*iter; str_iter++) {
						s.push_back(*str_iter);
					}
					inc_node->push_back(StrToInt(s));
					s="";
				}
				else {
					for (string::iterator str_iter=str.begin()+*(iter-1)+1; str_iter!=str.begin()+*iter; str_iter++) {
						s.push_back(*str_iter);
					}
					inc_node->push_back(StrToInt(s));
					s="";
				}
				
			}
			for (string::iterator str_iter=str.begin()+*(length.end()-1)+1; str_iter!=str.end(); str_iter++) {
					s.push_back(*str_iter);
			}
			inc_node->push_back(StrToInt(s));
		} 

		unsigned int StrToInt(string s) {
			unsigned int a[s.size()];
			unsigned int value = 0;
			unsigned int i = 0; 
			for (string::iterator c=s.begin(); c!=s.end(); c++) {
				a[i] = *c -'0';
				i++;
			}
			for ( i=0; i<s.size(); i++) {
				value += a[i]*pow(10, (s.size()-1-i));
			}
			return value;
		}
		unsigned int GetLinkID() {
			return link_id;
		}
		unsigned int GetSourceNodeID() {
			return source_node_id;
		}
		unsigned int GetDestinationNodeID() {
			return destination_node_id;
		}
		unsigned int GetLinkCost() {
			return link_cost;
		}
	private:
		string link_info;
		unsigned int link_id;
		unsigned int source_node_id;
		unsigned int destination_node_id;
		unsigned int link_cost;
};

#endif
