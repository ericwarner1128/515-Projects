/** CS515 Assignment 9
File: unorderedmap.cpp
Name: Eric Warner
Section: 1
Date: 4/6/2015
Collaboration Declaration: assistance received from TA, PAC etc.
*/
// Below are the stubs for SOME methods
// Make sure all methods in unorderedmap.h are implemented

template <typename KEY, typename T, typename H>
UnorderedMap<KEY, T, H>::UnorderedMap(){
	tablesize = 101;
	entrysize = 0;
	array = new Entry*[tablesize];
	for( int i=0; i<101; i++ )
	{
		array[i] = 0;
	}
}
	
template <typename KEY, typename T, typename H>
UnorderedMap<KEY, T, H>::~UnorderedMap(){
	for( int i=0; i<tablesize; i++ )
	{
		Entry * cur = array[i];
		Entry * prev = cur;
		while( cur != 0 )
		{
			cur = cur->next;
			delete prev;
			prev = cur;
		}
	};
	delete [] array;
}

template <typename KEY, typename T, typename H>
bool UnorderedMap<KEY, T, H>::insert(const KEY & newkey, const T & data){
	std::size_t key = H()(newkey); 
	key = key % tablesize;
	
	if( find(newkey) != end() && entrysize != 0 )
	{
		return false;
	}
	int * i = &(tablesize);
	// rehash if needed
	if( entrysize/tablesize >= 2 )
	{
		rehash();
	}
	//-------------------------
	
	// insert ent into position
	if( array[key] != 0 )
	{
		Entry * ent = new Entry( newkey, data );
		ent->next = array[key];
		array[key] = ent;
		entrysize++;
		return true;
	}
	else
	{
		Entry * ent = new Entry( newkey, data );
		array[key] = ent;
		ent->next = 0;
		entrysize++;
		return true;
	}
	//--------------------------
	return false;
}

template <typename KEY, typename T, typename H>
bool UnorderedMap<KEY, T, H>::erase(const KEY & k ){
	// see if key is in the map
	std::size_t key = H()(k);
	key = key % tablesize;
	
    Entry * cur = array[key];
	Entry * prev = cur;
	if( cur == 0 )
	{
		return false;
	}
	if( cur->key == k )
	{
		array[key] = cur->next;
		cur->next = 0;
		delete cur;
		entrysize--;
		return true;
	}
	while( cur->key != k && cur != 0 )
	{
		prev = cur;
		cur = cur->next;
	}
	if( cur == 0 )
	{
		return false;
	}
	else
	{
		prev->next = cur->next;
		delete cur;
		entrysize--;
		return true;
	}
	//-----------------------------
}

template <typename KEY, typename T, typename H>
void UnorderedMap<KEY, T, H>::rehash( ){
	
	// make a new array with size the next prim number 
	// past the old array size * 2
	int newsize = nextPrime( tablesize * 2 );
	Entry ** temp;
	temp = new Entry*[newsize];
	for( int i=0; i<newsize; i++ )
	{
		temp[i] = 0;
	}
	//------------------------------
	
	// run through the old array and insert all the 
	// entries into the new array
	for( int i=0; i< tablesize; i++ )
	{
		Entry * cur = array[i];
		while( cur != 0 )
		{
			std::size_t key = H()(cur->key);
			key = key % newsize;
			Entry * ent = new Entry( cur->key, cur->data );
			if( temp[key] != 0 )
			{
				ent->next = temp[key];
				temp[key] = ent;
			}
			else
			{
				temp[key] = ent;
				ent->next = 0;
			}
			cur = cur->next;
		}
	}
	//-------------------------------
	
	// delete the old array 
	for( int i=0; i<tablesize; i++ )
	{
		Entry * cur1 = array[i];
		Entry * prev = cur1;
		while( cur1 != 0 )
		{
			cur1 = cur1->next;
			delete prev;
			prev = cur1;
		}
	}
	delete [] array;
	//-------------------------------
	
	// set the old array equal to the new and change the 
	// size to the new size;
	array = temp;
	tablesize = newsize;
	//-------------------------------
}



template <typename KEY, typename T, typename H>
typename UnorderedMap<KEY, T, H>::Iterator UnorderedMap<KEY, T, H>::find( const KEY & x ) const{
	// stub code; need to implement !!!
	
	// index into the correct part of the array
	std::size_t key = H()(x);
	key = key % tablesize;
	//-------------------------------
	
	// find the right element in the bucket
	Entry * found = array[key];
	while( found != 0 )
	{
		if( found->key == x )
		{
			UnorderedMap<KEY, T, H>::Iterator it( found );
			it.map = this;
			return it;
		}
		else
		{
			found = found->next;
		}
	}
	//--------------------------------
	
	// if entry was never found
	return end(); 
}


template<typename KEY, typename T, typename H>
T& UnorderedMap<KEY, T, H>::operator[](const KEY & k){
	// stub code; need to implement !!!
	//static T tmp;
	UnorderedMap<KEY, T, H>::Iterator it( find( k ) );
	if( it != end( ) )
	{
		return it->data;
	}
	else
	{
		T val = 0;
	    insert( k, val );
		it = find( k );
		return it->data;
	}
    //return tmp;
}

template <typename KEY, typename T, typename H>
typename UnorderedMap<KEY, T, H>::Iterator UnorderedMap<KEY, T, H>::Iterator::operator++(int i){
	// stub code; need to implement !!!
	if( _cur->next != 0 )
	{
		_cur = _cur->next;
		UnorderedMap<KEY, T, H>::Iterator it( _cur );
		it.map = map;
	    return it;
	}
	else
	{
		Entry * temp = 0;
		std::size_t key = H()(_cur->key);
	    key = (key % map->tablesize)+1;
		if( key == map->tablesize )
		{
			_cur = 0;
			return map->end( );
		}
		temp = map->array[key];
		while( temp == 0 && key < map->tablesize-1 )
		{
			temp = map->array[++key];
		}
		_cur = temp;
		UnorderedMap<KEY, T, H>::Iterator it( temp );
		it.map = map;
		return it;
	}
	
}

template <typename KEY, typename T, typename H>
typename UnorderedMap<KEY, T, H>::Iterator UnorderedMap<KEY, T, H>::begin() const{
	// stub code; need to implement !!!
	Entry * cur = 0;
	int count = 0;
	while( array[count] == 0 && count != tablesize )
	{
		count++;
	}
	if( count == tablesize )
	{
		return end( );
	}
	cur = array[count];
	UnorderedMap<KEY, T, H>::Iterator it( cur );
	it.map = this;
	return it;
}

template <typename KEY, typename T, typename H>
typename UnorderedMap<KEY, T, H>::Iterator UnorderedMap<KEY, T, H>::end() const{
	// stub code; need to implement !!!
	Entry * cur = array[tablesize-1];
	while(cur!=0)
	{
		cur = cur->next;
	}
	UnorderedMap<KEY, T, H>::Iterator it( cur );
	it.map = this;
	return it;
}

template <typename KEY, typename T, typename H>
typename UnorderedMap<KEY, T, H>::Entry* UnorderedMap<KEY, T, H>::Iterator::operator->(){
	return _cur;
}

template <typename KEY, typename T, typename H>
typename UnorderedMap<KEY, T, H>::Entry& UnorderedMap<KEY, T, H>::Iterator::operator*(){
	return *_cur;
}


template <typename KEY, typename T, typename H>
bool UnorderedMap<KEY, T, H>::Iterator::operator!=(Iterator it){
	return _cur != it._cur;
}

template <typename KEY, typename T, typename H>
bool UnorderedMap<KEY, T, H>::Iterator::operator==(Iterator it){
	return _cur == it._cur;
}

// Internal method to test if a positive number is prime (not efficient)
bool isPrime( int n ){
	if( n == 2 || n == 3 )
		return true;
	if( n == 1 || n % 2 == 0 )
		return false;
	for( int i = 3; i * i <= n; i += 2 )
		if( n % i == 0 )
			return false;
	return true;
}

// Internal method to return a prime number at least as large as n.
// Assumes n > 0.
int nextPrime( int n ){
	if( n % 2 == 0 )
		n++;
	for( ; !isPrime( n ); n += 2 )
		;
	return n;
}

