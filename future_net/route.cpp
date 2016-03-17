#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "lib_io.h"
#include "lib_time.h"
#include "route.h"
#include "lib_record.h"
#include <stdio.h>
#include "stdlib.h"

typedef unsigned int uint;
#define MAX_LINE_LEN 4000
//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand)
{
    NodeSet *node_set = new NodeSet;
	EdgeSet *edge_set = new EdgeSet;
	uint source_node;
	uint destination_node;
	vector<uint> *pass_node = new vector<uint>;
	vector<uint> *opt_link = new vector<uint>;
	vector<string> *parse_graph = new vector<string>;
    
    CsvParser(topo, edge_num, parse_graph);
    NewGraph(node_set, edge_set, parse_graph);
    
	NewDem(demand, pass_node);
	source_node = *pass_node->begin();
	pass_node->erase(pass_node->begin());
	destination_node = *pass_node->begin();
	pass_node->erase(pass_node->begin());
	print_time("Get graph and demand");
    
	for(vector<uint>::iterator iter=pass_node->begin(); iter!=pass_node->end(); iter++) {
		cout << *iter << endl;
	}
	
	
	pathtree path_tree(source_node, destination_node, node_set, pass_node);
	path_tree.FindPath();
	print_time("Find path finished");
	path_tree.GetOptPath(opt_link);
	print_time("Get opt path finished");

	if(!opt_link->empty()) {
		for(vector<uint>::iterator iter=opt_link->begin(); iter!=opt_link->end(); iter++) {
			record_result(*iter);
			cout << *iter << endl;
		}
	}
	
	path_tree.FreePath();
	free(pass_node);
	free(parse_graph);
	free(opt_link);
	node_set->FreeNode();
	free(node_set);
	edge_set->FreeEdge();
	free(edge_set);
	
}

void CsvParser(char *topo[5000], int edge_num, vector<string>* par_graph) {
    string str;
	for(int i=0; i<edge_num; i++) {
    	for(int j=0; j<MAX_LINE_LEN+2 && topo[i][j]!=0 && topo[i][j]!='\n'; j++) {
    		str.push_back(topo[i][j]);
    	}
    	string str_line=str;
    	str="";
    	par_graph->push_back(str_line);

    }
	//cout << "The total number of lines is " << par_graph->size() << endl;
}

void NewGraph(NodeSet *node_set, EdgeSet *edge_set, vector<string> *parse_graph) {
	unsigned int LinkID = 0;
	unsigned int SourceID = 0;
	unsigned int DestinationID = 1;
	unsigned Cost = 1;
	bool LinkExist=false;
	vector<Edge *> *edges;
	Node *pNode;
	LineParser line_parser;
	for (vector<string>::iterator str=parse_graph->begin(); str!=parse_graph->end(); str++) {
		line_parser.StrParser(*str);
		LinkID = line_parser.GetLinkID();
		SourceID = line_parser.GetSourceNodeID();
		DestinationID = line_parser.GetDestinationNodeID();
		Cost = line_parser.GetLinkCost();
		//cout << LinkID << '|' << SourceID << '|' << DestinationID << '|' << Cost << endl;
	
		if( !node_set->IsExist(SourceID) ) {
			if( !node_set->IsExist(DestinationID) ) {
				Node *pSourceNode = new Node;
				pSourceNode->SetNodeID(SourceID);
				node_set->AddNode(SourceID, pSourceNode);
				
				Node *pDestinationNode = new Node;
				pDestinationNode->SetNodeID(DestinationID);
				node_set->AddNode(DestinationID, pDestinationNode);
				
				Edge *pEdge = new Edge;
				pEdge->EdgeInit(LinkID, pSourceNode, pDestinationNode, Cost);
				edge_set->AddEdge(LinkID, pEdge);
		        pSourceNode->AddToEdge(pEdge);
		        
		        //cout << "new s node new d node" << endl;
			} 
			else {
				Node *pSourceNode = new Node;
				pSourceNode->SetNodeID(SourceID);
				node_set->AddNode(SourceID, pSourceNode);
				
				Edge *pEdge = new Edge;
				pEdge->EdgeInit(LinkID, pSourceNode, node_set->GetNode(DestinationID), Cost);
				edge_set->AddEdge(LinkID, pEdge);
				
		        pSourceNode->AddToEdge(pEdge);
		        
		        //cout << "new s node old d node" << endl;
			} 
		}
		
		else if ( !node_set->IsExist(DestinationID) ) {
				Node *pDestinationNode = new Node;
				pDestinationNode->SetNodeID(DestinationID);
				node_set->AddNode(DestinationID, pDestinationNode);
				
		        Edge *pEdge = new Edge;
				pEdge->EdgeInit(LinkID, node_set->GetNode(SourceID), pDestinationNode, Cost);
				edge_set->AddEdge(LinkID, pEdge);
				
				node_set->GetNode(SourceID)->AddToEdge(pEdge);
				//cout << "old s node new d node" << endl;
			}
			else {
				pNode=node_set->GetNode(SourceID);
				edges=pNode->GetEdgeSet();
				for(vector<Edge *>::iterator iter = edges->begin(); iter != edges->end(); iter++) {
					if(DestinationID == (*iter)->GetToNode()->GetNodeID()) {
						if(Cost < (*iter)->GetEdgeCost()) {
							(*iter)->SetEdgeID(LinkID);
							(*iter)->SetEdgeCost(Cost);
						}
						LinkExist=true;
					}
				}
				if(!LinkExist) {
					Edge *pEdge = new Edge;
					pEdge->EdgeInit(LinkID, node_set->GetNode(SourceID), node_set->GetNode(DestinationID), Cost);
					edge_set->AddEdge(LinkID, pEdge);
					
					node_set->GetNode(SourceID)->AddToEdge(pEdge);
					//cout << "old s node old d node" << endl;
				}
			}
    }
    while (!parse_graph->empty()) {
    	parse_graph->erase(parse_graph->begin());
    }
}

void NewDem(char *dem, vector<unsigned int> *pass_node) {
	LineParser dem_parser;
	string str;
	for(int j=0; j<MAX_LINE_LEN+2 && dem[j]!=0 && dem[j]!='\n'; j++) {
    		str.push_back(dem[j]);
    	}
	dem_parser.GetDem(str, pass_node);
}

pathtree::pathtree(uint s, uint d, NodeSet* node_set, vector<uint>  *pass) {
	_source_node = s;
	_destination_node = d;
	_node_set = node_set;
	pass_node_set = pass;
}


bool pathtree::UseNode(uint node) {
    for(vector<uint>::iterator iter=pass_node_set->begin(); iter!=pass_node_set->end(); iter++) {
		if(*iter == node) return true;
	}
	return false;
}

void pathtree::FindPath() {
	Node *pNode;
	vector<Edge *> *edges;
	uint path_set_size;
	//initialization
	path *pPath= new path;
	pPath->NewNode(_source_node); //remember to free the vector when update link status.
	_path_set.push_back(pPath);
	//path set loop
	while(!_path_set.empty()) {
		path_set_size=_path_set.size();
		for(vector<path *>::iterator path_iter=_path_set.begin(); path_iter!=_path_set.end(); path_iter++) {
			pNode = _node_set->GetSquNode( (*path_iter)->GetLastNode() ); //? get the last node in the path.
			edges = pNode->GetEdgeSet(); //get the current node's edges, which point to its neighbour nodes.
			//edge loop for every path.
			vector<uint> *new_node_set = new vector<uint>;
			//if edge is null or it points to useless node, the corresponding new node set will be empty, and the path will terminated.
			for(vector<Edge *>::iterator iter = edges->begin(); iter != edges->end(); iter++) {
				if( !((*path_iter)->NodeExist( (*iter)->GetToNode()->GetNodeID())) ) {
					new_node_set->push_back( (*iter)->GetToNode()->GetNodeID() );
				} 
			}
			_next_node_set.push_back(new_node_set);
		}

		//update the path tree when the path set loop finished.
		for(vector<vector<uint> *>::iterator path_iter=_next_node_set.begin(); path_iter!=_next_node_set.end(); path_iter++) {
			for(vector<uint>::iterator iter=(*path_iter)->begin(); iter!=(*path_iter)->end(); iter++) {
				if(*iter == _destination_node) {
					if( (*(_path_set.begin()+(path_iter-_next_node_set.begin())))->UsePath(pass_node_set) ) {
						path* use_path = new path;
						*use_path = *(*(_path_set.begin()+(path_iter-_next_node_set.begin())));
						use_path->NewNode(_destination_node);
						_use_path_set.push_back(use_path);
					}
				}
				else
					{
						if(UseNode(*iter)) {
							path* new_path = new path;
							*new_path = *(*(_path_set.begin()+(path_iter-_next_node_set.begin())));
							new_path->NewNode(*iter);
							new_path->NewUseNode();
							_path_set.push_back(new_path);
						}
						else {
							path* new_path = new path;
							*new_path = *(*(_path_set.begin()+(path_iter-_next_node_set.begin())));
							new_path->NewNode(*iter);
							_path_set.push_back(new_path);
						}
					}
			}
		}

		//erase the old path and free the memory.
		for(uint i=0; i<path_set_size; i++) {
			free( *(_path_set.begin()) );
			_path_set.erase(_path_set.begin());
		}
	    
	    while(!_next_node_set.empty()) {
			free( *(_next_node_set.begin()) );
			_next_node_set.erase(_next_node_set.begin());
		}
		cout << _path_set.size() << endl;
		print_time("path iteration");
	}
	
}

void pathtree::FreePath() {
	while(!_use_path_set.empty()) {
		free(*(_use_path_set.begin()));
		_use_path_set.erase(_use_path_set.begin());
	}
}
void pathtree::TreeInfo() {
	cout << _use_path_set.size() << endl;
}

void pathtree::GetOptPath(vector<uint> *ppath) {
	uint min_cost;
	uint num=0;
	path *opt_path;
	if(_use_path_set.empty()) ppath->clear();
	else {
		for(vector<path *>::iterator path_iter=_use_path_set.begin(); path_iter!=_use_path_set.end(); path_iter++) {
			_path_cost_set.push_back((*path_iter)->GetPathCost(_node_set));
		}
		min_cost = *(_path_cost_set.begin())+1;
		for(vector<uint>::iterator cost_iter=_path_cost_set.begin(); cost_iter!=_path_cost_set.end(); cost_iter++) {
			if(*cost_iter < min_cost) {
				min_cost = *cost_iter;
				num = cost_iter-_path_cost_set.begin();
			}
		}
		opt_path = *(_use_path_set.begin()+num);
		opt_path->EdgeInfo(_node_set, ppath);
		}
}