#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

#define MAX_INTERSECTIONS 10
#define BASE_GREEN_DURATION 20
#define YELLOW_DURATION 5

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
    int green_ns;
    int green_ew;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Intersection;

// Global variables
Intersection intersections[MAX_INTERSECTIONS];
int active_intersections = 0;

// Function prototypes
void initializeIntersection(int id);
void* controlIntersection(void* arg);
void updateTrafficDensity(Intersection* intersection);
void adjustTiming(Intersection* intersection);
const char* getStateString(LightState state);
void displayIntersectionStatus(Intersection* intersection);
void cleanupIntersection(Intersection* intersection);
void logStatus(Intersection* intersection);

void initializeIntersection(int id) {
    intersections[id].id = id;
    intersections[id].northSouth = RED;
    intersections[id].eastWest = GREEN;
    intersections[id].isActive = true;
    intersections[id].traffic_density_ns = rand() % 11;
    intersections[id].traffic_density_ew = rand() % 11;
    intersections[id].green_ns = BASE_GREEN_DURATION;
    intersections[id].green_ew = BASE_GREEN_DURATION;
    pthread_mutex_init(&intersections[id].mutex, NULL);
    pthread_cond_init(&intersections[id].cond, NULL);
}

void* controlIntersection(void* arg) {
    Intersection* intersection = (Intersection*)arg;
    while (intersection->isActive) {
        pthread_mutex_lock(&intersection->mutex);
        updateTrafficDensity(intersection);
        adjustTiming(intersection);
        
        // North-South Green
        intersection->northSouth = GREEN;
        intersection->eastWest = RED;
        displayIntersectionStatus(intersection);
        logStatus(intersection);
        pthread_mutex_unlock(&intersection->mutex);
        sleep(intersection->green_ns);
        
        pthread_mutex_lock(&intersection->mutex);
        intersection->northSouth = YELLOW;
        displayIntersectionStatus(intersection);
        logStatus(intersection);
        pthread_mutex_unlock(&intersection->mutex);
        sleep(YELLOW_DURATION);
        
        // East-West Green
        pthread_mutex_lock(&intersection->mutex);
        intersection->northSouth = RED;
        intersection->eastWest = GREEN;
        displayIntersectionStatus(intersection);
        logStatus(intersection);
        pthread_mutex_unlock(&intersection->mutex);
        sleep(intersection->green_ew);
        
        pthread_mutex_lock(&intersection->mutex);
        intersection->eastWest = YELLOW;
        displayIntersectionStatus(intersection);
        logStatus(intersection);
        pthread_mutex_unlock(&intersection->mutex);
        sleep(YELLOW_DURATION);
    }
    return NULL;
}

void updateTrafficDensity(Intersection* intersection) {
    intersection->traffic_density_ns = rand() % 11;
    intersection->traffic_density_ew = rand() % 11;
}

void adjustTiming(Intersection* intersection) {
    if (intersection->traffic_density_ns > intersection->traffic_density_ew) {
        intersection->green_ns = BASE_GREEN_DURATION + 5;
        intersection->green_ew = BASE_GREEN_DURATION - 5;
    } else {
        intersection->green_ns = BASE_GREEN_DURATION - 5;
        intersection->green_ew = BASE_GREEN_DURATION + 5;
    }
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
    printf("North-South: %s (Traffic Density: %d/10, Green Time: %ds)\n", 
           getStateString(intersection->northSouth),
           intersection->traffic_density_ns,
           intersection->green_ns);
    printf("East-West: %s (Traffic Density: %d/10, Green Time: %ds)\n",
           getStateString(intersection->eastWest),
           intersection->traffic_density_ew,
           intersection->green_ew);
    printf("----------------------------------------\n");
}

void logStatus(Intersection* intersection) {
    FILE* logFile = fopen("traffic_log.txt", "a");
    if (logFile) {
        fprintf(logFile, "Intersection %d: NS: %s (%d), EW: %s (%d)\n",
                intersection->id,
                getStateString(intersection->northSouth), intersection->traffic_density_ns,
                getStateString(intersection->eastWest), intersection->traffic_density_ew);
        fclose(logFile);
    }
}

void cleanupIntersection(Intersection* intersection) {
    intersection->isActive = false;
    pthread_mutex_destroy(&intersection->mutex);
    pthread_cond_destroy(&intersection->cond);
}

int main() {
    srand(time(NULL));
    pthread_t threads[MAX_INTERSECTIONS];
    int num_intersections;
    
    printf("Enter number of intersections (max %d): ", MAX_INTERSECTIONS);
    scanf("%d", &num_intersections);
    if (num_intersections > MAX_INTERSECTIONS || num_intersections <= 0) {
        num_intersections = 3; // Default value
    }
    
    printf("Initializing %d intersections...\n\n", num_intersections);
    
    for (int i = 0; i < num_intersections; i++) {
        initializeIntersection(i);
        pthread_create(&threads[i], NULL, controlIntersection, &intersections[i]);
        active_intersections++;
    }
    
    sleep(120);
    
    printf("\nShutting down simulator...\n");
    for (int i = 0; i < active_intersections; i++) {
        cleanupIntersection(&intersections[i]);
        pthread_join(threads[i], NULL);
    }
    
    printf("Simulation ended successfully.\n");
    return 0;
}
