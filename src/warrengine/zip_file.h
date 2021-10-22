
namespace war
{

// ----------------------------------------------------------------------------
// the packing has to be forced to 1-byte here because that's how the data
// is stored inside the ZIP file. If we don't force this, the compiler
// pads and aligns variables in memory whch makes the struct not match
// the file layout and we get invalid data in the struct.

#pragma pack(push)
#pragma pack(1)

struct Zip_File_Header final
{
	i32 local_file_header_signature;
	i16 version_needed_to_extract;
	i16 general_purpose_bit_flag;
	i16 compression_method;
	i16 last_mod_file_time;
	i16 last_mod_file_date;
	i32 crc_32;
	i32 compressed_size;
	i32 uncompressed_size;
	i16 file_name_length;
	i16 extra_field_length;
};

#pragma pack(pop)

// ----------------------------------------------------------------------------

struct Zip_File final
{
	struct toc_entry final
	{
		toc_entry( std::string_view zip_filename, std::string_view filename, i32 offset, i32 size );
		std::string zip_filename;
		std::string filename;
		i32 offset_from_start_of_file = 0;
		i32 size = 0;
	};

	std::unordered_map<std::string, toc_entry> table_of_contents;

	void scan_and_build_table_of_contents();
	[[nodiscard]] Zip_File::toc_entry* get_toc_entry_for_filename( std::string_view filename );
	[[nodiscard]] std::vector<char> get_data_for_filename( std::string_view filename );
};

}
