#pragma once

// ----------------------------------------------------------------------------

// the packing has to be forced to 1-byte here because that's how the data
// is stored inside the ZIP file. If we don't force this, the compiler
// pads and aligns variables in memory whch makes the struct not match
// the file layout and we get invalid data in the struct.

#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */

struct zip_local_file_header
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

#pragma pack(pop)   /* restore original alignment from stack */

// ----------------------------------------------------------------------------

struct w_zip_toc_entry
{
	w_zip_toc_entry( std::string zip_filename, std::string filename, int offset, int size );
	std::string zip_filename = "";
	std::string filename = "";
	int offset_from_start_of_file = 0;
	int size = 0;
};

// ----------------------------------------------------------------------------

struct w_io_zip
{
	std::map<std::string, std::unique_ptr<w_zip_toc_entry>> table_of_contents;

	void scan_and_build_table_of_contents();;
	bool toc_contains_filename( std::string filename );
	w_zip_toc_entry* get_toc_entry_for_filename( std::string filename );
	std::unique_ptr<std::vector<char>> get_data_for_filename( std::string filename );
};
