// -*- C++ -*-

/**
 *  @file   frozen_pointset.hpp
 *  @brief  
 *
 *  - 2009-02-26 Sylvain Bougerel <sylvain.bougerel.devel@gmail.com>
 *    Creation of the file.
 *
 *  - (next change goes here)
 */

#ifndef SPATIAL_FROZEN_POINTSET_HPP
#define SPATIAL_FROZEN_POINTSET_HPP

#include "bits/spatial.hpp"
#include "bits/spatial_set_macro.hpp"
#include "bits/spatial_traits.hpp"
#include "bits/spatial_exceptions.hpp"
#include "bits/spatial_assert.hpp"
#include "bits/spatial_details.hpp"
#include "bits/spatial_function.hpp"
#include "bits/spatial_node.hpp"
#include "bits/spatial_mapping.hpp"
#include "bits/spatial_range.hpp"
#include "bits/spatial_geometry.hpp"
#include "bits/spatial_neighbor.hpp"
#include "bits/spatial_kdtree_base.hpp"
#include "bits/spatial_kdtree.hpp"
#include "bits/spatial_relaxed_kdtree.hpp"
#include "bits/spatial_node.tpp"
#include "bits/spatial_mapping.tpp"
#include "bits/spatial_range.tpp"
#include "bits/spatial_neighbor.tpp"
#include "bits/spatial_kdtree_base.tpp"
#include "bits/spatial_kdtree.tpp"
#include "bits/spatial_relaxed_kdtree.tpp"
#include "bits/spatial_unset_macro.hpp"

namespace spatial
{
  
  template<dimension_type Rank, typename Key,
	   typename Compare = bracket_less<Key>,
	   typename Alloc = std::allocator<Key> >
  struct frozen_pointset
    : details::Kdtree<details::Static_rank<Rank>, Key, Compare, Alloc, true>
  {
  private:
    typedef details::Kdtree<details::Static_rank<Rank>, Key, Compare,
			    Alloc, true>               base_type;
    typedef frozen_pointset<Rank, Key, Compare, Alloc> Self;

  public:

    frozen_pointset() { }

    explicit frozen_pointset(const Compare& compare)
      : base_type(details::Static_rank<Rank>())
    { }

    frozen_pointset(const Compare& compare, const Alloc& alloc)
      : base_type(details::Static_rank<Rank>(), compare, alloc)
    { }

    frozen_pointset(const frozen_pointset& other, bool balancing = false)
      : base_type(other, balancing)
    { }

    frozen_pointset&
    operator=(const frozen_pointset& other)
    {
      return static_cast<frozen_pointset<Rank, Key, Compare, Alloc>&>
	(base_type::operator=(other));
    }
  };

  /**
   *  @brief  This specialization allows one to specify the dimension for the
   *  point set at run time.
   */
  template<typename Key, typename Compare, typename Alloc>
  struct frozen_pointset<0, Key, Compare, Alloc>
    : details::Kdtree<details::Dynamic_rank, Key, Compare, Alloc, true>
  {
  private:
    typedef details::Kdtree<details::Dynamic_rank,
			    Key, Compare, Alloc, true> base_type;
    typedef frozen_pointset<0, Key, Compare, Alloc>    Self;

  public:
    frozen_pointset() { }

    explicit frozen_pointset(dimension_type dim)
      : base_type(details::Dynamic_rank(dim))
    { }

    frozen_pointset(dimension_type dim, const Compare& compare)
      : base_type(details::Dynamic_rank(dim), compare)
    { }

    explicit frozen_pointset(const Compare& compare)
      : base_type(compare)
    { }

    frozen_pointset(dimension_type dim, const Compare& compare,
		    const Alloc& alloc)
      : base_type(details::Dynamic_rank(dim), compare, alloc)
    { }

    frozen_pointset(const Compare& compare, const Alloc& alloc)
      : base_type(details::Dynamic_rank(), compare, alloc)
    { }

    frozen_pointset(const frozen_pointset& other, bool balancing = false)
      : base_type(other, balancing)
    { }

    frozen_pointset&
    operator=(const frozen_pointset& other)
    {
      return static_cast<frozen_pointset<0, Key, Compare, Alloc>&>
	(base_type::operator=(other));
    }
  };

  /**
   *  @brief  pointset with dynamic rank support. The rank of the pointset can
   *  be determined at run time and does not need to be fixed at compile time.
   */
  template<typename Key,
	   typename Compare = bracket_less<Key>,
	   typename Alloc = std::allocator<Key> >
  struct frozen_runtime_pointset
    : details::Kdtree<details::Dynamic_rank, Key, Compare, Alloc, true>
  {
  private:
    typedef details::Kdtree<details::Dynamic_rank,
			    Key, Compare, Alloc, true>     base_type;
    typedef frozen_runtime_pointset<Key, Compare, Alloc>   Self;

  public:
    frozen_runtime_pointset() { }

    explicit frozen_runtime_pointset(dimension_type dim)
      : base_type(details::Dynamic_rank(dim))
    { }

    frozen_runtime_pointset(dimension_type dim, const Compare& compare)
      : base_type(details::Dynamic_rank(dim), compare)
    { }

    explicit frozen_runtime_pointset(const Compare& compare)
      : base_type(compare)
    { }

    frozen_runtime_pointset(dimension_type dim, const Compare& compare,
			    const Alloc& alloc)
      : base_type(details::Dynamic_rank(dim), compare, alloc)
    { }

    frozen_runtime_pointset(const Compare& compare, const Alloc& alloc)
      : base_type(details::Dynamic_rank(), compare, alloc)
    { }

    frozen_runtime_pointset(const frozen_runtime_pointset& other,
			    bool balancing = false)
      : base_type(other, balancing)
    { }

    frozen_runtime_pointset&
    operator=(const frozen_runtime_pointset& other)
    {
      return static_cast<frozen_runtime_pointset<Key, Compare, Alloc>&>
	(base_type::operator=(other));
    }
  };

}

#endif // SPATIAL_FROZEN_POINTSET_HPP
