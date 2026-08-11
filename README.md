# Airport Runway Management System

A simple **Operating Systems project** built with **C++ and POSIX Threads (pthreads)** that simulates airport runway management using threads, mutexes, semaphores, queues, and priority scheduling.

## Features

* Multithreading using POSIX threads
* Landing and takeoff queues
* Priority queue for emergency flights
* Emergency flights get higher priority
* Semaphore-based runway management
* Mutex-based thread synchronization
* Limited landing and takeoff queue capacity
* Thread-safe console output
* Simulated flight operations

## Operating System Concepts

This project demonstrates:

* **Multithreading**
* **Mutex**
* **Semaphore**
* **Critical Section**
* **Thread Synchronization**
* **Priority Scheduling**
* **Shared Resource Management**
* **Queue Management**

## How It Works

Each flight is created as a separate thread.

A flight can be:

* `LANDING`
* `TAKEOFF`
* `EMERGENCY`

Normal flights are added to their respective queues, while emergency flights are added to a priority queue.

The controller manages the runway according to the following priority:

```text
EMERGENCY
    ↓
LANDING
    ↓
TAKEOFF
```

Emergency flights are assigned a priority value. A flight with a higher priority value is processed first.

## Synchronization

### Runway Semaphore

The runway is represented by a semaphore with an initial value of `1`.

```cpp
sem_init(&runway, 0, 1);
```

This ensures that only **one flight can use the runway at a time**.

### Mutex

A mutex protects the shared queues from simultaneous access by multiple threads.

```cpp
pthread_mutex_lock(&qLock);
```

Another mutex is used to prevent multiple threads from printing to the console simultaneously.


## Requirements

* C++
* C++11 or later
* POSIX Threads (`pthread`)
* Linux, WSL, or an environment supporting pthreads


## Example Output

```text
========== AIRPORT RUNWAY SYSTEM ==========

Flight 100 entered LANDING queue
Flight 101 entered TAKEOFF queue
EMERGENCY Flight 102 added with priority 8
Flight 103 entered LANDING queue
EMERGENCY Flight 104 added with priority 5

Controller started

EMERGENCY Flight 102 Priority 8 ON RUNWAY
EMERGENCY Flight 102 DONE

EMERGENCY Flight 104 Priority 5 ON RUNWAY
EMERGENCY Flight 104 DONE

Flight 100 LANDING
Flight 100 LANDED

Flight 101 TAKING OFF
Flight 101 TOOK OFF

Controller stopped

Simulation complete!
```

## Purpose

The purpose of this project is to demonstrate how **Operating System synchronization and scheduling concepts** can be applied to a real-world airport runway management scenario.

## Author

**Md. Sahidul Islam**

Computer Science & Engineering
East West University
