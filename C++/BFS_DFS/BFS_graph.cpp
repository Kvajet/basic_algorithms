/**
 * BFS algorithm implemented for graph based structures
 * defined by list, f.e. std::list or {} initializer.
 * 
 * Example of possible usage:
 *      std::list< std::pair< int , int > > list1 = { { 0 , 1 } , { 1 , 2 } , { 2 , 3 } };
 *      BFS_graph< int > g1( list1 );
 *
 *      g1.SetStart( 0 );
 *      g1.SetEnd( 3 );
 *      g1.Process();
 *      g1.PrintResult();
 *
 *      std::cout << g1;
 * End of example.
 * 
 * Implemented as a template so the type is supported in case
 * it supports { == , < , << } operators.
 * 
 * BFS_graph.cpp requires CAlgorithmInterface.cpp and TNode.cpp
 * to run properly. (You can implement your own, as you wish.)
 * 
 * @author: Jan Tichý
 * @email:  kvajet@gmail.com
 * @date:   23.10.2020
 */

#include "CAlgorithmInterface.cpp"
#include <list>
#include <queue>

template < typename T >
class BFS_graph : public CAlgorithmInterface< T >
{
public:
    BFS_graph( const std::list< std::pair< T , T > > & edges );
    
    bool Process() override;
    
private:
};

template < typename T >
BFS_graph< T >::BFS_graph( const std::list< std::pair< T , T > > & edges )
    : CAlgorithmInterface< T >( edges )
{
}
    
template < typename T >
bool BFS_graph< T >::Process()
{
    TNode< T > * current = *( std::lower_bound( this->m_nodes.begin() , this->m_nodes.end() , TNode< T >( this->m_start ) , this->Comp ) );
    TNode< T > * nextHelper = nullptr;

    for( auto & it : this->m_nodes )
        it->Reset();
    
    bool found = false;

    current->Open();

    std::queue< TNode< T > * > q;
    q.push( current );

    while( ! q.empty() )
    {
        current = q.front();
        q.pop();

        if( *current == this->m_end )
        {
            found = true;
            break;
        }

        while( ( nextHelper = current->Next() ) != nullptr )
        {
            nextHelper->Open();
            nextHelper->Parent( current );
            q.push( nextHelper );
        }

        current->Close();
    }

    if( found )
    {
        this->ReconstructPath();
        return true;
    }

    return false;
}
