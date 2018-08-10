/*  Routines for reading/writing Intel INHX8M and INHX32 files

    Copyright 2002 Brandon Fosdick (BSD License)
*/

#ifndef INTELHEXH
#define INTELHEXH

#include <iostream>
#include <map>
#include <vector>

#include <unistd.h>

namespace intelhex
{

    #define	HEX_FORMAT_INHX8M	0x01
    #define	HEX_FORMAT_INHX32	0x02

    class   hex_data;
    typedef hex_data container;
    typedef uint32_t address_type;
    typedef uint8_t value_type;

    //The data set that results from parsing a hex file
    struct hex_data
    {
	//Each line of the hex file generates a block of memory at a particular address
	typedef	std::vector<value_type>	data_container;		//Element container
	typedef	std::map<address_type, data_container> container;   //List of data blocks

	typedef	container::iterator	iterator;
	typedef	container::reverse_iterator	reverse_iterator;
	typedef	data_container::size_type	size_type;
    private:
	value_type  _fill;	    // Value returned for unset addresses
	char	format;				//Format of the parsed file (necessary?)
	bool	segment_addr_rec;		// Uses/Has a segment address record
	bool	linear_addr_rec;		// Uses/Has a linear address record
	container   blocks;			// List of data blocks

    public:
	hex_data() : _fill(0), segment_addr_rec(false), linear_addr_rec(false) {}
	hex_data(const std::string &s) : _fill(0), segment_addr_rec(false), linear_addr_rec(false)
	{
	    load(s);
	}
	iterator    begin() { return blocks.begin(); }
	iterator    end() { return blocks.end(); }

	void	compact();		// Merge adjacent blocks
	void	clear();		//Delete everything
	void	erase(address_type);	// Erase a single element
	void 	erase(address_type first, address_type last);  // Erase [first, last]
	value_type  fill()  { return _fill; }
	void	    fill(value_type f)  { _fill = f; }
	size_type   size();
	size_type   size_below_addr(address_type);
	size_type   size_in_range(address_type, address_type);    //number of words in [lo, hi)
	address_type   max_addr_below(address_type);

	address_type   min_address() const;	// Lowest address
	address_type   max_address() const;	// Highest address

	bool	is_set(address_type);

	value_type& operator[](address_type);	//Array access operator
	value_type  get(address_type);		// Return the value at address
	void	set(address_type, value_type);	// Set the value at address

	void	load(const std::string&);	// Load from a file
	void	read(std::istream &);			// Read data from an input stream
	void	write(const char *);			//Save hex data to a hex file
	void	write(std::ostream &);			//Write all data to an output stream
	void	tidy(size_type length);			// Make things pretty
    };

    bool compare(hex_data&, hex_data&, value_type, address_type, address_type);
}
#endif
