/*
 * Priority Queue implemented with a min binary heap as underlying data structure
 *
 * Supports insert(), extract(), peek(), remove()
 * */
// TODO use hash table to speed up operations
// TODO make size dynamic
// TODO move to seperare header and c file
// TODO write large scale test
// TODO look at other efficiency improvements
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

struct PriorityQueue {
    // TODO use size_t or whatever 
    // TODO make size dynamic
    int heap[1024]; 
    int size;
    int capacity;
};

struct PriorityQueue init_PriorityQueue()
{
    struct PriorityQueue priority_queue;
    priority_queue.size = 0;
    priority_queue.capacity = 1024;
    return priority_queue;
}


int get_parent_index(int i)
{
    return (i - 1) / 2;
}

int get_left_child_index(int i)
{
    return (i * 2) + 1;
}

int get_right_child_index(int i)
{
    return (i * 2) + 2;
}


void bubble_up_PriorityQueue(struct PriorityQueue *priority_queue, int index)
{
    int parent_index, value, parent_value;
    // return if this is the first element
    if (index == 0) {
        return;
    }
    parent_index = get_parent_index(index);
    value = priority_queue->heap[index];
    parent_value = priority_queue->heap[parent_index];
    // return if we've bubbled up as far as possible
    if (value > parent_value)
        return;
    // swap current node value with parent value and bubble up further 
    priority_queue->heap[index] = parent_value;
    priority_queue->heap[parent_index] = value;
    bubble_up_PriorityQueue(priority_queue, parent_index);
}

int get_bubble_down_index_PriorityQueue(struct PriorityQueue *priority_queue, int index)
{
    int left_index, right_index, left_value, right_value, highest_index;
    highest_index = priority_queue->size - 1;  // convenience variable
    left_index = get_left_child_index(index);
    right_index = get_right_child_index(index);
    // return if the node has no children
    if (left_index > highest_index && right_index > highest_index)
        return -1;
    if (left_index > highest_index)
        return right_index;
    if (right_index > highest_index)
        return left_index;
    right_value = priority_queue->heap[right_index];
    left_value = priority_queue->heap[left_index];
    if (left_value < right_value)
        return left_index;
    else
        return right_index;

}

void bubble_down_PriorityQueue(struct PriorityQueue *priority_queue, int index)
{
    int bubble_down_index, value, child_value;
    bubble_down_index = get_bubble_down_index_PriorityQueue(priority_queue, index);
    // return if no children
    if (bubble_down_index == -1)
        return;
    // otherwise swap the values and bubble down again
    value = priority_queue->heap[index];
    child_value = priority_queue->heap[bubble_down_index];
    // return if we've bubbled up as far as possible
    if (child_value > value)
        return;
    // swap current node value with parent value and bubble up further 
    priority_queue->heap[index] = child_value;
    priority_queue->heap[bubble_down_index] = value;
    bubble_down_PriorityQueue(priority_queue, bubble_down_index);
}


void insert_PriorityQueue(struct PriorityQueue *priority_queue, int n)
{
    // add to end of heap
    priority_queue->heap[priority_queue->size] = n;
    priority_queue->size++;  // TODO could be done in the operation above. what's good practice?
    bubble_up_PriorityQueue(priority_queue, priority_queue->size - 1);
}

int extract_PriorityQueue(struct PriorityQueue *priority_queue)
{
    // TODO handle get from empty queue
    int return_value;
    // swap root element with leftmost element on bottom level and remove it
    return_value = priority_queue->heap[0];
    priority_queue->heap[0] = priority_queue->heap[priority_queue->size - 1];
    priority_queue->size--;
    // bubble down new root element
    bubble_down_PriorityQueue(priority_queue, 0);
    return return_value;
}

int peek_PriorityQueue(struct PriorityQueue *priority_queue)
{
    // TODO handle empty queue
    return priority_queue->heap[0];
}
 
void remove_PriorityQueue(struct PriorityQueue *priority_queue, int n)
{
    // TODO should this remove all instances of n or just one? currently just one
    int i, remove_index = -1;
    // search heap for value
    for (i = 0; i < priority_queue->size; ++i)
    {
        if (priority_queue->heap[i] == n)
        {
            remove_index = i;
            break;
        }
    }
    if (remove_index == -1)
        return;
    // move last value to swap index and remove last value
    priority_queue->heap[remove_index] = priority_queue->heap[priority_queue->size - 1];
    priority_queue->size--;
    // bubble up and bubble down the swapped index
    bubble_up_PriorityQueue(priority_queue, remove_index);
    bubble_down_PriorityQueue(priority_queue, remove_index);
}

void debug_print(struct PriorityQueue *priority_queue)
{
    for (int i = 0; i < priority_queue->size; ++i)
    {
        printf("%d ", priority_queue->heap[i]);
    }
    printf("\n");
}

int main()
{
    struct PriorityQueue priority_queue = init_PriorityQueue();

    srand(time(NULL));
    for (int i = 0; i < 20; ++i)
    {
        insert_PriorityQueue(&priority_queue, 20 - i);
    }
    remove_PriorityQueue(&priority_queue, 4);
    remove_PriorityQueue(&priority_queue, 7);
    remove_PriorityQueue(&priority_queue, 11);
    remove_PriorityQueue(&priority_queue, 18);
    printf("Peek %d\n", peek_PriorityQueue(&priority_queue));
    while (priority_queue.size)
    {
        printf("%d\n", extract_PriorityQueue(&priority_queue));
    }
    return 0;
}
