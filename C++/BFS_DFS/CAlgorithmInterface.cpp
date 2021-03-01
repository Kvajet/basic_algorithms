/**
 * CLagorithmInterface is inteface class for pathfinding algorithms,
 * g. e. DFS_graph.cpp and BFS_graph.cpp.
 * 
 * @author: Jan Tichý
 * @email:  kvajet@gmail.com
 * @date:   23.10.2020
 */

#include "TNode.cpp"
#include <list>
#include <vector>
#include <algorithm>
#include <ostream>
#include <functional>
#include <queue>

template < typename T >
class CAlgorithmInterface
{
public:
    CAlgorithmInterface( const std::list< std::pair< T , T > > & edges );
    ~CAlgorithmInterface();
    
    void AddEdge( const std::pair< T , T > & edge );
    void SetStart( const T & content );
    void SetEnd  ( const T & content );

    const std::vector< TNode< T > * > & GetNodes() const;
    
    virtual bool Process() = 0;
    void PrintResult() const;
    const std::list< TNode< T > * > & GetResult() const;

public:
    void AddNode( const T & content );
    void ReconstructPath();

    std::vector< TNode< T > * > m_nodes;
    std::list< TNode< T > * > m_resList;
    T m_start, m_end;
    std::function< bool( TNode< T > * , const TNode< T > & ) > Comp = []( TNode< T > * lhs , const TNode< T > & rhs ) -> bool { return *lhs < rhs ; };
};

template < typename T >
CAlgorithmInterface< T >::CAlgorithmInterface( const std::list< std::pair< T , T > > & edges )
{
    for( const auto & it : edges )
        if( ! ( it.first == it.second ) )
            AddEdge( it );
}

template < typename T >
CAlgorithmInterface< T >::~CAlgorithmInterface()
{
    for( auto & it : m_nodes )
        delete it;
}

template < typename T >
void CAlgorithmInterface< T >::AddEdge( const std::pair< T , T > & edge )
{
    AddNode( edge.first  );
    AddNode( edge.second );

    auto node1 = std::lower_bound( m_nodes.begin() , m_nodes.end() , TNode< T >( edge.first ) , Comp );
    auto node2 = std::lower_bound( m_nodes.begin() , m_nodes.end() , TNode< T >( edge.second ) , Comp );

    if( node2 != m_nodes.end() )
        (*node1)->AddClose( *node2 );
    if( node1 != m_nodes.end() )
        (*node2)->AddClose( *node1 );
}

template < typename T >
void CAlgorithmInterface< T >::SetStart( const T & content )
{
    m_start = content;
}

template < typename T >
void CAlgorithmInterface< T >::SetEnd( const T & content )
{
    m_end = content;
}

template < typename T >
void CAlgorithmInterface< T >::PrintResult() const
{
    std::cout << "Path length: " << m_resList.size() << "\n";
    if( m_resList.size() > 0 )
    {
        std::cout << "Path: ";
        for( const auto & it : m_resList )
            if( &it != &m_resList.back() )
                std::cout << it->GetContent() << " ";
        std::cout << m_resList.back()->GetContent() << "\n";
    }
}

template < typename T >
const std::vector< TNode< T > * > & CAlgorithmInterface< T >::GetNodes() const
{
    return m_nodes;
}

template < typename T >
std::ostream & operator << ( std::ostream & os , const CAlgorithmInterface< T > & obj )
{
    for( const auto & it : obj.GetNodes() )
        os << it;
    return os;
}

template < typename T >
void CAlgorithmInterface< T >::AddNode( const T & content )
{
    auto it = std::lower_bound( m_nodes.begin() , m_nodes.end() , TNode< T >( content ) , Comp );

    if( it == m_nodes.end() )
        m_nodes.push_back( new TNode< T >( content ) );
    else if( **it == TNode< T >( content ) )
        return;
    else
        m_nodes.emplace( it , new TNode< T >( content ) );
}

template < typename T >
void CAlgorithmInterface< T >::ReconstructPath()
{
    m_resList.clear();

    TNode< T > * current = *( std::lower_bound( m_nodes.begin() , m_nodes.end() , TNode< T >( m_end ) , Comp ) );
    while( current != nullptr )
    {
        m_resList.push_front( current );
        current = current->GetParent();
    }
}

template < typename T >
const std::list< TNode< T > * > & CAlgorithmInterface< T >::GetResult() const
{
    return m_resList;
}
