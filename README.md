# **Philosophers**

A 42 school project on concurrency in C. It is based on Dijkstra's famous dining philosophers problem, illustrating the challenges of avoiding deadlocks while managing multiple threads and shared resources.

The Dining Philosophers problem is a classic synchronization challenge in computer science, illustrating the complexities of allocating limited resources (forks, in this case) among processes (philosophers) in a deadlock-free and starvation-free manner. This project implements a solution using threads and mutexes (in the standard version) and semaphores (in the bonus version).

## Status

Finished the 20/09/2023, Grade : 125%

## **Description**

- One or more philosophers sit at a round table.There is a large bowl of spaghetti in the middle of the table.
- The philosophers alternatively eat, think, or sleep - successively - never at the same time.
- There are also forks on the table. There are as many forks as philosophers.
- a philosopher takes their right and their left forks to eat, one in each hand.
- When a philosopher has finished eating, they put their forks back on the table and start sleeping. Once awake, they start thinking again. The simulation stops when
a philosopher dies of starvation.
- Every philosopher needs to eat and should never starve.
- Philosophers don’t speak with each other.
- Philosophers don’t know if another philosopher is about to die.
- Philosophers are sitting in a circular disposition : Philosopher number 1 sits next to philosopher number number_of_philosophers. Any other philosopher number N sits between philosopher number N - 1 and philosopher number N + 1.

the simulation program takes the following parameter

- **`number_of_philosophers`** The number of philosophers and also the number of forks.
- **`time_to_die`** (in milliseconds): If a philosopher didn’t start eating time_to_die milliseconds since the beginning of their last meal or the beginning of the simulation, they die.
- **`time_to_eat`** (in milliseconds): The time it takes for a philosopher to eat.
During that time, they will need to hold two forks.
- **`time_to_sleep`** (in milliseconds): The time a philosopher will spend sleeping.
- **`number_of_times_each_philosopher_must_eat (optional argument)`**: If all philosophers have eaten at least number_of_times_each_philosopher_must_eat times, the simulation stops. If not specified, the simulation stops when a philosopher dies.

## **Project Structure**

The project is divided into two parts:

- **Standard Version (`philo/`):** Uses pthreads and mutexes for synchronization.
- **Bonus Version (`philo_bonus/`):** Uses processes and semaphores.

### **Standard Version**

- **`philo.c`**: Main program entry : initializes the simulation, starting with parsing arguments and setting up the dining table environment for the philosophers
- **`init.c`**: Initialization of simulation. Responsible for initializing the mutexes, philosophers, and forks. It sets up the data structures and ensures that everything is correctly linked
- **`parsing.c`**: Argument parsing.
- **`philosopher_life.c`**: Defines the lifecycle of a philosopher. Contains the logic for the philosophers' lifecycle, including eating, thinking, and sleeping routines.
- **`simulation.c`**: Manages the simulation's execution, including starting threads for each philosopher and monitoring their status to detect any philosopher's death or the completion of dining
- **`utils.c`**, **`exit.c`**: Utility and cleanup functions.

### **Bonus Version**

- **`philo_bonus.c`**: Main program entry for the semaphore version.
- **`parsing_bonus.c`**, **`philosophers_bonus.c`**, **`simulation_bonus.c`**: Corresponding implementations using semaphores.
- **`utils_bonus.c`**, **`utils_bonus2.c`**: Utility functions.

## **Building the Project**

```
# For the standard version
cd philo && make

# For the bonus version
cd philo_bonus && make
```

## **Running the Simulation**

```
# Standard version
./philo n_philo time_die time_eat time_sleep [n_each_philo_must_eat]

# Bonus version
./philo_bonus n_philo time_die time_eat time_sleep [n_each_philo_must_eat]
```

# **Implementation Details**

**Synchronization Mechanisms Detailed**

- **Mutex Initialization and Destruction**: At the start, mutexes are initialized for each fork and for controlling access to shared state. They are destroyed and memory is freed upon simulation end to prevent resource leaks.
- **Locks for Resource Access**: Before a philosopher can change their state (e.g., start eating), they must acquire locks on the necessary resources (forks) and their own state to ensure consistency and prevent data races.
- **Condition Monitoring**: The simulation monitors the condition of each philosopher in a loop, utilizing mutexes to safely read and update shared state without conflict.

## Threads Used

### **1. Philosopher Threads**

- **Purpose**: Each philosopher in the simulation is represented by a thread. These threads run concurrently, simulating the philosophers' actions such as eating, thinking, and sleeping. The lifecycle of each philosopher (i.e., their routine of picking up forks, eating, putting down forks, thinking, and sleeping) is encapsulated within these threads.
- **Functionality**: Depending on the number of philosophers specified at the start of the simulation, a corresponding number of threads are created, each executing the **`philo_routine`** function or **`philone_routine`** in the case of a single philosopher. These routines control the behavior and state transitions of the philosophers, ensuring they follow the specified sequence of actions while also checking for conditions that might lead to their death (not eating within a certain time frame).

### **2. Simulation Monitor Thread**

- **Purpose**: This thread runs in parallel to the philosopher threads and is responsible for monitoring the state of the simulation. It checks whether any philosopher has died or if all philosophers have finished eating the specified number of meals (if a maximum number of meals is set).
- **Functionality**: The simulation monitor thread executes the **`simulation_routine`** function, which periodically checks the status of each philosopher to determine if the simulation conditions are still met. If a philosopher dies or all philosophers have completed their mea

## Mutexes Used

### **1. Forks Mutexes**

- **Purpose**: These mutexes represent the forks available on the table. Each fork has an associated mutex to ensure that only one philosopher can hold a fork at any given time, thus preventing race conditions when accessing shared fork resources.

### **2. Philosopher Mutex (`philo_lock`)**

- **Purpose**: Each philosopher has an associated mutex that is used to protect access to their state information, including whether they are eating, their death time, and their meal count. This ensures that read and write operations to a philosopher's state are mutually exclusive, preventing inconsistencies.
- **State Monitored**: This mutex monitors and protects the individual state of each philosopher, such as their **`is_eating`**, **`death_time`**, and **`meal_count`** attributes.

### **3. Write Lock (`write_lock`)**

- **Purpose**: This mutex is used to synchronize access to the standard output. It ensures that messages from different philosophers do not interleave on the console, providing clear and sequential output regarding the actions and states of philosophers.
- **State Monitored**: It does not directly monitor a state but ensures sequential access to the console for output operations.

### **4. Death Lock (`death_lock`)**

- **Purpose**: Used to protect access to the shared simulation state that indicates whether the simulation should continue or if it has been terminated due to a philosopher's death or completion of the simulation conditions.
- **State Monitored**: This mutex monitors and manages access to the simulation's overall state, particularly the **`is_over`** flag within the **`t_table`** structure, which indicates if the simulation should end.

### **5. Time Lock (`time_lock`)**

- **Purpose**: This mutex is used to synchronize access to the simulation's start time. It ensures that the calculation of time elapsed since the start of the simulation is consistent across all threads.
- **State Monitored**: It protects access to the **`start_time`** attribute within the **`t_table`** structure, ensuring accurate and consistent timing calculations for actions and state changes of philosophers.
