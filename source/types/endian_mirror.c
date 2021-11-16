/* Hanami Zero (C) 2021: types/endian_mirror.c */
#include "0hana/types.h"

#ifdef  test

test {
	{	
		byte Byte =     0x12;
		endian_mirror(s(Byte), &(Byte));
		subtest(Byte == 0x12);
	}
	{
		int16_t Word =  0x1234;
		endian_mirror(s(Word), &(Word));
		subtest(Word == 0x3412);
	}
	{
		int32_t Dword =  0x12345678;
		endian_mirror(s(Dword), &(Dword));
		subtest(Dword == 0x78563412);
	}
	{
		int64_t Qword =  0x1234567800112233;
		endian_mirror(s(Qword), &(Qword));
		subtest(Qword == 0x3322110078563412);
	}
	{
		char cstring[] = "Hello World!";
		endian_mirror(s(cstring), cstring);
		subtest(s(cstring) == 13 && strncmp(cstring, "\0!dlroW olleH", s(cstring)) == 0);
	}
}

#endif//test

void endian_mirror(i z Bytes, io a const Datum) {
	for(z X = 0; X < Bytes / 2; X++) {
		byte Byte         = ((byte*)Datum)[X];
		((byte*)Datum)[X] = ((byte*)Datum)[Bytes - 1 - X];
		((byte*)Datum)[Bytes - 1 - X]              = Byte;
	}
}
