%{
#include "hw2_checker.hpp"
%}

%union { char* token_string; }
%token MODEL INPUTS OUTPUTS CLOCK END NAMES LINEFEED
%token <token_string> BOOL ID

%%

blif            : input_list output_list commands END
                | input_list output_list commands
                | model_name input_list output_list commands END
                ;
model_name      : MODEL ID
                ;
input_list      : input_list input
                | input
                ;
input           : INPUTS { id_list.clear(); } ids ID
                {
                    push_id($4);
                    id_it source_it;
                    std::pair< id_it,bool > ret;
                    ret = node_list.insert(string("_source"));
                    source_it = ret.first;
                    if(ret.second == true) { // new node
                        Vertex V = add_vertex(g);
                        g[V].node_it = source_it;
                        g[V].cell_delay = 0;
                        g[V].min_delay = 0;
                        g[V].max_delay = 0;
                        g[V].node_status = nonvisited;
                        vertex_map[*source_it] = V;
                    }

                    list< id_it >::iterator it;
                    for(it = id_list.begin(); it != id_list.end(); it++) {
                        add_edge( vertex_map[*source_it] , vertex_map[**it] , g);
                    }
                }
                ;
output_list     : output_list output
                | output
                ;
output          : OUTPUTS { id_list.clear(); } ids ID
                {
                    push_id($4);
                    id_it target_it;
                    std::pair< id_it,bool > ret;
                    ret = node_list.insert(string("_target"));
                    target_it = ret.first;
                    if(ret.second == true) { // new node
                        Vertex V = add_vertex(g);
                        g[V].node_it = target_it;
                        g[V].cell_delay = 0;
                        g[V].min_delay = 0;
                        g[V].max_delay = 0;
                        g[V].node_status = nonvisited;
                        vertex_map[*target_it] = V;
                    }

                    list< id_it >::iterator it;
                    for(it = id_list.begin(); it != id_list.end(); it++) {
                        add_edge( vertex_map[**it] , vertex_map[*target_it] , g);
                    }
                }
                ;
ids             : ids ID { push_id($2); }
                | ids BOOL { push_id($2); }
                |
                ;
commands        : commands command
                | command
                ;
command         : NAMES { id_list.clear(); names_count++; } ids ID output_covers
                {
                    id_it target_it;
                    std::pair< id_it,bool > ret;
                    ret = node_list.insert(string($4));
                    target_it = ret.first;
                    Vertex current_node;
                    if(ret.second == true) { // new node
                        Vertex V = add_vertex(g);
                        g[V].node_it = target_it;
                        g[V].node_status = nonvisited;
                        vertex_map[*target_it] = V;
                        current_node = V;
                    } else {
                        current_node = vertex_map[*target_it];
                    }
                    g[current_node].cell_delay = 2 - (names_count % 2); // odd: delay 1, even: delay 2
                    g[current_node].min_delay = 0;
                    g[current_node].max_delay = 0;

                    list< id_it >::iterator it;
                    list< output_cover >::iterator output_it;

                    for(it = id_list.begin(); it != id_list.end(); it++) {
                        add_edge( vertex_map[**it] , current_node , g);
                    }

                    function_file << *target_it << " =";
                    for(output_it = output_cover_list.begin();
                            output_it != output_cover_list.end();
                            output_it++) {
                        it = id_list.begin();
                        if(output_it->second.compare("1") == 0) {
                            // sum-of-product
                            if(output_it != output_cover_list.begin()) {
                                function_file << " +";
                            }
                            for(string::iterator char_it = output_it->first.begin();
                                    char_it != output_it->first.end();
                                    char_it++) {
                                if(*char_it == '0') {
                                    function_file << ' ' << **it << "\'";
                                } else if(*char_it == '1') {
                                    function_file << ' ' << **it;
                                }
                                it++;
                            }
                        } else {
                            for(string::iterator char_it = output_it->first.begin();
                                    char_it != output_it->first.end();
                                    char_it++) {
                                if(char_it != output_it->first.begin()) {
                                    function_file << " +";
                                }
                                if(*char_it == '1') {
                                    function_file << ' ' << **it << "\'";
                                } else if(*char_it == '0') {
                                    function_file << ' ' << **it;
                                }
                                it++;
                            }
                        }
                    }
                    function_file << endl;
                    output_cover_list.clear();
                }
                ;
output_covers   : output_covers output_cover
                | output_cover
                ;
output_cover    : BOOL BOOL { push_output_cover($1, $2); }
                ;

%%

#include "hw2_checker.cpp"
