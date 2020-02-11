#include <limits>

template<typename T>
class BypassAllocator{
public:

	/*Typedefs needed by other classes (not used in this class, but 
	these need to be defined for the classes that use this allocator)*/
	typedef		T					value_type;
    typedef		value_type*			pointer;
    typedef		const value_type*	const_pointer;
    typedef		value_type&			reference;
    typedef		const value_type&	const_reference;
    typedef		std::size_t			size_type;
    typedef		std::ptrdiff_t		difference_type;

    template<typename U>
    struct rebind{
        typedef BypassAllocator<U> other;
    };

    inline BypassAllocator(){}
    inline ~BypassAllocator(){}
    inline BypassAllocator(BypassAllocator const&){}

    template<typename U>
    inline BypassAllocator(BypassAllocator<U> const&) {}

    inline T* address(T& r){
		return &r;
	}
    inline const T* address(const T& r){
		return &r;
	}

    inline T* allocate(size_t size){
		void* p = malloc(size * sizeof(T));

		if(!p)
			throw std::bad_alloc();

		return (T*) p;
    }

    inline void deallocate(T* p, size_t){
		free(p); 
    }

    inline size_t max_size() const{
		/* Windows API has a macro called max, which in the case of the following
		 * call to std::numeric_limits gets expanded. This is ofcourse not something
		 * that we want so we need to undefine the macro.
		*/
		#ifdef max
		#undef max
		#endif
		return std::numeric_limits<size_t>::max() / sizeof(T);
	}

    inline void construct(T* p, const T& t){
		//Placement new is not overridden anywhere, so we can use it
		new (p) T(t);
	}
    inline void destroy(T* p){
		p->~T();
	}

    inline bool operator==(BypassAllocator const&){
		return true;
	}
    inline bool operator!=(BypassAllocator const& a){
		return !operator==(a);
	}
};