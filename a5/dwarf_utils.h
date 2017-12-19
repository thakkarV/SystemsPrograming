#ifndef DWARF_UTILS
#define DWARF_UTILS 

// load/unload
void do_load_elf(char * path);
void do_unload_elf(void);

// resgister methods
unsigned long get_register(unsigned reg_num);
void set_register(unsigned reg_num, unsigned long value);

#endif
