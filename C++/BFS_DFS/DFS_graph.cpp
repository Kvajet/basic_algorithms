/**
 * DFS algorithm implemented for graph based structures
 * defined by list, f.e. std::list or {} initializer.
 * Implemented by iteration, not by recursion.
 * 
 * Example of possible usage:
 *      std::list< std::pair< int , int > > list1 = { { 0 , 1 } , { 1 , 2 } , { 2 , 3 } };
 *      DFS_graph< int > g1( list1 );
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
 * DFS_graph.cpp requires CAlgorithmInterface.cpp and TNode.cpp
 * to run properly. (You can implement your own, as you wish.)
 * 
 * @author: Jan Tichý
 * @email:  kvajet@gmail.com
 * @date:   23.10.2020
 */

#include "CAlgorithmInterface.cpp"
#include <list>

template < typename T >
class DFS_graph : public CAlgorithmInterface< T >
{
public:
    DFS_graph( const std::list< std::pair< T , T > > & edges );
    
    bool Process();

private:
};

template < typename T >
DFS_graph< T >::DFS_graph( const std::list< std::pair< T , T > > & edges )
    : CAlgorithmInterface< T >( edges )
{
}
    
template < typename T >
bool DFS_graph< T >::Process()
{
    TNode< T > * current = *( std::lower_bound( this->m_nodes.begin() , this->m_nodes.end() , TNode< T >( this->m_start ) , this->Comp ) );
    TNode< T > * nextPos = nullptr, * nextHelper = nullptr;

    for( auto & it : this->m_nodes )
        it->Reset();

    bool someNotFound = false;
    bool found = false;

    while( current != nullptr )
    {
        someNotFound = false;

        if( *current == this->m_end )
        {
            found = true;
            break;
        }

        nextHelper = current->Next();
        if( nextHelper != nullptr )
        {
            nextPos = nextHelper;
            someNotFound = true;
        }

        if( ! someNotFound && current->GetParent() == nullptr )
            break;

        if( ! someNotFound && current->GetParent() )
        {
            current->Close();
            current = current->GetParent();
            continue;
        }

        nextPos->Parent( current );
        current->Open();
        current = nextPos;
    }

    if( found )
    {
        this->ReconstructPath();
        return true;
    }

    return false;
}
