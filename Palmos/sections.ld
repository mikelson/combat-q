/* DO NOT EDIT!
   This file was automatically generated by m68k-palmos-multigen v2.2
   from sections.def  */

MEMORY
{
	perfsectres : ORIGIN = 0x0, LENGTH = 32768
	miscsectres : ORIGIN = 0x0, LENGTH = 32768
}

SECTIONS
{
	perfsect : { *(perfsect) } > perfsectres
	miscsect : { *(miscsect) } > miscsectres
}
