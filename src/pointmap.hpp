// -*- C++ -*-

/**
 *  @file   pointmap.hpp
 *  @brief
 *
 *  - 2009-02-26 Sylvain Bougerel <sylvain.bougerel.devel@gmail.com>
 *    Creation of the file.
 *
 *  - (next change goes here)
 */

#ifndef SPATIAL_POINTMAP_HPP
#define SPATIAL_POINTMAP_HPP

#include "bits/spatial.hpp"
#include "bits/spatial_relaxed_kdtree.hpp"

namespace spatial
{

  template<dimension_type Rank, typename Key, typename Mapped,
           typename Compare = bracket_less<Key>,
           typename BalancingPolicy = loose_balancing,
           typename Alloc = std::allocator<std::pair<Key, Mapped> > >
  struct pointmap
    : details::Relaxed_kdtree<details::Static_rank<Rank>, Key, Mapped, Compare,
                              BalancingPolicy, Alloc, false>
  {
  private:
    typedef
    details::Relaxed_kdtree<details::Static_rank<Rank>, Key, Mapped, Compare,
                            BalancingPolicy, Alloc, false>   base_type;
    typedef pointmap<Rank, Key, Mapped, Compare, BalancingPolicy,
		     Alloc>                                  Self;

  public:
    pointmap() { }

    explicit pointmap(const Compare& compare)
      : base_type(details::Static_rank<Rank>())
    { }

    pointmap(const Compare& compare, const BalancingPolicy& balancing)
      : base_type(details::Static_rank<Rank>(), compare, balancing)
    { }

    pointmap(const Compare& compare, const BalancingPolicy& balancing,
             const Alloc& alloc)
      : base_type(details::Static_rank<Rank>(), compare, balancing, alloc)
    { }

    pointmap(const pointmap& other)
      : base_type(other)
    { }

    pointmap&
    operator=(const pointmap& other)
    { return static_cast<Self&>(base_type::operator=(other)); }
  };

  /**
   *  Specialization for @ref pointmap with runtime rank support. The rank of
   *  the @ref pointmap can be determined at run time and does not need to be
   *  fixed at compile time. Using:
   *  @code
   *    struct point { ... };
   *    struct mapped { ... };
   *    pointmap<0, point> my_set;
   *  @endcode
   *  ...is therefore completely equivalent to:
   *  @code
   *    struct point { ... };
   *    struct mapped { ... };
   *    runtime_pointmap<point> my_set;
   *  @endcode
   *
   *  @see runtime_pointmap for more information about how to use this
   *  container.
   */
  template<typename Key, typename Mapped, typename Compare,
	   typename BalancingPolicy, typename Alloc>
  struct pointmap<0, Key, Mapped, Compare, BalancingPolicy, Alloc>
    : details::Relaxed_kdtree<details::Dynamic_rank, Key, Mapped, Compare,
                              BalancingPolicy, Alloc, false>
  {
  private:
    typedef
    details::Relaxed_kdtree<details::Dynamic_rank, Key, Mapped, Compare,
			    BalancingPolicy, Alloc, false> base_type;
    typedef pointmap<0, Key, Mapped, Compare, BalancingPolicy,
		     Alloc>                                Self;

  public:
    pointmap() { }

    explicit pointmap(dimension_type dim)
      : base_type(details::Dynamic_rank(dim))
    { except::check_rank_argument(dim); }

    pointmap(dimension_type dim, const Compare& compare)
      : base_type(details::Dynamic_rank(dim), compare)
    { except::check_rank_argument(dim); }

    pointmap(dimension_type dim, const Compare& compare,
                     const BalancingPolicy& policy)
      : base_type(details::Dynamic_rank(dim), compare, policy)
    { except::check_rank_argument(dim); }

    pointmap(dimension_type dim, const Compare& compare,
                     const BalancingPolicy& policy, const Alloc& alloc)
      : base_type(details::Dynamic_rank(dim), compare, policy, alloc)
    { except::check_rank_argument(dim); }

    explicit pointmap(const Compare& compare)
      : base_type(details::Dynamic_rank(), compare)
    { }

    pointmap(const Compare& compare, const BalancingPolicy& policy)
      : base_type(details::Dynamic_rank(), compare, policy)
    { }

    pointmap(const Compare& compare, const BalancingPolicy& policy,
                     const Alloc& alloc)
      : base_type(details::Dynamic_rank(), compare, policy, alloc)
    { }

    pointmap(const pointmap& other)
      : base_type(other)
    { }

    pointmap&
    operator=(const pointmap& other)
    { return static_cast<Self&>(base_type::operator=(other)); }
  };

  /**
   *  @brief  pointmap with runtime rank support. The rank of the pointmap can
   *  be determined at run time and does not need to be fixed at compile time.
   */
  template<typename Key, typename Mapped,
           typename Compare = bracket_less<Key>,
           typename BalancingPolicy = loose_balancing,
           typename Alloc = std::allocator<std::pair<Key, Mapped> > >
  struct runtime_pointmap
    : details::Relaxed_kdtree<details::Dynamic_rank, Key, Mapped, Compare,
                              BalancingPolicy, Alloc, false>
  {
  private:
    typedef
    details::Relaxed_kdtree<details::Dynamic_rank, Key, Mapped, Compare,
			    BalancingPolicy, Alloc, false>  base_type;
    typedef runtime_pointmap<Key, Mapped, Compare, BalancingPolicy,
			     Alloc>                         Self;

  public:
    runtime_pointmap() { }

    explicit runtime_pointmap(dimension_type dim)
      : base_type(details::Dynamic_rank(dim))
    { except::check_rank_argument(dim); }

    runtime_pointmap(dimension_type dim, const Compare& compare)
      : base_type(details::Dynamic_rank(dim), compare)
    { except::check_rank_argument(dim); }

    runtime_pointmap(dimension_type dim, const Compare& compare,
                     const BalancingPolicy& policy)
      : base_type(details::Dynamic_rank(dim), compare, policy)
    { except::check_rank_argument(dim); }

    runtime_pointmap(dimension_type dim, const Compare& compare,
                     const BalancingPolicy& policy, const Alloc& alloc)
      : base_type(details::Dynamic_rank(dim), compare, policy, alloc)
    { except::check_rank_argument(dim); }

    explicit runtime_pointmap(const Compare& compare)
      : base_type(details::Dynamic_rank(), compare)
    { }

    runtime_pointmap(const Compare& compare, const BalancingPolicy& policy)
      : base_type(details::Dynamic_rank(), compare, policy)
    { }

    runtime_pointmap(const Compare& compare, const BalancingPolicy& policy,
                     const Alloc& alloc)
      : base_type(details::Dynamic_rank(), compare, policy, alloc)
    { }

    runtime_pointmap(const runtime_pointmap& other)
      : base_type(other)
    { }

    runtime_pointmap&
    operator=(const runtime_pointmap& other)
    { return static_cast<Self&>(base_type::operator=(other)); }
  };

}

#endif // SPATIAL_POINTMAP_HPP
