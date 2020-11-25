/*!
 * @file   tries.h
 * @brief  Header for home-made implementations of Tries structures
 *         Thoses structures can be used to effectively perform prefix/suffix
 *         search on strings.
 * @author lhm
 * @date   21/10/2020
 */

#include <cstdint>
#include <iostream>
#include <iterator>
#include <sstream>
#include <set>
#include <iomanip>
#include <memory>

#ifndef TRIES_H
#define TRIES_H

namespace TRIES {

/*!
 * \brief Virtual base clas for tries
 */
class BTRIE
{
    friend class TRIE_p;
    friend class TRIE_s;

protected:
    static const uint8_t           SUFFIX   = '#'  ;
    static const uint8_t           MAX_SONS = 255  ;
    int                            m_freq          ;
    std::unique_ptr<BTRIE>         m_sons[MAX_SONS];

public:
    BTRIE(): m_freq(0) {
        for ( int i = 0; i < MAX_SONS; i++ ) { m_sons[i] = nullptr; }
    }

    BTRIE(const BTRIE& p_o)
    {
        m_freq = p_o.m_freq;
        for ( int i = 0; i < MAX_SONS; i++ ) { m_sons[i] = nullptr; }
    }

    virtual ~BTRIE(){}

public:
    virtual void insert  ( std::string&& p_str ) {
        BTRIE* cur(this);
        for ( const auto& c : p_str ) {
            int d(c);
            if ( !cur->m_sons[d] ) { cur->m_sons[d] = std::make_unique<BTRIE>(BTRIE()); }
            cur = cur->m_sons[d].get();
            ++cur->m_freq;
        }
        if ( !cur->m_sons[SUFFIX] ) { cur->m_sons[SUFFIX] = std::make_unique<BTRIE>(BTRIE()); }
    }

    virtual bool contains( std::string&& p_str ) {
        const BTRIE* cur(this);

        for ( auto& c : p_str ) {
            int d(c);
            if ( !cur->m_sons[d] ) { return false; }
            cur = cur->m_sons[d].get();
        }
        return cur->m_sons[SUFFIX].get();
    }
};

/*!
 * \brief Prefix tree
 */
class TRIE_p : public BTRIE
{
public:
    TRIE_p() { }
    virtual ~TRIE_p() { }

public:
    std::string prefix( std::string&& p_str ) const {
        const BTRIE* cur(this);
        std::string l_res("");
        for ( const auto& c : p_str ) {
            int d(c);
            if ( !cur->m_sons[d] ) { return l_res; }
            l_res += c;
            if (  cur->m_sons[d]->m_freq == 1 ) { return l_res; }
            cur = cur->m_sons[d].get();
        }
        return l_res;
    }
};

/*!
 * \brief Suffix tree
 */
class TRIE_s : public BTRIE
{
public:
    TRIE_s() { }
    virtual ~TRIE_s() { }

public:
    virtual void insert( std::string&& p_str ) {
        uint8_t c;
        for ( std::size_t i = 0; i < p_str.size(); i++ )
        {
            BTRIE* cur = this;
            for (  std::size_t j = i; j < p_str.size(); j++ )
            {
                c = p_str[j];
                if ( cur->m_sons[c] ) { cur->m_sons[c] = std::make_unique<BTRIE>(BTRIE()); }
                cur = cur->m_sons[c].get();
            }
            cur->m_sons[SUFFIX] = nullptr;
        }
    }
};

} // Namespace TRIES

#endif // TRIES_H
