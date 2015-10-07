/** CS515 Assignment 11
File: graph.cpp
Name: Eric Warner
Section: 1
Date: 4/20/2015
Collaboration Declaration: assistance received from TA, PAC etc.
*/ 

#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <map>
#include <utility>
#include <fstream> 
#include <set>
#include "DisjointSet.h"

using namespace std;

// ----------------------- main --------------------------
int main( int argc, char** argv ){
	
	if( argc > 2 )
	{
		exit(0);                          // arg checker
	}
	
	map< int, set< pair< string, string > > > graph; // initalize the map
	map< int, set< pair< string, string > > > leastGraph;
	set< string > nodes;
	
	ifstream infile( argv[1] );
	
	if( !infile.is_open( ) )
	{
		exit(0);                          // exit if the file does not
	}                                     // open 
	
	//--------------------------
	string start;
	string end;               // used to store the data from the file
	int weight = 0;           // stream.
	//--------------------------
	
	
	//------------------------------------------------------------------
	// Inserting all the data from the data file and adding it as a 
	// pair of chars representing a link to a map that maps it depending 
	// on the weight.
	//------------------------------------------------------------------
	while( !infile.eof( ) )
	{
		infile >> start;
		infile >> end;
		infile >> weight;
		//cout << start << '\t' << end << '\t' << weight << endl;
		
		pair<string , string> link( start, end );
		
		graph[weight].insert( link );
		
		if( nodes.find( start ) == nodes.end( ) )
		{
			nodes.insert( start );
		}
		if( nodes.find( end ) == nodes.end( ) )
		{
			nodes.insert( end );
		}
	}
	//------------------------------------------------------------------
	
	//------
	set< string >::iterator itN = nodes.begin( );
	while( itN != nodes.end( ) )
	{
		cout << *itN << " ";
		itN++;
	}
	cout << endl;
	//------
	
	//------------------------------------------------------------------
	// used to trace through the map to make sure the all edges have
	// been added 
	//------------------------------------------------------------------
	map< int, set< pair< string, string > > >::iterator itG = graph.begin( );
	while( itG != graph.end( ) )
	{
		cout << itG->first << " | ";
		
		set< pair< string, string > >::iterator itS = (itG->second).begin( );
		while( itS != (itG->second).end( ) )
		{
			cout << itS->first << "--" << itS->second << "    ";
			itS++;
		}
		cout << endl;
		itG++;
	}
	//------------------------------------------------------------------
	
	//------------------------------------------------------------------
	// For each node create a new disjoint set and then go through and
	// add those sets together until all the nodes are in the same set
	//------------------------------------------------------------------
	cout << endl;
	DisjointSet<string> sets;
	itN = nodes.begin();
	for( int i=0; i<nodes.size(); i++ )
	{
		sets.createSet( *itN );
		cout << *itN << " ";
		itN++;
	}
	cout << endl;
	//--------------
	itG = graph.begin( );
	
	while( itG != graph.end( ) )
	{
		set< pair< string, string > >::iterator itS = (itG->second).begin( );
		while( itS != (itG->second).end() )
		{
			if( sets.unionSets( itS->first, itS->second ) )
			{
				pair<string , string> link( itS->first, itS->second );
		
				leastGraph[itG->first].insert( link );
			}
			itS++;
		}
		itG++;
	}
	//------------------------------------------------------------------
	itG = leastGraph.begin( );
	while( itG != leastGraph.end( ) )
	{
		cout << itG->first << " | ";
		
		set< pair< string, string > >::iterator itS = (itG->second).begin( );
		while( itS != (itG->second).end( ) )
		{
			cout << itS->first << "--" << itS->second << "    ";
			itS++;
		}
		cout << endl;
		itG++;
	}
	//------------------------------------------------------------------

}
