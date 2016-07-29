#ifndef PROJECT2_GOLDEN
#define PROJECT2_GOLDEN

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <string>
#include <map>
#include <set>
#include <list>
#include <utility>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/breadth_first_search.hpp>

using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::string;
using std::set;
using std::list;
using std::map;
using std::pair;
using std::ofstream;
using std::ifstream;

typedef set< string >::iterator id_it;
typedef pair< string, string > output_cover;

typedef enum { nonvisited, searched, visited } status;

struct node { id_it node_it; unsigned cell_delay; unsigned min_delay; unsigned max_delay; status node_status; unsigned exp_min_schedule; unsigned exp_max_schedule; };

using namespace boost;
typedef adjacency_list<listS, setS, bidirectionalS, node> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::out_edge_iterator out_edge_it_t;
typedef graph_traits<Graph>::in_edge_iterator in_edge_it_t;
typedef pair< out_edge_it_t, out_edge_it_t > out_edge_range_t;
typedef pair< in_edge_it_t, in_edge_it_t > in_edge_range_t;

set<string> node_list; // all nodes in file
map< string, Vertex > vertex_map; // map node set element to graph vertex

list< id_it > id_list; // nodes per name/input/output line
list<output_cover> output_cover_list; // bool per name line

Graph g;
ofstream function_file;
unsigned names_count = 0;

extern FILE* yyin;
extern int yylex (void);
extern int yylineno;
extern char* yytext;
extern "C" int yyparse (void);

void yyerror(const char *str)
{
	fprintf(stderr,"error: %s @ L:%4d (",str, yylineno);
    for( int i = 0; i < strlen(yytext); i++ ) {
        if( yytext[i] < ' ' ) {
            fprintf(stderr,"\"0x%x\"",yytext[i]);
        } else {
            fprintf(stderr,"%c",yytext[i]);
        }
    }
    fprintf(stderr,")\n");
	exit(1);
}

int yywrap()
{
	return 1;
} 

void push_id(const char *str)
{
	id_it it;
	std::pair< id_it,bool > ret;
	ret = node_list.insert(string(str));
	it = ret.first;
	id_list.push_back(it);
	if(ret.second == true) { // new node
		Vertex V = add_vertex(g);
		g[V].node_it = it;
        g[V].cell_delay = 0;
        g[V].min_delay = 0;
        g[V].max_delay = 0;
        g[V].node_status = nonvisited;
		vertex_map[*it] = V;
	}
}

void push_output_cover( const char *input_vec, const char *output_val)
{
	output_cover_list.push_back( output_cover( string(input_vec), string(output_val) ) );
}

//struct my_visitor : boost::default_bfs_visitor{
////*(g[s].node_it)
//    void initialize_vertex(const Graph::vertex_descriptor &s, const Graph &g) const {
//      std::cout << "Initialize: " << *g[s].node_it << std::endl;
//    }
//    void discover_vertex(const Graph::vertex_descriptor &s, const Graph &g) const {
//      std::cout << "Discover: " << *g[s].node_it << std::endl;
//    }
//    void examine_vertex(const Graph::vertex_descriptor &s, const Graph &g) const {
//      std::cout << "Examine vertex: " << *g[s].node_it << std::endl;
//    }
////    void examine_edge(const Graph::edge_descriptor &e, const Graph &g) const {
////      std::cout << "Examine edge: " << g[e] << std::endl;
////    }
////    void tree_edge(const Graph::edge_descriptor &e, const Graph &g) const {
////      std::cout << "Tree edge: " << g[e] << std::endl;
////    }
////    void non_tree_edge(const Graph::edge_descriptor &e, const Graph &g) const {
////      std::cout << "Non-Tree edge: " << g[e] << std::endl;
////    }
////    void gray_target(const Graph::edge_descriptor &e, const Graph &g) const {
////      std::cout << "Gray target: " << g[e] << std::endl;
////    }
////    void black_target(const Graph::edge_descriptor &e, const Graph &g) const {
////      std::cout << "Black target: " << g[e] << std::endl;
////    }
//    void finish_vertex(const Graph::vertex_descriptor &s, const Graph &g) const {
//      std::cout << "Finish vertex: " << *g[s].node_it << std::endl;
//    }
//};

#endif
