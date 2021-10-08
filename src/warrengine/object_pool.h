
namespace war
{

// object_pool is a round-robin type pool. calling "get_next" will return a
// pointer to the next object in the pool.

template<typename T>
struct Object_Pool
{
	// the allocated chunk of memory for the objects in the pool
	std::vector<T> objects;

	// convenience pointer to the data inside the "objects" vector
	T* data_ptr = nullptr;

	// how many objects have been added into this pool since the last time it
	// was reset
	int32_t count = 0;

	// which object we're going to use next. this wraps around the vector
	// above, endlessly re-using objects.
	int32_t idx = 0;

	Object_Pool() = default;
	virtual ~Object_Pool() = default;

	void init_to_size( int32_t size )
	{
		// pre-allocate the needed number of objects in the pool
		objects.resize( size );
		data_ptr = objects.data();
	}

	// returns a pointer to the next object in the pool. it is up to the caller
	// to initialize/clear this object before use.
	[[nodiscard]] T* get_next()
	{
		assert( objects.size() > 0 );		// need to call init_to_size first!

		auto result = data_ptr + idx;

		// move the idx ahead by 1, wrapping around from the end to the start
		idx = ( idx + 1 ) % objects.size();

		count++;

		// make sure we're not overwriting objects - if this fires, increase
		// your pool size by calling "init_to_size" with a larger value.
		assert( count < objects.capacity() + 1 );

		// return a pointer to the current object
		return result;
	}

	// resets the pool index to the first element
	void reset()
	{
		idx = count = 0;
	}

	[[nodiscard]] bool empty()
	{
		return ( count == 0 );
	}

	[[nodiscard]] int32_t capacity()
	{
		return (int32_t)objects.capacity();
	}
};

}
