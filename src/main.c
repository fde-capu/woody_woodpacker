#include "woody.h"

dumpster		*elf;
injector		*inj;
bool				elf_alloc = false;
bool				elf_data_alloc = false;
bool				inj_alloc = false;
bool				inj_data_alloc = false;
bool				inj_bin_alloc = false;

void	elf_init(char *vict)
{
	read_original_elf(vict);
	elf->bit_class =
		elf->data[EI_CLASS] == ELFCLASS64 ? 64
		: elf->data[EI_CLASS] == ELFCLASS32 ? 32
		: 0;
	if (elf->bit_class == 64)
	{
		_E64 = (Elf64_Ehdr*)elf->data;
		_P64 = (Elf64_Phdr*)(elf->data + _E64->e_phoff);
		_S64 = (Elf64_Shdr*)(elf->data + _E64->e_shoff);
		elf->phnum = _E64->e_phnum == PN_XNUM ?
			_S64->sh_info : _E64->e_phnum;
		elf->shnum = _E64->e_shnum == 0 ?
			_S64[0].sh_size : _E64->e_shnum;
		elf->shstrndx = _E64->e_shstrndx == SHN_XINDEX ?
			_S64[0].sh_link : _E64->e_shstrndx;
	}
}

void	inject(const char *woody, const char *buzz)
{
	read_blob(buzz);

	// Full clone.
	ft_memcpy(inj->data, elf->data, elf->data_size);

	// Positions.
	Elf64_Ehdr* EE = (Elf64_Ehdr*)elf->data;
	Elf64_Ehdr* IE = (Elf64_Ehdr*)inj->data;
	Elf64_Phdr* EP = (Elf64_Phdr*)(elf->data + EE->e_phoff);
	Elf64_Phdr* IP = (Elf64_Phdr*)(inj->data + EE->e_phoff);
	Elf64_Phdr* IPX = 0;
	Elf64_Shdr* IS = (Elf64_Shdr*)(inj->data + EE->e_shoff);

	// Find IPX, Phdr responible for .text.
	size_t i = 0;
	while (i < elf->phnum)
	{
		if (IP[i].p_flags & PF_X && 
			(IE->e_entry >= IP[i].p_offset &&
			 IE->e_entry < IP[i].p_offset + IP[i].p_filesz))
		{
			IPX = &IP[i];
		}
		i++;
	}
	___die(!IPX, "Original does not have a `.text` section.");

	// Clean .text area (for clarity).
	char *h = (void*)IE + IPX->p_offset;
	for (size_t i = 0; i < IPX->p_filesz; i++)
		*h++ = 0;	

	// Hard ajust last jump
	size_t inj_offset = IPX->p_filesz;
	int32_t* jump = (void*)inj->bin + (inj->bin_size - sizeof(int32_t));
	*jump = - inj->bin_size - IPX->p_filesz - 0;
	*jump = -inj->bin_size - IPX->p_filesz + 1;

	// Translate e_entry portion (assuming there is padding room).
	size_t org_offset = 0;
	ft_memcpy((void*)IE + IPX->p_offset + org_offset, (void*)EE + IPX->p_offset, IPX->p_filesz);

	// Inject blob.
	ft_memcpy((void*)IE + IE->e_entry + inj_offset, (void*)inj->bin, inj->bin_size);
	IE->e_entry += inj_offset;

	// Adjust parameters.
	IPX->p_filesz += inj->bin_size;
	IPX->p_memsz += inj->bin_size;

//	i = 0;
//	while (i < elf->shnum)
//	{
//		if (IS[i].sh_offset == IE->e_entry)
//		{
//			IS[i].sh_size += inj->bin_size;
//		}
//		else if (IS[i].sh_offset > IE->e_entry)
//		{
//			IS[i].sh_offset += inj->bin_size;
//		}
//		i++;
//	}

(void)IS; (void)EP;

	file_out_to_file(woody, inj->data, inj->data_size);
}

int		main(int argc, char **argv)
{
	___die(argc != 2, "Usage: `woody_woodpacker binary_file`");
	elf_init(argv[1]);
	inject("woody", "WOODY_blob.bin");
	return free_all();
}
