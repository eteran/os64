 /* multiboot.h - the header for Multiboot */
 /* Copyright (C) 1999, 2001  Free Software Foundation, Inc.

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

#ifndef MULTIBOOT_20100223_H_
#define MULTIBOOT_20100223_H_

/* The magic number for the Multiboot header. */
#define MULTIBOOT_HEADER_MAGIC          0x1BADB002

/* The flags for the Multiboot header. */
#define MULTIBOOT_HEADER_FLAGS          0x00010003

/* The magic number passed by a Multiboot-compliant boot loader. */
#define MULTIBOOT_BOOTLOADER_MAGIC      0x2BADB002

/* C symbol format. HAVE_ASM_USCORE is defined by configure. */
#ifdef HAVE_ASM_USCORE
#define EXT_C(sym) _ ## sym
#else
#define EXT_C(sym) sym
#endif

#ifndef ASM
typedef unsigned char      __u8;
typedef unsigned short     __u16;
typedef unsigned int       __u32;
typedef unsigned long long __u64;

#pragma pack (push, 1)

/* The Multiboot header. */
struct multiboot_header {
	__u32 magic;
	__u32 flags;
	__u32 checksum;
	__u32 header_addr;
	__u32 load_addr;
	__u32 load_end_addr;
	__u32 bss_end_addr;
	__u32 entry_addr;
	__u32 mode_type;
	__u32 width;
	__u32 height;
	__u32 depth;
};

/* The symbol table for a.out. */
struct aout_symbol_table {
	__u32 tabsize;
	__u32 strsize;
	__u32 addr;
	__u32 reserved;
};

/* The section header table for ELF. */
struct elf_section_header_table {
	__u32 num;
	__u32 size;
	__u32 addr;
	__u32 shndx;
};

/* The Multiboot information. */
struct multiboot_info {
	__u32 flags;
	__u32 mem_lower;
	__u32 mem_upper;
	__u32 boot_device;
	__u32 cmdline;
	__u32 mods_count;
	__u32 mods_addr;
	union {
		aout_symbol_table        aout_sym;
		elf_section_header_table elf_sec;
	} u;
	__u32 mmap_length;
	__u32 mmap_addr;

	__u32 drives_length;
	__u32 drives_addr;

	__u32 config_table;

	__u32 boot_loader_name;

	__u32 apm_table;

	__u32 vbe_control_info;
	__u32 vbe_mode_info;
	__u16 vbe_mode;
	__u16 vbe_interface_seg;
	__u16 vbe_interface_off;
	__u16 vbe_interface_len;
};

/* The module structure. */
struct module {
	__u32 mod_start;
	__u32 mod_end;
	__u32 cmdline;
	__u32 reserved;
};

/* The memory map. Be careful that the offset 0 is base_addr
but no size. */

struct memory_map {
	__u32 size;
	__u64 base_addr;
	__u64 length;
	__u32 type;
};
#pragma pack(pop)
#endif /* ! ASM */

#endif
