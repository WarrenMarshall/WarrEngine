
namespace war
{

// ----------------------------------------------------------------------------
// the packing has to be forced to 1-byte here because that's how the data
// is stored inside the ZIP file. If we don't force this, the compiler
// pads and aligns variables in memory whch makes the struct not match
// the file layout and we get invalid data in the struct.

#pragma pack(push)
#pragma pack(1)

struct Zip_File_Header
{
	int32_t local_file_header_signature;
	int16_t version_needed_to_extract;
	int16_t general_purpose_bit_flag;
	int16_t compression_method;
	int16_t last_mod_file_time;
	int16_t last_mod_file_date;
	int32_t crc_32;
	int32_t compressed_size;
	int32_t uncompressed_size;
	int16_t file_name_length;
	int16_t extra_field_length;
};

#pragma pack(pop)

// ----------------------------------------------------------------------------

struct Zip_File
{
	struct toc_entry
	{
		toc_entry( std::string_view zip_filename, std::string_view filename, int32_t offset, int32_t size );
		std::string zip_filename;
		std::string filename;
		int32_t offset_from_start_of_file = 0;
		int32_t size = 0;
	};

	std::unordered_map<std::string, toc_entry> table_of_contents;

	void scan_and_build_table_of_contents();
	[[nodiscard]] Zip_File::toc_entry* get_toc_entry_for_filename( std::string_view filename );
	[[nodiscard]] std::vector<char> get_data_for_filename( std::string_view filename );
};

}
