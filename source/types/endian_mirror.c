/* Hanami Zero (C) 2021: types/endian_mirror.c */
#include <stdint.h>
#include "0hana/types.h"

#ifdef  test

test {
	{	
		e Byte = 0x12;
		endian_mirror(s(Byte), a(Byte));
		subtest(Byte == 0x12);
	}
	{
		int16_t Word = 0x1234;
		endian_mirror(s(Word), a(Word));
		subtest(Word == 0x3412);
	}
	{
		int32_t Dword = 0x12345678;
		endian_mirror(s(Dword), a(Dword));
		subtest(Dword == 0x78563412);
	}
	{
		int64_t Qword = 0x1234567800112233;
		endian_mirror(s(Qword), a(Qword));
		subtest(Qword == 0x3322110078563412);
	}
	{
		e cstring[] = "Hello World!";
		endian_mirror(s(cstring), cstring);
		subtest(s(cstring) == 13 && strncmp(cstring, "\0!dlroW olleH", s(cstring)) == 0);
	}
}

#endif//test

v endian_mirror(x c Bytes, o c Datum) {
	for(x X = 0; X < Bytes / 2; X++) {
		e Byte         = ((e*)Datum)[X];
		((e*)Datum)[X] = ((e*)Datum)[Bytes - 1 - X];
		((e*)Datum)[Bytes - 1 - X]           = Byte;
	}
}
