
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
	int local_file_header_signature;
	short version_needed_to_extract;
	short general_purpose_bit_flag;
	short compression_method;
	short last_mod_file_time;
	short last_mod_file_date;
	int crc_32;
	int compressed_size;
	int uncompressed_size;
	short file_name_length;
	short extra_field_length;
};

#pragma pack(pop)

// ----------------------------------------------------------------------------

struct Zip_File final
{
	struct toc_entry final
	{
		toc_entry( std::string_view zip_filename, std::string_view filename, int offset, int size );
		std::string zip_filename;
		std::string filename;
		int offset_from_start_of_file = 0;
		int size = 0;
	};

	std::unordered_map<std::string, toc_entry> table_of_contents;

	void scan_and_build_table_of_contents();
	[[nodiscard]] Zip_File::toc_entry* get_toc_entry_for_filename( std::string_view filename );
	[[nodiscard]] std::vector<char> get_data_for_filename( std::string_view filename );
};

}
