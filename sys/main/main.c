#include <defs.h>
#include <sys/kstdio.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/irq.h>

#define PG_SZ (0x1000)

void test_print()
{
    char *str="Hello World\n";
    char c='A';
    int a=10;

    //printf("~~Character: %c, Hex: %x, String: %s Address of String: %p\n", c,a,str,str);
    printf("~~Character: %c, Integer: %d, Hex: %x, String: %s Address of String: %p\n", c,a,a,str,str);
}

void start(uint32_t* modulep, void* physbase, void* physfree)
{
	struct smap_t {
		uint64_t base, length;
		uint32_t type;
	}__attribute__((packed)) *smap;
	while(modulep[0] != 0x9001) modulep += modulep[1]+2;
	for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
		if (smap->type == 1 /* memory */ && smap->length != 0) {
			printf("Available Physical Memory [%x-%x]\n", smap->base, smap->base + smap->length);
			printf("No of pages: %d\n", (smap->length/PG_SZ));
		}
	}
printf("Physbase: %p, Physfree: %p\n", physbase, physfree);
	test_print();
	// kernel starts here
	while(1);
}

#define INITIAL_STACK_SIZE 4096
char stack[INITIAL_STACK_SIZE];
uint32_t* loader_stack;
extern char kernmem, physbase;


void boot(void)
{
	// note: function changes rsp, local stack variables can't be practically used
	register char *temp1, *temp2;
	__asm__(
		"movq %%rsp, %0;"
		"movq %1, %%rsp;"
		:"=g"(loader_stack)
		:"r"(&stack[INITIAL_STACK_SIZE])
	);
	reload_gdt();
	reload_idt();
	setup_tss();
	start(
		(uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
		&physbase,
		(void*)(uint64_t)loader_stack[4]
	);
	for(
		temp1 = "!!!!! start() returned !!!!!", temp2 = (char*)0xb8000;
		*temp1;
		temp1 += 1, temp2 += 2
	) *temp2 = *temp1;

	test_print();	

        /* for(
                temp1 = "!!!!! end() returned !!!!!", temp2 = (char*)0xb8e00;
                *temp1;
                temp1 += 1, temp2 += 2
        ) *temp2 = *temp1; */ 

    /*
	puts((char *)"Hello\n");
    int z, a[10];
    z = printf("Testing string in printf: %s :and character: %c \n:-)\n", "hello guyz this is printf\n", 'x');
    printf("\nnum_char %d\n", z);
    z = printf("Integer %d, %%, %d, %", 12323, 0);
    printf("\nnum_char %d\n", z);
    z = printf("Negative Integer %d is a negative num :P", -10);
    printf("\nnum_char %d\n", z);
    z = printf("\nHex value of %d is %x\n", 0x8234A678, 0x8234a678);
    printf("\nnum_char %d\n", z);
    z = printf("\n\nPointer a[0] %p %d\n", &(a[0]), sizeof(int*));
    printf("\nnum_char %d\n", z);
    z = printf("\n\nPointer a[1] %p %d\n", &(a[1]), sizeof(int*));
    printf("\nnum_char %d\n", z);
    z = printf("\n\nPointer z %p %d\n", &z, sizeof(int*));
    int *t1 = &a[0];
    int *t2 = &a[1];
    printf("Difference in address %d\n", (t2 - t1));
    printf("\nnum_char %d\n", z);
    volatile int qqq = 0;

    printf("%d", 1/qqq);*/

    //int a=10;
    //printf("Address of a:%p\n", &a);

    //clear_screen();
    test_print();

	while(1) {
    };
}