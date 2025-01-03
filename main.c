#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

#define MAX_INTERSECTIONS 10
#define RED_DURATION 30
#define YELLOW_DURATION 5
#define GREEN_DURATION 25

typedef enum {
    RED,
    YELLOW,
    GREEN
} LightState;

typedef struct {
    LightState northSouth;
    LightState eastWest;
    int id;
    bool isActive;
    int traffic_density_ns;
    int traffic_density_ew;
    pthread_mutex_t mutex;
} Intersection;

// Global variables
Intersection intersections[MAX_INTERSECTIONS];
int active_intersections = 0;

// Function prototypes
void initializeIntersection(int id);
void* controlIntersection(void* arg);
void updateTrafficDensity(Intersection* intersection);
const char* getStateString(LightState state);
void displayIntersectionStatus(Intersection* intersection);
void cleanupIntersection(Intersection* intersection);

void initializeIntersection(int id) {
    intersections[id].id = id;
    intersections[id].northSouth = RED;
    intersections[id].eastWest = GREEN;
    intersections[id].isActive = true;
    intersections[id].traffic_density_ns = 0;
    intersections[id].traffic_density_ew = 0;
    pthread_mutex_init(&intersections[id].mutex, NULL);
}

void* controlIntersection(void* arg) {
    Intersection* intersection = (Intersection*)arg;
    int cycle_count = 0;
    
    while (intersection->isActive) {
        pthread_mutex_lock(&intersection->mutex);
        
        // Update traffic density every 5 cycles
        if (cycle_count % 5 == 0) {
            updateTrafficDensity(intersection);
        }
        
        // Main traffic light logic
        if (intersection->northSouth == GREEN) {
            sleep(GREEN_DURATION);
            intersection->northSouth = YELLOW;
        } else if (intersection->northSouth == YELLOW) {
            sleep(YELLOW_DURATION);
            intersection->northSouth = RED;
            intersection->eastWest = GREEN;
        } else if (intersection->eastWest == GREEN) {
            sleep(GREEN_DURATION);
            intersection->eastWest = YELLOW;
        } else if (intersection->eastWest == YELLOW) {
            sleep(YELLOW_DURATION);
            intersection->eastWest = RED;
            intersection->northSouth = GREEN;
        }
        
        displayIntersectionStatus(intersection);
        cycle_count++;
        
        pthread_mutex_unlock(&intersection->mutex);
    }
    
    return NULL;
}

void updateTrafficDensity(Intersection* intersection) {
    // Simulate random traffic density (0-10)
    intersection->traffic_density_ns = rand() % 11;
    intersection->traffic_density_ew = rand() % 11;
}

const char* getStateString(LightState state) {
    switch (state) {
        case RED: return "RED";
        case YELLOW: return "YELLOW";
        case GREEN: return "GREEN";
        default: return "UNKNOWN";
    }
}

void displayIntersectionStatus(Intersection* intersection) {
    printf("\nIntersection %d Status:\n", intersection->id);
    printf("North-South: %s (Traffic Density: %d/10)\n", 
           getStateString(intersection->northSouth),
           intersection->traffic_density_ns);
    printf("East-West: %s (Traffic Density: %d/10)\n",
           getStateString(intersection->eastWest),
           intersection->traffic_density_ew);
    printf("----------------------------------------\n");
}

void cleanupIntersection(Intersection* intersection) {
    intersection->isActive = false;
    pthread_mutex_destroy(&intersection->mutex);
}

int main() {
    srand(time(NULL));
    pthread_t threads[MAX_INTERSECTIONS];
    
    // Initialize and start intersections
    printf("Traffic Light Simulator Starting...\n");
    printf("Initializing 3 intersections...\n\n");
    
    for (int i = 0; i < 3; i++) {
        initializeIntersection(i);
        pthread_create(&threads[i], NULL, controlIntersection, &intersections[i]);
        active_intersections++;
    }
    
    // Run simulation for 2 minutes
    sleep(120);
    
    // Cleanup and exit
    printf("\nShutting down simulator...\n");
    for (int i = 0; i < active_intersections; i++) {
        cleanupIntersection(&intersections[i]);
        pthread_join(threads[i], NULL);
    }
    
    printf("Simulation ended successfully.\n");
    return 0;
}
