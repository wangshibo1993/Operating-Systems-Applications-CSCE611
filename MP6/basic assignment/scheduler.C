/*
 File: scheduler.C
 
 Author:
 Date  :
 
 */

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "scheduler.H"
#include "thread.H"
#include "console.H"
#include "utils.H"
#include "assert.H"
#include "simple_keyboard.H"
#include "simple_timer.H"
#include "blocking_disk.H"
/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* METHODS FOR CLASS   S c h e d u l e r  */
/*--------------------------------------------------------------------------*/


//extern SimpleTimer timer(100);
extern BlockingDisk * SYSTEM_DISK;


Scheduler::Scheduler()
{
  //assert(false);
  head_number = 0;
  tail_number = 0;
  thread_number = 0;
  Console::puts("Constructed Scheduler.\n");
}



void Scheduler::yield() 
{
  // Console::puts("Ticks is "); Console::puti(timer.getticks()); Console::puts(" \n");
  //assert(false);
  if(Machine::interrupts_enabled())
      Machine::disable_interrupts();
  
  
 if( (BlockingDisk::blocked_thread_number > 0) && (SYSTEM_DISK->is_ready())  )
  { 
    unsigned long temp = BlockingDisk::blocked_head;
    BlockingDisk:: blocked_head = (BlockingDisk:: blocked_head + 1) % MAX_NUMBER;
    BlockingDisk::blocked_thread_number --;
    Console::puts("Now we resume this read thread\n");
    Console::puts("hhh, now we have  ");Console::puti( BlockingDisk::blocked_thread_number); Console::puts(" blocked thread\n");
   /* Console::puts("But befor resume, on the ready queue, we have ");
  unsigned long j = head_number;
  for(int i = 0; i < thread_number; i++)
  {
     Console::puts("No. ");Console::puti( (ready_queue[j])->ThreadId()); Console::puts(" thread\n");
     j = (j+1)%MAX_NUMBER;
  }  */

    resume( BlockingDisk::blocked_queue[temp]);
  }  




  if(thread_number > 0)
  {
    Thread* next_thread = ready_queue[head_number];
    ready_queue[head_number] = NULL;
    head_number = (head_number + 1) % MAX_NUMBER;
    thread_number--;
    //Console::puts("next, we yield to No. ");Console::puti( next_thread->ThreadId()); Console::puts(" thread\n");
    Thread::dispatch_to(next_thread);
  }

  else if(thread_number == 0)
  {
    head_number = 0;
    tail_number = 0;
     Console::puts("All threads are completed and dequeued.\n");
  }


}



/*
bool Scheduler::on_yield_check()
{
     if(BlockingDisk::blocked_thread_number == 0)  //No blocked I/O thread, nothing needed to be done
       return false;

     else if(SYSTEM_DISK->is_ready())
       return true;
    
     else
       return false;

     

}*/

/*
static Thread* BlockingDisk::blocked_queue[MAX_NUMBER];
static unsigned long BlockingDisk::blocked_head ;
static unsigned long BlockingDisk::blocked_tail ;
static unsigned long BlockingDisk::blocked_thread_number ;
*/






void Scheduler::resume(Thread * _thread)
{
  if(Machine::interrupts_enabled())   
      Machine::disable_interrupts(); 
  
  ready_queue[tail_number]=_thread;
  tail_number = (tail_number + 1) % MAX_NUMBER;
  thread_number++;
  /* Console::puts("After resume, on the ready queue, we have ");
  unsigned long j = head_number;
  for(int i = 0; i < thread_number; i++)
  {
     Console::puts("No. ");Console::puti( (ready_queue[j])->ThreadId()); Console::puts(" thread\n");
     j = (j+1)%MAX_NUMBER;
  } */
  
  //assert(false);
}





void Scheduler::add(Thread * _thread)
{
  if(Machine::interrupts_enabled())   
      Machine::disable_interrupts(); 
  ready_queue[tail_number]=_thread;
  tail_number = (tail_number + 1) % MAX_NUMBER;
  thread_number++;
  //assert(false);
}



unsigned long Scheduler::ThreadNumber()
{ 
  return thread_number;
}

void Scheduler::terminate(Thread * _thread) 
{
 // if(!Machine::interrupts_enabled())   
   //   Machine::enable_interrupts(); 
  if(_thread == (Thread::CurrentThread()))
    yield(); 
 //assert(false);
}
