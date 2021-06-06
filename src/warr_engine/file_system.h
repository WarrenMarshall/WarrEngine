
namespace war
{

struct file_system
{
	inline static file_zip zip_file;

	static void init();

	[[nodiscard]] static bool file_exists_on_disk( std::string_view filename );
	static void create_path_if_not_exist( std::string_view path );

	[[nodiscard]] static bool file_exists_on_disk_or_in_zip( std::string_view filename );
	[[nodiscard]] static std::unique_ptr<file_mem> load_binary_file( std::string_view filename );
	[[nodiscard]] static std::unique_ptr<file_mem_text> load_text_file( std::string_view filename );
	[[nodiscard]] static std::unique_ptr<file_mem_text> load_text_file_raw( std::string_view filename );
	static void scan_folder_for_ext( std::vector<std::string>* filenames, std::string_view folder, std::string_view extension );

	// loads a file into memory, contained in a w_mem_file object. the lifetime
	// of this object will be managed by the caller.
	//
	// - first, look at the hard disk to see if the file is there.
	//		- if it is, load it and we're done
	//		- if it is NOT, load it from the ZIP files found in our folder
	//		- if it's not in there either, then it doesn't exist - ERROR!
	//
	// this pattern means that files on disk override files in ZIPs. this allows
	// you to work on loose files during development and then ZIP everything up
	// for release.

	template <typename T>
	static std::unique_ptr<T> load_file_into_memory( std::string_view filename )
	{
		// look on hard drive

		if( file_system::file_exists_on_disk( filename ) )
		{
			// file found, load it

			std::ifstream file( filename.data(), std::ios::binary | std::ios::ate );
			std::streamsize size = file.tellg();
			file.seekg( 0, std::ios::beg );

			std::unique_ptr<T> mem_file = std::make_unique<T>( (int)size + 1 );
			file.read( mem_file->buffer.data(), size );

			return std::move( mem_file );
		}

		// look in ZIP files

		auto toc_entry = file_system::zip_file.get_toc_entry_for_filename( filename );
		if( toc_entry == nullptr )
		{
			// file not found in a ZIP file either - ERROR!
			log_fatal( "File not found on disk OR in a zip file : [{}]", filename );
			return nullptr;
		}

		// file found in table of contents, load it from ZIP

		auto mem_file = std::make_unique<T>( toc_entry->size + 1 );
		mem_file->buffer = file_system::zip_file.get_data_for_filename( filename );

		return std::move( mem_file );
	}
};

}
