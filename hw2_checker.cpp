#include "hw2_checker.hpp"

int main(int argc, char** argv)
{
	//function_file.open("function.out");
	//function_file << "Node function:" << endl;

	yyin = fopen(argv[1], "r");
	yyparse();
	fclose(yyin);

	set<string>::iterator it;
	in_edge_it_t in_edge_it;
	out_edge_it_t out_edge_it;
    bool checker_failed;
	//string query_node;
    
    //it = node_list.begin();
    //while(it != node_list.end()) {
    //    cout << "* " << *it << endl;   
    //    it++;
    //}

    //std::pair<Graph::vertex_iterator,Graph::vertex_iterator> vs = boost::vertices(g);
    //for(Graph::vertex_iterator v_it = vs.first; v_it != vs.second; v_it++) {
    //    cout << "** " << *(g[*v_it].node_it) << endl;
    //}
    if( argc >= 3 ) {
        ifstream exp_file;
        string exp_file_token;
        unsigned mode = 0; // 0: asap, 1: alap
        unsigned schedule_slot = 0;

        checker_failed = false;

        exp_file.open(argv[2]);

        exp_file >> exp_file_token; 
        while(!exp_file.eof()) {
            //cout << "\"" << exp_file_token << "\" ";
            if( exp_file_token.compare("ALAP") == 0 ) {
                //cout << exp_file_token;
                mode = 1; // switch to alap mode
            } else if( exp_file_token.compare("ASAP") == 0 ) {
                //cout << exp_file_token;
                mode = 0; // switch to asap mode
            } else if( exp_file_token.compare("Scheduling") == 0 ) {
                //cout << exp_file_token;
            } else if( *(exp_file_token.end() - 1) == ':' ) { 
                // start input schedule result
                exp_file_token.erase(exp_file_token.end() - 1);
                schedule_slot = atoi(exp_file_token.c_str());
                //cout << "slot: " << schedule_slot << endl;
            } else {
                it = node_list.find(exp_file_token);
                if ( it == node_list.end() ) {
                    cerr << "Unexpected token: " << exp_file_token << endl;
                    return 2;
                }
                Vertex current_node = vertex_map[*(it)];
                if( mode == 1 ) {
                    g[current_node].exp_max_schedule = schedule_slot - 1;
                } else {
                    g[current_node].exp_min_schedule = schedule_slot - 1;
                }
            }
            
            exp_file >> exp_file_token; 
        }
        cout << endl;
    }

    // begin ASAP

    list< Vertex > vertex_list;
    Vertex _source = vertex_map[*(node_list.find("_source"))];
    vertex_list.push_back(_source);

    while(! vertex_list.empty()) {

        Vertex current_node = vertex_list.front();
        vertex_list.pop_front();	

	    //cout << "Query node: " << *(g[current_node].node_it) << endl;
        //cout << "Cell Delay: " << g[current_node].cell_delay << endl;
		//cout << "Predecessor: ";
		in_edge_range_t in_edge_range = in_edges(current_node, g);
		if(in_edge_range.first == in_edge_range.second) { // empty
			//cout << "-";
        }
        g[current_node].node_status = visited;
		for(in_edge_it = in_edge_range.first; in_edge_it != in_edge_range.second; in_edge_it++) {
			if(in_edge_it != in_edge_range.first) {
				//cout << ", ";
            }
			Vertex source = boost::source(*in_edge_it, g);
            if(g[source].min_delay + g[current_node].cell_delay > g[current_node].min_delay) {
                g[current_node].min_delay = g[source].min_delay + g[current_node].cell_delay;
            }
            if(g[source].node_status != visited) {
                if (g[current_node].node_status == visited) {
                    g[current_node].node_status = searched;
                    vertex_list.push_back(current_node);
                }
			    //cout << *(g[source].node_it);
            } else {
			    //cout << "**" << *(g[source].node_it);
            }
		}
		//cout << endl;
		//cout << "Successor: ";
		out_edge_range_t out_edge_range = out_edges(current_node, g);
		if(out_edge_range.first == out_edge_range.second) { // empty
			//cout << "-";
        }
		for(out_edge_it = out_edge_range.first; out_edge_it != out_edge_range.second; out_edge_it++) {
		    if(out_edge_it != out_edge_range.first){
				//cout << ", ";
            }
			Vertex target = boost::target(*out_edge_it, g);
            if(g[target].node_status == nonvisited) {
                if(g[current_node].node_status == visited) {
                    g[target].node_status = searched;
                    vertex_list.push_back(target);
                }
			    //cout << *(g[target].node_it);
            } else if (g[target].node_status == visited) {
			    //cout << "**" << *(g[target].node_it);
            } else if (g[target].node_status == searched) {
			    //cout << "*" << *(g[target].node_it);
            }
		}
		//cout << endl;
    }
    
    cout << "ASAP" << endl;
    std::pair<Graph::vertex_iterator,Graph::vertex_iterator> vs = boost::vertices(g);
    for(Graph::vertex_iterator v_it = vs.first; v_it != vs.second; v_it++) {
        if ( g[*v_it].cell_delay != 0 ) { 
            if( argc >= 3 ) {
                if( g[*v_it].min_delay - g[*v_it].cell_delay != g[*v_it].exp_min_schedule ) {
                    cout << "Wrong Anser: "
                        << "\"" << *(g[*v_it].node_it) << "\"" 
                        << "(" << g[*v_it].min_delay - g[*v_it].cell_delay << ":" << g[*v_it].exp_min_schedule << ")" << endl;
                    checker_failed = true;
                }
            } else {
                if ( g[*v_it].node_status == visited ) {
                    cout << "**";
                }
                cout  << "\"" << *(g[*v_it].node_it) << "\""
                    << "(" << g[*v_it].min_delay - g[*v_it].cell_delay << ":" << g[*v_it].exp_min_schedule << ")" << endl;
            }
        }
    }
    if( argc >= 3 ) {
        if( checker_failed ) {
            cout << "ASAP Failed" << endl;
            return 1;
        } else {
            cout << "ASAP Passed" << endl;
        }
    }

    // begin ALAP
    Vertex _target = vertex_map[*(node_list.find("_target"))];
    for(Graph::vertex_iterator v_it = vs.first; v_it != vs.second; v_it++) {
        g[*v_it].node_status = nonvisited;
        g[*v_it].max_delay = g[_target].min_delay;
    }
    vertex_list.clear();
    vertex_list.push_back(_target);

    while(! vertex_list.empty()) {

        Vertex current_node = vertex_list.front();
        vertex_list.pop_front();	

	    //cout << "Query node: " << *(g[current_node].node_it) << endl;
        //cout << "Cell Delay: " << g[current_node].cell_delay << endl;
		//cout << "Successor: ";
		out_edge_range_t out_edge_range = out_edges(current_node, g);
		if(out_edge_range.first == out_edge_range.second) { // empty
			//cout << "-";
        }
        g[current_node].node_status = visited;
		for(out_edge_it = out_edge_range.first; out_edge_it != out_edge_range.second; out_edge_it++) {
			if(out_edge_it != out_edge_range.first) {
				//cout << ", ";
            }
			Vertex target = boost::target(*out_edge_it, g);
            if(g[target].max_delay - g[target].cell_delay < g[current_node].max_delay) {
                g[current_node].max_delay = g[target].max_delay - g[target].cell_delay;
            }
            if(g[target].node_status != visited) {
                if (g[current_node].node_status == visited) {
                    g[current_node].node_status = searched;
                    vertex_list.push_back(current_node);
                }
			    //cout << *(g[target].node_it);
            } else {
			    //cout << "**" << *(g[target].node_it);
            }
		}
		//cout << endl;
		//cout << "Predecessor: ";
		in_edge_range_t in_edge_range = in_edges(current_node, g);
		if(in_edge_range.first == in_edge_range.second) { // empty
			//cout << "-";
        }
		for(in_edge_it = in_edge_range.first; in_edge_it != in_edge_range.second; in_edge_it++) {
			if(in_edge_it != in_edge_range.first) {
				//cout << ", ";
            }
			Vertex source = boost::source(*in_edge_it, g);
            if(g[source].node_status == nonvisited) {
                if(g[current_node].node_status == visited) {
                    g[source].node_status = searched;
                    vertex_list.push_back(source);
                }
			    //cout << *(g[source].node_it);
            } else if (g[source].node_status == visited) {
			    //cout << "**" << *(g[source].node_it);
            } else if (g[source].node_status == searched) {
			    //cout << "*" << *(g[source].node_it);
            }
		}
		//cout << endl;
    }
    
    cout << "ALAP" << endl;
    for(Graph::vertex_iterator v_it = vs.first; v_it != vs.second; v_it++) {
        if ( g[*v_it].cell_delay != 0 ) { 
            if( argc >= 3 ) {
                if( g[*v_it].max_delay - g[*v_it].cell_delay != g[*v_it].exp_max_schedule ) {
                    cout << "Wrong Anser: "
                        << "\"" << *(g[*v_it].node_it) << "\"" 
                        << "(" << g[*v_it].max_delay - g[*v_it].cell_delay << ":" << g[*v_it].exp_max_schedule << ")" << endl;
                    checker_failed = true;
                }
            } else { 
                if ( g[*v_it].node_status == visited ) {
                    cout << "**";
                }
                cout  << "\"" << *(g[*v_it].node_it) << "\""
                    << "(" << g[*v_it].max_delay - g[*v_it].cell_delay << ":" << g[*v_it].exp_max_schedule << ")" << endl;
            }
        }
    }
    if( argc >= 3 ) {
        if( checker_failed ) {
            cout << "ALAP Failed" << endl;
            return 1;
        } else {
            cout << "ALAP Passed" << endl;
        }
    }
    
	return 0;
}
