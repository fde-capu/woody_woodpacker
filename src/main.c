#include "woody.h"

char debug = 1;

t_elf	elf; // Yes, global.

void  read_file(char *filename)
{
	int			fd;
	int			filesize;
	long int	bytes_read;

	fd = open(filename, O_RDONLY);
	___die (fd == -1, "Failed to open file.");
	filesize = get_filesize(fd);
	___die (filesize == -1, "Failed to get file size.");
	elf.data = malloc(filesize + 1);
	___die (!elf.data, "Could not allocate.");
	bytes_read = read(fd, elf.data, filesize);
	___die (!bytes_read, "Is file empty?");
	___die (bytes_read == -1, "Error reading file");
	elf.data[bytes_read] = 0; // Null-terminate.

	// Esses dois apontam pro mesmo endereço zero?
	elf.phdr = (typeof(elf.phdr))elf.data;
	elf.ehdr = (typeof(elf.ehdr))elf.data;

//	___deb hex_dump(elf.data, bytes_read);
//	___deb lin_dump(elf.data, bytes_read);
//	___deb hex_dump(elf.ehdr, sizeof(elf.phdr));

	___die (elf.ehdr->e_ident[EI_MAG0] != ELFMAG0 ||
		elf.ehdr->e_ident[EI_MAG1] != ELFMAG1 ||
		elf.ehdr->e_ident[EI_MAG2] != ELFMAG2 ||
		elf.ehdr->e_ident[EI_MAG3] != ELFMAG3,
		"Invalid ELF file."); // Verify ELF header 
}

//void  decrypt_file(t_elf *elf)
//{
//	Elf64_Ehdr  *ehdr;
//	Elf64_Phdr  *phdr;
//	//  t_dv        *dv;
//	//  void        *ptr; 
//	int i = -1;
//
//	ehdr = (Elf64_Ehdr*)elf.data;
//	phdr = (Elf64_Phdr*)elf.data + ehdr->e_phoff;
//	while (++i < ehdr->e_phnum)
//	{
//		if(phdr[i].p_vaddr == phdr[i].p_paddr && phdr[i].p_memsz == phdr[i].p_filesz)
//		{
//			elf.phdr = &phdr[i];
//		} 
//	}
//	printf("\n%ld", elf.phdr->p_memsz);
//	return;
//
//	printf("decypt: %s", (char *)elf.data);
//}

int	main(int ac, char **av)
{
	ft_memset(&elf, 0, sizeof(elf));
	if (ac != 2)
		die("Usage: `woody_woodpacker binary_file`");
	read_file(av[1]);
//	decrypt_file(&elf);
	free(elf.data);
	return (0);
}
