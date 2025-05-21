#include "defs.h"
#include "log.h"

// checkpoint 3: Dining philosophers problem

#define ROUNDS 50000
#define NTHRS  5

static uint64 count;
static int eating[NTHRS];
static int pids[NTHRS];

// TODO: Define semaphores you need here.

static void eat(uint64 id) {
    int left = (id + NTHRS - 1) % NTHRS;
    int right = (id + 1) % NTHRS;

    yield();
    __atomic_store_n(&eating[id], 1, __ATOMIC_RELEASE);
    yield();
    int lefteating = __atomic_load_n(&eating[left], __ATOMIC_ACQUIRE);
    yield();
    int righteating = __atomic_load_n(&eating[right], __ATOMIC_ACQUIRE);

    // if i'm eating, then the left and right forks are used by me.
    // so the left and right philosophers cannot eat.

    assert_str(
        lefteating == 0 && righteating == 0, "philosopher %d eats with left %d and right %d", id,
        lefteating, righteating);

    uint64 value = __atomic_add_fetch(&count, 1, __ATOMIC_RELEASE);
    if (value % 1000 == 0) {
        infof("philosophers eat %d times in total", value);
    }
    yield();

    __atomic_store_n(&eating[id], 0, __ATOMIC_RELEASE);
}

static void philosopher(uint64 id) {
    int left = (id + NTHRS - 1) % NTHRS;
    int right = id;

    while (__atomic_load_n(&count, __ATOMIC_ACQUIRE) <= ROUNDS) {
        // implement your Dining philosophers problem here.

        // ==== pick up the left and the right forks ====
        // TODO: Your code here.

        // ==== now the philosopher can eat ====
        eat(id);

        // ==== put down two forks ====
        // TODO: Your code here.

    }
    exit(0);
}

void checkpoint3(void) {
    count = 0;
    memset(eating, 0, sizeof(eating));
    
    // TODO: Initialize semaphores here.


    infof("-> checkpoint 3 starts");

    for (int i = 0; i < NTHRS; i++) {
        pids[i] = create_kthread(philosopher, i);
    }

    // wait for all threads to finish
    for (int i = 0; i < NTHRS; i++) {
        wait(pids[i], NULL);
    }

    infof("-> checkpoint 3 passed");
}
