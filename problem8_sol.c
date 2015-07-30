#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define BFS 1

#define CAPACITY 100000
#define MIN(x,y) ((x) < (y) ? (x) : (y))

typedef struct
{
    // the index of the first element in the queue
    int head;

    // storage for the elements in the queue
    int states[CAPACITY];

    // the size of the queue
    int size;
} queue;

queue q;

struct eastcity {
    int index;
    int pop;
};

struct flight {
    int origin;
    int dest;
    int cap;
    int time;
};

int N; // number of cities

int Wn; // number of west coast cities
int *Wlist; // list of weat coast cities (just index)

int En; // number of east coast cities
struct eastcity *Elist; // list of east coast city structs
int total_people = 0;

int Fn; // number of flights
struct flight *Flist;

#define INFINITY INT_MAX
#define NODE(n, t) (N*(t) + (n))

struct edge {
    int dest;
    int weight;
    struct edge *next;
};

struct edge **adjlist;
int *visited;
int *pred;
int cur_length = 0;

void add_edge(int i, int j, int weight) {
    struct edge *e = adjlist[i];
    struct edge *new;
    while (e != NULL) {
	if (e->dest == j)
	    break;
	e = e->next;
    }

    if (e != NULL)
	e->weight = weight;
    else {
	new = malloc(sizeof(struct edge));
	new->dest = j;
	new->weight = weight;
	new->next = adjlist[i];
	adjlist[i] = new;
    }
}

int get_edge_weight(int i, int j) {
    struct edge *e = adjlist[i];
    while (e != NULL) {
	if (e->dest == j)
	    return e->weight;
	e = e->next;
    }
    return 0;
}

void construct_graph(int time, int *sourceret, int *sinkret) {
    struct flight f;
    int duration, start_time;
    int num_nodes = (time+1)*N + 2;
    int source = num_nodes - 2;
    int sink = num_nodes - 1;
    adjlist = malloc(sizeof(struct edge*) * num_nodes);
    visited = malloc(sizeof(int) * num_nodes);
    pred = malloc(sizeof(int) * num_nodes);
    cur_length = num_nodes;
    
    for (int i = 0; i < En; i++)
	add_edge(source, NODE(Elist[i].index, 0), Elist[i].pop);
    for (int i = 0; i < Wn; i++)
	for (int t = 0; t <= time; t++)
	    add_edge(NODE(Wlist[i], t), sink, INFINITY);

    for (int n = 0; n < N; n++)
	for (int t = 0; t < time; t++)
	    add_edge(NODE(n, t), NODE(n, t+1), INFINITY);
    
    for (int i = 0; i < Fn; i++) {
	f = Flist[i];
	duration = Flist[i].time;
	start_time = 0;
	while (start_time + duration <= time) {
	    add_edge(NODE(f.origin, start_time), NODE(f.dest, start_time+duration), f.cap);
	    start_time++;
	}
    }

    *sourceret = source;
    *sinkret = sink;
}

void construct_initial_graph(int* sinkret) {
    struct flight f;
    int num_nodes = N + 1;
    int sink = num_nodes - 1;
    adjlist = malloc(sizeof(struct edge*) * num_nodes);
    visited = malloc(sizeof(int) * num_nodes);
    pred = malloc(sizeof(int) * num_nodes);
    cur_length = num_nodes;
    
    for (int i = 0; i < Wn; i++)
	add_edge(NODE(Wlist[i], 0), sink, INFINITY);

    for (int i = 0; i < Fn; i++) {
	f = Flist[i];
	add_edge(NODE(f.origin, 0), NODE(f.dest, 0), f.cap);
    }

    *sinkret = sink;    
}

int follow_pred(int source, int sink) {
    int curr = sink;
    int max_flow = INFINITY;
    int edge_weight;
    while (curr != source) {
	edge_weight = get_edge_weight(pred[curr], curr);
	max_flow = max_flow < edge_weight ? max_flow : edge_weight;
	curr = pred[curr];
    }
    return max_flow;
}

void update_graph(int source, int sink, int max_flow) {
    int curr = sink;
    int edge_weight, edge_weight_rev;
    while (curr != source) {
	edge_weight = get_edge_weight(pred[curr], curr);
	edge_weight_rev = get_edge_weight(curr, pred[curr]);
	add_edge(pred[curr], curr, edge_weight - max_flow);
	add_edge(curr, pred[curr], edge_weight_rev + max_flow);
	curr = pred[curr];
    }
}

int deq(void)
{
    int ind = q.states[q.head];

    q.head = (q.head + 1) % CAPACITY;
    q.size--;

    return ind;
}

void enq(int ind)
{
    int last = (q.head + q.size) % CAPACITY;
    q.states[last] = ind;
    q.size++;

    return;
}

int do_bfs(int source, int sink) {
    (void)memset(visited, 0, cur_length * sizeof(int));
    q.size = 0;
    q.head = 0;
    int ind;
    struct edge *e;
    enq(source);
    visited[source] = 1;
    while (q.size > 0) {
	ind = deq();
	if (ind == sink)
	    return 1;
	
	e = adjlist[ind];
	while (e != NULL) {
	    if (!visited[e->dest] && e->weight > 0) {
		visited[e->dest] = 1;
		pred[e->dest] = ind;
		enq(e->dest);
	    }
	    e = e->next;
	}
    }
    return 0;
}

int add_flow_bfs(int source, int sink) {
    if (!do_bfs(source, sink))
	return 0;

    int max_flow = follow_pred(source, sink);
    update_graph(source, sink, max_flow);
    return max_flow;
}


int dfs(int source, int sink) {
    if (source == sink)
	return 1;

    struct edge *e = adjlist[source];
    while (e != NULL) {
	if (!visited[e->dest] && e->weight > 0) {
	    visited[e->dest] = 1;
	    pred[e->dest] = source;
	    int flow_added = dfs(e->dest, sink);
	    if (flow_added == 1)
		return 1;
	}
	e = e->next;
    }

    return 0;
}

int do_dfs(int source, int sink) {
    (void)memset(visited, 0, cur_length * sizeof(int));
    return dfs(source, sink);
}

int add_flow_dfs(int source, int sink) {
    if (!do_dfs(source, sink))
	return 0;

    int max_flow = follow_pred(source, sink);
    update_graph(source, sink, max_flow);
    return max_flow;

}

int get_max_flow(int source, int sink) {
    int total_flow = 0;
    int new_flow;
    while (1) {
	if (BFS)
	    new_flow = add_flow_bfs(source, sink);
	else
	    new_flow = add_flow_dfs(source, sink);
	if (new_flow == 0)
	    return total_flow;
	total_flow += new_flow;
    }
}

void read_input() {
    scanf("%d %d %d %d", &N, &En, &Wn, &Fn);
    Elist = malloc(En * sizeof(struct eastcity));
    Wlist = malloc(Wn * sizeof(int));
    Flist = malloc(Fn * sizeof(struct flight));
    for (int i = 0; i < En; i++)
	scanf("%d", &Elist[i].index);
    for (int i = 0; i < En; i++) {
	scanf("%d", &Elist[i].pop);
	total_people += Elist[i].pop;
    }
    for (int i = 0; i < Wn; i++)
	scanf("%d", &Wlist[i]);
    for (int i = 0; i < Fn; i++)
	scanf("%d %d %d %d", &Flist[i].origin, &Flist[i].dest,
	      &Flist[i].cap, &Flist[i].time);
}

int possible(int time) {
    int source, sink;
    construct_graph(time, &source, &sink);
    int max_flow = get_max_flow(source, sink);
    if (max_flow == total_people)
	return 1;

    return 0;
}

int main(void){
    int min_time = 1;
    read_input();

    int sink;
    construct_initial_graph(&sink);
    for (int i = 0; i < En; i++) {
	if (!do_bfs(Elist[i].index, sink)) {
	    printf("-1\n");	
	    return 0;
	}
    }

    while (!possible(min_time)) {
	min_time++;
    }

    printf("%d\n", min_time);
}

