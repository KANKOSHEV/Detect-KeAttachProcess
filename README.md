# Detect-KeAttachProcess
Detect-KeAttachProcess - by iterating through all processes as well as checking the context of the thread.

Recently I started studying the KeAttachProcess system. 
And to be more specific, how exactly can you find out that your process is attached. 
And so, as soon as we start looking at the function, 
we see that it first gets the current context of the thread from which Keattachprocess was called.

![Alt text](https://i.ibb.co/WDYWQhX/scr1.png "Optional title")

Also note that the PROCESS is passed to the "v2" variable from the argument, 
respectively, this is the target process to which the function is attached in the future.

The current thread of the process from which the function is called, 
as well as the PROCESS from the argument, are passed to the "KiAttachProcess" function.

![Alt text](https://i.ibb.co/hWZXj5t/image.png "Optional title")

We switch to this function, and after studying it a little, 
we see that the target process (v2) is written to offset "0xB8" the current thread context.

![Alt text](https://i.ibb.co/1LVbv2Y/image.png "Optional title")

The further plan is as follows: 

1) Go through all the processes 
2) Go through all the threads of each process in the system 
3) Look in the context of each thread, what is the offset "0xB8".

If the current process is not attached to anything, 
usually the current "PEPROCESS" of the process itself is stored in the offset "0xB8", 
and if the current process was attached to any process,
then the "PEPROCESS" target process will be written to the offset "0xB8".

I have implemented a check for this for every process in the system, and here is my result:

![Alt Text](https://i.ibb.co/Gvm1gsX/image.png)
