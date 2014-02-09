// -*- C++ -*-
//
// Copyright Sylvain Bougerel 2009 - 2013.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file COPYING or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/**
 *  \file   spatial_equal.hpp
 *  Contains the definition of the equal iterators. These iterators
 *  walk through all items in the container that are equal to a key given in
 *  parameter of the iterator.
 */

#ifndef SPATIAL_EQUAL_HPP
#define SPATIAL_EQUAL_HPP

#include "spatial_bidirectional.hpp"
#include "../traits.hpp"
#include "spatial_rank.hpp"
#include "spatial_except.hpp"
#include "spatial_compress.hpp"
#include "spatial_assign.hpp"
#include "spatial_preorder.hpp"

namespace spatial
{
  namespace details
  {
    template <typename Container>
    struct equal_key : Container::key_compare
    {
      equal_key() { }

      equal_key(const typename Container::key_compare& cmp,
                const typename Container::key_type& model_)
        : Container::key_compare(cmp), model(model_) { }

      typename Container::key_compare comp() const
      { return static_cast<typename Container::key_compare>(*this); }

      typename Container::key_type model;
    };

    template <typename Container>
    inline bool
    right_traversal(typename Container::mode_type::const_node_ptr node,
                    dimension_type dim,
                    typename Container::rank_type,
                    const equal_key<Container>& equal)
    {
      return !equal.comp()(dim, equal.model, const_key(node));
    }

    /**
     *  Return a boolean indicating whether all of \c x's coordinates are
     *  equal to \c y's coordinates.
     *
     *  The key \c x and \c y are tested across all dimesions using the
     *  comparator \c cmp provided by a container.
     *  \tparam Rank Either \static_rank or \dynamic_rank.
     *  \tparam Key A key type defined in the container as the \c Compare.
     *  \tparam Compare A \trivial_compare type defined in the same
     *  container as \c Key.
     *  \param rank The magnitude of the rank.
     *  \param cmp The comparator used to find equality between the \c x and \c
     *  y coordinates.
     *  \param x The key \c x.
     *  \param y The key \c y.
     */
    template <typename Container>
    inline bool
    stop_traversal(typename Container::mode_type::const_node_ptr node,
                    typename Container::rank_type rank,
                    const equal_key<Container>& equal)
    {
      dimension_type i = 0;
      for (; i < rank()
             && !equal.comp()(i, const_key(node), equal.model)
             && !equal.comp()(i, equal.model, const_key(node));
           ++i) { }
      return (i == rank());
    }

    template <typename Container>
    inline bool
    left_traversal(typename Container::mode_type::const_node_ptr node,
                   dimension_type dim,
                   const equal_key<Container>& equal,
                   relaxed_invariant_tag)
    {
      return !equal.comp()(dim, const_key(node), equal.model);
    }

    template <typename Container>
    inline bool
    left_traversal(typename Container::mode_type::const_node_ptr node,
                   dimension_type dim,
                   const equal_key<Container>& equal,
                   strict_invariant_tag)
    {
      return equal.comp()(dim, equal.model, const_key(node));
    }

    template <typename Container>
    inline bool
    left_traversal(typename Container::mode_type::const_node_ptr node,
                   dimension_type dim,
                   typename Container::rank_type,
                   const equal_key<Container>& equal)
    {
      return left_traversal
        (node, dim, equal,
         typename Container::mode_type::invariant_category());
    }

  } // namespace details

  /**
   *  This type provides an iterator to iterate through all elements of a
   *  container that match a given key, passed as a parameter to the
   *  constructor. The given key is called the model.
   *
   *  \tparam Container The container upon which these iterator relate to.
   *  \headerfile equal_iterator.hpp
   */
  template <typename Container>
  class equal_iterator
    : public details::Bidirectional_iterator
      <typename container_traits<Container>::mode_type,
       typename container_traits<Container>::rank_type>
  {
    //! The preorder iterator without its criterion
    typedef typename details::Bidirectional_iterator
    <typename container_traits<Container>::mode_type,
     typename container_traits<Container>::rank_type> Base;

  public:
    using Base::node;
    using Base::node_dim;
    using Base::rank;

    //! The type used to store the model key to be looked up in the container.
    typedef typename container_traits<Container>::key_type key_type;

    //! The comparison functor used to compare keys.
    typedef typename container_traits<Container>::key_compare key_compare;

    //! \defctor
    equal_iterator() { }

    /**
     *  Build an equal iterator from a container's iterator type.
     *
     *  This constructor should be used in the general case where the dimension
     *  for the node pointed to by \c iter is not known. The dimension of the
     *  node will be recomputed from the given iterator by iterating through all
     *  parents until the header node has been reached. This iteration is
     *  bounded by \Olog when the container is perfectly balanced.
     *
     *  \param container The container being iterated.
     *  \param model_ The key to look for.
     *  \param iter An iterator on the type Ct.
     */
    equal_iterator(Container& container, const key_type& model_,
                   typename container_traits<Container>::iterator iter)
      : Base(container.rank(), iter.node, modulo(iter.node, container.rank())),
        _query(container.key_comp(), model_) { }

    /**
     *  Build an equal iterator from the node and current dimension of a
     *  container's element.
     *
     *  This constructor should be used only when the dimension of the node
     *  pointed to by iter is known. If in doubt, use the other
     *  constructor. This constructor perform slightly faster than the other,
     *  since the dimension does not have to be calculated. Note however that
     *  the calculation of the dimension in the other iterator takes slightly
     *  longer than \Olog in general, and so it is not likely to affect the
     *  performance of your application in any major way.
     *
     *  \param container The container being iterated.
     *  \param model_ The key to look for.
     *  \param ptr An iterator on the type Ct.
     *  \param dim The node's dimension for the node pointed to by node.
     *  \param container The container being iterated.
     */
    equal_iterator
    (Container& container, const key_type& model_, dimension_type dim,
     typename container_traits<Container>::mode_type::node_ptr ptr)
      : Base(container.rank(), ptr, dim), _query(container.key_comp(), model_)
    { }

    //! Increments the iterator and returns the incremented value. Prefer to
    //! use this form in \c for loops.
    equal_iterator<Container>& operator++()
    {
      details::assign(node, node_dim,
                      preorder_increment(node, node_dim, rank(), _query));
      return *this;
    }

    //! Increments the iterator but returns the value of the iterator before
    //! the increment. Prefer to use the other form in \c for loops.
    equal_iterator<Container> operator++(int)
    {
      equal_iterator<Container> x(*this);
      details::assign(node, node_dim,
                      preorder_increment(node, node_dim, rank(), _query));
      return x;
    }

    //! Decrements the iterator and returns the decremented value. Prefer to
    //! use this form in \c for loops.
    equal_iterator<Container>& operator--()
    {
      details::assign(node, node_dim,
                      preorder_decrement(node, node_dim, rank(), _query));
      return *this;
    }

    //! Decrements the iterator but returns the value of the iterator before
    //! the decrement. Prefer to use the other form in \c for loops.
    equal_iterator<Container> operator--(int)
    {
      equal_iterator<Container> x(*this);
      details::assign(node, node_dim,
                      preorder_decrement(node, node_dim, rank(), _query));
      return x;
    }

    //! Return the value of the model key used to find equal keys in the
    //! container.
    key_type model() const { return _query.model; }

    //! Return the functor used to compare keys in this iterator.
    key_compare key_comp() const { return _query.comp(); }

  private:
    //! The model key used to find equal keys in the container.
    details::equal_key<Container> _query;
  };

  /**
   *  This type provides an iterator to iterate through all elements of a
   *  container that match a given key, passed as a parameter to the
   *  constructor. The given key is called the model.
   *
   *  The values returned by this iterator will not be mutable.
   *
   *  \tparam Ct The container upon which these iterator relate to.
   */
  template <typename Container>
  class equal_iterator<const Container>
    : public details::Const_bidirectional_iterator
      <typename container_traits<Container>::mode_type,
       typename container_traits<Container>::rank_type>
  {
    //! The preorder iterator without its criterion.
    typedef details::Const_bidirectional_iterator
    <typename container_traits<Container>::mode_type,
     typename container_traits<Container>::rank_type> Base;

  public:
    using Base::node;
    using Base::node_dim;
    using Base::rank;

    //! The type used to store the model key to be looked up in the container.
    typedef typename container_traits<Container>::key_type key_type;

    //! The comparison functor used to compare keys.
    typedef typename container_traits<Container>::key_compare key_compare;

    //! \defctor
    equal_iterator() { }

    /**
     *  Build an equal iterator from a container's iterator type.
     *
     *  This constructor should be used in the general case where the dimension
     *  for the node pointed to by \c iter is not known. The dimension of the
     *  node will be recomputed from the given iterator by iterating through all
     *  parents until the header node has been reached. This iteration is
     *  bounded by \Olog when the container is perfectly balanced.
     *
     *  \param container The container being iterated.
     *  \param model_ The key to look for.
     *  \param iter An iterator from the container.
     */
    equal_iterator(const Container& container, const key_type& model_,
                   typename container_traits<Container>::const_iterator iter)
      : Base(container.rank(), iter.node, modulo(iter.node, container.rank())),
        _query(container.key_comp(), model_) { }

    /**
     *  Build an equal iterator from the node and current dimension of a
     *  container's element.
     *
     *  This constructor should be used only when the dimension of the node
     *  pointed to by iter is known. If in doubt, use the other
     *  constructor. This constructor perform slightly faster since the
     *  dimension does not have to be calculated.
     *
     *  \param container The container being iterated.
     *  \param model_ The key to look for.
     *  \param dim The dimension associated with \c ptr when checking the
     *  invariant in \c container.
     *  \param ptr A pointer to a node belonging to \c container.
     */
    equal_iterator
    (const Container& container, const key_type& model_, dimension_type dim,
     typename container_traits<Container>::mode_type::const_node_ptr ptr)
      : Base(container.rank(), ptr, dim), _query(container.key_comp(), model_)
    { }

    //! Convertion of an iterator into a const_iterator is permitted.
    equal_iterator(const equal_iterator<Container>& iter)
      : Base(iter.rank(), iter.node, iter.node_dim),
        _query(iter.key_comp(), iter.model()) { }

    //! Increments the iterator and returns the incremented value. Prefer to
    //! use this form in \c for loops.
    equal_iterator<const Container>& operator++()
    {
      details::assign(node, node_dim,
                      preorder_increment(node, node_dim, rank(), _query));
      return *this;
    }

    //! Increments the iterator but returns the value of the iterator before
    //! the increment. Prefer to use the other form in \c for loops.
    equal_iterator<const Container> operator++(int)
    {
      equal_iterator<const Container> x(*this);
      details::assign(node, node_dim,
                      preorder_increment(node, node_dim, rank(), _query));
      return x;
    }

    //! Decrements the iterator and returns the decremented value. Prefer to
    //! use this form in \c for loops.
    equal_iterator<const Container>& operator--()
    {
      details::assign(node, node_dim,
                      preorder_decrement(node, node_dim, rank(), _query));
      return *this;
    }

    //! Decrements the iterator but returns the value of the iterator before
    //! the decrement. Prefer to use the other form in \c for loops.
    equal_iterator<const Container> operator--(int)
    {
      equal_iterator<const Container> x(*this);
      details::assign(node, node_dim,
                      preorder_decrement(node, node_dim, rank(), _query));
      return x;
    }

    //! Returns the value of the model key used to find equivalent keys in the
    //! container.
    key_type model() const { return _query.model; }

    //! Returns the functor used to compare keys in this iterator.
    key_compare key_comp() const { return _query.comp(); }

  private:
    //! The model key used to find equal keys in the container.
    details::equal_key<Container> _query;
  };

  template <typename Container>
  inline equal_iterator<Container>
  equal_end(Container& container,
            const typename equal_iterator<Container>::key_type& model)
  {
    return equal_iterator<Container>
      (container, model, container.dimension() - 1,
       container.end().node); // At header, dim = rank - 1
  }

  template <typename Container>
  inline equal_iterator<const Container>
  equal_cend(const Container& container,
             const typename equal_iterator<Container>::key_type& model)
  { return equal_end(container, model); }

  /**
   *  Find the first element in \c container that compares equally to \c model,
   *  using \c container's \c key_compare comparator.
   *
   *  \tparam Container The container type being iterated.
   *  \param container The container being iterated.
   *  \param model A model to find matches among other keys stored in the
   *  container.
   */
  ///@{
  template <typename Container>
  inline equal_iterator<Container>
  equal_begin(Container& container,
              const typename equal_iterator<Container>::key_type& model)
  {
    if (container.empty()) return equal_end(container, model);
    typename equal_iterator<Container>::node_ptr parent
      = container.end().node->parent;
    typename equal_iterator<Container>::node_ptr node;
    dimension_type dim;
    details::assign(node, dim,
                    preorder_minimum(parent, 0, container.rank(),
                                     details::equal_key<Container>
                                     (container.key_comp(), model)));
    return equal_iterator<Container>(container, model, dim, node);
  }

  template <typename Container>
  inline equal_iterator<const Container>
  equal_cbegin(const Container& container,
               const typename equal_iterator<Container>::key_type& model)
  { return equal_begin(container, model); }
  ///@}
} // namespace spatial

#endif // SPATIAL_EQUAL_HPP
