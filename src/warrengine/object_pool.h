
namespace war
{

// object_pool is a round-robin type pool. calling "get_next" will return a
// pointer to the next object in the pool.

template<typename T>
struct Object_Pool
{
	// which object we're going to use next. this wraps around the vector
	// above, endlessly, re-using objects.
	size_t idx = 0;

	Object_Pool() = default;
	virtual ~Object_Pool() = default;

	void init_to_size( size_t num_objects )
	{
		// pre-allocate the needed number of objects in the pool
		_objects.resize( num_objects );
	}

	// returns a pointer to the next object in the pool. it is up to the caller
	// to initialize/clear this object before use.
	[[nodiscard]] T* get_next()
	{
		auto result = &( _objects[ idx ] );

		// move the idx ahead by 1, wrapping around from the end to the start
		idx = ( idx + 1 ) % _objects.size();

		_num_objects_in_pool++;

		// return a pointer to the current object
		return result;
	}

	// resets the pool index to the first element
	void reset()
	{
		idx = _num_objects_in_pool = 0;
	}

	[[nodiscard]] bool empty()
	{
		return ( _num_objects_in_pool == 0 );
	}

	[[nodiscard]] size_t num_objects_in_pool()
	{
		return _num_objects_in_pool;
	}

	[[nodiscard]] T* data()
	{
		return _objects.data();
	}

	[[nodiscard]] size_t capacity()
	{
		return _objects.capacity();
	}

protected:

	std::vector<T> _objects;

	// how many objects have been added into this pool since the last time it
	// was reset
	size_t _num_objects_in_pool = 0;
};

}

