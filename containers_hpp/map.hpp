#ifndef __MAP_HPP__
# define __MAP_HPP__

# include "containers.hpp"
# include "utils/utils.hpp"
# include "bst/bst.hpp"
# include "iterators/map_iterator.hpp"

namespace ft{

template < class Key,
			class T,
			class Compare = ft::less< Key >,
			class Alloc = std::allocator< ft::pair<const Key,T> >
			>
class map
{
	public:
		typedef Key                                             						key_type;
		typedef T                                               						mapped_type;
		typedef ft::pair< const Key, T >                        						value_type;
		typedef Compare                                         						key_compare;
		typedef Alloc                                           						allocator_type;
		typedef typename allocator_type::size_type										size_type;
		typedef typename allocator_type::reference              						reference;
		typedef typename allocator_type::const_reference        						const_reference;
		typedef typename allocator_type::pointer                						pointer;
		typedef typename allocator_type::const_pointer          						const_pointer;
		typedef typename allocator_type::difference_type								difference_type;
		typedef typename ft::MapIterator< map<Key, T, Compare, Alloc>, false  >			iterator;
		typedef typename ft::MapIterator< const map<Key, T, Compare, Alloc>, true >		const_iterator;
		typedef typename ft::reverse_iterator<iterator>									reverse_iterator;
		typedef typename ft::reverse_iterator<const_iterator>							const_reverse_iterator;
		typedef typename std::allocator< BST< value_type, key_compare > >				BST_allocator_type;
		typedef typename BST_allocator_type::pointer									BST_pointer;
	
	private:
		class value_compare
		{
			friend class map;
			protected:
				Compare comp;
				value_compare (Compare c) : comp(c) {}
			public:
				typedef bool result_type;
				typedef value_type first_argument_type;
				typedef value_type second_argument_type;
				bool operator() (const value_type& x, const value_type& y) const
				{
					return comp(x.first, y.first);
				}
		};

		key_compare							_compare;
		allocator_type						_allocker;
		size_type							_size;
		BST_pointer							_bst;
		BST_allocator_type					_bst_allocker;
		BST_pointer							_dummy_end;

	private:
		void	init_dummy( void )
		{
			_dummy_end = _bst_allocker.allocate(1);
		}

		void	destroy_dummy( void )
		{
			_bst_allocker.deallocate(_dummy_end, 1);
		}

	//KEEP ME TO PROVE THAT BINARY TREE TO CORRECTEUR
	public:
		BST_pointer getBST( void ) const {return _bst;};

	public:
		explicit map (const key_compare& comp = key_compare(),
				const allocator_type& alloc = allocator_type()) : _compare(comp), _allocker(alloc), _size(0), _bst(NULL), _dummy_end(NULL) { init_dummy(); }
		
		map (const map& x) : _compare(x._compare), _allocker(x._allocker), _size(x._size), _bst(NULL), _dummy_end(NULL) {
			init_dummy();
			if (x._bst)
				_bst = x._bst->copy_this();
		 }

		template <class InputIterator>
		map (InputIterator first, InputIterator last,
		const key_compare& comp = key_compare(),
		const allocator_type& alloc = allocator_type()) :  _compare(comp), _allocker(alloc), _size(0), _bst(NULL), _dummy_end(NULL) { 
			init_dummy();
			insert(first, last);
		}

		~map( void ) { destroy_dummy(); destroy_bst(); }

		map&	operator=(const map& x) {
			if (this != &x)
			{
				_compare = x._compare;
				destroy_bst();
				if (x._bst)
					_bst = x._bst->copy_this();
				_size = x._size;
				_bst_allocker = x._bst_allocker;
			}
			return (*this);
		}

	private:
		void destroy_bst( void ) {
			if (_bst)
			{
				_bst_allocker.destroy(_bst);
				_bst_allocker.deallocate(_bst, 1);
				_bst = NULL;
				_size = 0;
			}
		};

	public:

		ft::pair<iterator,bool> insert (const value_type& val){
			ft::pair<BST_pointer, bool>	ret;
			ft::pair<iterator, bool> 	convert;

			if (!_bst)
			{
				_bst = _bst_allocker.allocate(1);
				_bst_allocker.construct(_bst, val);
				ret.first = _bst;
				ret.second = true;
			}
			else
			{
				ret = _bst->insert(val);
				balance(_bst);
				_bst = rotate(_bst);
				_bst->parent = NULL;
			}
			
			if (ret.second)
			{
				convert.second = true;
				convert.first = iterator(ret.first, _dummy_end, 0);
				_size++;
			}
			else
			{
				convert.second = false;
				convert.first = iterator(ret.first, _dummy_end, 0);
			}
			return (convert);
		}

		template <class InputIterator>
  		void insert (InputIterator first, InputIterator last){
			for (; first != last; first++)
				insert(*first);
		}

		iterator insert (iterator position, const value_type& val)
		{
			static_cast<void> (position);
			ft::pair<iterator, bool> ret = insert(val);
			return (ret.first);
		}

		void		erase( iterator position )
		{	
			if (!_bst)
				return ;
			else
			{
				if (position.base() == _bst)
				{
					_bst = destroy(_bst);
					if (_bst)
					{
						balance(_bst);
						_bst = rotate(_bst);
						_bst->parent = NULL;
					}
					_size--;
					return ;
				}
				else
				{
					BST_pointer d_stroy = position.base();
					BST_pointer parent = d_stroy->parent;
					BST_pointer parent2 = parent->parent;

					if (parent->left == d_stroy)
						parent->left = destroy(d_stroy);
					else
						parent->right = destroy(d_stroy);
					balance(parent);
					while (parent2 != NULL)
					{
						if (parent2->left == parent)
							parent2->left = rotate(parent);
						else
							parent2->right = rotate(parent);
						balance(parent2);
						parent = parent2;
						parent2 = parent2->parent;
					}
					balance(_bst);
					_bst = rotate(_bst);
					_bst->parent = NULL;
					_bst->recursive_balancing();
					_size--;
					return ;
				}
			}
		}

		void		erase(iterator first, iterator last)
		{
			while (first != last)
				erase(first++);
		}

		size_type	erase(const key_type & k)
		{
			if (!_bst)
				return 0;
			if (_bst->elem.first == k)
			{
				_bst = destroy(_bst);
				if (_bst)
				{
					balance(_bst);
					_bst = rotate(_bst);
					_bst->parent = NULL;
				}
				_size--;
				return 1;
			}
			if (_bst->erase_elem(k))
			{
				balance(_bst);
				_bst = rotate(_bst);
				_bst->parent = NULL;
				_size--;
				return 1;
			}
			else
				return 0;
		}

	//ITERATORS//
	public:
		iterator begin( void ) {
			if (_bst)
				return (iterator(_bst->find_start(), _dummy_end, 0));
			else
				return (iterator(_dummy_end, _dummy_end, 1));
		}

		const_iterator begin() const {
			if (_bst)
				return (const_iterator(_bst->find_start(), _dummy_end, 0));
			else
				return (const_iterator(_dummy_end, _dummy_end, 1));
		};


		iterator end( void ){
			if (_bst)
				return (iterator(_bst->find_end(), _dummy_end, 1));
			else
				return (iterator(_dummy_end, _dummy_end, 1));
		}

		const_iterator end( void ) const {
			if (_bst)
				return (const_iterator(_bst->find_end(), _dummy_end, 1));
			else
				return (const_iterator(_dummy_end, _dummy_end, 1));
		}

		iterator find (const key_type& k){
			if (_bst)
			{
				BST_pointer tmp = _bst->find_by_key(k);
				if (tmp)
					return (iterator(tmp, _dummy_end, 0));
				else
					return (end());
			}
			else
				return (iterator(_dummy_end, _dummy_end, 1));	
		}

		const_iterator find (const key_type& k) const{
			if (_bst)
			{
				BST_pointer tmp = _bst->find_by_key(k);
				if (tmp)
					return (const_iterator(tmp, _dummy_end, 0));
				else
					return (end());
			}
			else
				return (const_iterator(_dummy_end, _dummy_end, 1));
		}

		size_type	count( const key_type& k) const{
			if (_bst)
			{
				BST_pointer tmp = _bst->find_by_key(k);
				if (tmp)
					return 1;
				else
					return 0;
			}
			else
				return 0;
		}

		iterator lower_bound (const key_type& k)
		{
			if (_bst)
			{
				iterator it = begin();
				iterator it_end = end();
				for (; it != it_end; it++)
				{
					if (!_compare(it->first, k))
						break;
				}
				return (it);
			}
			else
				return (iterator(_dummy_end, _dummy_end, 1));
		}

		const_iterator lower_bound (const key_type& k) const
		{
			if (_bst)
			{
				const_iterator it = begin();
				const_iterator it_end = end();
				for (; it != it_end; it++)
				{
					if (!_compare(it->first, k))
						break;
				}
				return (it);
			}
			else
				return (const_iterator(_dummy_end, _dummy_end, 1));
		}
	
		iterator upper_bound (const key_type& k){
			if (_bst)
			{
				iterator it = begin();
				iterator it_end = end();
				for (; it != it_end; it++)
				{
					if (!_compare(it->first, k))
					{
						if (it->first == k)
							it++;
						break;
					}
				}
				return (it);
			}
			else
				return (iterator(_dummy_end, _dummy_end, 1));
		}

		const_iterator upper_bound (const key_type& k) const{
			if (_bst)
			{
				const_iterator it = begin();
				const_iterator it_end = end();
				for (; it != it_end; it++)
				{
					if (!_compare(it->first, k))
					{
						if (it->first == k)
							it++;
						break;
					}
				}
				return (it);
			}
			else
				return (const_iterator(_dummy_end, _dummy_end, 1));
		}

		ft::pair<iterator,iterator>             equal_range (const key_type& k){
			return (ft::make_pair(lower_bound(k), upper_bound(k)));
		}

		ft::pair<const_iterator,const_iterator> equal_range (const key_type& k) const{
			return (ft::make_pair(lower_bound(k), upper_bound(k)));
		}

		reverse_iterator rbegin( void ) {
			return reverse_iterator(end()); 
		};
		reverse_iterator rend( void ) {
			return reverse_iterator(begin()); 
		};
		
		const_reverse_iterator rbegin( void ) const { 
			return const_reverse_iterator(end()); 
		};
		const_reverse_iterator rend( void ) const { 
			return const_reverse_iterator(begin()); 
		};

		mapped_type& operator[] (const key_type& k)
		{
			return (*((this->insert(ft::make_pair(k, mapped_type()))).first)).second;
		}

	public:
		void 			clear() { destroy_bst(); };
		size_type		size( void ) const { return _size; }
		size_type		max_size() const { return _bst_allocker.max_size(); }
		bool			empty( void ) const { return (_size == 0 ? true : false); }
		allocator_type	get_allocator( void ) const { return _allocker; }
		key_compare		key_comp() const { return _compare; }
		void			swap (map& x) { 
			size_type 		tmp_size = _size;
			BST_pointer 	tmp_bst = _bst;
			BST_pointer		tmp_dummy = _dummy_end;

			_size = x._size;
			_bst = x._bst;
			_dummy_end = x._dummy_end;

			x._size = tmp_size;
			x._bst = tmp_bst;
			x._dummy_end = tmp_dummy;
		};
		value_compare 	value_comp() const { return value_compare(_compare); };
};

template <class Key, class T, class Compare, class Alloc>
void swap (map<Key,T,Compare,Alloc>& x, map<Key,T,Compare,Alloc>& y){
	x.swap(y);
}

template <class Key, class T, class Compare, class Alloc>
bool operator== ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ){
	if (lhs.size() == rhs.size())
	{
		typename map<Key,T,Compare,Alloc>::const_iterator it1 = lhs.begin();
		typename map<Key,T,Compare,Alloc>::const_iterator it2 = rhs.begin();

		for (; it1 != lhs.end(); it1++)
		{
			if (*it1 != *it2)
				return false;
			it2++;
		}
		return true;
	}
	else
		return false;
}

template <class Key, class T, class Compare, class Alloc>
bool operator!= ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ){
	return (!(lhs == rhs));
}

template <class Key, class T, class Compare, class Alloc>
bool operator<  ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ){
	return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
}

template <class Key, class T, class Compare, class Alloc>
bool operator<= ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ){
	return (!(rhs < lhs));
}
	
template <class Key, class T, class Compare, class Alloc>
bool operator>  ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ){
	return (rhs < lhs);
}

template <class Key, class T, class Compare, class Alloc>
bool operator>= ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ){
	return (!(lhs < rhs));
}

}

#endif
