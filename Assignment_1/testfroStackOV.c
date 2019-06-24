#include <linux/module.h>       // Needed by all modules
#include <linux/kernel.h>       // KERN_INFO
#include <linux/sched.h>        // for_each_process, pr_info
#include <unistd.h>


  int main(){
    struct task_struct *task;

    for_each_process(task) {
        printk("Task %s (pid = %d)\n",p->comm, task_pid_nr(p));
    }


      return 0;
  }