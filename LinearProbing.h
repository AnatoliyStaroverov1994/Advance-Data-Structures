#ifndef LINEAR_PROBING_H
#define LINEAR_PROBING_H

#include <vector>
#include <algorithm>
#include <functional>
#include <string>
using namespace std;

int nextPrime2( int n );


template <typename HashedObj>
class HashTable_2
{
  public:
    explicit HashTable_2( int size = 101 ) : array( nextPrime2( size ) )
      { makeEmpty( ); }

    bool contains( const HashedObj & x ) {
        return isActive( findPos( x ) );
    }

    void makeEmpty( ) {
        currentSize = 0;
        for( auto & entry : array )
            entry.info = EMPTY;
    }

    bool insert( const HashedObj & x )  {
            // Insert x as active
        int currentPos = findPos( x );
        if( isActive( currentPos ) )
            return false;

        if( array[ currentPos ].info != DELETED )
            ++currentSize;

        array[ currentPos ].element = x;
        array[ currentPos ].info = ACTIVE;

            // Rehash; see Section 5.5
        if( currentSize > array.size( ) / 2 )
            rehash( );

        return true;
    }
    
    bool insert( HashedObj && x ) {
            // Insert x as active
        int currentPos = findPos( x );
        if( isActive( currentPos ) )
        
            return false;

        if( array[ currentPos ].info != DELETED )
            ++currentSize;

        array[ currentPos ] = std::move( x );
        array[ currentPos ].info = ACTIVE;

            // Rehash; see Section 5.5
        if( currentSize > array.size( ) / 2 )
            rehash( );

        return true;
    }
    
    int coll(){ return collisionLinearProbHT ; }
    
    bool remove( const HashedObj & x ) {
        int currentPos = findPos( x );
        if( !isActive( currentPos ) )
            return false;

        array[ currentPos ].info = DELETED;
        return true;
    }

    enum EntryType { ACTIVE, EMPTY, DELETED };

  private:
    struct HashEntry
    {
        HashedObj element;
        EntryType info;

        HashEntry( const HashedObj & e = HashedObj{ }, EntryType i = EMPTY )
          : element{ e }, info{ i } { }
        
        HashEntry( HashedObj && e, EntryType i = EMPTY )
          : element{ std::move( e ) }, info{ i } { }
    };
    
    vector<HashEntry> array;
    int currentSize;
    int collisionLinearProbHT = 0;

    bool isActive( int currentPos ) const
      { return array[ currentPos ].info == ACTIVE; }

    int findPos( const HashedObj & x ) 
    {
        int offset = 1;
        int currentPos = myhash( x );

        while( array[ currentPos ].info != EMPTY &&
               array[ currentPos ].element != x )
        {
            currentPos += offset;  // Compute ith probe
            
            if( currentPos >= array.size( ) )
                currentPos -= array.size( );
            collisionLinearProbHT++;
        }
        
        return currentPos;
    }

    void rehash( )
    {
        vector<HashEntry> oldArray = array;

            // Create new double-sized, empty table
        array.resize( nextPrime( 2 * oldArray.size( ) ) );
        for( auto & entry : array )
            entry.info = EMPTY;

            // Copy table over
        currentSize = 0;
        for( auto & entry : oldArray )
            if( entry.info == ACTIVE )
                insert( std::move( entry.element ) );
    }

    size_t myhash( const HashedObj & x ) const
    {
        static hash<HashedObj> hf;
        return hf( x ) % array.size( );
    }
};

#endif