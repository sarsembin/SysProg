#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/gfp.h>
#include <linux/mm_types.h>
#include <linux/types.h>

// Part 2 task: type getconf PAGE_SIZE.
#define PAGE_SIZE 4096
// Then I created my own struct containing array of
// 8 unsigned integers and found it's size using sizeof() fucntion.
struct mystruct {
   unsigned int arrayOfIntegers[8];
};

// Part 1 task: I used module_param(name, type, perm)
// to pass parameters to the module when installed.
// Name - name of parameter, type - type of parameter,
// Perm is permision which is specififed in octal form.
// For example 0660 - allows user and groups to read/write.
static int param = 2000; //default value = 2000.
module_param(param, int, 0660);

// Part 3 task:
// I used alloc_pages(unsigned int gfp_mask, unsigned int order)
// function to allocate enough memory pages to contain the number 
// structs that are passed with module parameter. Before that I used 
// getnsdayofday to get current time and print it right before 
// alloc_pages. The gfp_mask flags tell the allocator how it should behave.
// The order is power of 2 number of pages to allocate.
// Then use free_page function to deallocate those pages.
struct page* mypages;
void pageAllocation(int temp1) {
   struct timespec t1, t2;
   int temp2 = param/temp1 + 1;
   int pow = 0;
   if(temp2 == 8) {
      pow = 3;
   } else if(temp2 == 16) {
      pow = 4;
   } else if(temp2 == 32) {
      pow = 5;
   }
   printk(KERN_INFO "2^%d=%d pages will bee allocated.", pow, temp2);
   getnstimeofday(&t1);
   // GFP_KERNEL allocation is performed on behalf of a process
   // running in the kernel space.
   mypages = alloc_pages(GFP_KERNEL, pow);
   __free_pages(mypages, pow);
   // Get time when finished deallocation to calculate
   // how many nanoseconds it took.
   getnstimeofday(&t2);
   printk(KERN_INFO "Time when we started: %lu. 
                     Time when we finished: %lu. How much did it take: %lu.", 
                     t1.tv_nsec, t2.tv_nsec, t2.tv_nsec - t1.tv_nsec);
}


int init_module(void) {
   printk(KERN_INFO "Starting Lab 10 - Page Allocation.\n");
   printk(KERN_INFO "Starting part 1:");
   printk(KERN_INFO "My module param: %d\n.", param);
   // Part 2:
   struct mystruct m;
   int temp1 = PAGE_SIZE/sizeof(m);
   printk(KERN_INFO "Starting part 2:");
   printk(KERN_INFO "The page size is: %d. 
                     Size of my struct: %d. Number of instances 
                     of my struct that would fit in 
                     with in a single kernel memory page: %d.\n", 
                     PAGE_SIZE, sizeof(m), temp1);
   printk(KERN_INFO "Starting part 3:");
   pageAllocation(temp1);
   return 0;
}

void cleanup_module(void) {
   printk(KERN_INFO "Cleanup module. End of lab 10.\n");
}