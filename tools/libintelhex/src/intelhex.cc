/*  Routines for reading/writing Intel INHX8M and INHX32 files

    Copyright 2002 Brandon Fosdick (BSD License)
*/

#include <fstream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "intelhex.h"

namespace intelhex
{

    #define	INH32M_HEADER	":020000040000FA"

    // Array access operator
    value_type& hex_data::operator[](address_type address)
    {
	// Start at the end of the list and find the first (last) block with an address
	//  less than addr
	reverse_iterator i = blocks.rbegin();
	while( i != blocks.rend() )
	{
	    if( i->first <= address )
	    {
		// Use the block if address is interior or adjacent to the block
		if( (address - i->first) <= i->second.size() )
		    return i->second[address - i->first];
		break;
	    }
	    ++i;
	}
	return blocks[address][0];
    }

    // Return the value at address, or _fill if not set
    value_type hex_data::get(address_type address)
    {
	// Start at the end of the list and find the first (last) block with an address
	//  less than addr
	reverse_iterator i = blocks.rbegin();
	while( i != blocks.rend() )
	{
	    if( i->first <= address )
	    {
		// Use the block if address is interior to the block
		if( (address - i->first) < i->second.size() )
		    return i->second[address - i->first];
		break;
	    }
	    ++i;
	}
	return _fill;
    }

    // Set the value at address or create a new element using value
    void hex_data::set(address_type address, value_type value)
    {
	if( value == fill() )	// Handle fill values
	{
	    erase(address);	// If the address is already set, erase it
	    return;
	}

	// Start at the end of the list and find the first (last) block with an address
	//  less than addr
	reverse_iterator i = blocks.rbegin();
	while( i != blocks.rend() )
	{
	    if( i->first <= address )
	    {
		// Use the block if address is interior or adjacent to the block
		const address_type index = address - i->first;
		if( index < i->second.size() )
		{
		    i->second[index] = value;
		    return;
		}
		else if( index == i->second.size() )
		{
		    i->second.push_back(value);
		    return;
		}
		break;
	    }
	    ++i;
	}
	blocks[address].push_back(value);	// Otherwise create a new block
    }

    // Merge adjacent blocks
    void hex_data::compact()
    {
	iterator previous = blocks.begin();
	iterator i = previous;

	for(++i; i != blocks.end(); ++i)
	{
	    if( (previous->first + previous->second.size()) == i->first )
	    {
		previous->second.insert(previous->second.end(), i->second.begin(), i->second.end());
		blocks.erase(i);
		i = previous;
	    }
	    previous = i;
	}
    }

    // Delete all allocated memory
    void hex_data::clear()
    {
	_fill = 0;
	format = HEX_FORMAT_INHX8M;
	linear_addr_rec = false;
	segment_addr_rec = false;
	blocks.clear();
    }

    // Erase a single element at the given address
    void hex_data::erase(address_type address)
    {
	for(iterator i=blocks.begin(); i!=blocks.end(); ++i)
	{
	    // The blocks are sorted, so if the byte to be deleted is
	    //  before the block it must be a blank address that's either
	    //  before the first block or after any previous blocks.
	    if( address < i->first )
		break;
	    // Ignore the block if address is past the end of the block
	    const address_type ope = i->first + i->second.size();
	    if( address >= ope )
		continue;
	    // address is now guaranteed to be >= i->first and < ope
	    // Copy trailing portion of the old block to a new block
	    if( (ope - address) > 1 )
	    {
		const address_type index = address-i->first+1;
		blocks[address+1].assign(i->second.begin()+index, i->second.end());
	    }
	    // Truncate or delete old block
	    const address_type size = address - i->first;
	    if( size )
		i->second.resize(size);
	    else
		blocks.erase(i);
	    break;
	}
    }

    // Erase [first, last]
    void hex_data::erase(address_type first, address_type last)
    {
	if( first > last )
	    std::swap(first, last);

	for(iterator i=blocks.begin(); (i!=blocks.end()) && (first<=last); ++i)
	{
	    const address_type ope = i->first + i->second.size();
	    if( first >= ope )	// Ignore all blocks with addresses < first
		continue;
	    // The blocks are sorted, so if the first byte to be deleted is
	    //  before the block it must be a blank address that's either
	    //  before the first block or after any previous blocks.
	    if( first < i->first )
	    {
		if( last < i->first )	// If the entire range is before the
		    return;		//  block there's nothing left to do
		first = i->first;   // Advance to the next non-blank address
	    }
	    // first is now guaranteed to be >= i->first and < ope
	    if( last < ope )	// Entire range is interior
	    {
		// Copy trailing portion of the old block to a new block
		if( (ope - last) > 1 )
		{
		    const address_type index = last-i->first+1;
		    blocks[last+1].assign(i->second.begin()+index, i->second.end());
		}
		// Truncate or delete old block
		const address_type size = first - i->first;
		if( size )
		    i->second.resize(size);
		else
		    blocks.erase(i);
		return;
	    }
	    else	// Truncate block
	    {
		const address_type size = first - i->first;
		if( size )
		    i->second.resize(size);
		else
		    blocks.erase(i--);
		first = ope;
	    }
	}
    }

    hex_data::size_type hex_data::size()
    {
	size_type s=0;

	for(iterator i=blocks.begin(); i!=blocks.end(); ++i)
	    s += i->second.size();

	return s;
    }

    // Returns the number of populated elements with addresses less than addr
    hex_data::size_type hex_data::size_below_addr(address_type addr)
    {
	size_type s=0;

	for(iterator i=blocks.begin(); i!=blocks.end(); ++i)
	{
	    if( (i->first + i->second.size()) < addr)
		s += i->second.size();
	    else if( i->first < addr )
		s += addr - i->first;
	}

	return s;
    }

    // number of words in [lo, hi)
    hex_data::size_type hex_data::size_in_range(address_type lo, address_type hi)
    {
	size_type s=0;

	for(iterator i=blocks.begin(); i!=blocks.end(); ++i)
	{
	    if( i->first < lo )
	    {
		const size_type a = i->first + i->second.size();
		if( a >= lo )
		    s += a  - lo;
	    }
	    else
	    {
		if( (i->first + i->second.size()) < hi)
		    s += i->second.size();
		else if( i->first < hi )
		    s += hi - i->first;
	    }
	}

	return s;
    }

    // Return the max address of all of the set words with addresses less than or equal to hi
    address_type hex_data::max_addr_below(address_type hi)
    {
	address_type s=0;

	for(iterator i=blocks.begin(); i!=blocks.end(); ++i)
	{
	    if( i->first <= hi)
	    {
		const address_type a = i->first + i->second.size() - 1;	//Max address of this block
		if( a > s )
		    s = a;
	    }
	}
	if( s > hi )
	    return hi;
	else
	    return s;
    }

    // Lowest address
    address_type hex_data::min_address() const
    {
	return blocks.begin()->first;
    }

    // Highest address
    address_type hex_data::max_address() const
    {
	return blocks.rbegin()->first + blocks.rbegin()->second.size() - 1;
    }

    //Return true if an element exists at addr
    bool hex_data::is_set(address_type addr)
    {
	// Start at the end of the list and find the first (last) block with an address
	//  less than addr
	reverse_iterator i = blocks.rbegin();
	while( (i!=blocks.rend()) && (i->first > addr))
	    ++i;

	if( (addr - i->first) > i->second.size() )
	    return false;
	else
	    return true;
    }

    // Load from a file
    void hex_data::load(const std::string &path)
    {
	std::ifstream f(path.c_str());
	read(f);
    }

    // Convert a string from hex to binary and append it to a block
    uint8_t hex2binary(hex_data::data_container& to, std::string& from)
    {
	value_type    sum = 0, value;
	uint8_t character;
	bool first = true;
	std::string::iterator i = from.begin();

	while( i != from.end() )
	{
	    character = *i;

	    if( (character >= '0') && (character <= '9') )
		character -= '0';
	    else if( (character >= 'A') && (character <= 'Z') )
		character -= 'A' - 10;
	    else if( (character >= 'a') && (character <= 'z') )
		character -= 'a' - 10;
	    else
		break;	// Bad character

	    if( first )
		value = character << 4;
	    else
	    {
		value |= character;
		to.push_back(value);
		sum += value;
	    }

	    first = !first;
	    ++i;
	}

	return sum;
    }

    // Read data from an input stream
    void hex_data::read(std::istream &s)
    {
	address_type   address;
	address_type   linear_address(0);
	std::string line;
	data_container buffer;

	while( (s.get() == ':') && s.good() )
	{
	    getline(s, line);		    // Read the whole line
	    if( line.size() <= 10 )	    // Ignore truncated lines
		break;
	    buffer.clear();
	    buffer.reserve(line.size()/2);  // Pre-allocate
	    if( hex2binary(buffer, line) )  // Ignore lines with bad checksums
		break;

	    address = buffer[1];
	    address = (address << 8) | buffer[2];
	    unsigned length = buffer[0];
	    const unsigned type = buffer[3];
	    value_type* data = &buffer[4];

	    switch(type)
	    {
		case 0: 	//Data block
		{
		    address += linear_address;
		    iterator i = blocks.begin();
		    for(; i != blocks.end(); ++i )  // Find a block that includes address
		    {
			address_type num = 0;
			// If the start of the new block is interior to an existing block...
			if( (i->first <= address) && ( (i->first + i->second.size()) > address) )
			{
			    // Store the portion of the new block that overlaps the existing block
			    const size_type index = address - i->first;
			    num = i->second.size() - index;
			    if( num > length )
				num = length;
			    std::copy(data, data+num, &(i->second[index]));
			}
			// If the end of the new block is interior to an existing block...
			if( (address < i->first) && ((address + length) > i->first) )
			{
			    // Create a new block for the non-overlapping portion
			    num = i->first - address;
			    if( num > length )
				num = length;
			    blocks[address].assign(data, data+num);
			}
			length -= num;
			address += num;
			data += num;
			// Bail out early if there's nothing left to do
			if( 0 == length )
			    break;
		    }
		    // Handle any leftover bytes
		    if( length )
			blocks[address].assign(data, data+length);
		    break;
		}
		case 1: break;	// Ignore EOF record
		case 2:		// Segment address record (INHX32)
		    segment_addr_rec = true;
		    break;
		case 4:		// Linear address record (INHX32)
		    if( (0 == address) && (2 == length) )
		    {
			linear_address = buffer[4];
			linear_address = (linear_address << 8) | buffer[5];
			linear_address <<= 16;
			linear_addr_rec = true;
		    }
		    break;
	    }
	}
    }

    // Write all data to a file
    void hex_data::write(const char *path)
    {
	std::ofstream	ofs(path);
	if( !ofs )	// Bail out on bad files
	    return;
	write(ofs);
	ofs.close();
    }

    // Write all data to an output stream
    void hex_data::write(std::ostream &os)
    {
	uint8_t	checksum;
	uint16_t	linear_address(0);

	if( !os )	    // Bail out on bad streams
	    return;

	os.setf(std::ios::hex, std::ios::basefield);	//Set the stream to ouput hex instead of decimal
	os.setf(std::ios::uppercase);			//Use uppercase hex notation
	os.fill('0');					//Pad with zeroes

	//If we already know that this is an INHX32M file, start with a segment address record
	//	otherwise check all of the blocks just to make sure
	if( linear_addr_rec )
	{
	    os << INH32M_HEADER << std::endl;
	}
	else
	{
	    for(iterator i=blocks.begin(); i!=blocks.end(); i++)
	    {
		if(i->first > 0xFFFF)	//Check the upper 16 bits
		{
		    linear_addr_rec = true;
		    os << INH32M_HEADER << std::endl;
		    break;	//Only need to find one
		}
	    }
	}

	for(iterator i=blocks.begin(); i!=blocks.end(); i++)
	{
	    // Check upper 16 bits of the block address for non-zero,
	    //  which indicates that a segment address record is needed
	    if( i->first > 0xFFFF )
	    {
		const uint16_t addr(i->first >> 16);
		//Has a record for this segment already been emitted?
		if( addr != linear_address )
		{
		    //Emit a new segment address record
		    os << ":02000004";
		    os.width(4);
		    os << addr;	//Address
		    // Create a checksum for the linear address record
		    checksum = 0x06 + addr + (addr >> 8);
		    checksum = 0x01 + ~checksum;
		    os.width(2);
		    // OSX (or maybe GCC), seems unable to handle uint8_t
		    //  arguments to a stream
		    os << static_cast<uint16_t>(checksum);	// Checksum byte
		    os << std::endl;
		    linear_address = addr;
		}
	    }
	    checksum = 0;
	    os << ':';	//Every line begins with ':'
	    os.width(2);
	    os << i->second.size();				//Length
	    checksum += i->second.size();
	    os.width(4);
	    os << static_cast<uint16_t>(i->first);		//Address
	    checksum += static_cast<uint8_t>(i->first);		// Low byte
	    checksum += static_cast<uint8_t>(i->first >> 8);	// High byte
	    os << "00";											//Record type
	    for(unsigned j=0; j<i->second.size(); ++j)	//Store the data bytes, LSB first, ASCII HEX
	    {
		os.width(2);
		// OSX (or maybe GCC), seems unable to handle uint8_t
		//  arguments to a stream
		os << static_cast<uint16_t>(i->second[j]);
		checksum += i->second[j];
	    }
	    checksum = 0x01 + ~checksum;
	    os.width(2);
	    // OSX (or maybe GCC), seems unable to handle uint8_t arguments to a stream
	    os << static_cast<uint16_t>(checksum);	// Checksum byte
	    os << std::endl;
	}
	os << ":00000001FF\n";			//EOF marker
    }

    // Make things pretty
    //  Truncate blocks to a given length as needed
    void hex_data::tidy(hex_data::size_type length)
    {
	for(iterator i=blocks.begin(); i!=blocks.end(); i++)
	{
	    if(i->second.size() > length)	//If the block is too long...
	    {
		//Make an interator that points to the first element to copy out of i->second
		data_container::iterator k(i->second.begin());
		advance(k, length);

		// Assign the extra elements to a new block and truncate the original
		blocks[i->first + length].assign(k, i->second.end());
		i->second.erase(k, i->second.end());
	    }
	}
    }

    //Compare two sets of hex data
    //	Return true if every word in hex1 has a corresponding, and equivalent, word in hex2
    bool compare(hex_data& hex1, hex_data& hex2, value_type mask, address_type begin, address_type end)
    {
	//Walk block list from hex1
	for( hex_data::iterator i = hex1.begin(); i != hex1.end(); ++i )
	{
	    //Walk the block
	    address_type addr(i->first);
	    for( hex_data::data_container::iterator j = i->second.begin(); j != i->second.end(); ++j, ++addr)
	    {
		if( (addr < begin) || (addr > end) )
		    continue;

		//Compare both sides through the given mask
		if( ((*j) & mask) != (hex2.get(addr) & mask) )
		    return false;
	    }
	}
	return true;
    }

}
