#include <bits/stdc++.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <random>
using namespace std;
// create struct for storing 3 different datatypes
struct Flight
{
    int id;
    string type;
    bool isEmergency;
};
// queue for lights
queue<Flight> landingQ;
queue<Flight> takeoffQ;
queue<Flight> emergencyQ;

// Semaphores
sem_t runway;

// this two semaphore woking like, there is space in airport parking or not
sem_t landingSlots;
sem_t takeoffSlots;

// two mutex , one for locking the queue updating and onother for printing lock
pthread_mutex_t qLock;
pthread_mutex_t printLock;

bool running = true;

// just printing the msg if the fuction call
void printMessage(const string &msg)
{
    pthread_mutex_lock(&printLock);
    cout << msg << endl;
    pthread_mutex_unlock(&printLock);
}

// landing, takeoff, emermency those three function just push the flight in the queue
void addLanding(Flight f)
{
    sem_wait(&landingSlots);

    pthread_mutex_lock(&qLock);
    landingQ.push(f);
    pthread_mutex_unlock(&qLock);

    printMessage("Flight " + to_string(f.id) + " in LANDING queue");
}

void addTakeoff(Flight f)
{
    sem_wait(&takeoffSlots);

    pthread_mutex_lock(&qLock);
    takeoffQ.push(f);
    pthread_mutex_unlock(&qLock);

    printMessage("Flight " + to_string(f.id) + " in TAKEOFF queue");
}

void addEmergency(Flight f)
{
    pthread_mutex_lock(&qLock);
    emergencyQ.push(f);
    pthread_mutex_unlock(&qLock);

    printMessage("EMERGENCY Flight " + to_string(f.id) + " added");
}

// this function check which time of filght it is then call the function
void *flightThread(void *arg)
{
    Flight f = *(Flight *)arg;

    if (f.isEmergency)
    {
        addEmergency(f);
    }
    else if (f.type == "LANDING")
    {
        addLanding(f);
    }
    else
    {
        addTakeoff(f);
    }

    return NULL;
}

// this is the main controller to control all the flights
void *controllerThread(void *arg)
{
    printMessage("\nController started\n\n");

    // infinite loop
    while (true)
    {
        Flight f;
        bool found = false;
        // here just emergency > landing > takeoff

        // first check if there is any emergency flight or not...of found any flight then store in f variable then  found = true for complete the flight as soon as possible
        pthread_mutex_lock(&qLock);
        if (!emergencyQ.empty())
        {
            f = emergencyQ.front();
            emergencyQ.pop();
            found = true;
        }
        pthread_mutex_unlock(&qLock);

        // if emergency flight exist then just check runway is available or not
        if (found)
        {
            sem_wait(&runway);

            printMessage("EMERGENCY Flight " + to_string(f.id) + " ON RUNWAY");
            sleep(2);
            printMessage("EMERGENCY Flight " + to_string(f.id) + " DONE");

            sem_post(&runway);
            continue;
        }

        // landing and takeoff similer to the emergency flight checking, if there is fight exist then take that and pop that queue , after the if runway available then done the filght
        pthread_mutex_lock(&qLock);
        if (!landingQ.empty())
        {
            f = landingQ.front();
            landingQ.pop();
            found = true;
        }
        else
        {
            found = false;
        }

        pthread_mutex_unlock(&qLock);

        if (found)
        {

            sem_post(&landingSlots);

            sem_wait(&runway);

            printMessage("Flight " + to_string(f.id) + " LANDING");
            sleep(2);
            printMessage("Flight " + to_string(f.id) + " LANDED");

            sem_post(&runway);
            continue;
        }
        pthread_mutex_lock(&qLock);
        if (!takeoffQ.empty())
        {
            f = takeoffQ.front();
            takeoffQ.pop();
            found = true;
        }
        else
        {
            found = false;
        }
        pthread_mutex_unlock(&qLock);

        if (found)
        {

            sem_post(&takeoffSlots);

            sem_wait(&runway);

            printMessage("Flight " + to_string(f.id) + " TAKING OFF");
            sleep(2);
            printMessage("Flight " + to_string(f.id) + " TOOK OFF");

            sem_post(&runway);
            continue;
        }

        // very important part if all the queue is empty then bool empty is true , if empty is true break from the infinite loop
        pthread_mutex_lock(&qLock);
        bool empty = emergencyQ.empty() && landingQ.empty() && takeoffQ.empty();
        pthread_mutex_unlock(&qLock);

        if (empty)
        {
            // here break it
            break;
        }
        sleep(3);
    }

    running = false;
    printMessage("\nController stopped\n\n");

    return NULL;
}

int main()
{

    srand(time(NULL));

    cout << "\n========== AIRPORT RUNWAY SYSTEM ==========\n"
         << endl;

    sem_init(&runway, 0, 1);
    sem_init(&landingSlots, 0, 5);
    sem_init(&takeoffSlots, 0, 5);

    pthread_mutex_init(&qLock, NULL);
    pthread_mutex_init(&printLock, NULL);

    vector<pthread_t> threads;
    // just concern about three type of flights
    vector<string> possibleoperation = {
        "LANDING",
        "TAKEOFF",
        "EMERGENCY"};

    // first I put all the flight in queue then i start the controller to handle those flights
    // I just randomly taking the flights
    for (int i = 0; i < 10; i++)
    {
        int val = rand() % 3;
        bool emergency = (possibleoperation[val] == "EMERGENCY");
        Flight *f = new Flight{100 + i, possibleoperation[val], emergency};
        pthread_t t;
        pthread_create(&t, NULL, flightThread, f);
        threads.push_back(t);
    }

    for (auto &t : threads)
    {
        pthread_join(t, NULL);
    }

    cout << "\nAll flights created and queued." << endl;

    // after finishing the flight creation then start the controller
    pthread_t controller;
    pthread_create(&controller, NULL, controllerThread, NULL);
    pthread_join(controller, NULL);

    sem_destroy(&runway);
    sem_destroy(&landingSlots);
    sem_destroy(&takeoffSlots);

    pthread_mutex_destroy(&qLock);
    pthread_mutex_destroy(&printLock);
    // I try to make this project as simple as possbile, if you think somewhere need improvement then you can say me ^_^ .
    cout << "\nSimulation complete!" << endl;

    return 0;
}