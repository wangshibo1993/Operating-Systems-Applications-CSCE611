/*
     File        : blocking_disk.c

     Author      : 
     Modified    : 

     Description : 

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "assert.H"
#include "utils.H"
#include "console.H"
#include "blocking_disk.H"
#include "simple_disk.H"
#include "machine.H"
#include "scheduler.H"
#include "thread.H"
/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR */
/*--------------------------------------------------------------------------*/

extern Scheduler* SYSTEM_SCHEDULER;

Thread* BlockingDisk::blocked_queue[MAX_NUMBER];
unsigned long BlockingDisk::blocked_head = 0;
unsigned long BlockingDisk::blocked_tail = 0;
unsigned long BlockingDisk::blocked_thread_number = 0;


BlockingDisk::BlockingDisk(DISK_ID _disk_id, unsigned int _size) 
  : SimpleDisk(_disk_id, _size) {}
   


bool BlockingDisk::is_ready()
{
     return SimpleDisk::is_ready();
}


/*--------------------------------------------------------------------------*/
/* SIMPLE_DISK FUNCTIONS */
/*--------------------------------------------------------------------------*/

void BlockingDisk::read(unsigned long _block_no, unsigned char * _buf) 
{
  // -- REPLACE THIS!!!
  //SimpleDisk::read(_block_no, _buf);
  if(Machine::interrupts_enabled())
      Machine::disable_interrupts();

  issue_operation(READ, _block_no);

  while(! is_ready() )
  {
       Console::puts("Ooops, reading is not ready \n ");
       blocked_queue[blocked_tail] = Thread::CurrentThread();
       blocked_tail = (blocked_tail + 1) % MAX_NUMBER;
       blocked_thread_number ++;
       Console::puts("We blocked the current thread No. "); Console::puti(Thread::CurrentThread()->ThreadId()); Console::puts("\n");
       Console::puts("now we have  ");Console::puti( blocked_thread_number); Console::puts(" blocked thread\n");
      
     // Console::puts("Ooops, reading is not ready \n ");
     // SYSTEM_SCHEDULER->resume(Thread::CurrentThread());
      SYSTEM_SCHEDULER->yield();
  } 

  /* read data from port */
  if( is_ready())
  {
    Console::puts("good, reading is ready now \n ");
    int i;
    unsigned short tmpw;
    for (i = 0; i < 256; i++)
    {
       tmpw = Machine::inportw(0x1F0);
       _buf[i*2]   = (unsigned char)tmpw;
       _buf[i*2+1] = (unsigned char)(tmpw >> 8);
      // Console::puts("buf[ ");Console::puti(i*2);Console::puts(" ]=  ");Console::putch(_buf[i*2]);Console::puts(" \n ");
      // Console::puts("buf[ ");Console::puti(i*2 + 1);Console::puts(" ]=  ");Console::putch(_buf[i*2+1]);Console::puts(" \n ");
    }


   //blocked_head = ( blocked_head + 1) % MAX_NUMBER;
  // blocked_thread_number --;
   // Console::puts("after that we have  ");Console::puti( blocked_thread_number); Console::puts(" blocked thread\n");
  }


}












void BlockingDisk::write(unsigned long _block_no, unsigned char * _buf)
{
  // -- REPLACE THIS!!!
  //SimpleDisk::read(_block_no, _buf);
  if(Machine::interrupts_enabled())
      Machine::disable_interrupts();


  issue_operation(WRITE, _block_no);
  //int mark = 0;
  //while(! is_ready() )            //(mark == 0)       
  //{    
      /* blocked_queue[blocked_tail] = Thread::CurrentThread();
       blocked_tail = (blocked_tail + 1) % MAX_NUMBER;
       blocked_thread_number ++;
       Console::puts("We blocked the current thread No. "); Console::puti(Thread::CurrentThread()->ThreadId()); Console::puts("\n");
       Console::puts("now we have  ");Console::puti( blocked_thread_number); Console::puts(" blocked thread\n");
       */
       Console::puts("Ooops, writing is not ready \n ");
      // SYSTEM_SCHEDULER->resume(Thread::CurrentThread());
       blocked_queue[blocked_tail] = Thread::CurrentThread();
       blocked_tail = (blocked_tail + 1) % MAX_NUMBER;
       blocked_thread_number ++;
       Console::puts("We blocked the current thread No. "); Console::puti(Thread::CurrentThread()->ThreadId()); Console::puts("\n");
       Console::puts("now we have  ");Console::puti( blocked_thread_number); Console::puts(" blocked thread\n");

       SYSTEM_SCHEDULER->yield();
// }
  
  if( is_ready())
  {
    Console::puts("good, writing is ready now \n ");
    int i;
    unsigned short tmpw;
    for (i = 0; i < 256; i++) 
    {
       tmpw = _buf[2*i] | (_buf[2*i+1] << 8);
       Machine::outportw(0x1F0, tmpw);
    }
  
    //blocked_head = ( blocked_head + 1) % MAX_NUMBER;
    //blocked_thread_number --;
  }

}
