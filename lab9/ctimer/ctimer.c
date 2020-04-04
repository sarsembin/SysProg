#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>



#define TIMER_TIMEOUT 1

static struct timer_list mytimer; // declare ctimer
struct timespec tv; // structure to get current time


void print_current_time(void) {
   getnstimeofday(&tv); // gets cuurent time in seconds(from timespec)
   long long hours = (tv.tv_sec/3600 + 6)%24; // conversion in hh/mm/ss
   long long minutes = (tv.tv_sec/60)%60;
   long long seconds = tv.tv_sec%60; 
   pr_info("Current time: %lld:%lld:%lld", hours, minutes, seconds);
}


static void timer_handler(struct timer_list *t1) { //runs with ctimer
   static size_t nseconds;
   nseconds += TIMER_TIMEOUT; //prints counter for eac tick in nsecs
   pr_info("------------------------------------------");
   pr_info("[timer_hanler] nseconds = %d\n", nseconds);
   print_current_time(); // current time
   mod_timer(t1, jiffies + TIMER_TIMEOUT * HZ); // using jiffies and Hz like in lecture
}


int init_module(void) {
   pr_info("[timer_init] Init module\n"); //inserts module to kernel
   timer_setup(&mytimer, timer_handler, 0); // activates timer if its inactive
   mod_timer(&mytimer, jiffies + TIMER_TIMEOUT * HZ);
   return 0;
}



void cleanup_module(void) { //sudo rmmod ctimer
   printk(KERN_INFO "Cleanup %s\n", TAG);
   del_timer(&mytimer); // deletes timer
}
