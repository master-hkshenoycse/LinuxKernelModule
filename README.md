# Peeking into Linux - Linux Kernel Modules  

This repository contains Linux Kernel Modules (LKMs) that provide insights into the Linux kernel by examining process states, memory usage, and address translation. These modules interact with key kernel structures such as `task_struct`, `mm_struct`, and `vm_area_struct` to retrieve system-level information.  

## Features  

- **List Running/Runnable Processes** – Traverses the process list and prints tasks in running or runnable states.  
- **Find Process with Maximum Heap Usage** – Identifies the process consuming the most heap memory.  
- **Kernel Stack Pointer of PID 1** – Retrieves the kernel stack pointer of the task with PID 1.  
- **Virtual-to-Physical Address Translation** – Determines if a given virtual address is mapped and retrieves its physical address.  
- **Process Memory Usage** – Computes virtual and physical memory usage for a specified process.  
- **Lazy Memory Allocation Demonstration** – Shows how Linux allocates memory lazily.
