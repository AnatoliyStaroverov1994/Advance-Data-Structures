#ifndef SEPARATE_CHAINING_H
#define SEPARATE_CHAINING_H

#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <functional>
using namespace std;


int nextPrime( int n );

template <typename HashedObj>

class HashTable_1
    
{
  public:
    explicit HashTable_1( int size = 101 ) : currentSize{ 0 }
      { theLists.resize( 101 ); }

    bool contains( const HashedObj & x ) const {
        auto & whichList = theLists[ myhash( x ) ];
        return find( begin( whichList ), end( whichList ), x ) != end( whichList );
    }

    void makeEmpty( ){
        for( auto & thisList : theLists )
            thisList.clear( );
    }

    
    bool insert( const  HashedObj  & x )  {
        /*
            Insert a Ivalue into the header of the linked list.
        
        */
        
        auto & whichList = theLists[ myhash( x ) ];
        
        if( find( begin( whichList ), end( whichList ), x ) != end( whichList) ){
            
            collisionChainingHT++;
            
            return false;
            
        }
        whichList.push_back( x );

            // Rehash
        if( ++currentSize > theLists.size( ) )
            rehash( );

        return true;
        
        
    }
    
    bool insert( HashedObj && x ){
        
        auto & whichList = theLists[ myhash( x ) ];  
        
        if( find( begin( whichList ), end( whichList ), x ) != end( whichList ) )
            collisionChainingHT++;
            return false;
        /*inserts the item in the linked list but not on top*/
        whichList.push_back( std::move( x ) );

            // Rehash; see Section 5.5
        if( ++currentSize > theLists.size( ) )
            rehash( );

        return true;
    }
    int coll(){
        
        return collisionChainingHT;
    }

    bool remove( const HashedObj & x ){
        auto & whichList = theLists[ myhash( x ) ];
        auto itr = find( begin( whichList ), end( whichList ), x );

        if( itr == end( whichList ) )
            return false;

        whichList.erase( itr );
        --currentSize;
        return true;
    }
    

  private:
    vector<list<HashedObj>> theLists;   // The array of Lists
    int collisionChainingHT = 0;
    
    int  currentSize;
    int SucRate = 0;

    void rehash( )
    {
        vector<list<HashedObj>> oldLists = theLists;

            // Create new double-sized, empty table
        theLists.resize( nextPrime( 2 * theLists.size( ) ) );
        for( auto & thisList : theLists )
            thisList.clear( );

            // Copy table over
        currentSize = 0;
        for( auto & thisList : oldLists )
            for( auto & x : thisList )
                insert( std::move( x ) );
    }

    size_t myhash( const HashedObj & x ) const
    {
        static hash<HashedObj> hf;
        return hf( x ) % theLists.size( );
    }
};

#endif
