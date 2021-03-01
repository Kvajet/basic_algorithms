/**
 * Node structure for DFS_graph and BFS_graph implementation.
 * @author: Jan Tichý
 * @email:  kvajet@gmail.com
 * @date:   23.10.2020
 */
#include <vector>
#include <ostream>
#include <algorithm>
#include <functional>

template< typename T >
class TNode
{
public:
    TNode( const T & elem );

    enum State : char {
        NOT_FOUND = 0, OPEN, CLOSED
    };

    TNode::State GetState() const;
    const T & GetContent() const;

    void NotFound();
    void Open();
    void Close();
    void Parent( TNode< T > * parent );
    void Reset();

    void AddClose( TNode< T > * next );
    const std::vector< TNode< T > * > & GetClose() const;
    TNode< T > * Next();
    TNode< T > * GetParent() const;

    bool operator < ( const TNode< T > & rhs ) const;
    bool operator == ( const TNode< T > & rhs ) const;

private:
    T m_content;
    size_t m_closeIndex = 0;
    TNode::State m_state = TNode::State::NOT_FOUND;
    std::vector< TNode< T > * > m_close;
    TNode< T > * m_prev = nullptr;
    std::function< bool( TNode< T > * , TNode< T > * ) > Comp = []( TNode< T > * lhs , TNode< T > * rhs ) -> bool { return *lhs < *rhs ; };
};

template < typename T >
TNode< T >::TNode( const T & elem )
    : m_content( elem ) , m_close( std::vector< TNode< T > * >() )
{
}

template < typename T >
typename TNode< T >::State TNode< T >::GetState() const
{
    return m_state;
}

template < typename T >
const T & TNode< T >::GetContent() const
{
    return m_content;
}

template < typename T >
void TNode< T >::NotFound()
{
    m_state = TNode::State::NOT_FOUND;
}

template < typename T >
void TNode< T >::Open()
{
    m_state = TNode::State::OPEN;
}

template < typename T >
void TNode< T >::Close()
{
    m_state = TNode::State::CLOSED;
}

template < typename T >
void TNode< T >::Parent( TNode< T > * parent )
{
    m_prev = parent;
}

template < typename T >
void TNode< T >::Reset()
{
    m_state = State::NOT_FOUND;
    m_closeIndex = 0;
    m_prev = nullptr;
}

template < typename T >
void TNode< T >::AddClose( TNode< T > * next )
{
    auto it = std::lower_bound( m_close.begin() , m_close.end() , next , Comp );

    if( it == m_close.end() )
        m_close.push_back( next );
    else if( **it == *next )
        return;
    else
        m_close.emplace( it , next );
}

template < typename T >
const std::vector< TNode< T > * > & TNode< T >::GetClose() const
{
    return m_close;
}

template < typename T >
TNode< T > * TNode< T >::Next()
{
    if( ! ( m_close.size() - m_closeIndex ) )
        return nullptr;
    for( auto & it : m_close )
        if( it->GetState() != State::NOT_FOUND )
            m_closeIndex++;
        else
            break;
    if( ! ( m_close.size() - m_closeIndex ) )
        return nullptr;
    return m_close[ m_closeIndex++ ];
}

template < typename T >
TNode< T > * TNode< T >::GetParent() const
{
    return m_prev;
}

template < typename T >
bool TNode< T >::operator < ( const TNode< T > & rhs ) const
{
    return this->m_content < rhs.GetContent();
}

template < typename T >
bool TNode< T >::operator == ( const TNode< T > & rhs ) const
{
    return this->m_content == rhs.GetContent();
}

template < typename T >
std::ostream & operator << ( std::ostream & os , const TNode< T > * obj )
{
    std::cout << "Node: " << obj->GetContent() << "\n";
    for( auto & it : obj->GetClose() )
        std::cout << "    To: " << it->GetContent() << "\n";
    return os;
}
