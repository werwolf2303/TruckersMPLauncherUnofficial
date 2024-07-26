// :mode=c++:
/*
decode.h - c++ wrapper for a base64 decoding algorithm

This is part of the libb64 project, and has been placed in the public domain.
For details, see http://sourceforge.net/projects/libb64
*/
#ifndef BASE64_DECODE_H
#define BASE64_DECODE_H

#include <cstring>
#include <iostream>

namespace base64
{
	extern "C"
	{
		#include "cdecode.h"
	}

	struct decoder
	{
		base64_decodestate _state;
		int _buffersize;

		decoder(int buffersize_in = BUFSIZ)
		: _buffersize(buffersize_in)
		{}

		int decode(char value_in)
		{
			return base64_decode_value(value_in);
		}

		int decode(const char* code_in, const int length_in, char* plaintext_out)
		{
			return base64_decode_block(code_in, length_in, plaintext_out, &_state);
		}

		void decode(std::istream& istream_in, std::ostream& ostream_in)
		{
			base64_init_decodestate(&_state);
			//
			const int N = _buffersize;
			char* code = new char[N];
			char* plaintext = new char[N];
			int codelength;
			int plainlength;

			do
			{
				istream_in.read((char*)code, N);
				codelength = istream_in.gcount();
				plainlength = decode(code, codelength, plaintext);
				ostream_in.write((const char*)plaintext, plainlength);
			}
			while (istream_in.good() && codelength > 0);
			//
			base64_init_decodestate(&_state);

			delete [] code;
			delete [] plaintext;
		}

		char* decode(const char* input)
		{
			/* set up a destination buffer large enough to hold the encoded data */
			char* output = (char*)malloc(400);
			/* keep track of our decoded position */
			char* c = output;
			/* store the number of bytes decoded by a single call */
			int cnt = 0;
			/* we need a decoder state */
			base64_decodestate s;

			/*---------- START DECODING ----------*/
			/* initialise the decoder state */
			base64_init_decodestate(&s);
			/* decode the input data */
			cnt = base64_decode_block(input, strlen(input), c, &s);
			c += cnt;
			/* note: there is no base64_decode_blockend! */
			/*---------- STOP DECODING  ----------*/

			/* we want to print the decoded data, so null-terminate it: */
			*c = 0;

			return output;
		}
	};

} // namespace base64



#endif // BASE64_DECODE_H

