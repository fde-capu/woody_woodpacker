#include "woody.h"

void	byte_is(unsigned char* h, char test, const char* msg)
{	if (*h == test) printf(" %s", msg); }

void	true_is(long long a, long long b, const char* msg)
{	if (a == b) printf(" %s", msg); }

void	byte_hex(unsigned char* h, size_t amount, const char* msg)
{
	hex_byte(h, amount);
	printf(" %s", msg);
}

void	pretty_print(t_elf* ex)
{
	unsigned char* h;
	t_elf e = *ex;

	printf("Elf64_Ehdr: %p\n", ex);

	printf("\\ e_ident (+%d)\n", 0);
	h = (unsigned char *)e.ehdr->e_ident;

	printf("|| magic    ");
	hex_byte(h, 4);
	___br;
	___deb printf("||      \\__ 7f 45 4c 46 .ELF <- check above to validate.\n");

	printf("|| class    ");
	h += 4;
	hex_byte(h, 1);
	byte_is(h, ELFCLASSNONE, "None architecture/Invalid.");
	byte_is(h, ELFCLASS32, "32-bit architecture.");
	byte_is(h, ELFCLASS64, "64-bit architecture.");
	___br;

	printf("|| data enc ");
	hex_byte(++h, 1);
	byte_is(h, ELFDATANONE, "Unknown data format.");
	byte_is(h, ELFDATA2LSB, "2-complement, litte-endian.");
	byte_is(h, ELFDATA2MSB, "2-complement, big-endian.");
	___br;

	printf("|| elf ver  ");
	if (++h == EV_NONE)
		byte_hex(h, EV_CURRENT, "Ivalid version.");
	else
		byte_hex(h, EV_CURRENT, "<- Represents current version (hex).");
	___br;

	printf("|| OS ABI   ");
	hex_byte(++h, 1);
	byte_is(h, ELFOSABI_NONE | ELFOSABI_SYSV, "UNIX System VABI.");
	byte_is(h, ELFOSABI_HPUX, "HP-UX.");
	byte_is(h, ELFOSABI_NETBSD, "NetBSD.");
	byte_is(h, ELFOSABI_LINUX, "Linux.");
	byte_is(h, ELFOSABI_SOLARIS, "Solaris.");
	byte_is(h, ELFOSABI_IRIX, "IRIX.");
	byte_is(h, ELFOSABI_FREEBSD, "FreeBSD.");
	byte_is(h, ELFOSABI_TRU64, "TRU64 UNIX.");
	byte_is(h, ELFOSABI_ARM, "ARM architecture ABI.");
	byte_is(h, ELFOSABI_STANDALONE, "Stand-alone (embedded).");
	___br;

	printf("|| abi ver  ");
	byte_hex(++h, 1, "<- Represents target object version.");
	___br;

	printf("|\\_________ padding start: %p (total: %ld)\n", h, h - e.ehdr->e_ident);

	printf("\\ e_type (+%ld) ", (unsigned char *)&e.ehdr->e_type - e.ehdr->e_ident);
	uint16_t u = e.ehdr->e_type;
	hex_byte(&u, sizeof(e.ehdr->e_type));
	true_is(u, ET_NONE, "Unkown type.");
	true_is(u, ET_REL, "A relocatable file.");
	true_is(u, ET_EXEC, "An executable file.");
	true_is(u, ET_DYN, "A shared object.");
	true_is(u, ET_CORE, "A core file.");

	___br;
	return ;

	printf("p_type %d: ", e.phdr->p_type);
	printf(" PT_NULL %b ", e.phdr->p_type & PT_NULL & true);
	printf(" PT_LOAD %b ", e.phdr->p_type & PT_LOAD & true);
	printf(" PT_INTERP %b ", e.phdr->p_type & PT_INTERP & true);
	printf(" PT_NOTE %b ", e.phdr->p_type & PT_NOTE & true);
	printf(" PT_SHLIB %b ", e.phdr->p_type & PT_SHLIB & true);
	printf(" PT_PHDR %b ", e.phdr->p_type & PT_PHDR & true);
	printf(" PT_LOPROC %b ", e.phdr->p_type & PT_LOPROC & true);
	printf(" PT_HIPROC %b ", e.phdr->p_type & PT_HIPROC & true);
	printf(" PT_GNU_STACK %b ", e.phdr->p_type & PT_GNU_STACK & true);
	printf ("0 %b ", e.phdr->p_type & 0 & true);
}
