#include<iostream>
#include<iomanip>
#include<ctime>
#include<cstdlib>

#include "macros.h"

const int NUM_HOURS = 1000;
const int UNLOAD_RATE_SH = 100; // Tons per hour
const int UNLOAD_RATE_WH = 20; // Tons per hour
const int NUM_PIERS = 15;
const int MAX_QUEUE_SIZE = 30;
const int MIN_SHIP_CARGO = 100;
const int MAX_SHIP_CARGO = 500;
const int MIN_WAREHOUSE_CARGO = 20000;
const int MAX_WAREHOUSE_CARGO = 30000;
const int MIN_ARRIVALS = 0;
const int MAX_ARRIVALS = 3;
const int SLEEP_DURATION = 50;

struct CargoType {
    int ship;
    int warehouse;
    int capacity;
};

struct Pier {
    Pier();
    static void shipToWarehouse(int&, int&, int);
    static void unloadWarehouse(int&);
    static void assignRandomCargoSize(CargoType&);
    static void initializeCargoType(CargoType&);
    void print(int);

    bool isEmpty() {return !(A.ship && B.ship && C.ship && D.ship);};
    int getSpaceAvailable(CargoType t) {return t.capacity - t.warehouse;};

    CargoType A;
    CargoType B;
    CargoType C;
    CargoType D;
};

int randInt(int, int);

void queueIncomingShips(int&, int&);
void operatePort(Pier[], int&);

void assignShips(int&, Pier[]);
void offloadShips(Pier[]);
void offload(Pier&, CargoType&);
void printPiers(Pier[]);
void unloadWarehouses(Pier[]);

Pier::Pier(void) {
    initializeCargoType(A);
    initializeCargoType(B);
    initializeCargoType(C);
    initializeCargoType(D);
}

void Pier::initializeCargoType(CargoType& t) {
    t.capacity = randInt(MIN_WAREHOUSE_CARGO, MAX_WAREHOUSE_CARGO);
    t.ship = 0;
    t.warehouse = 0;
}

void Pier::print(int n) {
    std::cout << "Pier "
              << std::setw(2) << std::setfill('0') << n << "       <"
              << std::setw(3) << std::setfill('0') << A.ship  << ","
              << std::setw(3) << std::setfill('0') << B.ship  << ","
              << std::setw(3) << std::setfill('0') << C.ship  << ","
              << std::setw(3) << std::setfill('0') << D.ship
              << ">";
    std::cout << "         |"
              << std::setw(5) << std::setfill('0') << A.warehouse
                              << "/" << A.capacity << "   "
              << std::setw(5) << std::setfill('0') << B.warehouse
                              << "/" << B.capacity<< "   "
              << std::setw(5) << std::setfill('0') << C.warehouse
                              << "/" << C.capacity<< "   "
              << std::setw(5) << std::setfill('0') << D.warehouse
                              << "/" << D.capacity
              << "|" << std::endl;
}

void Pier::assignRandomCargoSize(CargoType& t) {
    t.ship = randInt(MIN_SHIP_CARGO, MAX_SHIP_CARGO);
}

void Pier::shipToWarehouse(int& s, int& w, int a) {
    // *** Determine the minimum of space in warehouse, cargo on ship, and unload rate.
    int toMove = std::min(std::min(s, UNLOAD_RATE_SH), a);
    // *** Subtract this from cargo on ship and add to cargo in warehouse
    s -= toMove;
    w += toMove;
}

void Pier::unloadWarehouse(int& w) {
    // ** Subtract WAREHOUSE_UNLOAD_RATE from cargo in warehouse.
    w = (w - UNLOAD_RATE_WH) > 0 ? (w - UNLOAD_RATE_WH) : 0;
}

int main() {
    Pier piers[NUM_PIERS];
    int ships_in_queue, num_arrivals;
    num_arrivals = ships_in_queue = 0;

    srand(time(NULL));

    for (int t = 0; t < NUM_HOURS; t++) {
        // * Ships arrive at queue.
        queueIncomingShips(num_arrivals, ships_in_queue);
        operatePort(piers, ships_in_queue);
        SLEEP(SLEEP_DURATION);
        CLEAR_WINDOW;
    }

    return 0;
}

void queueIncomingShips(int& n, int& q) {
    // ** Generate a random number between 0 and 3.
    n = randInt(MIN_ARRIVALS, MAX_ARRIVALS);
    // ** Add number of ships arriving to the number currently in the queue.
    // *** If the maximum queue size is met, ignore the incoming ships.
    q = (q + n > 30) ? 30 : (q + n);
}

void operatePort(Pier p[], int& q) {
    // * Dock ship.
    assignShips(q, p);
    // * Move cargo from ship to warehouse.
    offloadShips(p);
    printPiers(p);
    // * Move cargo out of warehouse.
    unloadWarehouses(p);
}

void printPiers(Pier p[]) {
    std::cout << std::setw(23) << std::setfill(' ') << "Ships"
              << std::setw(53) << "Warehouses" << std::endl;
    for (int i = 0; i < NUM_PIERS; i++) {
        p[i].print(i);
    }
}

int randInt(int min, int max) {
    return min + (std::rand() % (max - min + 1));
}

void assignShips(int& q, Pier p[]) {
    // ** Iterate through each pier until an open one is found, if found, assign cargo to port.

    for (int i = 0; i < NUM_PIERS; i++)
        // *** If there is at least one ship in the queue, look for an open pier and dock.
        if (p[i].isEmpty() && (q > 0)) {
            // **** Generate a random number between 100 and 500 for each cargo type.
            p[i].assignRandomCargoSize(p[i].A);
            p[i].assignRandomCargoSize(p[i].B);
            p[i].assignRandomCargoSize(p[i].C);
            p[i].assignRandomCargoSize(p[i].D);
            // **** Decrement queue size by 1.
            q--;
        }
}

void offload(Pier& p, CargoType& t) {
    p.shipToWarehouse(t.ship, t.warehouse, p.getSpaceAvailable(t));
}

void offloadShips(Pier p[]) {
    // ** Iterate through all piers..
    for (int i = 0; i < NUM_PIERS; i++) {
        // ** Determine the amount of cargo to move from ship to warehouse.
        offload(p[i], p[i].A);
        offload(p[i], p[i].B);
        offload(p[i], p[i].C);
        offload(p[i], p[i].D);
    }
}

void unloadWarehouses(Pier p[]) {
    // ** Iterate through all piers.
    for (int i = 0; i < NUM_PIERS; i++) {
        p[i].unloadWarehouse(p[i].A.warehouse);
        p[i].unloadWarehouse(p[i].B.warehouse);
        p[i].unloadWarehouse(p[i].C.warehouse);
        p[i].unloadWarehouse(p[i].D.warehouse);
    }
}
